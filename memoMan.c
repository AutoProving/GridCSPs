// Copyright 2019 Andreas Ommundsen, Lucia Fuentes Villodres, Daniel Notland, Nora Hobæk Hovland, Simen Lone
// This file is licensed under MIT License, as specified in the file LICENSE located at the root folder of this repository.
#include <stdlib.h>
#include "helpers.h"


void killAlphabetMap(AlphabetMap *target) {

    for (int i = 0; i < target->sizeAlphabet; ++i)
        free(target->N2S[i]);
    free(target->N2S);
    free(target->S2N);
}

void killColorMap(ColorMap *target) {
    free(target->map);
}

void killConstraint(Constraint *target) {
    free(target->pairs);
}

void killLCI(LCInstance *target) {

    int rows = target->nRows;
    int cols = target->nColumns;

    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            killAlphabetMap(&target->IntermediateColors[x][y]);
        }
        free(target->IntermediateColors[x]);
    }
    free(target->IntermediateColors);

    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            killAlphabetMap(&target->FinalColors[x][y]);
        }
        free(target->FinalColors[x]);
    }
    free(target->FinalColors);

    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            killColorMap(&target->colorMap[x][y]);
        }
        free(target->colorMap[x]);
    }
    free(target->colorMap);

    for (int x = 0; x < rows - 1; ++x) {
        for (int y = 0; y < cols; ++y) {
            killConstraint(&target->vConstraints[x][y]);
        }
        free(target->vConstraints[x]);
    }
    free(target->vConstraints);

    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols - 1; ++y) {
            killConstraint(&target->hConstraints[x][y]);
        }
        free(target->hConstraints[x]);
    }
    free(target->hConstraints);

    free(target);
}
