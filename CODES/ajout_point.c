#include "ajout_point.h"


int tab_point_ajout(int *TabTri, double *tabP2, int *Tabarete, double *Tab_longueur,FILE* mesh, double *NewPt)
{
  //NOMBRE D ARETES
  fseek(mesh,0,SEEK_SET);
  int nbEdges_maillage = nbE(mesh);
  fseek(mesh,0,SEEK_SET);
  int nbPoints = nbP(mesh);

  //CALCUL DU MINIMUM DES LONGUEURS DES ARETES

  //POINT ET POINT MILIEU
  double *pt1 = calloc(2,sizeof(double));
  double *pt2 = calloc(2,sizeof(double));
  double *PT = calloc(2,sizeof(double));
  //double *NewPt = calloc(4*nbPoints,sizeof(double));
  int point_1 = 0,point_2 = 0;
  double point_1_x = 0,point_1_y = 0;
  double point_2_x = 0,point_2_y = 0;
  double min = 50;

  for (int i = 0; i < 2*nbEdges_maillage; i+=2)
  {
    pt1[0] = tabP2[(Tabarete[i]-1)*2];
    pt1[1] = tabP2[((Tabarete[i]-1)*2)+1];
    pt2[0] = tabP2[(Tabarete[i+1]-1)*2];
    pt2[1] = tabP2[((Tabarete[i+1]-1)*2)+1];

    Tab_longueur[i/2] = sqrt(pow(pt1[0] - pt2[0], 2) + pow(pt1[1] - pt2[1], 2));
    //fprintf(stderr, "%lf\n", Tab_longueur[i/2]);
  }

  for (int i = 0; i < nbEdges_maillage; i++)
  {
    if ( Tab_longueur[i] < min)
    {
      min = Tab_longueur[i];
    }
  }
  //fprintf(stderr, "Le minimum est : %lf\n", min);
//fin du remplisage de tabLongueur
  int Taille_NewPt = 0;
  for (int i = 0; i < 2*nbEdges_maillage; i+=2)
  {
      if (Tab_longueur[i]>min)
      {
        pt1[0] = tabP2[(Tabarete[i]-1)*2];
        pt1[1] = tabP2[((Tabarete[i]-1)*2)+1];
        pt2[0] = tabP2[(Tabarete[i+1]-1)*2];
        pt2[1] = tabP2[((Tabarete[i+1]-1)*2)+1];

        PT[0] = (pt1[0] + pt2[0])/2.;
        PT[1] = (pt1[1] + pt2[1])/2.;

        Taille_NewPt += 2;
        NewPt[Taille_NewPt-2] = PT[0];
        NewPt[Taille_NewPt-1] = PT[1];
        fprintf(stderr, "%lf , %lf \n",NewPt[Taille_NewPt-2], NewPt[Taille_NewPt-1]);
      }
  }
  free(pt1);
  free(pt2);
  free(PT);
  return(Taille_NewPt/2);
}
