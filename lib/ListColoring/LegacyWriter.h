#pragma once

/**
 * @file ListColoring/LegacyWriter.h
 * Provides writer to a legacy-style format.
 *
 * The format is self-explanatory. Find an example in "listColoringExample.tex"
 * file in the source repository.
 */

#include <ListColoring/ListColoring.h>

#include <iostream>
#include <string>

namespace ListColoring {
namespace Legacy {

/**
 * @brief Writes an instance description to a stream in legacy format.
 */
void write(std::ostream& os, const ProblemInstance& instance);

/**
 * @brief Returns an instance description in legacy format.
 */
std::string write(const ProblemInstance& instance);

}
}
