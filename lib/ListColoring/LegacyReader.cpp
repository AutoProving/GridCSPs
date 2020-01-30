// Copyright (c) 2019-2020 Mateus de Oliveira Oliveira and Contributors. 
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

#include <ListColoring/LegacyReader.h>

#include <algorithm>
#include <iterator>
#include <memory>
#include <streambuf>

namespace ListColoring {
namespace Legacy {
namespace {

class Token {
public:
    Token(std::string token)
        : token_(token)
    {}

    std::string asString() {
        return token_;
    }

    int asInt() {
        int ret;
        if (std::sscanf(token_.c_str(), "%d", &ret) != 1) {
            throw ReaderError(token_ + ": integer format error");
        }
        return ret;
    }

    bool isInt() {
        int tmp;
        return std::sscanf(token_.c_str(), "%d", &tmp) == 1;
    }
private:
    std::string token_;
};

template<class It>
class Tokenizer {
public:
    Tokenizer(It beg, It end)
        : cur_(beg)
        , end_(end)
        , state_(State::FREE)
    {}

    Token nextToken() {
        while (state_ != State::IN_TOKEN) {
            processChar();
        }
        while (cur_ != end_ && state_ == State::IN_TOKEN) {
            processChar();
        }
        return Token(token_);
    }

    bool seekEof() {
        while (cur_ != end_ && state_ != State::IN_TOKEN) {
            processChar();
        }
        return cur_ == end_;
    }

private:
    enum class State {
        FREE,
        IN_TOKEN,
        COMMENT_1,
        COMMENT
    };

    void processChar() {
        if (cur_ == end_)
            throw ReaderError("unexpected eof");
        char c = *cur_++;
        switch(state_) {
        case State::FREE:
            if (std::isspace(c)) {
                break;
            } else if (c == '/') {
                state_ = State::COMMENT_1;
            } else {
                state_ = State::IN_TOKEN;
                token_ = {c};
            }
            break;
        case State::IN_TOKEN:
            if (std::isspace(c)) {
                state_ = State::FREE;
            } else if (c == '/') {
                state_ = State::COMMENT_1;
            } else {
                token_.push_back(c);
            }
            break;
        case State::COMMENT_1:
            if (std::isspace(c)) {
                throw ReaderError("unexpected '/'");
            } else if (c == '/') {
                state_ = State::COMMENT;
            } else {
                throw ReaderError("unexpected '/'");
            }
            break;
        case State::COMMENT:
            if (c == '\n') {
                state_ = State::FREE;
            } else {
                break;
            }
            break;
        }
    }

    It cur_, end_;
    State state_;
    std::string token_;
};

template<class TT>
std::vector<std::string> readColors(TT& tokenizer) {
    int size = tokenizer.nextToken().asInt();
    std::vector<std::string> ret(size);
    for (int i = 0; i < size; i++) {
        int id = tokenizer.nextToken().asInt();
        std::string symbol = tokenizer.nextToken().asString();
        ret[id] = symbol;
    }
    return ret;
}

template<class TT>
std::vector<int> readColorMap(TT& tokenizer) {
    int size = tokenizer.nextToken().asInt();
    std::vector<int> ret(size);
    for (int i = 0; i < size; i++) {
        int id = tokenizer.nextToken().asInt();
        int mm = tokenizer.nextToken().asInt();
        ret[id] = mm;
    }
    return ret;
}

template<class TT>
Constraint readOption(TT& tokenizer) {
    int fst = tokenizer.nextToken.asInt();
    int snd = tokenizer.nextToken.asInt();
    return {fst, snd};
}

template<class TT>
class Reader {
public:
    Reader(TT& tokenizer)
        : tokenizer_(tokenizer)
        , i_(0)
        , j_(0)
    {}

    ProblemInstance read() {
        while (!tokenizer_.seekEof())
            readControl();
        checkInstance();
        return *instance_;
    }

private:
    void readHeader() {
        int height = tokenizer_.nextToken().asInt();
        int width = tokenizer_.nextToken().asInt();
        instance_ = std::make_unique<ProblemInstance>(height, width);
    }

    void readColorLists() {
        i_ = tokenizer_.nextToken().asInt();
        j_ = tokenizer_.nextToken().asInt();
    }

    void readIntermediateColors() {
        using ::ListColoring::Legacy::readColors;
        checkInstance();
        for (const std::string& symb : readColors(tokenizer_)) {
            instance_->intermediateColors(i_, j_).addSymbol(symb);
        }
    }

    void readFinalColors() {
        using ::ListColoring::Legacy::readColors;
        checkInstance();
        for (const std::string& symb : readColors(tokenizer_)) {
            instance_->finalColors(i_, j_).addSymbol(symb);
        }
    }

    void readColorMap() {
        using ::ListColoring::Legacy::readColorMap;
        checkInstance();
        instance_->colorMap(i_, j_) = readColorMap(tokenizer_);
    }

    void readConstraints(ConstraintContainer& constraints) {
        if (tokenizer_.seekEof())
            return;
        Token token = tokenizer_.nextToken();
        while (token.isInt()) {
            int fst = token.asInt();
            int snd = tokenizer_.nextToken().asInt();
            constraints.insert({fst, snd});
            if (tokenizer_.seekEof())
                return;
            token = tokenizer_.nextToken();
        }
        control_ = token.asString();
    }

    void readVConstraints() {
        checkInstance();
        i_ = tokenizer_.nextToken().asInt();
        j_ = tokenizer_.nextToken().asInt();
        readConstraints(instance_->verticalConstraints(i_, j_));
    }

    void readHConstraints() {
        checkInstance();
        i_ = tokenizer_.nextToken().asInt();
        j_ = tokenizer_.nextToken().asInt();
        readConstraints(instance_->horizontalConstraints(i_, j_));
    }

    void readControl() {
        std::string control = control_.empty()
                            ? tokenizer_.nextToken().asString()
                            : control_;
        control_.clear();
        if (control == "LIST_COLORING")
            readHeader();
        else if (control == "COLOR_LISTS")
            readColorLists();
        else if (control == "INTERMEDIATE_COLORS")
            readIntermediateColors();
        else if (control == "FINAL_COLORS")
            readFinalColors();
        else if (control == "COLOR_MAP")
            readColorMap();
        else if (control == "VERTICAL_CONSTRAINTS")
            readVConstraints();
        else if (control == "HORIZONTAL_CONSTRAINTS")
            readHConstraints();
        else
            throw ReaderError("unknown control: " + control);
    }

    void checkInstance() {
        if (!instance_)
            throw ReaderError("expected a header");
    }

    TT& tokenizer_;
    int i_, j_;
    std::unique_ptr<ProblemInstance> instance_;
    std::string control_;
};

template<class It>
ProblemInstance read(It begin, It end) {
    Tokenizer<It> tokenizer(begin, end);
    return Reader<Tokenizer<It>>(tokenizer).read();
}

}

ReaderError::ReaderError(const std::string& message)
    : std::runtime_error("Legacy::ReaderError: " + message)
{}

ProblemInstance read(std::istream& is) {
    return read(std::istreambuf_iterator<char>(is),
                std::istreambuf_iterator<char>());
}

ProblemInstance read(const std::string& description) {
    return read(description.begin(), description.end());
}

}
}
