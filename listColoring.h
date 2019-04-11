// Copyright 2019 Mateus de Oliveira Oliveira
// This file is licensed under MIT License, as specified in the file LISENSE located at the root folder of this repository.

#include "odd.h"

///////////////////// List Coloring Instance Type //////////////

typedef struct{
	int nColors;
	int* map; // This is simply a vector of ints. map[i]=j means that color i in one set of colors is mapped to color j in the other set
} ColorMap; 


typedef struct{
	int color1;
	int color2; 
} ColorPair; 

typedef struct{
	int nConstraints;
	ColorPair* pairs; // sorted vectors of constraints
} Constraint; 

typedef struct{
	int nRows;
	int nColumns;
	AlphabetMap** IntermediateColors;
	AlphabetMap** FinalColors;
	ColorMap** colorMap; // Converts IntermediateColors into FinalColors
	Constraint** vConstraints;
	Constraint** hConstraints;
} LCInstance;

void readIntermediateColors(FILE* reader, AlphabetMap* intermediateColors, int i, int j);
void readFinalMap(FILE* reader, AlphabetMap* finalColors, int i, int j);
void readColorMap(FILE* reader, ColorMap* colorMAp, int i, int j);
void readVConstraints(FILE* reader, Constraint* vConstraints, int i, int j);
void readHConstraints(FILE* reader, Constraint* hConstraints, int i, int j);
void readLCInstance(char* filename, LCInstance* instance); 

void printIntermediateColors(FILE* f, AlphabetMap* intermediateColors, int i, int j);
void printFinalMap(FILE* f, AlphabetMap* finalColors, int i, int j);
void printColorMap(FILE* f, ColorMap* colorMap, int i, int j);
void printVConstraints(FILE* f, Constraint* vConstraints, int i, int j);
void printHConstraints(FILE* f, Constraint* hConstraints, int i, int j);
void printLCInstance(char* filename, LCInstance* instance); 


void rowToODD(LCInstance* instance, int i, ODD* resultingODD); // Takes an instance of the List Coloring problem and constructs an
					     			 // ODD accepting precisely those sequences of colors which are 
								 // horizontally compatible
					      

Layer* nextLayer(LCInstance *instance, int i, int j, Layer** m); // Constructs next layer. 
Layer* nextIncrement(LCInstance *instance, Layer *left, Layer *up, int i, int j);
