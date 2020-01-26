#include "maillage.h"

//-------------------------------------------------------------------------------------
//----------- FONCTIONS POUR CHERCHER LE NOMBRE D ELEMENTS DANS LE FICHIER ------------
//-------------------------------------------------------------------------------------


//RETOURNE NOMBRE DE POINTS DU FICHIER MESH BORD COMPRIS
int nbP(FILE* mesh)
{
	int nbpoints = 0;
	long debut_donne = 0;
	char texte[200] = {0};
	debut_donne = rechercherPoints(mesh); //on place le curseur au bon endroit "Vertices"
	fgets(texte,200,mesh);
	nbpoints = atoi(texte);
	return nbpoints;
}

//RETOURNE NOMBRE DE TRIANGLES DU FICHIER MESH
int nbT(FILE* mesh)
{
	int nbtriangles = 0;
	long debut_donne = 0;
	char texte[200] = {0};
	debut_donne = rechercherTriangles(mesh); //on place le curseur au bon endroit "Triangles"
	fgets(texte,200,mesh);
	nbtriangles = atoi(texte);
	return nbtriangles;

}

//RETOURNE LE NOMBRE D'ARETES DU FICHIER MESH
int nbE(FILE* mesh)
{
	int nbedges = 0;
	long debut_donne = 0;
	char texte[200] = {0};
	debut_donne = rechercherEdges(mesh); //on place le curseur au bon endroit "Edges"
	fgets(texte,200,mesh);
	nbedges = atoi(texte);
	return nbedges;

}

//-------------------------------------------------------------------------------------
//----------- FONCTIONS POUR REMPLIR LES TABLEAUX POINTS / TRIANGLES / ARETES ---------
//-------------------------------------------------------------------------------------

void remplissage(FILE* mesh, double* tabP, int* tabT, int* tabE )
{
	long debut_donne = 0;
	char texte[200] = {0};
	int nbPoints = 0, nbTriangles = 0,nbEdges = 0;

	//REMPLISSAGE TABLEAU DE POINTS (ON LES ENTRE 2 PAR 2)
	debut_donne = rechercherPoints(mesh);
	fgets(texte,200,mesh);
	nbPoints = atoi(texte);
	double *varP = NULL;
	varP = calloc(3,sizeof(double));
	for(int i=0; i<2.*nbPoints; i+=2)
	{
		fscanf(mesh,"%le %le %le",&varP[0],&varP[1],&varP[2]);
		tabP[i] = varP[0];
		tabP[i+1] = varP[1];
		//long position=ftell(mesh);
		//fseek(mesh,1,SEEK_CUR);
	}
	free(varP);

	//ON SE REPLACE AU DEBUT DU FICHIER
	fseek(mesh,0,SEEK_SET);

	//REMPLISSAGE DU TABLEAU DES TRIANGLES (ON LES ENTRE 3 PAR 3)
	double *varT = NULL;
	varT = calloc(4,sizeof(double));
	debut_donne = rechercherTriangles(mesh);
	fgets(texte,200,mesh);
	nbTriangles = atoi(texte);
	//free(var);
	for(int i=0; i<3.*nbTriangles; i+=3)
	{
		fscanf(mesh,"%le %le %le %le",&varT[0],&varT[1],&varT[2],&varT[3]);
		tabT[i] = varT[0];
		tabT[i+1] = varT[1];
		tabT[i+2] = varT[2];
	}
	free(varT);

	//ON SE REPLACE AU DEBUT DU FICHIER
	fseek(mesh,0,SEEK_SET);

	//REMPLISSAGE DU TABLEAU DES ARETES (ON LES ENTRE 2 PAR 2)
	double *varE = NULL;
	varE = calloc(3,sizeof(double));
	debut_donne = rechercherEdges(mesh);
	fgets(texte,200,mesh);
	nbEdges = atoi(texte);
	for(int i=0; i<2.*nbEdges; i+=2)
	{
		fscanf(mesh,"%le %le %le",&varE[0],&varE[1],&varE[2]);
		tabE[i] = varE[0];
		tabE[i+1] = varE[1];
	}
	free(varE);
}

//-------------------------------------------------------------------------------------
//----------- FONCTIONS POUR CHERCHER LE MOT CLE DANS LE FICHIER MESH -----------------
//-------------------------------------------------------------------------------------

long rechercherPoints(FILE* mesh)
{
	char texte[200] = {0};
	char mot_rechercher[] = "Vertices";

	while(strstr(texte,mot_rechercher)==NULL)
	{
		fgets(texte,200,mesh); //fgets peut lire jusqu'à une ligne à la fois
	}
	return ftell(mesh); //Cela permet de positionner le curseur au bon endroit
}

long rechercherTriangles(FILE* mesh)
{
	char texte[200] = {0};
	char mot_rechercher[] = "Triangles";

	while(strstr(texte,mot_rechercher)==NULL)
	{
		fgets(texte,200,mesh); //fgets peut lire jusqu'à une ligne à la fois
	}
	return ftell(mesh); //Cela permet de positionner le curseur au bon endroit
}

long rechercherEdges(FILE* mesh)
{
	char texte[200] = {0};
	char mot_rechercher[] = "Edges";

	while(strstr(texte,mot_rechercher)==NULL)
	{
		fgets(texte,200,mesh); //fgets peut lire jusqu a une ligne
	}
	return ftell(mesh); //Cela permet de positionner le curseur au bon endroit
}
