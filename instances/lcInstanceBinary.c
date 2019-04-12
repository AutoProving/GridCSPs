/**
 * Copyright 2019 Lucia Fuentes Villodres
 * This file is licensed under MIT License, as specified in the file LISENSE located at the root folder of this repository.
 */

#include "listColoring.h"
#include <stdio.h>
#include <stdlib.h>

#define ROWS 4
#define COLUMNS 5
#define ALPHABETSIZE 9
#define NHCONSTRAINTS 13
#define NVCONSTRAINTS 6

LCInstance *createListColoring();
void printPair(AlphabetMap *alphabet, ColorPair *pair);
void destroyLCInstance(LCInstance *lc);


char *colors[] = {"b", "bb", "bg", "br", "g", "gb", "gg", "gr", "rr"};

int main(int argc, char* argv[]){
    LCInstance *lc = createListColoring();

    
    printf("INTERMEDIATE COLORS\n");
    for(int i=0; i<lc->IntermediateColors[0][0].sizeAlphabet; i++){
        fprintf(stdout, "%d = %s\n", lc->IntermediateColors[0][0].S2N[i], 
            lc->IntermediateColors[0][0].N2S[lc->IntermediateColors[0][0].S2N[i]]);
    }
    
    printf("\nHORIZONTAL CONSTRAINTS\n");
    for(int i=0; i<lc->hConstraints[0][0].nConstraints; i++)
        printPair(lc->IntermediateColors[0], &lc->hConstraints[0][0].pairs[i]);

    printf("\nVERTICAL CONSTRAINTS\n");
    for(int i=0; i<lc->vConstraints[0][0].nConstraints; i++)
        printPair(lc->IntermediateColors[0], &lc->vConstraints[0][0].pairs[i]);

    destroyLCInstance(lc);
    return EXIT_SUCCESS;
    
}



LCInstance * createListColoring(){
    LCInstance * lc = (LCInstance*)calloc(1, sizeof(LCInstance));
    lc->nRows = ROWS;
    lc ->nColumns = COLUMNS;

    AlphabetMap *alphabet = (AlphabetMap*)calloc(sizeof(AlphabetMap), 1); //VALGRIND LEAK
    alphabet->sizeAlphabet = ALPHABETSIZE;
    alphabet->S2N = (int*)calloc(sizeof(int), ALPHABETSIZE);
    alphabet->N2S = (char**)calloc(ALPHABETSIZE, sizeof(char*));

    for(int i=0; i<ALPHABETSIZE; i++){
        alphabet->S2N[i] = i;
        alphabet->N2S[i] = colors[i];
    }

    lc->IntermediateColors = (AlphabetMap**)calloc(sizeof(AlphabetMap*), ROWS);
    for (int i = 0; i<ROWS; i++){
        lc->IntermediateColors[i] = (AlphabetMap*)calloc(sizeof(AlphabetMap), COLUMNS);
        for(int j=0; j<COLUMNS; j++)
            lc->IntermediateColors[i][j] = *alphabet;
    }


    //??????????????????????? finalcolors 0, 1 
    lc->FinalColors = NULL;
    lc->colorMap = NULL; //rr -> 1, else to 0


    //horizontal constraints
    ColorPair *horizontalPairs = (ColorPair*)calloc(NHCONSTRAINTS, sizeof(ColorPair));
    horizontalPairs[0].color1 = 0;
    horizontalPairs[0].color2 = 1;

    //covers  i = 1, 2, 3, 4, 5, 6
    for(int i = 1; i<4; i++){
        //(bx, bg = 2)
        horizontalPairs[i].color1 = i;
        horizontalPairs[i].color2 = 2;

        //(bx, rr = 8)
        horizontalPairs[i+3].color1 = i;
        horizontalPairs[i+3].color2 = 8;
    }

    //covers i = 7, 8, 9, 10, 11, 12
    for (int i = 5; i<8; i++){
        //(rr = 8, gx)
        horizontalPairs[i+2].color1 = 8;
        horizontalPairs[i+2].color2 = i;

        //(gx, g = 4)
        horizontalPairs[i+5].color1 = i;
        horizontalPairs[i+5].color2 = 4;
    }

    //vertical constraints
    ColorPair *verticalPairs = (ColorPair*)calloc(NVCONSTRAINTS, sizeof(ColorPair));
    //(xb, gb)
    verticalPairs[0].color1 = 1;
    verticalPairs[0].color2 = 5;
    verticalPairs[1].color1 = 5;
    verticalPairs[1].color2 = 5;

    //(xb, rr)
    verticalPairs[2].color1 = 1;
    verticalPairs[2].color2 = 8;
    verticalPairs[3].color1 = 5;
    verticalPairs[3].color2 = 8;

    //(rr, xg)
    verticalPairs[4].color1 = 8;
    verticalPairs[4].color2 = 2;
    verticalPairs[5].color1 = 8;
    verticalPairs[5].color2 = 6;


    Constraint *vConstraints = (Constraint*)calloc(1, sizeof(Constraint)); //VALGRIND LEAK
    vConstraints->nConstraints = NVCONSTRAINTS;
    vConstraints->pairs = verticalPairs;

    Constraint *hConstraints = (Constraint*)calloc(1, sizeof(Constraint)); //VALGRIND LEAK
    hConstraints->nConstraints = NHCONSTRAINTS;
    hConstraints->pairs = horizontalPairs;


    lc->vConstraints = (Constraint**)calloc(sizeof(Constraint*), ROWS);
    for (int i=0; i<ROWS; i++){
        lc->vConstraints[i] = (Constraint*)calloc(sizeof(Constraint), COLUMNS);
        for(int j=0; j<COLUMNS; j++)
            lc->vConstraints[i][j] = *vConstraints;
    }

    lc->hConstraints = (Constraint**)calloc(sizeof(Constraint*), ROWS);
    for (int i=0; i<ROWS; i++){
        lc->hConstraints[i] = (Constraint*)calloc(sizeof(Constraint), COLUMNS);
        for(int j=0; j<COLUMNS; j++)
            lc->hConstraints[i][j] = *hConstraints;
    }

    return lc;
}


void printPair(AlphabetMap *alphabet, ColorPair *pair){
    if(!pair || !alphabet) return;
    fprintf(stdout, "(%s, %s)\n", alphabet->N2S[pair->color1], alphabet->N2S[pair->color2]);
}


void destroyLCInstance(LCInstance *lc){
    for(int i=0; i<ROWS; i++){
        free(lc->IntermediateColors[i]);
        free(lc->hConstraints[i]);
        free(lc->vConstraints[i]);
    }

    free(lc->IntermediateColors);
    free(lc->hConstraints);
    free(lc->vConstraints);

    free(lc);
}



