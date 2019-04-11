#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "listColoring.h"

NumSymbol** reconstructSolution(LCInstance* lci)
{
    ODD* L;

    if (isThereSolution(lci, L))
    {
        ODD aux = L[lci->nRows-1];
        NumSymbol** M = (NumSymbol**)malloc(lci->nRows);
        M[lci->nRows-1] = (NumSymbol*)malloc(lci->nColumns); 
        M[lci->nRows-1] = getPath(&L[lci->nRows-1]);
        for (int i = lci->nRows-2; i >= 0; i--)
        {
            ODD aux = cleanIncompatible(&L[i], M[i+1], i);
            M[i] = (NumSymbol*)malloc(lci->nColumns);
        }
        return M;
    }
    
    return 0;


}
