#include "listColoring.h"
#include <stdio.h>
#include <stdlib.h>


void printIntermediateColors(FILE* f, AlphabetMap* intermediateColors, int i, int j)
{
	for (int colorIndex = 0; colorIndex < intermediateColors->sizeAlphabet; colorIndex++)
	{
		fprintf(f, "%d %s\n", intermediateColors->S2N[colorIndex], intermediateColors->N2S[colorIndex]);
	}
}

void printFinalMap(FILE* f, AlphabetMap* finalColors, int i, int j)
{
	for (int colorIndex = 0; colorIndex < finalColors->sizeAlphabet; colorIndex++)
	{
		fprintf(f, "%d %s\n", finalColors->S2N[colorIndex], finalColors->N2S[colorIndex]);
	}
}

void printColorMap(FILE* f, ColorMap* colorMap, int i, int j)
{	
	for (int colorIndex = 0; colorIndex < colorMap->nColors; colorIndex++)
	{
		fprintf(f, "%d %d\n", colorIndex, colorMap->map[colorIndex]);
	}
}

void printVConstraints(FILE* f, Constraint* vConstraints, int i, int j)
{	
	for (int constraintIndex = 0; constraintIndex < vConstraints->nConstraints; constraintIndex++)
	{
		fprintf(f, "%d %d\n", vConstraints->pairs[constraintIndex].color1, vConstraints->pairs[constraintIndex].color2);
	}
}

void printHConstraints(FILE* f, Constraint* hConstraints, int i, int j)
{
	for (int constraintIndex = 0; constraintIndex < hConstraints->nConstraints; constraintIndex++)
	{
		fprintf(f, "%d %d\n", hConstraints->pairs[constraintIndex].color1, hConstraints->pairs[constraintIndex].color2);
	}
}

void printLCInstance(char* filename, LCInstance* instance)
{
	FILE* f = fopen(filename, "w");
	if (f == NULL)
	{
		printf("Error opening the file!\n");
		exit(1);
	}
	fprintf(f, "LIST_COLORING %d %d\n", instance->nRows, instance->nColumns);

	int index = 0;
	for (int i = 0; i < instance->nRows; i++)
	{
		for(int j = 0; j < instance->nColumns; j++)
		{
			fprintf(f, "\nCOLOR_LISTS %d %d\nINTERMEDIATE_COLORS %d\n", i, j, instance->IntermediateColors[i][j].sizeAlphabet);
			printIntermediateColors(f, &instance->IntermediateColors[i][j], i, j);
			fprintf(f, "FINAL_COLORS %d\n", instance->FinalColors[i][j].sizeAlphabet);
			printFinalMap(f, &instance->FinalColors[i][j], i, j);
			fprintf(f, "COLOR_MAP %d\n", instance->colorMap[i][j].nColors);
			printColorMap(f, &instance->colorMap[i][j], i, j);
		}	
	}
    fprintf(f, "%s", "\n");
	//Vertical Constraints
	index = 0;
	for (int i = 0; i < instance->nRows - 1; i++)
	{
		for (int j = 0; j < instance->nColumns; j++)
		{
			fprintf(f, "VERTICAL_CONSTRAINTS %d %d\n", i, j);
			printVConstraints(f, &instance->vConstraints[i][j], i, j);
		}
            index++;
	}

    index = 0; 
    fprintf(f, "%s", "\n");
	//Horizontal Constraints	
	for (int i = 0; i < instance->nRows; i++)
	{
		for (int j = 0; j < instance->nColumns - 1; j++)
		{		
			fprintf(f, "HORIZONTAL_CONSTRAINTS %d %d\n", i, j);
			printHConstraints(f, &instance->hConstraints[i][j], i, j);
		}
        index++;
	}



}



