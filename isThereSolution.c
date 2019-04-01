//
// Created by howiemalowie on 4/1/19.
//

#include "listColoring.h"
#include <stdbool.h>

bool isThereSolution(LCInstance* I){

    ODD* L = malloc(I->nRows * sizeof(ODD));
    L[0] = rowToODD(I, 0);
    ODD aux = L[0];

    for(int i = 1; i < nRows-1; i++){
        for(int j = 0; j < nColumns-1; j++){
            aux[j] = nextLayer(I, i, j, &(L[0].layerSequence));
        }
        lazyPowerSet(&aux);
        minimize(&aux);
        L[i] = aux;
    }

    if(L[I->nRows-1] != NULL){
        return 1;
    }
    return 0;
}