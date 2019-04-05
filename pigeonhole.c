// Copyright 2019 Andreas Ommundsen, PLEASE ADD YOUR NAMES HERE AS YOU CONTRIBUTE
// This file is licensed under MIT License, as specified in the file LICENSE located at the root folder of this repository.
#include "listColoring.h"
#include <stdlib.h>
#include <stdio.h>

AlphabetMap *colorMap();

LCInstance *pigeonholeInstance(int rows, int columns) {

    printf("Rows/Cols: %d/%d\n", rows, columns);

    LCInstance *pigeon = malloc(sizeof(*pigeon)); // FREE ME

    pigeon->nRows = rows;
    pigeon->nColumns = columns;

    AlphabetMap *interFinal = colorMap();
    pigeon->IntermediateColors = &interFinal;
    pigeon->FinalColors = &interFinal;

    // Freeing anything?

    printf("Test: %lu\n", sizeof(pigeon));

    return pigeon;
}

AlphabetMap *colorMap() {

    AlphabetMap *colorMap = malloc(sizeof(AlphabetMap)); // FREE ME
    colorMap->sizeAlphabet = 9;
    colorMap->N2S = malloc((unsigned long) colorMap->sizeAlphabet * sizeof(char *)); // FREE ME

    colorMap->N2S[0] = "b";
    colorMap->N2S[1] = "g";
    colorMap->N2S[2] = "rr";
    colorMap->N2S[3] = "bb";
    colorMap->N2S[4] = "bg";
    colorMap->N2S[5] = "br";
    colorMap->N2S[6] = "gb";
    colorMap->N2S[7] = "gg";
    colorMap->N2S[8] = "gr";

    colorMap->S2N = malloc((unsigned long) colorMap->sizeAlphabet * sizeof(int)); // FREE ME
    for (int i = 0; i < colorMap->sizeAlphabet; ++i)
        colorMap->S2N[i] = i;

    return colorMap;
}
