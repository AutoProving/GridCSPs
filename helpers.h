#ifndef HELPERS_H
#define HELPERS_H

#include "listColoring.h"

// SHOW CATEGORY

void showColorMap(ColorMap *colorMap);

void showColorPair(ColorPair *colorPair);

void showConstraint(Constraint *constraint);

void showAlphabetMap(AlphabetMap *alphaMap);

void showLCI(LCInstance *lci);

// KILL CATEGORY

void killAlphabetMap(AlphabetMap *target);

void killColorMap(ColorMap *target);

void killConstraint(Constraint *target);

void killLCI(LCInstance *target);

#endif
