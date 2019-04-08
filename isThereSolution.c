//
// Created by howiemalowie on 4/1/19.
//

//#include "listColoring.h"
#include <stdbool.h>
//#include "/home/howiemalowie/ODDs/powerSet.c"
//#include "rowToODD.c"

bool isThereSolution(LCInstance* I){

    ODD* L = malloc(I->nRows * sizeof(ODD));
    rowToODD(I, 0, &L[0]);
    ODD aux = L[0];
    ODD powSet;
    int left, row;
    Layer next;

    int r = I->nRows;
    int c = I->nColumns;

    for(int i = 1; i < r-1; i++){
        for(int j = 0; j < c-1; j++){
            if(j==0){
                left = r-2;
                row = i-1;
            }
            else{
                left = j-1;
                row = i;
            }

            aux.layerSequence[j] = nextIncrement(I, &L[row].layerSequence[left], &L[i-1].layerSequence[j], i, j);
        }
        powerSetODD(&aux, &powSet);
        //minimize(&aux);
        L[i] = powSet;
    }

    if(&L[I->nRows-1] != NULL){
        return 1;
    }
    return 0;
}