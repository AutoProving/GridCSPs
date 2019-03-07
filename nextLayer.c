#include "listColoring.h"

void nextLayer(LCInstance *instance, int i, int j, ODD *odd, Layer *layer)
{
}

//return symbol a iff every transition leading to state l at the right frontier of layer is equal to a
//else error/exception
NumSymbol memo(State l, Layer *layer)
{

    NumSymbol a;
    int first = 1;

    for (int i = 0; i < layer->transitions.nTransitions; i++)
    {
        if (first && layer->transitions.set[i].s2 == l)
        {
            a = layer->transitions.set[i].a;
        }
        else if (layer->transitions.set[i].s2 == l && a != layer->transitions.set[i].a)
        {
            exit(-1);
        }
    }

    return a;
}

void nextIncrement(LCInstance *instance, Layer *left, Layer *up, Layer *result, AlphabetMap **map, int i, int j)
{
    result->leftStates = left->leftStates;
    result->rightStates = up->rightStates;
    result->initialStates = left->initialStates;
    result->finalStates = up->finalStates;
    result->initialFlag = up->initialFlag;
    result->finalFlag = up->finalFlag;
    result->map = map[i][j];

    result->transitions.nTransitions = 0;

    //Assume this is the size for now
    result->transitions.set = malloc(up->transitions.nTransitions * left->transitions.nTransitions);

    //result->transitions = {(l, c, r): (memo(l ,left), c) element of H_(i, j-1) exists an (l', b, r')
    //element over over.transitions.set such that (b, c) element of V_(i-1, j)
    for (int y = 0; y < left->leftStates.nStates; y++)
    {
        State l = left->leftStates.set[y];
        NumSymbol m = memo(l, left);

        //First find for which index H_(i, j-1) is equal to memo(l, left).
        for (int x = 0; x < instance->hConstraints[i][j - 1].nConstraints; x++)
        {
            ColorPair ph = instance->hConstraints[i][j - 1].pairs[x];

            // Found c
            if (ph.color1 == m)
            {

                //Find (l', b, r') in over.transitions.set such that (b, c) is in V_(i-1, j)
                for (t = 0; t < up->transitions.nTransitions; t++)
                {

                    //Then find the symbol equal to the second element
                    for (int z = 0; z < instance->vConstraints[i - 1][j].nConstraints; z++)
                    {
                        ColorPair pv = instance->vConstraints[i - 1][j].pairs[z];

                        //FOUND b
                        if (pv.color2 == ph.color2)
                        {
                            result->transitions.set[result->transitions.nTransitions].s1 = l;                         //?
                            result->transitions.set[result->transitions.nTransitions].a = ph.color2;                  //?
                            result->transitions.set[result->transitions.nTransitions].s2 = up->transitions.set[t].s2; // not sure
                            ++result->transitions.nTransitions;
                        }
                    }
                }
            }
        }
    }
}

/*
typedef struct{
	int nRows;
	int nColumns;
	AlphabetMap** IntermediateColors;
	AlphabetMap** FinalColors;
	ColorMap** colorMap; // Converts IntermediateColors into FinalColors
	Constraint** vConstraints;
	Constraint** hConstraints;
} LCInstance;

typedef struct{
	int color1;
	int color2; 
} ColorPair; 

typedef struct{
	int nConstraints;
	ColorPair* pairs; // sorted vectors of constraints
} Constraint; 
*/
