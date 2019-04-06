// Copyright 2019 Andreas Ommundsen, ... PLEASE ADD YOUR NAMES HERE AS YOU CONTRIBUTE
// This file is licensed under MIT License, as specified in the file LICENSE located at the root folder of this repository.
#include "listColoring.h"
#include <stdlib.h>
#include <stdio.h>

AlphabetMap *defaultColorMap(void);

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
    }

    pigeon->FinalColors = pigeon->IntermediateColors;

    return pigeon;
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
