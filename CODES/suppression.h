#ifndef SUPPRESSION
#define SUPPRESSION
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "maillage.h"
# include "cercle.h"

typedef struct Element Element;
struct Element
{
    int nombre;
    Element *suivant;
};

typedef struct File File;
struct File
{
    Element *premier;
};


File *initialiser();
void afficherFile(File *file);
int defiler(File *file);
void enfiler(File *file, int numeroTriangle);
void suppression_triangles(int **tabT,int **tabAr,double** tabP,double *tabP_init,FILE* mesh_init,FILE* mesh_delaunay,int nbEdges,int *tabE);
int arete_comune(int *tab_Triangles_ref,int nbT1,int nbT2,int nbEdges,int *tabE);
int ref_non_rempl(int *tab_Triangles_ref,int nbTriangles,File *maFile);
int recherche_zone2(int *tab_Triangles_ref,int nbTriangles);
int  modification_arete(int **tabTri,int **TabAr,int nbTriangles);
int recherche_zone_conserver(int *tab_Triangles_ref,int nbTriangles,int numero_zone);


#endif
