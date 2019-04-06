// Copyright 2019 Andreas Ommundsen, ... PLEASE ADD YOUR NAMES HERE AS YOU CONTRIBUTE
// This file is licensed under MIT License, as specified in the file LICENSE located at the root folder of this repository.
#include "listColoring.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

AlphabetMap *defaultColorMap(void);

int s2n(char *str);

ColorPair *defaultColorPairs();

Constraint **buildVerticalMatrix(int rows, int columns);

LCInstance *pigeonholeInstance(int rows, int columns) {

    LCInstance *pigeon = malloc(sizeof(*pigeon)); // FREE ME

    pigeon->nRows = rows;
    pigeon->nColumns = columns;

    AlphabetMap *defaultMap = defaultColorMap();

    pigeon->IntermediateColors = malloc((unsigned long) rows * sizeof(AlphabetMap *)); // FREE ME
    for (int x = 0; x < rows; ++x) {
        pigeon->IntermediateColors[x] = malloc((unsigned long) columns * sizeof(AlphabetMap)); // FREE ME
        for (int y = 0; y < columns; ++y)
            pigeon->IntermediateColors[x][y] = *defaultMap;
//            pigeon->IntermediateColors[x][y] = *defaultColorMap();
    }

    // !ACHTUNG! FinalColors is now an ALIAS of IntermediateColors. TODO Ask Mateus.
    pigeon->FinalColors = pigeon->IntermediateColors;

    pigeon->vConstraints = buildVerticalMatrix(rows, columns);

//    pigeon->vConstraints

    return pigeon;
}

int s2n(char *str) {

    if (!strcmp(str, "b"))
        return 0;
    if (!strcmp(str, "g"))
        return 1;
    if (!strcmp(str, "rr"))
        return 2;
    if (!strcmp(str, "bb"))
        return 3;
    if (!strcmp(str, "bg"))
        return 4;
    if (!strcmp(str, "br"))
        return 5;
    if (!strcmp(str, "gb"))
        return 6;
    if (!strcmp(str, "gg"))
        return 7;
    if (!strcmp(str, "gr"))
        return 8;
    else {
        fprintf(stderr, "Bad input @ s2n().\n");
        fprintf(stderr, "Input: %s\n", str);
        exit(EXIT_FAILURE);
    }
}

AlphabetMap *defaultColorMap(void) {

    int alphaSize = 9;
    AlphabetMap *defaultMap = malloc(sizeof(AlphabetMap));

    defaultMap->sizeAlphabet = alphaSize;

    defaultMap->N2S = malloc((unsigned long) alphaSize * sizeof(char *)); // FREE ME
    defaultMap->N2S[0] = "b";
    defaultMap->N2S[1] = "g";
    defaultMap->N2S[2] = "rr";
    defaultMap->N2S[3] = "bb";
    defaultMap->N2S[4] = "bg";
    defaultMap->N2S[5] = "br";
    defaultMap->N2S[6] = "gb";
    defaultMap->N2S[7] = "gg";
    defaultMap->N2S[8] = "gr";

    defaultMap->S2N = malloc((unsigned long) alphaSize * sizeof(int)); // FREE ME
    for (int i = 0; i < alphaSize; ++i)
        defaultMap->S2N[i] = i; // TODO ASK MATEUS ABOUT THE ORDINALITY OF THE COLOR PERMUTATIONS.

    return defaultMap;
}

Constraint **buildVerticalMatrix(int rows, int columns) {

    int i = rows - 1;
    int j = columns + 1;
    int numOfConstraints = 12;

    ColorPair *defaultPairs = defaultColorPairs();

    Constraint **vMatrix = malloc((unsigned long) i * sizeof(Constraint *)); // FREE ME
    for (int x = 0; x < i; ++x) {
        vMatrix[x] = malloc((unsigned long) j * sizeof(Constraint)); // FREE ME
        for (int y = 0; y < j; ++y) {
            vMatrix[x][y].nConstraints = numOfConstraints;
            vMatrix[x][y].pairs = defaultPairs;
        }
    }
    return vMatrix;
}

ColorPair *defaultColorPairs() {

    int numOfConstraints = 12;
    ColorPair *defaultPairs = malloc((unsigned long) numOfConstraints * sizeof(ColorPair)); // FREE ME
    defaultPairs[0] = (ColorPair) {.color1 = s2n("bb"), .color2 = s2n("bb")};
    defaultPairs[1] = (ColorPair) {.color1 = s2n("bb"), .color2 = s2n("gb")};
    defaultPairs[2] = (ColorPair) {.color1 = s2n("gb"), .color2 = s2n("bb")};
    defaultPairs[3] = (ColorPair) {.color1 = s2n("gb"), .color2 = s2n("gb")};

    defaultPairs[4] = (ColorPair) {.color1 = s2n("bb"), .color2 = s2n("rr")};
    defaultPairs[5] = (ColorPair) {.color1 = s2n("gb"), .color2 = s2n("rr")};

    defaultPairs[6] = (ColorPair) {.color1 = s2n("rr"), .color2 = s2n("bg")};
    defaultPairs[7] = (ColorPair) {.color1 = s2n("rr"), .color2 = s2n("gg")};

    defaultPairs[8] = (ColorPair) {.color1 = s2n("bg"), .color2 = s2n("bg")};
    defaultPairs[9] = (ColorPair) {.color1 = s2n("bg"), .color2 = s2n("gg")};
    defaultPairs[10] = (ColorPair) {.color1 = s2n("gg"), .color2 = s2n("bg")};
    defaultPairs[11] = (ColorPair) {.color1 = s2n("gg"), .color2 = s2n("gg")};

    return defaultPairs;
}
