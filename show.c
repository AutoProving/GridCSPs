// Copyright 2019 Andreas Ommundsen, Lucia Fuentes Villodres, Daniel Notland, Nora Hobæk Hovland, Simen Lone
// This file is licensed under MIT License, as specified in the file LICENSE located at the root folder of this repository.
#include <stdlib.h>
#include <stdio.h>
#include "helpers.h"

void showColorMap(ColorMap *colorMap) {

    printf("\t│ ColorMap - nColors: %d\n\t│\t", colorMap->nColors);
    for (int i = 0; i < colorMap->nColors; ++i) {
        printf("(map[%d] = %d)  ", i, colorMap->map[i]);
    }
    printf("\n\t│\n");
}

void showColorPair(ColorPair *colorPair) {

    printf("(%-2d,%2d)  ", colorPair->color1, colorPair->color2);
}

void showConstraint(Constraint *constraint) {

    printf("\t│ Constraint - nConstraints: %d\n\t│\t", constraint->nConstraints);
    for (int i = 0; i < constraint->nConstraints; ++i) {
        showColorPair(&constraint->pairs[i]);
    }
    printf("\n\t│\n");
}

void showAlphabetMap(AlphabetMap *alphaMap) {

    printf("\t│ AlphabetMap - size: %d\n\t|\t(S2N[i] = N2S[i])\n\t│\t", alphaMap->sizeAlphabet);
    for (int i = 0; i < alphaMap->sizeAlphabet; ++i) {
        printf("(%d = %s)  ", alphaMap->S2N[i], alphaMap->N2S[i]);
    }
    printf("\n\t│\n");
}

void showLCI(LCInstance *lci) {

    int rows = lci->nRows;
    int cols = lci->nColumns;

    printf("LCInstance[%d][%d]\n", rows, cols);

    printf("IntermediateColors[%d][%d]\n", rows, cols);
    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            printf("\tIntermediateColors[%d][%d]\n", x, y);
            showAlphabetMap(&lci->IntermediateColors[x][y]);
        }
    }

    printf("\n\n\n");

    printf("FinalColors[%d][%d]\n", rows, cols);
    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            printf("\tFinalColors[%d][%d]\n", x, y);
            showAlphabetMap(&lci->FinalColors[x][y]);
        }
    }

    printf("\n\n\n");

    printf("ColorMap[%d][%d]\n", rows, cols);
    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            printf("\tColorMap[%d][%d]\n", x, y);
            showColorMap(&lci->colorMap[x][y]);
        }
    }

    printf("\n\n\n");

    printf("VerticalConstraints[%d][%d]\n", rows - 1, cols);
    for (int x = 0; x < rows - 1; ++x) {
        for (int y = 0; y < cols; ++y) {
            printf("\tVerticalConstraints[%d][%d]\n", x, y);
            showConstraint(&lci->vConstraints[x][y]);
        }
    }

    printf("\n\n\n");

    printf("HorizontalConstraints[%d][%d]\n", rows, cols - 1);
    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols - 1; ++y) {
            printf("\tHorizontalConstraints[%d][%d]\n", x, y);
            showConstraint(&lci->hConstraints[x][y]);
        }
    }
}
