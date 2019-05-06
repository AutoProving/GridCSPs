//
// Created by howiemalowie on 4/1/19.
//

//#include "listColoring.h"
#include <stdbool.h>
//#include "/home/howiemalowie/ODDs/powerSet.c"
//#include "rowToODD.c"

void copyLayer(Layer *copyTo, Layer *copyFrom)
{
    copyTo->map.N2S = malloc(copyFrom->map.sizeAlphabet * sizeof(char));
    copyTo->map.S2N = malloc(copyFrom->map.sizeAlphabet * sizeof(int));
    copyTo->map = copyFrom->map;

    copyTo->leftStates.set = malloc(copyFrom->leftStates.nStates * sizeof(State));
    copyTo->leftStates = copyFrom->leftStates;

    copyTo->rightStates.set = malloc(copyFrom->rightStates.nStates * sizeof(State));
    copyTo->rightStates = copyFrom->rightStates;

    copyTo->initialStates.set = malloc(copyFrom->initialStates.nStates * sizeof(State));
    copyTo->initialStates = copyFrom->initialStates;

    copyTo->finalStates.set = malloc(copyFrom->finalStates.nStates * sizeof(State));
    copyTo->finalStates = copyFrom->finalStates;

    copyTo->transitions.set = malloc(copyFrom->transitions.nTransitions * sizeof(Transition));
    copyTo->transitions = copyFrom->transitions;

    copyTo->width = copyFrom->width;
    copyTo->initialFlag = copyFrom->initialFlag;
    copyTo->finalFlag = copyFrom->finalFlag;
}

bool isThereSolution(LCInstance* I){

    ODD* L = malloc(I->nRows * sizeof(ODD));
    rowToODD(I, 0, &L[0]);
    ODD aux = L[0];
    ODD powSet;
    Layer *next;

    int r = I->nRows-1;
    int c = I->nColumns-1;

    printf("Starting nextLayer\n");
    for(int i = 1; i < r; i++){
        for(int j = 0; j < c; j++){
            printf("Going into nextLayer no. %d \n", i*(j+1));
            if(j==0){
                next = nextIncrement(I, NULL, &L[i-1].layerSequence[j], i, j);
            }
            else{
                next = nextIncrement(I, &aux.layerSequence[j-1], &L[i-1].layerSequence[j], i, j);
            }
            printf("Copying layer no. %d\n", i*(j+1));
            copyLayer(&aux.layerSequence[j], next);
        }
        powerSetODD(&aux, &powSet);
        /* BLOCKED:
         * minimize(&aux);
         */
        L[i] = powSet;
    }
    printf("NextLayer complete\n");

    if(&L[I->nRows-1] != NULL){
        return 1;
    }
    return 0;
}

