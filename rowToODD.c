#include "listColoring.h"
#include "odd.h"
#include <stdlib.h>


void rowToODD(LCInstance* instance, int i, ODD* resultingODD);
void addTransitions(LCInstance* instance, int i, ODD* resultingODD);

// Takes an instance of the List Coloring problem and constructs an
// ODD accepting precisely those sequences of colors which are
// horizontally compatible
void rowToODD(LCInstance* instance, int i, ODD* resultingODD) {
}


// TODO: What about final transitions
void addTransitions(LCInstance* instance, int i, ODD* resultingODD) {

    for (int j = 0; j < resultingODD->nLayers; j++) {

        if (j == 0) { //Initial transitions
            resultingODD->layerSequence[j].transitions.nTransitions = instance->IntermediateColors[i][j].sizeAlphabet;
            resultingODD->layerSequence[j].transitions.set = malloc(
                    sizeof(Transition) * resultingODD->layerSequence[j].transitions.nTransitions);

            for (int trans = 0; trans < instance->IntermediateColors[i][j].sizeAlphabet; trans++) {
                Transition transition;
                transition.s1 = 0;
                transition.a = trans;
                transition.s2 = trans;
                resultingODD->layerSequence[j].transitions.set[trans] = transition;
            }
        } else { // All other transitions
            resultingODD->layerSequence[j].transitions.nTransitions = instance->hConstraints[i][j].nConstraints;
            resultingODD->layerSequence[j].transitions.set = malloc(
                    sizeof(Transition) * resultingODD->layerSequence[j].transitions.nTransitions);

            for (int trans = 0; trans < resultingODD->layerSequence[j].transitions.nTransitions; trans++) {
                Transition transition;
                transition.s1 = instance->hConstraints[i][j].pairs[trans].color1;
                transition.a = instance->hConstraints[i][j].pairs[trans].color2;
                transition.s2 = instance->hConstraints[i][j].pairs[trans].color2;
                resultingODD->layerSequence[j].transitions.set[trans] = transition;
            }
        }
    }
}