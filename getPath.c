#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "listColoring.h"

NumSymbol *getPath(ODD odd)
{
    NumSymbol *path = malloc(odd.nLayers * sizeof(NumSymbol));
    int numLayers = odd.nLayers - 1;
    int pathIndex = numLayers;
    Layer finalLayer = odd.layerSequence[numLayers];
    int lastState;

    for (int finalstate = 0; finalstate < finalLayer.finalStates.nStates; finalstate++)
    {
        for (int j = 0; j < finalLayer.transitions.nTransitions; j++)
        {
            if (finalLayer.transitions.set[j].s2 == finalLayer.finalStates.set[finalstate])
            {
                path[pathIndex] = finalLayer.transitions.set[j].a;
                lastState = finalLayer.transitions.set[j].s1;
                break;
            }
        }

        for (int i = numLayers; i > 0; i--)
        {
            pathIndex -= 1;
            Layer previousLayer = odd.layerSequence[i];
            int numTransitions = previousLayer.transitions.nTransitions;
            for (int j = 0; j < numTransitions; j++)
            {
                if (previousLayer.transitions.set[j].s2 == lastState)
                {
                    path[pathIndex] = previousLayer.transitions.set[j].a;
                    lastState = previousLayer.transitions.set[j].s1;
                    break;
                }
            }
            continue;
        }
        break;
    }
    return path;
}