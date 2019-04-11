#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <omp.h>
#include "odd.h"
#include "listColoring.h"

ODD* cleanIncompatible(LCInstance* grid, NumSymbol* path, int i, ODD* odd){
    
    for (int j=0; j<grid->nColumns;j++){
    	TransitionContainer *transitions= &odd->layerSequence[j].transitions;
    	Constraint *constraints = &grid->vConstraints[i][j];
    	
    	Transition* newSet = malloc(transitions->nTransitions * sizeof(Transition));
    	int count = 0;

    	for (int t=0; t<transitions->nTransitions;t++){
    		Transition *current = &transitions->set[t]; 
    		ColorPair *check;
			check->color1= current->a;
			check->color2= path[j];

    		if(inConstraints(check,constraints)){
    			count++;
    			newSet[count]=*current;
    		}
    		//free(transitions->set[t]); not sure how to free old set of transitions
    	}
    	transitions->nTransitions=count;
    	transitions->set=newSet;
    }
    return odd;
}

bool inConstraints(ColorPair *check,Constraint *constraints){
	int low=0;
	int high = constraints->nConstraints;
	int middle =(high-low)/2;
	while(true){

        int comparison = (constraints->pairs[middle].color1)-(check->color1);

        if(comparison == 0){
            return true;
        }
        else if(low >= high){
            return false;
        }
        else if(comparison < 0){
            low = middle+1;
        }
        else{
            high =middle-1;
        }
        middle =(high-low)/2;
    }

}