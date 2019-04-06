#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "listColoring.h"

LCInstance *pigeonholeInstance(int rows, int columns);

int s2n(char *str);

int main() {

    LCInstance *piggy = pigeonholeInstance(2, 2);

    printf("nRows: %d\n", piggy->nRows);
    printf("nCols: %d\n", piggy->nColumns);

    printf("%s\n", "IntermediateColors:");
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


    int vConRow = piggy->nRows - 1;
    int vConCol = piggy->nColumns + 1;

    for (int x = 0; x < vConRow; ++x) {
        for (int y = 0; y < vConCol; ++y) {
            printf("nConstraints[%d][%d]: %d\n", x, y, piggy->vConstraints[x][y].nConstraints);
            for (int i = 0; i < piggy->vConstraints[x][y].nConstraints; ++i) {
                printf("(%d, %d) ", piggy->vConstraints[x][y].pairs[i].color1, piggy->vConstraints[x][y].pairs[i].color2);
            }
            printf("\n");
            for (int i = 0; i < piggy->vConstraints[x][y].nConstraints; ++i) {
                printf("(%s, %s) ", piggy->IntermediateColors[0][0].N2S[piggy->vConstraints[x][y].pairs[i].color1],
                       piggy->IntermediateColors[0][0].N2S[piggy->vConstraints[x][y].pairs[i].color2]);
            }
            printf("\n");
        }
    }

    return 0;
}
