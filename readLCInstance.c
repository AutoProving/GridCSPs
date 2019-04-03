// Copyright 2019 Haavard Notland
// This file is licensed under MIT License, as specified in the file LISENSE located at the root folder of this repository.


#include <stdlib.h>
#include <string.h>
#include "listColoring.h"

//Rewrites file to not contain any comments
void removeComments(char* filename){
    FILE *reader = fopen(filename, "r");
    FILE *writer = fopen("commentsRemoved.txt", "w");
    char line[256];
    int position;

    while(fgets(line, sizeof(line), reader)){
        char *commentPosition = strstr(line, "//");

        //Remove blank lines or line with only comments
        if(strlen(line) == 1 || commentPosition == line){
            continue;
        }

        //Remove comment but keep line to be read
        if(commentPosition != NULL){
            position = line - commentPosition;
            fprintf(writer, "%.*s\n", abs(position), line);
        }
        //No comment, print whole line
        else{
            fprintf(writer, "%s",  line);
        }
    }

    int _ = fclose(reader);
    _ = fclose(writer);

}

void readIntermediateColors(FILE* reader, AlphabetMap* intermediateColors, int i, int j){

    char line[256], c[32];
    while(fgets(line, sizeof(line), reader)) {
        char *keywordPosition = strstr(line, "INTERMEDIATE_COLORS");

        if(keywordPosition != NULL){
            break;
        }
    }

    //??? Bind read integer to sizeAlphabet
    sscanf(line, "%s %d", c, &intermediateColors->sizeAlphabet);

    intermediateColors->S2N = malloc(intermediateColors->sizeAlphabet * sizeof(int));
    intermediateColors->N2S = malloc(intermediateColors->sizeAlphabet * sizeof(char*));

    for(int k=0; k< intermediateColors->sizeAlphabet; k++){
        while(fgets(line, sizeof(line), reader)){
               intermediateColors->N2S[k] = malloc(sizeof(line));
               sscanf(line, "%d %s", &intermediateColors->S2N[k], intermediateColors->N2S[k]);
               break;

        }
    }


}

void readFinalMap(FILE* reader, AlphabetMap* finalColors, int i, int j){

    char line[256], c[32];
    while(fgets(line, sizeof(line), reader)) {
        char *keywordPosition = strstr(line, "FINAL_COLORS");

        if(keywordPosition != NULL){
            break;
        }
    }

    //??? bind read integer to sizeAlphabet
    sscanf(line, "%s %d", c, &finalColors->sizeAlphabet);

    finalColors->S2N = malloc(finalColors->sizeAlphabet * sizeof(int));
    finalColors->N2S = malloc(finalColors->sizeAlphabet * sizeof(char*));

    for(int k=0; k< finalColors->sizeAlphabet; k++){
        while(fgets(line, sizeof(line), reader)){
                finalColors->N2S[k] = malloc(sizeof(line));
                sscanf(line, "%d %s", &finalColors->S2N[k], finalColors->N2S[k]);
                break;

        }
    }


}

void readColorMap(FILE* reader, ColorMap* colorMap, int i, int j){

    char line[256], c[32];
    while(fgets(line, sizeof(line), reader)) {
        char *keywordPosition = strstr(line, "COLOR_MAP");

        if(keywordPosition != NULL){
            break;
        }
    }

    //??? Bind read integer to nColors
    sscanf(line, "%s %d", c, &colorMap->nColors);

    colorMap->map = malloc(colorMap->nColors * sizeof(int));
    int m, n;
    for(int k=0; k<colorMap->nColors; k++){
        while(fgets(line, sizeof(line), reader)){
                sscanf(line, "%d %d", &m, &n);
                colorMap->map[m] = n;
                break;

        }
    }



}

void readVConstraints(FILE* reader, Constraint* vConstraints, int i, int j){
    char line[256], c[32];
    int m, n;
    int nrOfConstraints = 1;

    vConstraints->pairs = malloc(sizeof(ColorPair));

    while(fgets(line, sizeof(line), reader)) {

        char *keyWordPosition = strstr(line, "CONSTRAINTS");
        if (keyWordPosition == NULL) {
            break;
        }
    }

    sscanf(line, "%d %d", &m, &n);
    vConstraints->pairs[nrOfConstraints-1].color1 = m;
    vConstraints->pairs[nrOfConstraints-1].color2 = n;

    while(fgets(line, sizeof(line), reader)) {

        char *keyWordPosition = strstr(line, "CONSTRAINTS");
        if (keyWordPosition != NULL) {
            break;
        }

        sscanf(line, "%d %d", &m, &n);
        nrOfConstraints++;
        ColorPair* tmp = realloc(vConstraints->pairs, nrOfConstraints * sizeof(ColorPair));
        vConstraints->pairs = tmp;
        vConstraints->pairs[nrOfConstraints-1].color1 = m;
        vConstraints->pairs[nrOfConstraints-1].color2 = n;
    }
    vConstraints->nConstraints = nrOfConstraints;

}

void readHConstraints(FILE* reader, Constraint* hConstraints, int i, int j){
    char line[256], c[32];
    int m, n;
    int nrOfConstraints = 1;

    hConstraints->pairs = malloc(sizeof(ColorPair));

    while(fgets(line, sizeof(line), reader)) {

        char *keyWordPosition = strstr(line, "CONSTRAINTS");
        if (keyWordPosition == NULL) {
            break;
        }
    }

    sscanf(line, "%d %d", &m, &n);
    hConstraints->pairs[nrOfConstraints-1].color1 = m;
    hConstraints->pairs[nrOfConstraints-1].color2 = n;

    while(fgets(line, sizeof(line), reader)) {

        char *keyWordPosition = strstr(line, "CONSTRAINTS");
        if (keyWordPosition != NULL) {
            break;
        }

        sscanf(line, "%d %d", &m, &n);
        nrOfConstraints++;
        ColorPair* tmp = realloc(hConstraints->pairs, nrOfConstraints * sizeof(ColorPair));
        hConstraints->pairs = tmp;
        hConstraints->pairs[nrOfConstraints-1].color1 = m;
        hConstraints->pairs[nrOfConstraints-1].color2 = n;
    }
    hConstraints->nConstraints = nrOfConstraints;

}

void readLCInstance(char* filename, LCInstance* instance){

    removeComments(filename);
    FILE *reader = fopen("commentsRemoved.txt", "r");
    char line[64], c[128], needle[64];

    while(fgets(line, sizeof(line), reader)){
        char *keywordPosition = strstr(line, "LIST_COLORING");

        if(keywordPosition != NULL){
            break;
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

                if(keyWordPosition != NULL){
                    break;
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

    instance->vConstraints = malloc((instance->nRows-1) * sizeof(Constraint));
    while(fgets(line, sizeof(line), reader)){
                char *keyWordPosition = strstr(line, "VERTICAL_CONSTRAINTS");

                if(keyWordPosition != NULL){
                    break;
                }
            }
    for(int i=0; i<instance->nRows-1; i++){

        instance->vConstraints[i] = malloc((instance->nColumns) * sizeof(Constraint));

        for(int j=0; j<instance->nColumns; j++){

            readVConstraints(reader, &(instance->vConstraints[i][j]), i, j);

        }

    }

    instance->hConstraints = malloc(instance->nRows * sizeof(Constraint));

    for(int i=0; i<instance->nRows; i++){

        instance->hConstraints[i] = malloc((instance->nColumns-1) * sizeof(Constraint));

        for(int j=0; j<instance->nColumns-1; j++){

            readHConstraints(reader, &(instance->hConstraints[i][j]), i, j);

        }

    }

}
