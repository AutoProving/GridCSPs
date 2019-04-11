#include <stdio.h>
#include <stdbool.h>
#include "listColoring.h"
// #include "isThereSolution.c"

int main(){
    // printf() displays the string inside quotation
    // nextLayer()
    LCInstance inst;
    readLCInstance("listColoringExample.tex", &inst);
    
    ODD firstRow; 
    rowToODD(&inst, 0, &firstRow);
    
    Layer *t1 = nextIncrement(&inst, NULL, &firstRow.layerSequence[0], 1, 0);
    Layer *t2 = nextIncrement(&inst, t1, &firstRow.layerSequence[0], 1, 1);
    

    printLCInstance("listColoringExample.txt", &inst);

    printf("hello\n");
    return 0;
}
