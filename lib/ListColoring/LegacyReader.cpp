#include <ListColoring/LegacyReader.h>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/container.hpp>

#include <algorithm>
#include <iterator>
#include <memory>
#include <streambuf>

namespace ListColoring {
namespace Legacy {
namespace {

template<class It>
ProblemInstance read(It begin, It end) {
    namespace x3 = boost::spirit::x3;
    namespace mpl = boost::mpl;
    using boost::fusion::vector;
    using boost::fusion::at;

    It oldBegin = begin;

    std::unique_ptr<ProblemInstance> instance;
    int i = 0, j = 0; // Indices of an element to be modified

    // Parser actions
    auto createInstance = [&instance](auto& ctx) {
        const auto& sz = x3::_attr(ctx);
        int height = at<mpl::int_<0>>(sz);
        int width = at<mpl::int_<1>>(sz);
        instance = std::make_unique<ProblemInstance>(height, width);
    };

    auto setIndices = [&i, &j](auto& ctx) {
        const auto& ind = x3::_attr(ctx);
        i = at<mpl::int_<0>>(ind);
        j = at<mpl::int_<1>>(ind);
    };

    auto setIntermediateColors = [&instance, &i, &j](auto& ctx) {
        const auto& map = x3::_attr(ctx);
        std::vector<std::string> alphabet(map.size());
        for (const auto& v : map) {
            int k = at<mpl::int_<0>>(v);
            const std::string& symbol = at<mpl::int_<1>>(v);
            alphabet[k] = symbol;
        }
        for (const auto& symbol : alphabet) {
            instance->intermediateColors(i, j).addSymbol(symbol);
        }
    };

    auto setFinalColors = [&instance, &i, &j](auto& ctx) {
        const auto& map = x3::_attr(ctx);
        std::vector<std::string> alphabet(map.size());
        for (const auto& v : map) {
            int k = at<mpl::int_<0>>(v);
            const std::string& symbol = at<mpl::int_<1>>(v);
            alphabet[k] = symbol;
        }
        for (const auto& symbol : alphabet) {
            instance->finalColors(i, j).addSymbol(symbol);
        }
    };

    auto setColorMap = [&instance, &i, &j](auto& ctx) {
        const auto& map = x3::_attr(ctx);
        ColorMap cmap(map.size());
        for (const auto& v : map) {
            int from = at<mpl::int_<0>>(v);
            int to = at<mpl::int_<1>>(v);
            cmap[from] = to;
        }
        instance->colorMap(i, j) = cmap;
    };

    auto setVConstraints = [&instance, &i, &j](auto& ctx) {
        const auto& desc = x3::_attr(ctx);
        for (const auto& c : desc) {
            int lt = at<mpl::int_<0>>(c);
            int rt = at<mpl::int_<1>>(c);
            instance->verticalConstraint(i, j).insert({lt, rt});
        }
    };

    auto setHConstraints = [&instance, &i, &j](auto& ctx) {
        const auto& desc = x3::_attr(ctx);
        for (const auto& c : desc) {
            int lt = at<mpl::int_<0>>(c);
            int rt = at<mpl::int_<1>>(c);
            instance->horizontalConstraint(i, j).insert({lt, rt});
        }
    };

    // Comments
    auto lineComment = x3::lexeme["//" >> *(x3::char_ - x3::eol) >> x3::eol];
    auto skipParser = x3::space | lineComment;

    // Header
    auto header = ("LIST_COLORING" >> x3::int_ >> x3::int_)[createInstance];

    // Color lists
    auto string = x3::lexeme[*(x3::char_ - x3::ascii::space)];
    auto alphabetMember = x3::int_ >> string;
    auto alphabetMap = *alphabetMember;
    auto intermediateColors = "INTERMEDIATE_COLORS" >> x3::int_
                           >> alphabetMap[setIntermediateColors];
    auto finalColors = "FINAL_COLORS" >> x3::int_
                    >> alphabetMap[setFinalColors];
    auto colorMap = "COLOR_MAP" >> x3::int_
                 >> (*(x3::int_ >> x3::int_))[setColorMap];
    auto colorLists = "COLOR_LISTS"
                   >> (x3::int_ >> x3::int_)[setIndices]
                   >> *(intermediateColors | finalColors | colorMap);

    // Constraints
    auto constraints = *(x3::int_ >> x3::int_);
    auto vconstraints = "VERTICAL_CONSTRAINTS"
                     >> (x3::int_ >> x3::int_)[setIndices]
                     >> constraints[setVConstraints];
    auto hconstraints = "HORIZONTAL_CONSTRAINTS"
                     >> (x3::int_ >> x3::int_)[setIndices]
                     >> constraints[setHConstraints];

    auto instanceDesc = header >> *(colorLists | hconstraints | vconstraints);

    bool success = x3::phrase_parse(
        begin,
        end,
        instanceDesc,
        skipParser
    );

    if (!success)
        throw ReaderError("spirit error");
    if (begin != end)
        throw ReaderError("spirit stopped at position "
                         + std::to_string(begin - oldBegin));
    if (!instance)
        throw ReaderError("no header detected");

    return *instance;
}

}

ReaderError::ReaderError(const std::string& message)
    : std::runtime_error("Legacy::ReaderError: " + message)
{}

ProblemInstance read(std::istream& is) {
    std::string description;
    std::copy(std::istreambuf_iterator<char>(is),
              std::istreambuf_iterator<char>(),
              std::back_inserter(description));
    return read(description);
}

ProblemInstance read(const std::string& description) {
    return read(description.begin(), description.end());
}

}
}
