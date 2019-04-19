// Copyright 2019 Andreas Ommundsen, Lucia Fuentes Villodres, Daniel Notland, Nora Hobæk Hovland, Simen Lone
// This file is licensed under MIT License, as specified in the file LICENSE located at the root folder of this repository.
#include "listColoring.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int s2n(char *str);

AlphabetMap *defaultAlphaMap(void);

ColorMap *defaultColorMap(void);

ColorPair *verticalColorPairs(void);

ColorPair *firstCategoryColorPairs(void);

ColorPair *secondCategoryColorPairs(void);

ColorPair *thirdCategoryColorPairs(void);

Constraint **buildVerticalMatrix(int rows, int columns);

Constraint **buildHorizontalMatrix(int rows, int columns);

LCInstance *pigeonholeInstance(int rows, int columns) {

    LCInstance *pigeon = malloc(sizeof(*pigeon)); // FREE ME

    pigeon->nRows = rows;
    pigeon->nColumns = columns;

    pigeon->IntermediateColors = malloc((unsigned long) rows * sizeof(AlphabetMap *)); // FREE ME
    for (int x = 0; x < rows; ++x) {
        pigeon->IntermediateColors[x] = malloc((unsigned long) columns * sizeof(AlphabetMap)); // FREE ME
        for (int y = 0; y < columns; ++y) {
            AlphabetMap *temp = defaultAlphaMap();
            pigeon->IntermediateColors[x][y] = *temp;
            free(temp);
        }
    }

    pigeon->FinalColors = malloc((unsigned long) rows * sizeof(AlphabetMap *)); // FREE ME
    for (int x = 0; x < rows; ++x) {
        pigeon->FinalColors[x] = malloc((unsigned long) columns * sizeof(AlphabetMap)); // FREE ME
        for (int y = 0; y < columns; ++y) {
            AlphabetMap *temp = defaultAlphaMap();
            pigeon->FinalColors[x][y] = *temp;
            free(temp);
        }
    }

    pigeon->colorMap = malloc((unsigned long) rows * sizeof(ColorMap *)); // FREE ME
    for (int x = 0; x < rows; ++x) {
        pigeon->colorMap[x] = malloc((unsigned long) columns * sizeof(ColorMap)); // FREE ME
        for (int y = 0; y < columns; ++y) {
            ColorMap *temp = defaultColorMap();
            pigeon->colorMap[x][y] = *temp;
            free(temp);
        }
    }

    pigeon->vConstraints = buildVerticalMatrix(rows, columns);

    pigeon->hConstraints = buildHorizontalMatrix(rows, columns);

    return pigeon;
}

Constraint **buildVerticalMatrix(int rows, int columns) {

    int i = rows - 1;
    int j = columns;
    int numOfConstraints = 12;

    Constraint **vMatrix = malloc((unsigned long) i * sizeof(Constraint *)); // FREE ME
    for (int x = 0; x < i; ++x) {
        vMatrix[x] = malloc((unsigned long) j * sizeof(Constraint)); // FREE ME
        for (int y = 0; y < j; ++y) {
            vMatrix[x][y].nConstraints = numOfConstraints;
            vMatrix[x][y].pairs = verticalColorPairs();
        }
    }
    return vMatrix;
}

Constraint **buildHorizontalMatrix(int rows, int columns) {

    int i = rows;
    int j = columns - 1;

    Constraint **hMatrix = malloc((unsigned long) i * sizeof(Constraint *)); // FREE ME
    for (int x = 0; x < i; ++x)
        hMatrix[x] = malloc((unsigned long) j * sizeof(Constraint)); // FREE ME

    // First Category
    for (int x = 0; x < i; ++x) {
        hMatrix[x][0].nConstraints = 3;
        hMatrix[x][0].pairs = firstCategoryColorPairs();
    }

    // Second Category
    for (int x = 0; x < i; ++x) {
        for (int y = 1; y < j-1; ++y) {
            hMatrix[x][y].nConstraints = 12;
            hMatrix[x][y].pairs = secondCategoryColorPairs();
        }
    }

    // Third Category
    for (int x = 0; x < i; ++x) {
        hMatrix[x][j - 1].nConstraints = 3;
        hMatrix[x][j - 1].pairs = thirdCategoryColorPairs();
    }

    return hMatrix;
}

AlphabetMap *defaultAlphaMap(void) {

    int alphaSize = 9;
    AlphabetMap *defaultMap = malloc(sizeof(AlphabetMap));

    defaultMap->sizeAlphabet = alphaSize;

    defaultMap->N2S = malloc((unsigned long) alphaSize * sizeof(char *)); // FREE ME
    defaultMap->N2S[0] = strdup("b");
    defaultMap->N2S[1] = strdup("g");
    defaultMap->N2S[2] = strdup("rr");
    defaultMap->N2S[3] = strdup("bb");
    defaultMap->N2S[4] = strdup("bg");
    defaultMap->N2S[5] = strdup("br");
    defaultMap->N2S[6] = strdup("gb");
    defaultMap->N2S[7] = strdup("gg");
    defaultMap->N2S[8] = strdup("gr");

    defaultMap->S2N = malloc((unsigned long) alphaSize * sizeof(int)); // FREE ME
    for (int i = 0; i < alphaSize; ++i)
        defaultMap->S2N[i] = i;

    return defaultMap;
}

ColorMap *defaultColorMap(void) {

    ColorMap *defaultMap = malloc(sizeof(ColorMap));

    defaultMap->nColors = 9;
    defaultMap->map = malloc((unsigned long) defaultMap->nColors * sizeof(int));
    for (int i = 0; i < defaultMap->nColors; ++i)
        defaultMap->map[i] = i;

    return defaultMap;
}

ColorPair *verticalColorPairs() {

    int numOfConstraints = 12;
    ColorPair *defaultPairs = malloc((unsigned long) numOfConstraints * sizeof(ColorPair)); // FREE ME
    defaultPairs[0] = (ColorPair) {s2n("bb"), s2n("bb")};
    defaultPairs[1] = (ColorPair) {s2n("bb"), s2n("gb")};
    defaultPairs[2] = (ColorPair) {s2n("gb"), s2n("bb")};
    defaultPairs[3] = (ColorPair) {s2n("gb"), s2n("gb")};

    defaultPairs[4] = (ColorPair) {s2n("bb"), s2n("rr")};
    defaultPairs[5] = (ColorPair) {s2n("gb"), s2n("rr")};

    defaultPairs[6] = (ColorPair) {s2n("rr"), s2n("bg")};
    defaultPairs[7] = (ColorPair) {s2n("rr"), s2n("gg")};

    defaultPairs[8] = (ColorPair) {s2n("bg"), s2n("bg")};
    defaultPairs[9] = (ColorPair) {s2n("bg"), s2n("gg")};
    defaultPairs[10] = (ColorPair) {s2n("gg"), s2n("bg")};
    defaultPairs[11] = (ColorPair) {s2n("gg"), s2n("gg")};

    return defaultPairs;
}

ColorPair *firstCategoryColorPairs() {

    int firstNum = 3;
    ColorPair *firstPairs = malloc((unsigned long) firstNum * sizeof(ColorPair)); // FREE ME
    firstPairs[0] = (ColorPair) {s2n("b"), s2n("bb")};
    firstPairs[1] = (ColorPair) {s2n("b"), s2n("bg")};
    firstPairs[2] = (ColorPair) {s2n("b"), s2n("rr")};
    return firstPairs;
}

ColorPair *secondCategoryColorPairs() {

    int secondNum = 12;
    ColorPair *secondPairs = malloc((unsigned long) secondNum * sizeof(ColorPair)); // FREE ME
    secondPairs[0] = (ColorPair) {s2n("bb"), s2n("bb")};
    secondPairs[1] = (ColorPair) {s2n("bb"), s2n("bg")};
    secondPairs[2] = (ColorPair) {s2n("bg"), s2n("bb")};
    secondPairs[3] = (ColorPair) {s2n("bg"), s2n("bg")};

    secondPairs[4] = (ColorPair) {s2n("bb"), s2n("rr")};
    secondPairs[5] = (ColorPair) {s2n("bg"), s2n("rr")};

    secondPairs[6] = (ColorPair) {s2n("rr"), s2n("gb")};
    secondPairs[7] = (ColorPair) {s2n("rr"), s2n("gg")};

    secondPairs[8] = (ColorPair) {s2n("gb"), s2n("gb")};
    secondPairs[9] = (ColorPair) {s2n("gb"), s2n("gg")};
    secondPairs[10] = (ColorPair) {s2n("gg"), s2n("gb")};
    secondPairs[11] = (ColorPair) {s2n("gg"), s2n("gg")};

    return secondPairs;
}

ColorPair *thirdCategoryColorPairs() {

    int thirdNum = 3;
    ColorPair *thirdPairs = malloc((unsigned long) thirdNum * sizeof(ColorPair)); // FREE ME
    thirdPairs[0] = (ColorPair) {s2n("rr"), s2n("g")};
    thirdPairs[1] = (ColorPair) {s2n("gb"), s2n("g")};
    thirdPairs[2] = (ColorPair) {s2n("gg"), s2n("g")};

    return thirdPairs;
}

/**
 * "Switch statement" for handling string as case input.
 */
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
