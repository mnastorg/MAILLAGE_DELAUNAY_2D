#include "main2.h"

int main (int argc, char* argv[])
{

  //DECLARATION DES VARIABLES
  FILE *mesh, *boite;
  int *tabT = NULL, *tabE = NULL;
  double *tabP2 = NULL;
  int *tabAr = NULL;
  double *tabP = NULL,*tX = NULL, *tY = NULL,*pT = NULL;
  pT = calloc(2,sizeof(double));
  double *Tab_longueur = NULL;
  double *NewPt = NULL;



  //---------------------------------------------------------------------------------------------------------
  //--------- LECTURE DU FICHIER MESH ET ECRITURE DES DONNEES DANS DES TABLEAUX -----------------------------
  //---------------------------------------------------------------------------------------------------------

  //OUVERTURE ET LECTURE DU FICHIER MESH
  mesh = fopen("TEST_MESH/trou.mesh","r");
  int nbPoints = 0, nbTriangles = 0, nbEdges = 0;

  //CALCUL DU NOMBRE DE POINTS
  nbPoints = nbP(mesh);
  fprintf(stderr,"nb de points : %d \n",nbPoints);

  //ON REPLACE LE CURSEUR AU DEBUT DU FICHIER
  fseek(mesh,0,SEEK_SET);

  //CALCUL DU NOMBRE DE TRIANGLES
  nbTriangles = nbT(mesh);
  //fprintf(stderr,"nb de triangles : %d \n",nbTriangles);

  //ON REPLACE LE CURSEUR AU DEBUT DU FICHIER
  fseek(mesh,0,SEEK_SET);

  //CALCUL DU NOMBRE D'ARETES
  nbEdges = nbE(mesh);
  //fprintf(stderr,"nb d'aretes : %d \n",nbEdges);

  //INITIALISATION DES TABLEAUX POUR STOCKAGE GEOMETRIE
  tabP = calloc(2.*nbPoints,sizeof(double)); //+1 par rapport au nb d'éléments
  tX = calloc(2,sizeof(double));
  tY = calloc(2,sizeof(double));
  tabE = calloc(2.*nbEdges,sizeof(int));
  tabT = calloc(3.*nbTriangles,sizeof(int));

  //ON REPLACE LE CURSEUR AU DEBUT DU FICHIER
  fseek(mesh,0,SEEK_SET);

  //ON REMPLIT LES TABLEAUX
  remplissage(mesh,tabP,tabT,tabE);

  //FERMETURE DU FICHIER MESH
  fclose(mesh);
  //-----------------------------------------------------------------------------------------------------------
  //-- CONSTRUCTION DE LA BOITE ENTOURANT LA GEOMETRIE INITIALE DECOUPEE EN 2 TRIANGLES -----------------------
  //-----------------------------------------------------------------------------------------------------------

  //CONSTRUCTION DE LA BOITE
  constructBoite(tX,tY,tabP,nbPoints);

  //STOCKAGE DES DONNEES DANS DES TABLEAUX EN SUIVANT LA MEME METHODE QU'AVANT POUR LA BOITE ENGLOBANTE
  boite = fopen("BoiteMesh.mesh","r");
  int nbPoints2 = 0;
  nbPoints2 = nbP(boite);
  //fprintf(stderr,"nb de points2 : %d \n",nbPoints2);
  fseek(boite,0,SEEK_SET);
  nbTriangles = nbT(boite);
  //fprintf(stderr,"nb de triangles2 : %d \n",nbTriangles);
  fseek(boite,0,SEEK_SET);
  int nbEdges_boite =0;
  nbEdges_boite = nbE(boite);
//*fprintf(stderr,"nb d'aretes2 : %d \n",nbEdges_boite);
  tabP2 = calloc(2.*nbPoints2,sizeof(double));
  tabAr = calloc(2.*nbEdges_boite,sizeof(int));
  tabT = realloc(tabT,(3.*nbTriangles)*sizeof(int));
  fseek(boite,0,SEEK_SET);
  remplissage(boite,tabP2,tabT,tabAr);
  fclose(boite);
  //fprintf(stderr,"voici les aretes de la boite");

  //------------------------------------------------------------------------------------------------------------
  //-- INSERTION DES POINTS DE LA GEOMETRIE DANS LA BOITE SUIVANT ALGO DE DELAUNAY -----------------------------
  //------------------------------------------------------------------------------------------------------------

  //tabP CONTIENT LES POINTS A INSERER (ISSU DE LA GEOMETRIE FICHIER MESH)
  //tabP2 CONTIENT LES POINTS COURANT (ACTUALISER A CHAQUE AJOUT DE POINTS)
  int cmpt = 0;
  for(int j=0; j<nbPoints; j++)
  {
    pT[0] = tabP[cmpt];
    pT[1] = tabP[cmpt+1];
    newT(&tabT,&tabP2,&tabAr,pT);
    cmpt += 2;
    //fprintf(stderr, " ok\n");
  }

  FILE* mesh_init,*mesh_delaunay;

  mesh_init=fopen("TEST_MESH/trou.mesh","r");
  mesh_delaunay=fopen("BoiteMesh.mesh","r");
  basculement(tabT,tabAr,tabP2,mesh_init,mesh_delaunay);
  fclose(mesh_init);
  fclose(mesh_delaunay);

  mesh_init=fopen("TEST_MESH/trou.mesh","r");
  mesh_delaunay=fopen("BoiteMesh_modifie.mesh","r");
  suppression_triangles(&tabT,&tabAr,&tabP2,tabP,mesh_init,mesh_delaunay,nbEdges,tabE);
  fclose(mesh_init);
  fclose(mesh_delaunay);

  FILE *mesh51;
  mesh51 = fopen("BoiteMesh_finie.mesh","r");
  if(mesh51 == NULL)
  {
    fprintf(stderr, "probleme \n");
  }
  NewPt = calloc(500,sizeof(double));
  int nbEdges_maillage = nbE(mesh51);
  Tab_longueur = calloc(nbEdges_maillage,sizeof(double));
  int newPt=tab_point_ajout(tabT,tabP2,tabAr,Tab_longueur,mesh51,NewPt);
  fclose(mesh51);


int cpt=0;
  double *point = calloc(2,sizeof(double));
  for( cpt =0 ; cpt < 2*newPt; cpt+=2)
  {
    point[0] = NewPt[cpt];
    point[1] = NewPt[cpt+1];
    fprintf(stderr, "Voici les points a ajouter %lf %lf \n",point[0],point[1]);
    ajout_pt(&tabT,&tabP2,&tabAr,tabP2,point);
  }

  free(point);

  FILE* final;
  final = fopen("BoiteMesh_finie.mesh","r");
  int nbtfinal = nbT(final);
  fprintf(stderr,"Nombre de nouveaux triangles : %d \n",nbtfinal);
  double *tabQ = calloc(nbTriangles,sizeof(double));
  tabQuali(tabQ, tabT, tabP2, nbtfinal);
  for(int i = 0; i < nbtfinal; i++)
  {
    fprintf(stderr, "qualite triangle = %d = %lf \n", i, tabQ[i]);
  }


  //Il y a une erreur de segmentation si l'on retire les free cela provient
  //de la fonction QUALITE joue sur tabE et tabP (si l'on retire un des 2 ca marche)
//  free(tabE);
  //free(tabAr);
  //free(tabQ);
  free(NewPt);
  free(tabP);
  free(tabT);
  free(tabE);
  free(tabP2);
  free(tX);
  free(tY);
  free(pT);
  free(Tab_longueur);

  return EXIT_SUCCESS;
}
