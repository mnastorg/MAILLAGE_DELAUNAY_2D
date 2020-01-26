#include "boite.h"

void constructBoite(double *tX, double *tY, double *tabP, int nbPoints)
{
  FILE* mesh;
  double test = 0;
  double *CoordBoite = NULL;
  CoordBoite = calloc(8,sizeof(double));

  //ON PROCEDE EN REGARDANT LE MAXIMUM ET MINIMUM DES POINTS
  test = 100;
  int i = 0;

  //BOUCLE POUR MINIMUM EN X
  for(i=0; i<2.*nbPoints; i+=2)
  {
    if(test > tabP[i])
    {
      test = tabP[i];
    }
  }
  tX[0] = test;

  //BOUCLE POUR MAXIMUM EN X
  test = -100;
  for(i=0; i<2.*nbPoints; i+=2)
  {
    if(test < tabP[i])
    {
      test = tabP[i];
    }
  }
  tX[1] = test;

  //BOUCLE POUR MINIMUM EN Y
  test = 100;
  for(i=1; i<2.*nbPoints; i+=2)
  {
    if(test > tabP[i])
    {
      test = tabP[i];
    }
  }
  tY[0]=test;

  //BOUCLE POUR MAXIMUM EN Y
  test = -100;
  for(i=1; i<2.*nbPoints; i+=2)
  {
    if(test<tabP[i])
    {
      test=tabP[i];
    }
  }
  tY[1]=test;

  double coord = 0.5;
  //DEFINITION DES COORDONNEES DE LA BOITE
  CoordBoite[0] = tX[0] - coord;
  CoordBoite[1] = tY[0] - coord;
  CoordBoite[2] = tX[0] - coord;
  CoordBoite[3] = tY[1] + coord;
  CoordBoite[4] = tX[1] + coord;
  CoordBoite[5] = tY[1] + coord;
  CoordBoite[6] = tX[1] + coord;
  CoordBoite[7] = tY[0] - coord;

  //ECRITURE DANS LE FICHIER BOITEMESH DES 4 POINTS ET DES 2 TRIANGLES
  mesh = fopen("BoiteMesh.mesh","w+");
  fprintf(mesh, "Vertices \n");
  fprintf(mesh, "%d \n",4);
  for(i=0; i<8; i+=2)
  {
    fprintf(mesh, "%lf %lf %lf \n",CoordBoite[i],CoordBoite[i+1],0.);
  }
  fprintf(mesh, "\n Triangles \n");
  fprintf(mesh, "%d \n",2);
  fprintf(mesh, "%d %d %d %d \n",1,2,4,0);
  fprintf(mesh, "%d %d %d %d \n",4,3,2,0);
  fprintf(mesh, "\n Edges \n %d \n",4);
  fprintf(mesh, "%d %d %d\n",1,2,1);
  fprintf(mesh, "%d %d %d\n",2,3,1);
  fprintf(mesh, "%d %d %d\n",3,4,1);
  fprintf(mesh, "%d %d %d ",4,1,1);


  fclose(mesh);
}
