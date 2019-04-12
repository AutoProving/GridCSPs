// Copyright 2019 Markus Ragnhildstveit, Amund Lindberg, Ole Magnus Morken, Guillermo Hoyo Bravo and Josep Barbera Muñoz
// This file is licensed under MIT License, as specified in the file LISENSE located at the root folder of this repository.

#include <stdlib.h>
#include <math.h>
#include "listColoring.h"

NumSymbol memo(State l, Layer *layer);
Layer *nextIncrement(LCInstance *instance, Layer *left, Layer *up, int i, int j);
void copyStates(StateContainer *copyTo, StateContainer *copyFrom);
void addTransitionsToLayer(LCInstance *instance, Layer *result, Layer *left, Layer *up, int i, int j);
void addTransitionsByVAndHConstraints(Layer *result, Layer *left, Transition *upT, Constraint *vC, Constraint *hC, NumSymbol b);

Layer *nextLayer(LCInstance *instance, int i, int j, Layer **m)
{
    if (j == 0)
    {
        return nextIncrement(instance, NULL, &m[i - 1][j], i, j);
    }
    else
    {
        return nextIncrement(instance, &m[i][j - 1], &m[i - 1][j], i, j);
    }
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

Layer *nextIncrement(LCInstance *instance, Layer *left, Layer *up, int i, int j)
{
    Layer *result = (Layer *)malloc(sizeof(Layer));
    if (left != NULL) // j != 0
    {
        copyStates(&result->leftStates, &left->rightStates);
        result->initialStates.nStates = 0;
        result->initialFlag = 0;
    }
    else // j == 0
    {
        result->leftStates.nStates = 1;
        result->initialStates.nStates = 1;
        result->leftStates.set = calloc(1, sizeof(int));
        result->initialStates.set = calloc(1, sizeof(int));
        result->initialFlag = 1;
    }
    result->width = fmax(result->leftStates.nStates, result->rightStates.nStates);

    copyStates(&result->rightStates, &up->rightStates);
    copyStates(&result->finalStates, &up->finalStates);
    result->finalFlag = up->finalFlag;
    result->map = instance->IntermediateColors[i][j];

    addTransitionsToLayer(instance, result, left, up, i, j);
    // result->transitions.nTransitions = 0;

    //assume i != 0.

    return result;
}

void addTransitionsToLayer(LCInstance *instance, Layer *result, Layer *left, Layer *up, int i, int j)
{
    result->transitions.nTransitions = 0;
    //assume i != 0.

    //Assume this is the size for now
    result->transitions.set = malloc(up->transitions.nTransitions * sizeof(Transition));
    Constraint *vC = &instance->vConstraints[i - 1][j];
    Constraint *hC = (j == 0 ? NULL : &instance->hConstraints[i][j - 1]);

    // Check for all transitions in up
    for (int transIndex = 0; transIndex < up->transitions.nTransitions; transIndex++)
    {
        for (int colorIndex = 0; colorIndex < instance->IntermediateColors[i][j].sizeAlphabet; colorIndex++)
        {
            NumSymbol b = instance->IntermediateColors[i][j].S2N[colorIndex];
            addTransitionsByVAndHConstraints(result, left, &up->transitions.set[transIndex], vC, hC, b);
        }
    }
}

void addTransitionsByVAndHConstraints(Layer *result, Layer *left, Transition *upT, Constraint *vC, Constraint *hC, NumSymbol b)
{
    NumSymbol a = upT->a;

    for (int vertical = 0; vertical < vC->nConstraints; vertical++)
    {
        if (vC->pairs[vertical].color1 == a && vC->pairs[vertical].color2 == b)
        {

            if (hC == NULL)
            {
                result->transitions.set[result->transitions.nTransitions].s1 = upT->s1;
                result->transitions.set[result->transitions.nTransitions].a = b;
                result->transitions.set[result->transitions.nTransitions].s2 = upT->s2;
                result->transitions.nTransitions++;
            }
            else
            {
                NumSymbol m = memo(upT->s1, left);
                for (int horisontal = 0; horisontal < hC->nConstraints; horisontal++)
                {
                    if (hC->pairs[horisontal].color1 == m && hC->pairs[horisontal].color2 == b)
                    {
                        result->transitions.set[result->transitions.nTransitions].s1 = upT->s1;
                        result->transitions.set[result->transitions.nTransitions].a = b;
                        result->transitions.set[result->transitions.nTransitions].s2 = upT->s2;
                        result->transitions.nTransitions++;
                    }
                }
            }
        }
    }
}

void copyStates(StateContainer *copyTo, StateContainer *copyFrom)
{
    copyTo->nStates = copyFrom->nStates;
    copyTo->set = malloc(copyTo->nStates * sizeof(State));
    for (int i = 0; i < copyFrom->nStates; i++)
    {
        copyTo->set[i] = copyFrom->set[i];
    }
}

// void copyAlphabetMap(AlphabetMap *copyTo, AlphabetMap *copyFrom)
// {
//     copyTo->sizeAlphabet = copyFrom->sizeAlphabet;
//     for(int i = 0; i < copyFrom->nStates; i++)
//     {
//         copyTo->set[i] = copyFrom->set[i];
//     }
// }