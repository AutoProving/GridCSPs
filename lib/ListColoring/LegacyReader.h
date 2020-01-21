#pragma once

/**
 * @file ListColoring/LegacyWriter.h
 * Provides reader to a legacy-style format.
 *
 * The format is self-explanatory. Find an example in "listColoringExample.tex"
 * file in the source repository.
 */

#include <ListColoring/ListColoring.h>

#include <stdexcept>
#include <string>
#include <iostream>

namespace ListColoring {
namespace Legacy {

/**
 * @brief A legacy reader format error.
 *
 * Could be either a tokenizer, parser or logic error.
 */
class ReaderError : public std::runtime_error {
public:
    ReaderError(const std::string& message);
};

/**
 * @brief Read an instance description in legacy format from stream.
 * @throws ReaderError In case of format error.
 */
ProblemInstance read(std::istream& is);

/**
 * @brief Read an instance description in legacy format from stream.
 * @throws ReaderError In case of format error.
 */
ProblemInstance read(const std::string& description);

}
}
