// Copyright (c) 2019-2020 Vasily Alferov, Mateus de Oliveira Oliveira and Contributors.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <ListColoring/Solver.h>

#include <ODDs/ODDs.h>
#include <ODDs/Operations.h>

#include <cassert>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <map>

namespace ListColoring {
namespace {

/**
 * @brief Incapsulates all operations with ODDs depending on selected mode.
 */
class ODDOperationsManager {
public:
    virtual ~ODDOperationsManager() = default;

    virtual ODDs::ODDBuilder rowBuilder(int rowId, int leftStates) = 0;
    virtual ODDs::ODD lazyPowerSet(int rowId, const ODDs::ODD& odd) = 0;
    virtual ODDs::ODD minimize(int rowId, const ODDs::ODD& odd) = 0;
    virtual void reportLayer(int rowId) = 0;
};

class MemoryManager : public ODDOperationsManager {
public:
    virtual ~MemoryManager() = default;

    virtual ODDs::ODDBuilder rowBuilder(int, int leftStates) override {
        return ODDs::ODDBuilder(leftStates);
    }

    virtual ODDs::ODD lazyPowerSet(int, const ODDs::ODD& odd) override {
        return ODDs::diagramLazyPowerSet(odd);
    }

    virtual ODDs::ODD minimize(int, const ODDs::ODD& odd) override {
        return ODDs::minimize(odd);
    }

    virtual void reportLayer(int) override {}
};

class DiskManager : public ODDOperationsManager {
public:
    DiskManager(const std::string& workDir)
        : workDir_(workDir)
    {}

    virtual ~DiskManager() = default;

    virtual ODDs::ODDBuilder rowBuilder(int rowId, int leftStates) override {
        namespace fs = std::filesystem;
        std::string layerPath = fs::path(workDir_) / layerDirName(rowId);
        return ODDs::ODDBuilder(leftStates, layerPath);
    }

    virtual ODDs::ODD lazyPowerSet(int rowId, const ODDs::ODD& odd) override {
        namespace fs = std::filesystem;
        std::string detPath = fs::path(workDir_) / detDirName(rowId);
        return ODDs::diagramLazyPowerSet(odd, detPath);
    }

    virtual ODDs::ODD minimize(int rowId, const ODDs::ODD& odd) override {
        namespace fs = std::filesystem;
        std::string tempPath = fs::path(workDir_) / "temp";
        std::string minPath = fs::path(workDir_) / minDirName(rowId);
        return ODDs::minimize(odd, minPath, tempPath);
    }

    virtual void reportLayer(int rowId) override {
        namespace fs = std::filesystem;
        std::string reportPath = fs::path(workDir_) / "layer.txt";
        std::ofstream fout(reportPath);
        fout << rowId;
    }

    std::optional<int> nextRowToProcess() {
        namespace fs = std::filesystem;
        std::string reportPath = fs::path(workDir_) / "layer.txt";
        if (!fs::exists(reportPath)) {
            return {};
        }
        std::ifstream fin(reportPath);
        int rowId;
        if (!(fin >> rowId)) {
            return {};
        }
        return rowId + 1;
    }

    std::string layerMinDirName(int rowId) {
        namespace fs = std::filesystem;
        return fs::path(workDir_) / minDirName(rowId);
    }

private:
    static std::string layerDirName(int rowId) {
        return "layer" + std::to_string(rowId);
    }

    static std::string detDirName(int rowId) {
        return "layer" + std::to_string(rowId)  + "_det";
    }

    static std::string minDirName(int rowId) {
        return "layer" + std::to_string(rowId) + "_min";
    }

    std::string workDir_;
};

ODDs::ODD firstRowODD(const ProblemInstance& instance, ODDOperationsManager& manager) {
    ODDs::ODDBuilder builder = manager.rowBuilder(0, 1);
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
        const auto& constraints = instance.horizontalConstraints(0, i - 1);
        for (const Constraint& option : constraints) {
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

VColorMap vColorMap(const ConstraintContainer& vconstraints) {
    VColorMap ret;
    for (const Constraint& option : vconstraints) {
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
    VColorMap vmap = vColorMap(instance.verticalConstraints(i - 1, 0));
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
    VColorMap vmap = vColorMap(instance.verticalConstraints(i - 1, j));
    ODDs::ODD::TransitionContainer transitions;
    StateStorage rightStates;
    for (const ODDs::ODD::Transition& transition : up.transitions) {
        auto from = transition.from;
        for (ODDs::ODD::Symbol symbol : vmap[transition.symbol]) {
            auto beg_it = leftStates.upStateBegin(from);
            auto end_it = leftStates.upStateEnd(from);
            for (auto it = beg_it; it != end_it; it++) {
                ODDs::ODD::Symbol m = it->first.second;
                if (!instance.horizontalConstraints(i, j - 1).count({m, symbol}))
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
                  int i,
                  ODDOperationsManager& manager) {
    ODDs::ODDBuilder builder = manager.rowBuilder(i, 1);
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
    return builder.build();
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

VColorMap transposedVColorMap(const ConstraintContainer& constraints) {
    VColorMap ret;
    for (const Constraint& option : constraints) {
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
        const auto& vc = instance_.verticalConstraints(row_, i);
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

/**
 * @brief Base class for the two options to store stats.
 */
class StatsStorageBase {
public:
    virtual ~StatsStorageBase() = default;

    virtual SolverStatsBase& get() = 0;
};

/**
 * @brief Stores a reference to SolverStats
 */
class RefStatsStorage : public StatsStorageBase {
public:
    RefStatsStorage(SolverStatsBase& wrapped)
        : wrapper_(wrapped)
    {}

    virtual ~RefStatsStorage() = default;

    virtual SolverStatsBase& get() override {
        return wrapper_.get();
    }

private:
    std::reference_wrapper<SolverStatsBase> wrapper_;
};

/**
 * @brief Stores SolverStats by value
 */
template<class Stats>
class ValueStatsStorage : public StatsStorageBase {
public:
    template<class... Args>
    ValueStatsStorage(Args&&... args)
        : stored_(std::forward<Args>(args)...)
    {}

    virtual ~ValueStatsStorage() = default;

    virtual SolverStatsBase& get() override {
        return stored_;
    }

private:
    Stats stored_;
};

class MockSolverStats : public SolverStatsBase {
public:
    virtual ~MockSolverStats() = default;

    virtual void onRawODD(const ODDs::ODD&) override {}
    virtual void onDeterminateODD(const ODDs::ODD&) override {}
    virtual void onMinimizedODD(const ODDs::ODD&) override {}
};

}

class Solver::Impl {
public:
    Impl(const ProblemInstance& instance)
        : instance_(instance)
        , stats_(std::make_unique<ValueStatsStorage<MockSolverStats>>())
        , manager_(std::make_unique<MemoryManager>())
    {}

    Impl(const ProblemInstance& instance, SolverStatsBase& stats)
        : instance_(instance)
        , stats_(std::make_unique<RefStatsStorage>(stats))
        , manager_(std::make_unique<MemoryManager>())
    {}

    ~Impl() = default;
    Impl(const Impl&) = delete;
    Impl& operator=(const Impl&) = delete;
    Impl(Impl&&) = default;
    Impl& operator=(Impl&&) = default;

    void diskMode(const std::string& dirName) {
        manager_.reset(new DiskManager(dirName));
    }

    void continueInterrupted(const std::string& dirName) {
        DiskManager* manager = new DiskManager(dirName);
        auto toContinue = manager->nextRowToProcess();
        if (!toContinue.has_value()) {
            diskMode(dirName);
            return;
        }
        startFrom_ = toContinue.value();
        for (int i = 0; i < startFrom_; i++) {
            auto odd = ODDs::readFromDirectory(manager->layerMinDirName(i));
            if (!odd.has_value()) {
                startFrom_ = i;
                manager_.reset(manager);
                return;
            }
            odds_.emplace_back(std::move(odd.value()));
        }
        manager_.reset(manager);
    }

    int startFrom() const {
        return startFrom_;
    }

    bool isThereSolution() {
        if (!nonEmptyColors()) {
            return false;
        }
        if (startFrom_ == 0) {
            addODD(0, firstRowODD(instance_, *manager_));
            startFrom_ = 1;
        }
        for (int i = startFrom_; i < instance_.get().height(); i++) {
            addODD(i, nextRow(instance_, odds_.back(), i, *manager_));
            odds_[i - 1].unload();
        }
        return !odds_.back().finalStates().empty();
    }

    void firstSteps(int k) {
        if (!nonEmptyColors()) {
            return;
        }
        if (startFrom_ == 0) {
            addODD(0, firstRowODD(instance_, *manager_));
            startFrom_ = 1;
        }
        for (int i = startFrom_; i < k; i++) {
            addODD(i, nextRow(instance_, odds_.back(), i, *manager_));
            odds_[i - 1].unload();
        }
        for (int i = 0; i < k; i++) {
            odds_[i].detachDir();
        }
    }

    Solution restoreSolution() {
        Solution ret(instance_.get().height(), instance_.get().width());
        LastPathSearcher lastPathSearcher(odds_.back());
        bool success = lastPathSearcher.find();
        odds_.back().unload();
        assert(success);
        ODDPath path = lastPathSearcher.path;
        insertPath(ret, instance_.get().height() - 1, path);
        for (int i = instance_.get().height() - 2; i >= 0; i--) {
            IntermediatePathSearcher searcher(odds_[i], path, instance_.get(), i);
            success = searcher.find();
            assert(success);
            odds_[i].unload();
            path = searcher.path;
            insertPath(ret, i, path);
        }
        return ret;
    }

private:
    std::reference_wrapper<const ProblemInstance> instance_;
    std::unique_ptr<StatsStorageBase> stats_;
    std::unique_ptr<ODDOperationsManager> manager_;
    std::vector<ODDs::ODD> odds_;
    int startFrom_ = 0;

    void addODD(int i, ODDs::ODD&& odd) {
        stats_->get().onRawODD(odd);
        odd = manager_->lazyPowerSet(i, odd);
        stats_->get().onDeterminateODD(odd);
        odd = manager_->minimize(i, odd);
        stats_->get().onMinimizedODD(odd);
        odds_.push_back(std::move(odd));
        manager_->reportLayer(i);
    }

    void insertPath(Solution& s, int row, const ODDPath& path) {
        for (int column = 0; column < instance_.get().width(); column++) {
            const auto& colorMap = instance_.get().colorMap(row, column);
            const auto& finalColors = instance_.get().finalColors(row, column);
            ODDs::ODD::Symbol intermediateColor = path[column];
            ODDs::ODD::Symbol finalColor = colorMap[intermediateColor];
            s.get(row, column) = finalColors.numberToSymbol(finalColor);
        }
    }

    bool nonEmptyColors() {
        for (int i = 0; i < instance_.get().height(); i++) {
            for (int j = 0; j < instance_.get().width(); j++) {
                if (instance_.get().intermediateColors(i, j).symbols().empty())
                    return false;
            }
        }
        return true;
    }
};

Solver::Solver(const ProblemInstance& instance)
    : impl_(std::make_unique<Impl>(instance))
{}

Solver::Solver(const ProblemInstance& instance, SolverStatsBase& stats)
    : impl_(std::make_unique<Impl>(instance, stats))
{}

Solver::~Solver() = default;

Solver::Solver(Solver&&) = default;

Solver& Solver::operator=(Solver&&) = default;

void Solver::diskMode(const std::string& dirName) {
    impl_->diskMode(dirName);
}

void Solver::continueInterrupted(const std::string& dirName) {
    impl_->continueInterrupted(dirName);
}

int Solver::startFrom() const {
    return impl_->startFrom();
}

bool Solver::isThereSolution() {
    return impl_->isThereSolution();
}

void Solver::firstSteps(int k) {
    impl_->firstSteps(k);
}

Solution Solver::restoreSolution() const {
    return impl_->restoreSolution();
}

}
