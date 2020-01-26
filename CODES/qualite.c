#include "qualite.h"

void tabQuali(double *tabQ, int *TabTri, double *TabPt, int nbT)
{
  double quali = 0;
  double mesure_aire = 0;
  double l1 = 0, l2 = 0, l3 = 0;
  double *p1 = calloc(2,sizeof(double));
  double *p2 = calloc(2,sizeof(double));
  double *p3 = calloc(2,sizeof(double));

  for(int i=0; i<3*nbT; i+=3)
  {
    p1[0] = TabPt[(TabTri[i]-1)*2];
    p1[1] = TabPt[((TabTri[i]-1)*2)+1];
    p2[0] = TabPt[(TabTri[i+1]-1)*2];
    p2[1] = TabPt[((TabTri[i+1]-1)*2)+1];
    p3[0] = TabPt[(TabTri[i+2]-1)*2];
    p3[1] = TabPt[((TabTri[i+2]-1)*2)+1];

    l1 = taille(p1,p2);
    l2 = taille(p2,p3);
    l3 = taille(p3,p1);


    mesure_aire = aire(p1,p2,p3);
    quali = (sqrt(3.)/12.)*((pow(l1,2)+pow(l2,2)+pow(l3,2))/mesure_aire);
    //quali = 4*sqrt(3)*mesure_aire/(pow(l1,2)+pow(l2,2)+pow(l3,2));
    tabQ[i/3] = quali;
  }

  FILE* histo = NULL;
  histo = fopen("histogramme.txt","w+");
  for(int i=0; i<nbT; i++)
  {
    fprintf(histo, "%lf\n",tabQ[i]);
  }
  fclose(histo);

}


double taille(double *x1, double *x2)
{
  double t = 0;
  t = sqrt(pow(x1[0]-x2[0],2) + pow(x1[1]-x2[1],2));

  return t;
}

double aire(double *p1, double *p2, double *p3)
{
  double area = 0;
  area = fabs((1./2)*((p2[0]-p1[0])*(p3[1]-p1[1])-(p2[1]-p1[1])*(p3[0]-p1[0])));

  return area;
}
