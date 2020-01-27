#ifndef CERCLE
#define CERCLE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_inscritm(double *p1,double *p2,double *p3,double *p4);
void init(double **TabVoisin,int nbTriangles);
void rtabVoisin(int *TabTri,double **TabVoisin,int nbTriangles);
int verifDansLeTriangle(double *p1,double *p2,double *p3,double *p4,double *CoordB);
int QTriangle(int *TabTri,int nbTriangles,double *Pt, double *TabPt, double **TabVoisin);
int LCirconscrit(int **Lc,int *TabTri,double *TabPt,int nbTriangles,double *Pt);
int Maj(int **Lc,int *TabTri,double *TabPt,int nbTriangles,double *Pt,int **ptcaviteOrdo);
void newT(int **TabTri,double **TabPt,int **Tabarete,double *Pt);
//void ajout_premier_point(int **TabTri,double **TabPt,int **Tabarete,double *Pt);
void ajout_pt(int **TabTri,double **TabPt,int **Tabarete,double *TabP,double *Pt);

//refaire la modif de l'ajout de mesh sur la fonction newT
//int testOrdo(int **Lc,int *TabTri,double *TabPt,int nbTriangles,double *Pt,int **ptcaviteOrdo,int TailleLc);
int verif(int *aretecou,int *areteinit,int aretesTaille);
int testOrdo(int **Lc,int *TabTri,int nbTriangles,int TailleLc,int **arete_cavite2);


#endif
