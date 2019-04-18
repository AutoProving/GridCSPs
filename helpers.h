// Copyright 2019 Andreas Ommundsen, Lucia Fuentes Villodres, Daniel Notland, Nora Hobæk Hovland, Simen Lone
// This file is licensed under MIT License, as specified in the file LICENSE located at the root folder of this repository.
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
