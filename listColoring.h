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
	ColorPair** pairs; // sorted vectors of constraints
} Constraint; 

typedef struct{
	int nRows;
	int nColumns;
	AlphabetMap** IntermediateColors;
	AlphabetMap** FinalColors;
	ColorMap** colorMap; // Converts IntermediateColors into FinalColors
	Constraint VConstraints;
	Constraint HConstraints;
} LCInstance;


//void readAlphabetMap(FILE* reader, AlphabetMap* map); // Reads nSymbols 
void readConstraint(FILE* reader, Constraint* constraint);
void readLCInstance(char* filename, LCInstance* instance); 

void rowToLevel(LCInstance* instance, int i, ODD* resultingODD); // Takes an instance of the List Coloring problem and constructs an
					     			 // ODD accepting precisely those sequences of colors which are 
								 // horizontally compatible
					      

void nextLayer(LCInstance* instance, int i, int j, ODD* odd, Layer* layer); // Constructs next layer. 


