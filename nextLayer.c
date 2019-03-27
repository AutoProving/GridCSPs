#include <stdlib.h>
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
        if (layer->transitions.set[i].s2 == l)
        {
            return layer->transitions.set[i].a;
        }
    }

    exit(-1);
}

void nextIncrement(LCInstance *instance, Layer *left, Layer *up, Layer *result, int i, int j)
{
    result->leftStates = left->rightStates;
    result->rightStates = up->rightStates;
    result->initialStates = left->initialStates;
    result->finalStates = up->finalStates;
    result->initialFlag = left->initialFlag;
    result->finalFlag = up->finalFlag;
    result->map = instance->IntermediateColors[i][j];

    result->transitions.nTransitions = 0;

    //assume i != 0.

    //Assume this is the size for now
    result->transitions.set = malloc(up->transitions.nTransitions);

    for (int transIndex = 0; transIndex < up->transitions.nTransitions; transIndex++) {

        NumSymbol a = up->transitions.set[transIndex].a;

        for (int colorIndex = 0; colorIndex < instance->IntermediateColors[i][j].sizeAlphabet; colorIndex++) {

            NumSymbol b = instance->IntermediateColors[i][j].S2N[colorIndex];

            for (int vertical = 0; vertical < instance->vConstraints[i-1][j].nConstraints; vertical++) {
                if (instance->vConstraints[i-1][j].pairs[vertical].color1 == a && instance->vConstraints[i-1][j].pairs[vertical].color2 == b) {
                    
                    if (j == 0) {
                        result->transitions.set[result->transitions.nTransitions].s1 = up->transitions.set[transIndex].s1;
                        result->transitions.set[result->transitions.nTransitions].a = b;
                        result->transitions.set[result->transitions.nTransitions].s2 = up->transitions.set[transIndex].s2;
                        result->transitions.nTransitions++;
                    } else {
                        NumSymbol m = memo(up->transitions.set[transIndex].s1, left);
                        for (int horisontal = 0; horisontal < instance->hConstraints[i][j-1].nConstraints; horisontal++) {
                            if (instance->hConstraints[i][j-1].pairs[horisontal].color1 == m && instance->hConstraints[i][j-1].pairs[horisontal].color2 == b) {
                                result->transitions.set[result->transitions.nTransitions].s1 = up->transitions.set[transIndex].s1;
                                result->transitions.set[result->transitions.nTransitions].a = b;
                                result->transitions.set[result->transitions.nTransitions].s2 = up->transitions.set[transIndex].s2;
                                result->transitions.nTransitions++;
                            }
                        }
                    }   
                }   
            }
        }
    }
}
