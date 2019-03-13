#include "listColoring.h"

void nextLayer(LCInstance *instance, int i, int j, ODD *odd, Layer *layer)
{
}

//return symbol a iff every transition leading to state l at the right frontier of layer is equal to a,
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

void nextIncrement(LCInstance *instance, Layer *left, Layer *up, Layer *result, int i, int j)
{
    result->leftStates = left->rightStates;
    result->rightStates = up->rightStates;
    result->initialStates = left->initialStates;
    result->finalStates = up->finalStates;
    result->initialFlag = left->initialFlag;
    result->finalFlag = up->finalFlag;
    result->map = instance->FinalColors[i][j]; // ?
    

    result->transitions.nTransitions = 0;

    //Assume this is the size for now
    result->transitions.set = malloc(up->transitions.nTransitions * left->transitions.nTransitions);

    for(int x = 0; x < instance->hConstraints[i][j-1].nConstraints; x++)
    {
        NumSymbol a = instance->hConstraints[i][j-1].c2;
        for(int y = 0; y < instance->vConstraints[i-1][j].nConstraints; y++)
        {
            NumSymbol b = instance->vConstraints[i-1][j].c2;
            if (a == b) {
                for(int z = 0; z < up->transitions.nTransitions; z++)
                {
                    if (b == up->transitions.set[z].a) {
                        for(int w = 0; w < left->transitions.nTrasitions; w++)
                        {
                            if (left->transitions.set[w].s1 == up->transitions.set[w].s1
                                    && left->transitions.set[w].s2 == up->transitions.set[w].s2
                                    && left->transitions.set[w].a == instance->hConstraints[i][j-1].c1) 
                            {
                                result->transitions.set[result->transitions.nTransitions].s1 = memo(instance->hConstraints[i][j-1].c1); //???
                                result->transitions.set[result->transitions.nTransitions].s2 = up->transitions.set[w].s2;
                                 result->transitions.set[result->transitions.nTransitions].a = b;
                            }
                            
                        }
                        
                    }
                    
                }
                
            }
        
            
        }
        
    }




    //result->transitions = {(l, c, r): (memo(l ,left), c) element of H_(i, j-1) exists an (l', b, r')
    //element over over.transitions.set such that (b, c) element of V_(i-1, j)
    // for (int y = 0; y < left->leftStates.nStates; y++)
    // {
    //     State l = left->leftStates.set[y];
    //     NumSymbol m = memo(l, left);

    //     //First find for which index H_(i, j-1) is equal to memo(l, left).
    //     for (int x = 0; x < instance->hConstraints[i][j - 1].nConstraints; x++)
    //     {
    //         ColorPair ph = instance->hConstraints[i][j - 1].pairs[x];

    //         // Found c
    //         if (ph.color1 == m)
    //         {
    //             int c = ph.color2;
    //             int notfoundB = 1;
    //             //Find (l', b, r') in over.transitions.set such that (b, c) is in V_(i-1, j)
    //             for (int t = 0; notfoundB && t < up->transitions.nTransitions; t++)
    //             {

    //                 //Then find the symbol equal to the second element
    //                 for (int z = 0; notfoundB && z < instance->vConstraints[i - 1][j].nConstraints; z++)
    //                 {
    //                     ColorPair pv = instance->vConstraints[i - 1][j].pairs[z];

    //                     //FOUND b
    //                     if (pv.color2 == c)
    //                     {
    //                         result->transitions.set[result->transitions.nTransitions].s1 = l;                           //?
    //                         result->transitions.set[result->transitions.nTransitions].a = c;                            //?
    //                         result->transitions.set[result->transitions.nTransitions].s2 = up->transitions.set[t].s2;   // not sure
    //                         ++result->transitions.nTransitions;
    //                         notfoundB = 0;
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }
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
