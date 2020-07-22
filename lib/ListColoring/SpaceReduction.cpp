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

#include <ListColoring/SpaceReduction.h>

#include <cassert>
#include <functional>

namespace ListColoring {
namespace {

ProblemInstance augmentIfNeeded(const ProblemInstance& pi) {
    if (pi.height() % 2 == 0)
        return pi;
    ProblemInstance ret(pi.height() + 1, pi.width());
    for (int i = 0; i < pi.height(); i++) {
        for (int j = 0; j < pi.width(); j++) {
            ret.intermediateColors(i, j) = pi.intermediateColors(i, j);
            ret.finalColors(i, j) = pi.finalColors(i, j);
            ret.colorMap(i, j) = pi.colorMap(i, j);
        }
    }
    for (int i = 0; i < pi.height() - 1; i++) {
        for (int j = 0; j < pi.width(); j++) {
            ret.verticalConstraints(i, j) = pi.verticalConstraints(i, j);
        }
    }
    for (int i = 0; i < pi.height(); i++) {
        for (int j = 0; j < pi.width() - 1; j++) {
            ret.horizontalConstraints(i, j) = pi.horizontalConstraints(i, j);
        }
    }
    // Last row
    {
        ODDs::ODD::AlphabetMap alphabet;
        alphabet.addSymbol("a");
        for (int i = 0; i < pi.width(); i++) {
            ret.intermediateColors(pi.height(), i) = alphabet;
            ret.finalColors(pi.height(), i) = alphabet;
            ret.colorMap(pi.height(), i) = {0};
        }
    }
    ConstraintContainer hor = {{0, 0}};
    for (int i = 0; i < pi.width() - 1; i++)
        ret.horizontalConstraints(pi.height(), i) = hor;
    for (int i = 0; i < pi.width(); i++) {
        ConstraintContainer vert;
        const auto& alphabet = pi.intermediateColors(pi.height() - 1, i);
        for (int j = 0; j < (int)alphabet.symbols().size(); j++) {
            vert.insert({j, 0});
        }
        ret.verticalConstraints(pi.height() - 1, i) = vert;
    }
    return ret;
}

using AnsCoordinates = std::vector<std::vector<std::pair<int, int>>>;

struct ReducedInstance {
    ProblemInstance instance;
    AnsCoordinates coordinates;
};

ConstraintContainer transpose(const ConstraintContainer& constraints) {
    ConstraintContainer ret;
    for (const Constraint& constraint : constraints) {
        ret.emplace(constraint.second, constraint.first);
    }
    return ret;
}

void copyLeftSquare(ProblemInstance& instance, int i, int j) {
    assert(i > 0);
    assert(j > 0);
    instance.intermediateColors(i, j) = instance.intermediateColors(i, j - 1);
    instance.finalColors(i, j) = instance.finalColors(i, j - 1);
    instance.colorMap(i, j) = instance.colorMap(i, j - 1);
    int alphabetSize = instance.intermediateColors(i, j).symbols().size();
    for (int k = 0; k < alphabetSize; k++) {
        instance.horizontalConstraints(i, j - 1).insert({k, k});
    }
    int upAlphabetSize = instance.intermediateColors(i - 1, j).symbols().size();
    for (int k = 0; k < upAlphabetSize; k++) {
        for (int t = 0; t < alphabetSize; t++) {
            instance.verticalConstraints(i - 1, j).insert({k, t});
        }
    }
}

ReducedInstance halvedInstance(const ProblemInstance& pp) {
    ProblemInstance ret(pp.height() / 2, 2 * pp.width() + pp.height() - 2);
    AnsCoordinates cds(pp.height());
    for (int i = 0; i < (int)cds.size(); i++)
        cds[i].resize(pp.width());
    for (int i = 0; i < ret.height(); i++) {
        for (int j = 0; j < pp.width() - i; j++) {
            cds[i][j] = {i, j};
            ret.intermediateColors(i, j) = pp.intermediateColors(i, j);
            ret.finalColors(i, j) = pp.finalColors(i, j);
            ret.colorMap(i, j) = pp.colorMap(i, j);
            if (i > 0) {
                ret.verticalConstraints(i - 1, j) = pp.verticalConstraints(i - 1, j);
            }
            if (j > 0) {
                ret.horizontalConstraints(i, j - 1) = pp.horizontalConstraints(i, j - 1);
            }
        }
        int base = pp.width() - i;
        for (int j = 0; j < 2 * i; j++) {
            copyLeftSquare(ret, i, base);
            base++;
        }
        if (i > 0) {
            int col = pp.width() - 1 - i;
            auto constraints = transpose(pp.horizontalConstraints(i, col));
            ret.verticalConstraints(i - 1, base - 1) = constraints;
        }
        for (int j = i + 1; j < pp.height() - i; j++) {
            int col = pp.width() - 1 - i;
            cds[j][col] = {i, base};
            ret.intermediateColors(i, base) = pp.intermediateColors(j, col);
            ret.finalColors(i, base) = pp.finalColors(j, col);
            ret.colorMap(i, base) = pp.colorMap(j, col);
            if (i > 0) {
                auto constraints = transpose(pp.horizontalConstraints(j, col));
                ret.verticalConstraints(i - 1, base) = constraints;
            }
            auto constraints = pp.verticalConstraints(j - 1, col);
            ret.horizontalConstraints(i, base - 1) = constraints;
            base++;
        }
        for (int j = 0; j < 2 * i; j++) {
            copyLeftSquare(ret, i, base);
            base++;
        }
        if (i > 0) {
            int row = pp.height() - 1 - i;
            int col = pp.width() - 1 - i;
            auto constraints = transpose(pp.verticalConstraints(row, col));
            ret.verticalConstraints(i - 1, base - 1) = constraints;
        }
        for (int j = pp.width() - 2 - i; j >= 0; j--) {
            int row = pp.height() - 1 - i;
            cds[row][j] = {i, base};
            ret.intermediateColors(i, base) = pp.intermediateColors(row, j);
            ret.finalColors(i, base) = pp.finalColors(row, j);
            ret.colorMap(i, base) = pp.colorMap(row, j);
            if (i > 0) {
                auto constraints = transpose(pp.verticalConstraints(row, j));
                ret.verticalConstraints(i - 1, base) = constraints;
            }
            auto constraints = transpose(pp.horizontalConstraints(row, j));
            ret.horizontalConstraints(i, base - 1) = constraints;
            base++;
        }
        assert(base == ret.width());
    }
    return {ret, cds};
}

void applyColors(const ProblemInstance& initial,
                 const std::vector<int>& colors,
                 ProblemInstance& to) {
    int row = to.height() - 1;
    for (int j = 0; j < (int)colors.size(); j++) {
        const auto& hIntermediate = initial.intermediateColors(row, j);
        const auto& hFinal = initial.finalColors(row, j);
        const auto& hColorMap = initial.colorMap(row, j);
        ODDs::ODD::AlphabetMap intermediateMap;
        intermediateMap.addSymbol(hIntermediate.numberToSymbol(colors[j]));
        ODDs::ODD::AlphabetMap finalMap;
        finalMap.addSymbol(hFinal.numberToSymbol(hColorMap[colors[j]]));
        to.intermediateColors(row, j) = intermediateMap;
        to.finalColors(row, j) = finalMap;
        to.colorMap(row, j) = {0};
        if (row > 0) {
            to.verticalConstraints(row - 1, j).clear();
            for (const auto& c : initial.verticalConstraints(row - 1, j)) {
                if (c.second == colors[j])
                    to.verticalConstraints(row - 1, j).insert({c.first, 0});
            }
        }
        {
            to.horizontalConstraints(row, j).clear();
            for (const auto& c : initial.horizontalConstraints(row, j)) {
                if (c.first == colors[j])
                    to.horizontalConstraints(row, j).insert({0, c.second});
            }
        }
        if (j > 0) {
            to.horizontalConstraints(row, j - 1) = {{0, 0}};
        }
    }
    int w = to.width();
    for (int j = 0; j < (int)colors.size(); j++) {
        const auto& hIntermediate = initial.intermediateColors(row + 1, j);
        const auto& hFinal = initial.finalColors(row + 1, j);
        const auto& hColorMap = initial.colorMap(row + 1, j);
        std::map<int, int> old2New;
        {
            to.intermediateColors(row, w - 1 - j) = {};
            Constraint kb{colors[j], std::numeric_limits<int>::min()};
            auto b = initial.verticalConstraints(row, j).lower_bound(kb);
            Constraint ke{colors[j], std::numeric_limits<int>::max()};
            auto e = initial.verticalConstraints(row, j).upper_bound(ke);
            for (auto it = b; it != e; it++) {
                const auto& symbol = hIntermediate.numberToSymbol(it->second);
                auto id = to.intermediateColors(row, w - 1 - j).addSymbol(symbol);
                old2New[it->second] = id;
            }
        }
        {
            const auto& ics = to.intermediateColors(row, w - 1 - j);
            int alphabetSize = ics.symbols().size();
            to.finalColors(row, w - 1 - j) = {};
            to.colorMap(row, w - 1 - j).resize(alphabetSize);
            for (int i = 0; i < alphabetSize; i++) {
                const auto& iSym = ics.numberToSymbol(i);
                auto iSymId = hIntermediate.symbolToNumber(iSym);
                const auto& fsym = hFinal.numberToSymbol(hColorMap[iSymId]);
                auto fSymId = to.finalColors(row, w - 1 - j).addSymbol(fsym);
                to.colorMap(row, w - 1 - j)[i] = fSymId;
            }
        }
        if (row > 0) {
            to.verticalConstraints(row - 1, w - 1 - j).clear();
            for (const auto& c : transpose(initial.verticalConstraints(row + 1, j))) {
                if (!old2New.count(c.second))
                    continue;
                Constraint fil{c.first, old2New[c.second]};
                to.verticalConstraints(row - 1, w - 1 - j).insert(fil);
            }
        }
        if (j > 0) {
            ConstraintContainer hcs;
            for (const auto& c : to.horizontalConstraints(row, w - 1 - j)) {
                if (!old2New.count(c.first))
                    continue;
                hcs.emplace(old2New[c.first], c.second);
            }
            to.horizontalConstraints(row, w - 1 - j) = hcs;
        }
        {
            to.horizontalConstraints(row, w - 2 - j).clear();
            for (const auto& c : transpose(initial.horizontalConstraints(row + 1, j))) {
                if (!old2New.count(c.second))
                    continue;
                Constraint fil{c.first, old2New[c.second]};
                to.horizontalConstraints(row, w - 2 - j).insert(fil);
            }
        }
    }
}

template<class R>
class Producer {
public:
    virtual ~Producer() = default;

    std::optional<R> getNext() {
        if (finished_)
            return {};
        next_ = {};
        while (!finished_ && !next_.has_value()) {
            step();
        }
        return next_;
    }

protected:
    void yield(const R& value) {
        next_ = value;
    }

    void stop() {
        finished_ = true;
    }

    virtual void step() = 0;

private:
    std::optional<R> next_ = {};
    bool finished_ = false;
};

/**
 * @warning Should not be exposed. Doesn't work for all ODDs. At least, assumes
 * that ODD is deterministic and that initialStates() = {0}.
 */
class ODDPathProducer : public Producer<std::vector<int>> {
public:
    ODDPathProducer(ODDs::ODD&& odd)
        : odd_(std::move(odd))
    {
        auto range = odd_.getLayer(0).transitions.proceedFrom(0);
        pos_ = {range.begin()};
        end_ = {range.end()};
        colors_ = {0};
        states_ = {0};
        updateLastTransition();
    }

    virtual ~ODDPathProducer() override = default;

protected:
    virtual void step() override {
        if (colors_.empty()) {
            stop();
        } else if (pos_.back() == end_.back()) {
            colors_.pop_back();
            states_.pop_back();
            pos_.pop_back();
            end_.pop_back();
            if (!pos_.empty() && pos_.back() != end_.back()) {
                pos_.back()++;
                updateLastTransition();
            }
        } else if ((int)pos_.size() == odd_.countLayers()) {
            if (odd_.finalStates().count(states_.back())) {
                yield(colors_);
            }
            pos_.back()++;
            updateLastTransition();
        } else {
            const auto& layer = odd_.getLayer(colors_.size());
            auto range = layer.transitions.proceedFrom(states_.back());
            pos_.push_back(range.begin());
            end_.push_back(range.end());
            colors_.push_back(0);
            states_.push_back(0);
            updateLastTransition();
        }
    }

private:
    void updateLastTransition() {
        if (pos_.back() != end_.back()) {
            colors_.back() = pos_.back()->symbol;
            states_.back() = pos_.back()->to;
        }
    }

    ODDs::ODD odd_;
    std::vector<int> colors_;
    std::vector<int> states_;
    std::vector<ODDs::ODD::TransitionContainer::Iterator> pos_;
    std::vector<ODDs::ODD::TransitionContainer::Iterator> end_;
};

ODDs::ODD buildODD(const ProblemInstance& pi, int row, int layers) {
    ODDs::ODDBuilder builder(1);
    int alphabetSize;
    builder.setInitialStates({0});
    {
        ODDs::ODD::TransitionContainer leftTransitions;
        alphabetSize = pi.intermediateColors(row, 0).symbols().size();
        for (int i = 0; i < alphabetSize; i++) {
            leftTransitions.insert({0, i, i});
        }
        builder.addLayer(
            pi.intermediateColors(row, 0),
            leftTransitions,
            alphabetSize
        );
    }
    for (int j = 1; j < layers; j++) {
        ODDs::ODD::TransitionContainer transitions;
        alphabetSize = pi.intermediateColors(row, j).symbols().size();
        for (const auto& c : pi.horizontalConstraints(row, j - 1)) {
            transitions.insert({c.first, c.second, c.second});
        }
        builder.addLayer(
            pi.intermediateColors(row, j),
            transitions,
            alphabetSize
        );
    }
    {
        ODDs::ODD::StateContainer finalStates;
        for (ODDs::ODD::State j = 0; j < alphabetSize; j++) {
            finalStates.insert(j);
        }
        builder.setFinalStates(finalStates);
    }
    return builder.build();
}

}

class SpaceReduction::Impl {
private:
    static ODDs::ODD buildODD_(const ProblemInstance& pi) {
        int row = (pi.height() - 1) / 2;
        int length = pi.width() - (pi.height() + 1) / 2;
        return buildODD(pi, row, length);
    }

public:
    Impl(const ProblemInstance& pi)
        : initial_(pi)
        , preprocessed_(augmentIfNeeded(pi))
        , ri_(halvedInstance(preprocessed_))
        , augmented_(pi.height() % 2)
        , colorsProducer_(buildODD_(pi))
        , cur_(ri_.instance)
    {
        auto colors = colorsProducer_.getNext();
        if (colors.has_value()) {
            applyColors(initial_, colors.value(), cur_.value());
        } else {
            cur_ = {};
        }
    }

    std::optional<ProblemInstance> nextInstance() {
        auto ret = cur_;
        if (!cur_.has_value())
            return cur_;
        auto colors = colorsProducer_.getNext();
        if (colors.has_value()) {
            applyColors(initial_, colors.value(), cur_.value());
        } else {
            cur_ = {};
        }
        return ret;
    }

    Solution restoreSolution(const Solution& reducedSolution) {
        Solution solution(initial_.get().height(), initial_.get().width());
        for (int i = 0; i < initial_.get().height(); i++) {
            for (int j = 0; j < initial_.get().width(); j++) {
                auto p = ri_.coordinates[i][j];
                solution.get(i, j) = reducedSolution.get(p.first, p.second);
            }
        }
        return solution;
    }

private:
    int alphabetSize(int j) const {
        return preprocessed_.intermediateColors(cur_.value().height() - 1, j)
                            .symbols()
                            .size();
    }

    std::reference_wrapper<const ProblemInstance> initial_;
    ProblemInstance preprocessed_;
    ReducedInstance ri_;
    bool augmented_;
    ODDPathProducer colorsProducer_;
    std::optional<ProblemInstance> cur_;
};

SpaceReduction::SpaceReduction(const ProblemInstance& pi)
    : impl_(std::make_unique<Impl>(pi))
{}

SpaceReduction::~SpaceReduction() = default;

SpaceReduction::SpaceReduction(SpaceReduction&&) = default;

SpaceReduction& SpaceReduction::operator=(SpaceReduction&&) = default;

std::optional<ProblemInstance> SpaceReduction::nextInstance() {
    return impl_->nextInstance();
}

Solution SpaceReduction::restoreSolution(const Solution& reducedSolution) const {
    return impl_->restoreSolution(reducedSolution);
}

}
