#include <stdio.h>
#include <stdbool.h>
#include "listColoring.h"

LCInstance *pigeonholeInstance(int rows, int columns);

int main(){

    LCInstance *piggy = pigeonholeInstance(1, 2);
    printf("Piggy Size: %lu\n", sizeof(piggy));

    printf("SizeAlpha: %d\n", piggy->FinalColors[0]->sizeAlphabet);
    return 0;
}
