// Copyright 2019 Haavard Notland
// This file is licensed under MIT License, as specified in the file LISENSE located at the root folder of this repository.


#include <stdlib.h>
#include <string.h>
#include "listColoring.h"

void readIntermediateColors(FILE* reader, AlphabetMap* intermediateColors, int i, int j){

    char line[64], c[32];
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
    sscanf(line, "%d", c, &intermediateColors[i][j]->sizeAlphabet);

    intermediateColors[i][j]->S2N = malloc(intermediateColors[i][j]->sizeAlphabet * sizeof(int));
    intermediateColors[i][j]->N2S = malloc(intermediateColors[i][j]->sizeAlphabet * sizeof(char*));

    for(int k=0; k< &intermediateColors->sizeAlphabet; k++){
        while(fgets(line, sizeof(line), reader)){
            if(line != NULL){
               intermediateColors[i][j]->N2S[k] = malloc(sizeof(line));
               sscanf(line, "%d %s", &intermediateColors[i][j]->S2N[k], intermediateColors[i][j]->N2S[k]);
               break;
            }
        }
    }


}

void readFinalMap(FILE* reader, AlphabetMap* finalColors, int i, int j){

    char line[64], c[32];
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
    sscanf(line, "%d", c, &finalColors[i][j]->sizeAlphabet);

    finalColors[i][j]->S2N = malloc(finalColors[i][j]->sizeAlphabet * sizeof(int));
    finalColors[i][j]->N2S = malloc(finalColors[i][j]->sizeAlphabet * sizeof(char*));

    for(int k=0; k< &finalColors[i][j]->sizeAlphabet; k++){
        while(fgets(line, sizeof(line), reader)){
            if(line != NULL){
                finalColors[i][j]->N2S[i] = malloc(sizeof(line));
                sscanf(line, "%d %s", &finalColors[i][j]->S2N[k], finalColors[i][j]->N2S[k]);
                break;
            }
        }
    }


}

void readColorMap(FILE* reader, ColorMap* colorMap, int i, int j){

    char line[64], c[32];
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
    sscanf(line, "%d", c, &colorMap[i][j]->nColors);

    colorMap[i][j]->map = malloc(colorMap[i][j]->nColors * sizeof(int));
    int m, n;
    for(int k=0; k<&colorMap[i][j]->nColors; k++){
        while(fgets(line, sizeof(line), reader)){
            if(line != NULL){
                //???
                sscanf(line, "%d %d", m, n);
                colorMap->map[m] = n;
                break;
            }
        }
    }



}

void readVConstraints(FILE* reader, Constraint* vConstraints, int i, int j){
    char line[64], c[32];

    &vConstraints[i][j]->nConstraints = 2;
    &vConstraints[i][j]->pairs = malloc(2 * sizeof(int));
    int m, n;
    for(int k=0; k<2; k++){
        while(fgets(line, sizeof(line), reader)){
            if(line != NULL){
                sscanf(line, "%d %d", m, n);
                &vConstraints[i][j]->pairs[i].color1 = m;
                &vConstraints[i][j]->pairs[i].color2 = n;
                break;
            }
        }
    }
}

void readHConstraints(FILE* reader, Constraint* hConstraints, int i, int j){
    char line[64], c[32];

    &hConstraints[i][j]->nConstraints = 2;
    &hConstraints[i][j]->pairs = malloc(2 * sizeof(int));
    int m, n;
    for(int k=0; k<2; k++){
        while(fgets(line, sizeof(line), reader)){
            if(line != NULL){
                sscanf(line, "%d %d", m, n);
                &hConstraints[i][j]->pairs[i].color1 = m;
                &hConstraints[i][j]->pairs[i].color2 = n;
                break;
            }
        }
    }
}

void readLCInstance(char* filename, LCInstance* instance){
    File *reader = fopen(filename, "r");
    char line[64], c[32];

    while(fgets(line, sizeof(line), reader)){
        char *keywordPosition = strstr(line, "LIST_COLORING");
        char *commentPosition = strstr(line, "//");
        if(keywordPosition != NULL){
            if(commentPosition == NULL || keyWordPosition < commentPosition){
                break;
            }
        }
    }

    //??? Bind read integers to nRows and nColumns
    sscanf(line, "%d %d", c, &instance->nRows, &instance->nColumns);

    //??? Allocate size of structs
    instance->IntermediateColors = malloc(instance->nRows * sizeof(AlphabetMap));
    instance->finalColors = malloc(instance->nRows * sizeof(AlphabetMap));
    instance->colormap = malloc(instance->nRows * sizeof(ColorMap));

    for(int i=0; i<instance->nRows; i++){

        //??? Allocate size of structs
        instance->IntermediateColors[i] = malloc(instance->nColumns * sizeof(AlphabetMap))
        instance->finalColors[i] = malloc(instance->nColumns * sizeof(AlphabetMap));
        instance->colormap[i] = malloc(instance->nColumns * sizeof(ColorMap));

        for(int j=0; j<instance->nColumns; j++){

            while(fgets(line, sizeof(line), reader)){
                char *keyWordPosition = strstr(line, ("COLOR_LISTS %d %d", i, j));
                char *commentPosition = strstr(line, "//");
                if(keyWordPosition != NULL){
                    if(commentPosition == NULL || keyWordPosition < commentPosition){
                        break;
                    }
                }
            }
            readIntermediateColors(reader, &instance->IntermediateColors, i, j);
            readFinalMap(reader, &instance->FinalColors, i, j);
            readColorMap(reader, &instance->colorMap, i, j);
        }
    }

    instance->vConstraints = malloc((instance->nRows-1) * sizeof(int));
    for(int i=0; i<instance->nRows-1; i++){

        instance->vConstraints[i] = malloc((instance->nColumns) * sizeof(int));

        for(int j=0; j<instance->nColumns; j++){

            while(fgets(line, sizeof(line), reader)){
                char *keyWordPosition = strstr(line, ("VERTICAL_CONSTRAINTS %d %d", i, j));
                char *commentPosition = strstr(line, "//");
                if(keyWordPosition != NULL){
                    if(commentPosition == NULL || keyWordPosition < commentPosition){
                        break;
                    }
                }
            }
            readVConstraints(reader, &instance->vConstraints, i, j);
        }

    }

    instance->hConstraints = malloc(instance->nRows * sizeof(int));
    for(int i=0; i<instance->nRows; i++){

        instance->hConstraints[i] = malloc((instance -> nColumns-1) * sizeof(int));

        for(int j=0; j<instance->nColumns-1; j++){
            while(fgets(line, sizeof(line), reader)){
                char *keyWordPosition = strstr(line, ("HORIZONTAL_CONSTRAINTS %d %d", i, j));
                char *commentPosition = strstr(line, "//");
                if(keyWordPosition != NULL){
                    if(commentPosition == NULL || keyWordPosition < commentPosition){
                        break;
                    }
                }
            }
            readHConstraints(reader, &instance->hConstraints, i, j);
        }
    }




}
