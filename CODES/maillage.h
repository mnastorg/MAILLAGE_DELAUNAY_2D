#ifndef MAILLAGE
#define MAILLAGE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int nbP(FILE* mesh);
int nbT(FILE* mesh);
int nbE(FILE* mesh);
void remplissage(FILE* mesh,double* tabP, int* tabT, int* tabE );
long rechercherPoints(FILE* mesh);
long rechercherTriangles(FILE* mesh);
long rechercherEdges(FILE* mesh);

#endif
