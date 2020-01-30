#include <ListColoring/Solver.h>

#include <ODDs/ODDs.h>
#include <ODDs/Operations.h>

#include <functional>
#include <limits>
#include <map>

namespace ListColoring {
namespace {

ODDs::ODD firstRowODD(const ProblemInstance& instance) {
    ODDs::ODDBuilder builder(1);
    builder.setInitialStates({0});
    ODDs::ODD::TransitionContainer firstTransitions;
    int firstAlphabetSize = instance.intermediateColors(0, 0).symbols().size();
    for (ODDs::ODD::State i = 0; i < firstAlphabetSize; i++) {
        firstTransitions.insert({0, i, i});
    }
    builder.addLayer(
        instance.intermediateColors(0, 0),
        firstTransitions,
        firstAlphabetSize
    );
    int rightSize = firstAlphabetSize;
    for (int i = 1; i < instance.width(); i++) {
        ODDs::ODD::TransitionContainer transitions;
        const auto& constraints = instance.horizontalConstraint(0, i - 1);
        for (const ConstraintOption& option : constraints) {
            transitions.insert({option.first, option.second, option.second});
        }
        rightSize = instance.intermediateColors(0, i).symbols().size();
        builder.addLayer(
            instance.intermediateColors(0, i),
            transitions,
            rightSize
        );
    }
    ODDs::ODD::StateContainer finalStates;
    for (ODDs::ODD::State i = 0; i < rightSize; i++) {
        finalStates.insert(i);
    }
    builder.setFinalStates(finalStates);
    return builder.build();
}

using VColorMap = std::map<ODDs::ODD::Symbol, std::vector<ODDs::ODD::Symbol>>;

VColorMap vColorMap(const Constraint& vconstraints) {
    VColorMap ret;
    for (const ConstraintOption& option : vconstraints) {
        ret[option.first].push_back(option.second);
    }
    return ret;
}

/**
 * @brief Stores the mapping of states from the previous row.
 */
class StateStorage {
public:
    using State = ODDs::ODD::State;
    using Color = ODDs::ODD::Symbol;
    using StateColorPair = std::pair<State, Color>;

    StateStorage() {}

    ODDs::ODD::State getId(State upState, Color newColor) {
        StateColorPair key{upState, newColor};
        auto it = s2p_.lower_bound(key);
        if (it != s2p_.end() && it->first == key) {
            return it->second;
        }
        ODDs::ODD::State id = p2s_.size();
        p2s_.emplace_back(upState, newColor);
        s2p_.emplace_hint(it, key, id);
        return id;
    }

    auto upStateBegin(State upState) const {
        return s2p_.lower_bound({
            upState,
            std::numeric_limits<Color>::min()
        });
    }

    auto upStateEnd(State upState) const {
        return s2p_.upper_bound({
            upState,
            std::numeric_limits<Color>::max()
        });
    }

    int size() const {
        return p2s_.size();
    }

    Color getColor(State downState) const {
        return p2s_[downState].second;
    }

private:
    std::vector<StateColorPair> p2s_;
    std::map<StateColorPair, ODDs::ODD::State> s2p_;
};

StateStorage expandLeftLayer(const ProblemInstance& instance,
                             const ODDs::ODD& odd,
                             ODDs::ODDBuilder& builder,
                             int i) {
    const ODDs::ODD::Layer& up = odd.getLayer(0);
    VColorMap vmap = vColorMap(instance.verticalConstraint(i - 1, 0));
    ODDs::ODD::TransitionContainer transitions;
    StateStorage rightStates;
    for (const ODDs::ODD::Transition& transition : up.transitions) {
        for (ODDs::ODD::Symbol symbol : vmap[transition.symbol]) {
            auto to = rightStates.getId(transition.to, symbol);
            transitions.insert({0, symbol, to});
        }
    }
    builder.addLayer(
        instance.intermediateColors(i, 0),
        transitions,
        rightStates.size()
    );
    return rightStates;
}

StateStorage expandLayer(const ProblemInstance& instance,
                         const ODDs::ODD& odd,
                         const StateStorage& leftStates,
                         ODDs::ODDBuilder& builder,
                         int i, int j) {
    const ODDs::ODD::Layer& up = odd.getLayer(j);
    VColorMap vmap = vColorMap(instance.verticalConstraint(i - 1, j));
    ODDs::ODD::TransitionContainer transitions;
    StateStorage rightStates;
    for (const ODDs::ODD::Transition& transition : up.transitions) {
        auto from = transition.from;
        for (ODDs::ODD::Symbol symbol : vmap[transition.symbol]) {
            auto beg_it = leftStates.upStateBegin(from);
            auto end_it = leftStates.upStateEnd(from);
            for (auto it = beg_it; it != end_it; it++) {
                ODDs::ODD::Symbol m = it->first.second;
                if (!instance.horizontalConstraint(i, j - 1).count({m, symbol}))
                    continue;
                transitions.insert({
                    it->second,
                    symbol,
                    rightStates.getId(transition.to, symbol)
                });
            }
        }
    }
    builder.addLayer(
        instance.intermediateColors(i, j),
        transitions,
        rightStates.size()
    );
     return rightStates;
}

ODDs::ODD nextRow(const ProblemInstance& instance,
                  const ODDs::ODD& odd,
                  int i) {
    ODDs::ODDBuilder builder(1);
    builder.setInitialStates({0});
    StateStorage map = expandLeftLayer(instance, odd, builder, i);
    for (int j = 1; j < instance.width(); j++) {
        map = expandLayer(instance, odd, map, builder, i, j);
    }
    ODDs::ODD::StateContainer finalStates;
    for (ODDs::ODD::State s : odd.finalStates()) {
        for (auto it = map.upStateBegin(s); it != map.upStateEnd(s); it++) {
            finalStates.insert(it->second);
        }
    }
    builder.setFinalStates(finalStates);
    return ODDs::minimize(builder.build());
}

using ODDPath = std::vector<ODDs::ODD::Symbol>;

struct LastPathSearcher {
    const ODDs::ODD& odd_;
    ODDPath path;
    std::vector<std::vector<bool>> vis;

    LastPathSearcher(const ODDs::ODD& odd)
      : odd_(odd)
      , path(odd_.countLayers())
      , vis(odd_.countLayers() + 1)
    {
        vis[0] = {false};
        for (int i = 0; i < odd_.countLayers(); i++) {
            vis[i + 1].resize(odd_.getLayer(i).rightStates);
        }
    }

    bool find(int i = 0, int j = 0) {
        vis[i][j] = true;
        if (i == odd_.countLayers()) {
            return odd_.finalStates().count(j);
        }
        int alphabetSize = odd_.getLayer(i).alphabet.symbols().size();
        for (ODDs::ODD::Symbol symbol = 0; symbol < alphabetSize; symbol++) {
            ODDs::ODD::Symbol to = odd_.getLayer(i).transitions.go(j, symbol);
            if (vis[i + 1][to]) {
                continue;
            }
            path[i] = symbol;
            if (find(i + 1, to)) {
                return true;
            }
        }
        return false;
    }
};

VColorMap transposedVColorMap(const Constraint& constraints) {
    VColorMap ret;
    for (const ConstraintOption& option : constraints) {
        ret[option.second].push_back(option.first);
    }
    return ret;
}

struct IntermediatePathSearcher {
    const ODDs::ODD& odd_;
    const ODDPath& prevPath_;
    const ProblemInstance& instance_;
    int row_;

    ODDPath path;
    std::vector<std::vector<bool>> vis;

    IntermediatePathSearcher(const ODDs::ODD& odd,
                             const ODDPath& prevPath,
                             const ProblemInstance& instance,
                             int row)
        : odd_(odd)
        , prevPath_(prevPath)
        , instance_(instance)
        , row_(row)
        , path(odd_.countLayers())
        , vis(odd_.countLayers() + 1)
    {
        vis[0] = {false};
        for (int i = 0; i < odd_.countLayers(); i++) {
            vis[i + 1].resize(odd_.getLayer(i).rightStates);
        }
    }

    bool find(int i = 0, int j = 0) {
        vis[i][j] = true;
        if (i == odd_.countLayers()) {
            return odd_.finalStates().count(j);
        }
        const auto& vc = instance_.verticalConstraint(row_, i);
        VColorMap map = transposedVColorMap(vc);
        for (ODDs::ODD::Symbol symbol : map[prevPath_[i]]) {
            ODDs::ODD::Symbol to = odd_.getLayer(i).transitions.go(j, symbol);
            if (vis[i + 1][to]) {
                continue;
            }
            path[i] = symbol;
            if (find(i + 1, to)) {
                return true;
            }
        }
        return false;
    }
};

}

class Solver::Impl {
public:
    Impl(const ProblemInstance& instance)
        : instance_(instance)
    {}

    ~Impl() = default;
    Impl(const Impl&) = delete;
    Impl& operator=(const Impl&) = delete;
    Impl(Impl&&) = default;
    Impl& operator=(Impl&&) = default;

    bool isThereSolution() {
        odds_.push_back(firstRowODD(instance_));
        for (int i = 1; i < instance_.get().height(); i++) {
            odds_.push_back(nextRow(instance_, odds_.back(), i));
        }
        return !odds_.back().finalStates().empty();
    }

    Solution restoreSolution() {
        Solution ret(instance_.get().height(), instance_.get().width());
        LastPathSearcher lastPathSearcher(odds_.back());
        assert(lastPathSearcher.find());
        ODDPath path = lastPathSearcher.path;
        insertPath(ret, instance_.get().height() - 1, path);
        for (int i = instance_.get().height() - 2; i >= 0; i--) {
            IntermediatePathSearcher searcher(odds_[i], path, instance_.get(), i);
            assert(searcher.find());
            path = searcher.path;
            insertPath(ret, i, path);
        }
        return ret;
    }

private:
    std::reference_wrapper<const ProblemInstance> instance_;
    std::vector<ODDs::ODD> odds_;

    void insertPath(Solution& s, int row, const ODDPath& path) {
        for (int column = 0; column < instance_.get().width(); column++) {
            const auto& colorMap = instance_.get().colorMap(row, column);
            const auto& finalColors = instance_.get().finalColors(row, column);
            ODDs::ODD::Symbol intermediateColor = path[column];
            ODDs::ODD::Symbol finalColor = colorMap[intermediateColor];
            s.get(row, column) = finalColors.numberToSymbol(finalColor);
        }
    }
};

Solver::Solver(const ProblemInstance& instance)
    : impl_(std::make_unique<Impl>(instance))
{}

Solver::~Solver() = default;

Solver::Solver(Solver&&) = default;

Solver& Solver::operator=(Solver&&) = default;

bool Solver::isThereSolution() {
    return impl_->isThereSolution();
}

Solution Solver::restoreSolution() const {
    return impl_->restoreSolution();
}

}
