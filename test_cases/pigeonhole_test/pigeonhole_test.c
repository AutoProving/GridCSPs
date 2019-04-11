// Copyright 2019 Andreas Ommundsen, Lucia Fuentes Villodres, Daniel Notland, Nora Hobæk Hovland, Simen Lone
// This file is licensed under MIT License, as specified in the file LICENSE located at the root folder of this repository.
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../../listColoring.h"

LCInstance *pigeonholeInstance(int rows, int columns);

int s2n(char *str);

int main() {

    LCInstance *piggy = pigeonholeInstance(4, 4);

    printf("nRows: %d\n", piggy->nRows);
    printf("nCols: %d\n", piggy->nColumns);

    printf("\x1b[34m");
    printf("IntermediateColors[%d][%d]\n", piggy->nRows, piggy->nColumns);
    for (int x = 0; x < piggy->nRows; ++x) {
        for (int y = 0; y < piggy->nColumns; ++y) {
            printf("\tAlphaSize[%d][%d]: %d\n", x, y, piggy->IntermediateColors[x][y].sizeAlphabet);
            for (int i = 0; i < piggy->IntermediateColors[x][y].sizeAlphabet; ++i) {
                printf("\t\t%d\t%s\n",
                       piggy->IntermediateColors[x][y].S2N[i],
                       piggy->IntermediateColors[x][y].N2S[i]);
            }
        }
    }

    printf("\x1b[33m");
    printf("\n\nFinalColors[%d][%d]\n", piggy->nRows, piggy->nColumns);
    for (int x = 0; x < piggy->nRows; ++x) {
        for (int y = 0; y < piggy->nColumns; ++y) {
            printf("\tAlphaSize[%d][%d]: %d\n", x, y, piggy->FinalColors[x][y].sizeAlphabet);
            for (int i = 0; i < piggy->FinalColors[x][y].sizeAlphabet; ++i) {
                printf("\t\t%d\t%s\n",
                       piggy->FinalColors[x][y].S2N[i],
                       piggy->FinalColors[x][y].N2S[i]);
            }
        }
    }

    printf("\x1b[34m");
    printf("\n\ncolorMap[%d][%d]\n", piggy->nRows, piggy->nColumns);
    for (int x = 0; x < piggy->nRows; ++x) {
        for (int y = 0; y < piggy->nColumns; ++y) {
            printf("\tcolorMap[%d][%d].nColors: %d\n\t", x, y, piggy->colorMap[x]->nColors);
            for (int i = 0; i < piggy->colorMap[x][y].nColors; ++i) {
                printf("\tmap[%d] = %d", i, piggy->colorMap[x][y].map[i]);
            }
            printf("\n");
        }
    }

    printf("\x1b[33m");
    int vConRow = piggy->nRows - 1;
    int vConCol = piggy->nColumns;
    printf("\nVertical Constraint Matrix[%d][%d]: \n", vConRow, vConCol);
    for (int x = 0; x < vConRow; ++x) {
        for (int y = 0; y < vConCol; ++y) {
            printf("\tnConstraints[%d][%d]: %d\n\t\t", x, y, piggy->vConstraints[x][y].nConstraints);
            for (int i = 0; i < piggy->vConstraints[x][y].nConstraints; ++i) {
                printf("(%-2d, %-2d)\t",
                       piggy->vConstraints[x][y].pairs[i].color1, piggy->vConstraints[x][y].pairs[i].color2);
            }
            printf("\n\t\t");
            for (int i = 0; i < piggy->vConstraints[x][y].nConstraints; ++i) {
                printf("(%-2s, %-2s)\t",
                       piggy->IntermediateColors[0][0].N2S[piggy->vConstraints[x][y].pairs[i].color1],
                       piggy->IntermediateColors[0][0].N2S[piggy->vConstraints[x][y].pairs[i].color2]);
            }
            printf("\n");
        }
    }
    printf("\n\n");

    int hConRow = piggy->nRows;
    int hConCol = piggy->nColumns - 1;
    printf("\x1b[34m");
    printf("Horizontal Constraint Matrix[%d][%d]: \n", hConRow, hConCol);
    for (int x = 0; x < hConRow; ++x) {
        for (int y = 0; y < hConCol; ++y) {
            printf("\tnConstraints[%d][%d]: %d\n\t\t", x, y, piggy->hConstraints[x][y].nConstraints);
            for (int i = 0; i < piggy->hConstraints[x][y].nConstraints; ++i) {
                printf("(%-2d, %-2d)\t",
                       piggy->hConstraints[x][y].pairs[i].color1, piggy->hConstraints[x][y].pairs[i].color2);
            }
            printf("\n\t\t");
            for (int i = 0; i < piggy->hConstraints[x][y].nConstraints; ++i) {
                printf("(%-2s, %-2s)\t",
                       piggy->IntermediateColors[0][0].N2S[piggy->hConstraints[x][y].pairs[i].color1],
                       piggy->IntermediateColors[0][0].N2S[piggy->hConstraints[x][y].pairs[i].color2]);
            }
            printf("\n");
        }
    }
    return 0;
}
