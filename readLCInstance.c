// Copyright 2019 Haavard Notland
// This file is licensed under MIT License, as specified in the file LISENSE located at the root folder of this repository.


#include <stdlib.h>
#include <string.h>
//#include "listColoring.h"

void readIntermediateColors(FILE* reader, AlphabetMap* intermediateColors, int i, int j){

    char line[256], c[32];
    while(fgets(line, sizeof(line), reader)) {
        char *keywordPosition = strstr(line, "INTERMEDIATE_COLORS");
        char *commentPosition = strstr(line, "//");

        if(keywordPosition != NULL){
            if(commentPosition == NULL || keywordPosition < commentPosition){
                break;
            }
        }
    }

    //??? Bind read integer to sizeAlphabet
    sscanf(line, "%s %d", c, &intermediateColors->sizeAlphabet);

    intermediateColors->S2N = malloc(intermediateColors->sizeAlphabet * sizeof(int));
    intermediateColors->N2S = malloc(intermediateColors->sizeAlphabet * sizeof(char*));

    for(int k=0; k< intermediateColors->sizeAlphabet; k++){
        while(fgets(line, sizeof(line), reader)){
            if(line != NULL){
               intermediateColors->N2S[k] = malloc(sizeof(line));
               sscanf(line, "%d %s", &intermediateColors->S2N[k], intermediateColors->N2S[k]);
               break;
            }
        }
    }


}

void readFinalMap(FILE* reader, AlphabetMap* finalColors, int i, int j){

    char line[256], c[32];
    while(fgets(line, sizeof(line), reader)) {
        char *keywordPosition = strstr(line, "FINAL_COLORS");
        char *commentPosition = strstr(line, "//");

        if(keywordPosition != NULL){
            if(commentPosition == NULL || keywordPosition < commentPosition){
                break;
            }
        }
    }

    //??? bind read integer to sizeAlphabet
    sscanf(line, "%s %d", c, &finalColors->sizeAlphabet);

    finalColors->S2N = malloc(finalColors->sizeAlphabet * sizeof(int));
    finalColors->N2S = malloc(finalColors->sizeAlphabet * sizeof(char*));

    for(int k=0; k< finalColors->sizeAlphabet; k++){
        while(fgets(line, sizeof(line), reader)){
            if(line != NULL){
                finalColors->N2S[i] = malloc(sizeof(line));
                sscanf(line, "%d %s", &finalColors->S2N[k], finalColors->N2S[k]);
                break;
            }
        }
    }


}

void readColorMap(FILE* reader, ColorMap* colorMap, int i, int j){

    char line[256], c[32];
    while(fgets(line, sizeof(line), reader)) {
        char *keywordPosition = strstr(line, "COLOR_MAP");
        char *commentPosition = strstr(line, "//");

        if(keywordPosition != NULL){
            if(commentPosition == NULL || keywordPosition < commentPosition){
                break;
            }
        }
    }

    //??? Bind read integer to nColors
    sscanf(line, "%s %d", c, &colorMap->nColors);

    colorMap->map = malloc(colorMap->nColors * sizeof(int));
    int m, n;
    for(int k=0; k<colorMap->nColors; k++){
        while(fgets(line, sizeof(line), reader)){
            if(line != NULL){
                //???
                sscanf(line, "%d %d", &m, &n);
                colorMap->map[m] = n;
                break;
            }
        }
    }



}

void readVConstraints(FILE* reader, Constraint* vConstraints, int i, int j){
    char line[256], c[32];

    vConstraints->nConstraints = 2;
    vConstraints->pairs = malloc(2 * sizeof(int));
    int m, n;
    for(int k=0; k<2; k++){
        while(fgets(line, sizeof(line), reader)){
            if(line != NULL){
                sscanf(line, "%d %d", &m, &n);
                vConstraints->pairs[i].color1 = m;
                vConstraints->pairs[i].color2 = n;
                break;
            }
        }
    }
}

void readHConstraints(FILE* reader, Constraint* hConstraints, int i, int j){
    char line[64], c[32];

    hConstraints->nConstraints = 2;
    hConstraints->pairs = malloc(2 * sizeof(int));
    int m, n;
    for(int k=0; k<2; k++){
        while(fgets(line, sizeof(line), reader)){
            if(line != NULL){
                sscanf(line, "%d %d", &m, &n);
                hConstraints->pairs[i].color1 = m;
                hConstraints->pairs[i].color2 = n;
                break;
            }
        }
    }
}

void readLCInstance(char* filename, LCInstance* instance){
    FILE *reader = fopen(filename, "r");
    char line[64], c[32], needle[64];

    while(fgets(line, sizeof(line), reader)){
        char *keywordPosition = strstr(line, "LIST_COLORING");
        char *commentPosition = strstr(line, "//");
        if(keywordPosition != NULL){
            if(commentPosition == NULL || keywordPosition < commentPosition){
                break;
            }
        }
    }

    //??? Bind read integers to nRows and nColumns
    sscanf(line, "%s %d %d", c, &instance->nRows, &instance->nColumns);

    //??? Allocate size of structs
    instance->IntermediateColors = malloc(instance->nRows * sizeof(AlphabetMap*));
    instance->FinalColors = malloc(instance->nRows * sizeof(AlphabetMap*));
    instance->colorMap = malloc(instance->nRows * sizeof(ColorMap*));


    for(int i=0; i<instance->nRows; i++){

        //??? Allocate size of structs
        instance->IntermediateColors[i] = malloc(instance->nColumns * sizeof(AlphabetMap));
        instance->FinalColors[i] = malloc(instance->nColumns * sizeof(AlphabetMap));
        instance->colorMap[i] = malloc(instance->nColumns * sizeof(ColorMap));

        for(int j=0; j<instance->nColumns; j++){

            while(fgets(line, sizeof(line), reader)){
                sprintf(needle, "COLOR_LISTS %d %d", i, j);
                char *keyWordPosition = strstr(line, needle);
                char *commentPosition = strstr(line, "//");
                if(keyWordPosition != NULL){
                    if(commentPosition == NULL || keyWordPosition < commentPosition){
                        break;
                    }
                }
            }

            AlphabetMap IntermediateColors;
            AlphabetMap FinalColors;
            ColorMap colorMap;


            readIntermediateColors(reader, &IntermediateColors, i, j);
            readFinalMap(reader, &FinalColors, i, j);
            readColorMap(reader, &colorMap, i, j);

            instance->IntermediateColors[i][j] = IntermediateColors;
            instance->FinalColors[i][j] = FinalColors;
            instance->colorMap[i][j] = colorMap;
        }
    }

    instance->vConstraints = malloc((instance->nRows-1) * sizeof(int));
    for(int i=0; i<instance->nRows-1; i++){

        instance->vConstraints[i] = malloc((instance->nColumns) * sizeof(int));

        for(int j=0; j<instance->nColumns; j++){

            while(fgets(line, sizeof(line), reader)){
                sprintf(needle, "VERTICAL_CONSTRAINTS %d %d", i, j);
                char *keyWordPosition = strstr(line, needle);
                char *commentPosition = strstr(line, "//");
                if(keyWordPosition != NULL){
                    if(commentPosition == NULL || keyWordPosition < commentPosition){
                        break;
                    }
                }
            }
            Constraint vConstraints;

            readVConstraints(reader, &vConstraints, i, j);

            instance->vConstraints[i][j] = vConstraints;
        }

    }

    instance->hConstraints = malloc(instance->nRows * sizeof(int));
    for(int i=0; i<instance->nRows; i++){

        instance->hConstraints[i] = malloc((instance -> nColumns-1) * sizeof(int));

        for(int j=0; j<instance->nColumns-1; j++){
            while(fgets(line, sizeof(line), reader)){
                sprintf(needle, "HORIZONTAL_CONSTRAINTS %d %d", i, j);
                char *keyWordPosition = strstr(line, needle);
                char *commentPosition = strstr(line, "//");
                if(keyWordPosition != NULL){
                    if(commentPosition == NULL || keyWordPosition < commentPosition){
                        break;
                    }
                }
            }
            Constraint hConstraints;

            readHConstraints(reader, &hConstraints, i, j);

            instance->hConstraints[i][j] = hConstraints;
        }
    }
}
