#include "listColoring.h"
#include "odd.h"
#include <stdlib.h>


void rowToODD(LCInstance* instance, int i, ODD* resultingODD);
void addTransitions(LCInstance* instance, int i, ODD* resultingODD);
void addTransitionsPerLayer(LCInstance* instance, int i, int j, ODD* resultingODD);


// Takes an instance of the List Coloring problem and constructs an
// ODD accepting precisely those sequences of colors which are
// horizontally compatible
void rowToODD(LCInstance* instance, int i, ODD* resultingODD) {
    addTransitions(instance, i, resultingODD);
}

void addTransitions(LCInstance* instance, int i, ODD* resultingODD) {
    for (int j = 0; j < resultingODD->nLayers; j++) {
        addTransitionsPerLayer(instance, i, j, resultingODD);
    }
}


// TODO: What about final and init transitions
void addTransitionsPerLayer(LCInstance* instance, int i, int j, ODD* resultingODD) {
    if (j == 0) { //Initial transitions goes from 0
        resultingODD->layerSequence[j].transitions.nTransitions = instance->IntermediateColors[i][j].sizeAlphabet;
        resultingODD->layerSequence[j].transitions.set = malloc(
                sizeof(Transition) * resultingODD->layerSequence[j].transitions.nTransitions);

        for (int trans = 0; trans < instance->IntermediateColors[i][j].sizeAlphabet; trans++) {
            resultingODD->layerSequence[j].transitions.set[trans].s1 = 0;
            resultingODD->layerSequence[j].transitions.set[trans].a = trans;
            resultingODD->layerSequence[j].transitions.set[trans].s2 = trans;
        }
    } else { // All other transitions
        resultingODD->layerSequence[j].transitions.nTransitions = instance->hConstraints[i][j].nConstraints;
        resultingODD->layerSequence[j].transitions.set = malloc(
                sizeof(Transition) * resultingODD->layerSequence[j].transitions.nTransitions);

        for (int trans = 0; trans < resultingODD->layerSequence[j].transitions.nTransitions; trans++) {
            resultingODD->layerSequence[j].transitions.set[trans].s1 = instance->hConstraints[i][j].pairs[trans].color1;
            resultingODD->layerSequence[j].transitions.set[trans].a = instance->hConstraints[i][j].pairs[trans].color2;
            resultingODD->layerSequence[j].transitions.set[trans].s2 = instance->hConstraints[i][j].pairs[trans].color2;
        }
    }
}