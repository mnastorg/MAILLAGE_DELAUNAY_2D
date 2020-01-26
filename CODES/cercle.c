#include "cercle.h"
#include "complex.h"
#include "maillage.h"

//-----------------------------------------------------------------------------------------------------
//-FONCTION POUR VERIFIER SI UN POINT APPARTIENT AU CERCLE CIRCONSCRIT D UN TRIANGLE ------------------
//-----------------------------------------------------------------------------------------------------

int test_inscritm(double *p1,double *p2,double *p3,double *p4)
{

  //CALCUL DES LONGUEURS ENTRE LES 3 POINTS
  double ab = sqrt(pow(p2[0]-p1[0],2)+pow(p2[1]-p1[1],2));
  double ac = sqrt(pow(p3[0]-p1[0],2)+pow(p3[1]-p1[1],2));
  double bc = sqrt(pow(p2[0]-p3[0],2)+pow(p2[1]-p3[1],2));

  //CALCUL DES 3 ANGLES DU TRIANGLE FORME PAR LES 3 POINTS
  double theta_A = acos(((p2[0]-p1[0])*(p3[0]-p1[0])+(p2[1]-p1[1])*(p3[1]-p1[1]))/(ab*ac));
  double theta_B = acos(((p3[0]-p2[0])*(p1[0]-p2[0])+(p3[1]-p2[1])*(p1[1]-p2[1]))/(bc*ab));
  double theta_C = acos(((p2[0]-p3[0])*(p1[0]-p3[0])+(p2[1]-p3[1])*(p1[1]-p3[1]))/(bc*ac));

  //CALCUL DES COEFFS POUR LE BARYCENTRE
  double a = sin(2*theta_A);
  double b = sin(2*theta_B);
  double c = sin(2*theta_C);

  //CALCUL DU CENTRE DU CERCLE CIRCONSCRIT
  double xc = (a*p1[0]+b*p2[0]+c*p3[0])/(a+b+c);
  double yc = (a*p1[1]+b*p2[1]+c*p3[1])/(a+b+c);
  //fprintf(stderr, "xc,yc = %lf %lf \n", xc, yc);

  //CALCUL DU RAYON DU CERCLE CIRCONSCRIT
  double r = sqrt(pow(p1[0]-xc,2)+pow(p1[1]-yc,2));
  //fprintf(stderr, "r = %lf \n", r);

  //CALCUL DE LA DISTANCE DU CERCLE AU POINT A AJOUTER
  double r2 = sqrt(pow(p4[0]-xc,2)+pow(p4[1]-yc,2));
  //fprintf(stderr, "dist C-P = %f \n", r2);

  //TEST SI POINT EST DEDANS OU NON
  if(r2 > r)
  {
    return 0; //PAS DANS LE CERCLE
  }
  else
  {
    return 1; //DANS LE CERCLE
  }
}

//--------------------------------------------------------------------------------------------------
//-------------- FONCTION POUR VERIFIER SI UN POINT EST DANS UN TRIANGLE ---------------------------
//--------------------------------------------------------------------------------------------------

//VERIFIE SI UN POINT EST DANS LE TRIANGLE ET RETOURNE LE SIGNE DES COORDONNEES BARY
int verifDansLeTriangle(double *p1,double *p2,double *p3,double *p4,double *CoordB)
{
  //CALCUL DE L'AIRE DU TRIANGLE
  double A = ((p2[0]-p1[0])*(p3[1]-p1[1])-(p2[1]-p1[1])*(p3[0]-p1[0]));

  //CALCUL DES COORDONNEES BARYCENTRIQUES
  double l2 = ((p3[1]-p1[1])*(p4[0]-p1[0])+(p1[0]-p3[0])*(p4[1]-p1[1]))/A;
  double l3 = ((p1[1]-p2[1])*(p4[0]-p1[0])+(p2[0]-p1[0])*(p4[1]-p1[1]))/A;
  double l1 = 1-l2-l3;

  //ENREGISTREMENT DES SIGNES DES COORDONNEES BARY
  if(l1 < 0)
  {
    CoordB[0] = -1;
  }
  else
  {
    CoordB[0] = 1;
  }
  if(l2<0)
  {
    CoordB[1] = -1;
  }
  else
  {
    CoordB[1] = 1;
  }
  if(l3<0)
  {
    CoordB[2] = -1;
  }
  else
  {
    CoordB[2] = 1;
  }

  //TEST : P EST DANS TRIANGLE SI TOUTES SES COORD BARY SONT POSITIVES
  if(l1<0 || l2<0 || l3<0)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

//-----------------------------------------------------------------------------------------------
//-------------- FONCTION DE CREATION TABLEAU DES VOISINS  --------------------------------------
//-----------------------------------------------------------------------------------------------


//INITIALISATION DU TABLEAU DES VOISINS. ON MET DES -1 PARTOUT
//LES -1 PERMETTENT DE MONTRER QUE LE TRIANGLE EST VOISIN DU BORD
void init(double **TabVoisin,int nbTriangles)
{
  for(int i=0; i<nbTriangles; i++)
  {
    TabVoisin[i][0] = -1;
    TabVoisin[i][1] = -1;
    TabVoisin[i][2] = -1;
  }
}

//CREATION DU TABLEAU DES VOISINS
void rtabVoisin(int *TabTri,double **TabVoisin,int nbTriangles)
{
  //INITIALISATION DES VARIABLES
  int i = 0, j = 0, k = 0, l = 0;
  int *e1 = NULL, *e2 = NULL;
  e1 = calloc(2,sizeof(double));
  e2 = calloc(2,sizeof(double));
  init(TabVoisin,nbTriangles);

  //BOUCLE SUR TOUS LES TRIANGLES (D'OU ITERATION TOUS LES 3)
  for (i=0; i<3*nbTriangles; i+=3)
  {
    //BOUCLE SUR SOMMETS DU ieme TRIANGLE
    for (j=0; j<3; j++)
    {
      //CHOIX ARETE. ON STOCKE DANS e1 ET e2 LES DEUX POINTS COMPOSANTS L'ARETE
      if(j == 0)
      {
        e1[0] = TabTri[i+1];
        e1[1] = TabTri[i+2];
      }
      else if(j == 1)
      {
        e1[0] = TabTri[i];
        e1[1] = TabTri[i+2];
      }
      else
      {
        e1[0] = TabTri[i];
        e1[1] = TabTri[i+1];
      }
      //ON REGARDE SUR TOUS LES TRIANGLES VOISINS
      //ON STOCKE LEUR ARETE COMME PRECEDEMMENT
      //ON REGARDE SI LES ARETES SONT LES MEMES
      //SI OUI ALORS LES TRIANGLES SONT VOISINS SINON NON
      for(k=0; k<3*nbTriangles; k+=3)
      {
        //ON NE REGARDE PAS LE MEME TRIANGLE
        if(k != i)
        {
          //ON PROCEDE COMME AVANT
          for(l=0; l<3; l++)
          {
            if(l == 0)
            {
              e2[0] = TabTri[k+1];
              e2[1] = TabTri[k+2];
            }
            else if (l == 1)
            {
              e2[0] = TabTri[k];
              e2[1] = TabTri[k+2];
            }
            else
            {
              e2[0] = TabTri[k];
              e2[1] = TabTri[k+1];
            }
            //TEST SI LES ARETES SONT LES MEMES EN REGARDANT LES DEUX SENS
            if ((e1[0]==e2[0] && e1[1]==e2[1]) || (e1[0]==e2[1] && e1[1]==e2[0]))
            {
              //MAJ DE LA TABLE DES VOISINS
              TabVoisin[i/3][j] = k/3+1; //on empeche d'avoir le numero 0 comme numero de triangle
              TabVoisin[k/3][l] = i/3+1;
            }
          }
        }
      }
    }
  }
free(e1);
free(e2);
}

//-----------------------------------------------------------------------------------------------
//-------------- FONCTION NUMERO TRIANGLE DANS LEQUEL SE TROUVE LE POINT A AJOUTER --------------
//-----------------------------------------------------------------------------------------------

int QTriangle(int *TabTri,int nbTriangles,double *Pt, double *TabPt, double **TabVoisin)
{

  //INITIALISATION DES VARIABLES
  double *CoordB = NULL;
  int test = 0;
  int numeroTri = 0;
  double *p1 = NULL, *p2 = NULL, *p3 = NULL;
  p1 = calloc(2,sizeof(double));
  p2 = calloc(2,sizeof(double));
  p3 = calloc(2,sizeof(double));
  CoordB = calloc(3,sizeof(double));
  double inter = 0;
  int i = 0;
  test = 0;
  int pt = 0;
  int Tr_prec=0;
  int ou=0,deja=0;
  int ici=0;
  //ON BOUCLE SUR TOUS LES TRIANGLES TANT QUE test = 0
  do
  {
    //EXTRACTION DES COORDONNEES DES POINTS DU TRIANGLE i
    // -1 CAR ON COMMENCE A 0
    p1[0] = TabPt[(TabTri[i]-1)*2];
    p1[1] = TabPt[((TabTri[i]-1)*2)+1];
    p2[0] = TabPt[(TabTri[i+1]-1)*2];
    p2[1] = TabPt[((TabTri[i+1]-1)*2)+1];
    p3[0] = TabPt[(TabTri[i+2]-1)*2];
    p3[1] = TabPt[((TabTri[i+2]-1)*2)+1];

    //TEST VERIFICATION DANS TRIANGLE
    test = verifDansLeTriangle(p1,p2,p3,Pt,CoordB);
    //fprintf(stderr,"ok %d \n",test);
    //SI ON EST DANS LE TRIANGLE ON RESSORT NUM TRIANGLE
    fprintf(stderr," coordonnes barycentriques %lf %lf %lf \n ",CoordB[0],CoordB[1],CoordB[2]);
    if(test == 1 && pt == 0)
    {
      inter = (i/3) + 1;
    }
    //SI ON EST PAS DANS LE TRIANGLE
    if(test == 0)
    {//modifier et ne faire q'un test a la fois

      if(CoordB[0]>0 && CoordB[1]<0 && CoordB[2]>0)
      {
        ici=1;
        inter = TabVoisin[i/3][1]; //numero du triangle
        if(inter<0)
        {
            inter=TabVoisin[i/3][2];
        }
        if(inter<0)
        {
           inter=TabVoisin[i/3][0];
        }
          i = 3*(inter-1); //on passe de la numerotation du triangle a son indice dans le tableau ligne
          Tr_prec=inter;
      }
      else if (CoordB[0]>0 && CoordB[1]<0 && CoordB[2]<0)
      {
       fprintf(stderr, "ici 2 \n");
       ici=2;
       if(deja==ici)
       {
           inter = TabVoisin[i/3][2]; //numero du triangle
         if(inter<0)
         {
           inter = TabVoisin[i/3][1]; //numero du triangle
         }
         if(inter<0)
         {
           inter = TabVoisin[i/3][0]; //numero du triangle
         }
       }
       else
       {
           inter = TabVoisin[i/3][1]; //numero du triangle
         if(inter<0)
         {
           inter = TabVoisin[i/3][2]; //numero du triangle
         }
         if(inter<0)
         {
           inter = TabVoisin[i/3][0]; //numero du triangle
         }
       }
       i = 3*(inter-1); //on passe de la numerotation du triangle a son indice dans le tableau ligne
       Tr_prec=inter;
      }
      else if(CoordB[0]>0 && CoordB[1]>0 && CoordB[2]<0 )
      {
        fprintf(stderr, "ici 3 \n");
        ici=3;
        if(deja==ici)
        {
          inter = TabVoisin[i/3][1]; //numero du triangle
          if(inter<0)
          {
            inter = TabVoisin[i/3][2]; //numero du triangle
          }
          if(inter<0)
          {
            inter = TabVoisin[i/3][0]; //numero du triangle
          }
        }
        else
        {
              inter = TabVoisin[i/3][2]; //numero du triangle
            if(inter<0)
            {
              inter = TabVoisin[i/3][1]; //numero du triangle
            }
            if(inter<0)
            {
              inter = TabVoisin[i/3][0]; //numero du triangle
            }
        }
        if(ou>50)
        {
          inter=TabVoisin[i/3][2];
        }
          i = 3*(inter-1); //on passe de la numerotation du triangle a son indice dans le tableau ligne
          Tr_prec=inter;
      }
      else if(CoordB[0]<0 && CoordB[1]>0 && CoordB[2]<0)
      {
        fprintf(stderr, "ici 4 \n");
        ici=4;
        if(deja==ici)
        {
            inter = TabVoisin[i/3][0]; //numero du triangle
          if(inter<0)
          {
            inter = TabVoisin[i/3][2]; //numero du triangle
          }
          if(inter<0)
          {
            inter = TabVoisin[i/3][1]; //numero du triangle
          }
        }
        else
        {
            inter = TabVoisin[i/3][2]; //numero du triangle
          if(inter<0)
          {
            inter = TabVoisin[i/3][0]; //numero du triangle
          }
          if(inter<0)
          {
            inter = TabVoisin[i/3][1]; //numero du triangle
          }
        }
        i = 3*(inter-1); //on passe de la numerotation du triangle a son indice dans le tableau ligne
        Tr_prec=inter;
      }
      else if(CoordB[0]<0 && CoordB[1]>0 && CoordB[2]>0)
      {
        fprintf(stderr, "ici 5 \n");
        ici=5;
        inter = TabVoisin[i/3][0]; //numero du triangle
        if(deja==ici)
        {
          if(inter<0)
          {
              inter=TabVoisin[i/3][1];
          }
          if(inter<0)
          {
              inter=TabVoisin[i/3][2];
          }
        }
        else
        {
          if(inter<0)
          {
              inter=TabVoisin[i/3][2];
          }
          if(inter<0)
          {
              inter=TabVoisin[i/3][1];
          }
        }
        i = 3*(inter-1); //on passe de la numerotation du triangle a son indice dans le tableau ligne
        Tr_prec=inter;
      }
      else if(CoordB[0]<0 && CoordB[1]<0 && CoordB[2]>0)
      {
        fprintf(stderr, "ici 6 \n");
        ici=6;
        if(deja==ici)
        {
            inter = TabVoisin[i/3][0]; //numero du triangle
          if(inter<0)
          {
            inter = TabVoisin[i/3][1]; //numero du triangle
          }
          if(inter<0)
          {
            inter = TabVoisin[i/3][2]; //numero du triangle
          }
        }
        else
        {
            inter = TabVoisin[i/3][1]; //numero du triangle
          if(inter<0)
          {
            inter = TabVoisin[i/3][0]; //numero du triangle
          }
          if(inter<0)
          {
            inter = TabVoisin[i/3][2]; //numero du triangle
          }
        }
        i = 3*(inter-1); //on passe de la numerotation du triangle a son indice dans le tableau ligne
        Tr_prec=inter;
      }
    }
    pt++;
    fprintf(stderr, "inter :%lf \n",inter );
    if(ou%2==0)
    {
      deja=ici;
    }
    ou++;
  } while(test==0);
//fprintf(stderr,"ok fin %d \n",test);
  numeroTri = inter;
  //fprintf(stderr, "numero du triangle %d \n",numeroTri );
  free(p1);
  free(p2);
  free(p3);
  free(CoordB);

  return numeroTri;

}

int LCirconscrit(int **Lc,int *TabTri,double *TabPt,int nbTriangles,double *Pt)
{

  int base=0;
  int encore=1;
  int k=0;
  double **TabVoisin = malloc(sizeof(*TabVoisin)*nbTriangles); //indice j i
  double *p1=NULL,*p2=NULL,*p3=NULL;
  p1=calloc(2,sizeof(double));
  p2=calloc(2,sizeof(double));
  p3=calloc(2,sizeof(double));
  for (int i=0;i<nbTriangles; i++)
  {
    TabVoisin[i]=malloc(sizeof(**TabVoisin)*3);
  }
  rtabVoisin(TabTri,TabVoisin,nbTriangles);//table des voisins avec valeurs nulles

  base=QTriangle(TabTri,nbTriangles,Pt,TabPt,TabVoisin); //numero du triangle representant la base, la ou se trouve le point a ajouter
  fprintf(stderr, "qt ok base %d \n",base);

//fprintf(stderr, "Voici la base %d \n",base );
  (*Lc)[0]=base; //la base est toujours considere comme un triangle a enlever
  //fprintf(stderr, "base %d \n",base );
  int nTri=base-1;
  int TailleLc=1,deja=0;
  int cmpt=0,recherche=0;
  int j=0,verif=0;
  //fprintf(stderr, "ok \n");
while(encore==1)
{
  encore=0;
  //on regarde les 3 voisins de Lc a cmpt
  if(TabVoisin[nTri][0]>0)//donc si on est pas sur un bord
  {
    //fprintf(stderr, "premier voisin  du triangle %d %lf \n",nTri+1,TabVoisin[nTri][0]);
    //fprintf(stderr, "triangle regarde %lf \n",TabVoisin[nTri][0]);

    j=(TabVoisin[nTri][0]-1)*3; //donne l'indice dans TabTri du triangle voisin
    //on veut maintenant recuperer les points associe a se triangle
    p1[0]=TabPt[(TabTri[j]-1)*2];//car la numerotation des tableau commence a 0
    p1[1]=TabPt[((TabTri[j]-1)*2)+1];
    p2[0]=TabPt[(TabTri[j+1]-1)*2];
    p2[1]=TabPt[((TabTri[j+1]-1)*2)+1];
    p3[0]=TabPt[(TabTri[j+2]-1)*2];
    p3[1]=TabPt[((TabTri[j+2]-1)*2)+1];
    //fprintf(stderr, "ok \n");

    verif=test_inscritm(p1,p2,p3,Pt);
    //fprintf(stderr, " %lf %lf %lf %lf %lf %lf %lf %lf \n",p1[0],p1[1],p2[0],p2[1],p3[0],p3[1],Pt[0],Pt[1]);
    //fprintf(stderr, "Test si le triangle %lf est dans le cercle circonscrit %d \n",TabVoisin[nTri][0],verif );

    if(verif==1)
    {
      //fprintf(stderr, "verif1 %d \n",verif );
      //on l'ajoute a Lc
      //on regarde si il n'est pas deja dans le tableau
      recherche=TabVoisin[nTri][0];
      //fprintf(stderr, "triangle rechercher d \n",recherche);
      for(int i=0;i<TailleLc;i++)
      {
        if(recherche==(*Lc)[i])
        {
          deja=1;
        }
      }
      if(deja==0)
      {
      TailleLc++;
      *Lc=realloc(*Lc,TailleLc*sizeof(int));
      (*Lc)[TailleLc-1]=TabVoisin[nTri][0];
      encore=1;
    }
    deja=0;
    }
    verif=0;
  }
  if(TabVoisin[nTri][1]>0)//donc si on est pas sur un bord
  {
    j=(TabVoisin[nTri][1]-1)*3;
    p1[0]=TabPt[(TabTri[j]-1)*2];//car la numerotation des tableau commence a 0
    p1[1]=TabPt[((TabTri[j]-1)*2)+1];
    p2[0]=TabPt[(TabTri[j+1]-1)*2];
    p2[1]=TabPt[(TabTri[j+1]-1)*2+1];
    p3[0]=TabPt[(TabTri[j+2]-1)*2];
    p3[1]=TabPt[(TabTri[j+2]-1)*2+1];
    verif=test_inscritm(p1,p2,p3,Pt);
  //  fprintf(stderr, " %lf %lf %lf %lf %lf %lf %lf %lf \n",p1[0],p1[1],p2[0],p2[1],p3[0],p3[1],Pt[0],Pt[1]);
    //fprintf(stderr, "Test si le triangle %lf est dans le cercle circonscrit %d \n",TabVoisin[nTri][1],verif );

    if(verif==1)
    {
      //on l'ajoute a Lc
      //on regarde si il n'est pas deja dans le tableau
      recherche=TabVoisin[nTri][1];
      for(int i=0;i<TailleLc;i++)
      {
        if(recherche==(*Lc)[i])
        {
          deja=1;
        }
      }
      if(deja==0)
      {
        TailleLc++;
        *Lc=realloc(*Lc,TailleLc*sizeof(int));
        (*Lc)[TailleLc-1]=TabVoisin[nTri][1];
        encore=1;
      }
      deja=0;
    }
    verif=0;
  }
  if(TabVoisin[nTri][2]>0)//donc si on est pas sur un bord
  {
    j=(TabVoisin[nTri][2]-1)*3;
    p1[0]=TabPt[(TabTri[j]-1)*2];//car la numerotation des tableau commence a 0
    p1[1]=TabPt[((TabTri[j]-1)*2)+1];
    p2[0]=TabPt[(TabTri[j+1]-1)*2];
    p2[1]=TabPt[((TabTri[j+1]-1)*2)+1];
    p3[0]=TabPt[(TabTri[j+2]-1)*2];
    p3[1]=TabPt[((TabTri[j+2]-1)*2)+1];
    verif=test_inscritm(p1,p2,p3,Pt);
  //  fprintf(stderr, " %lf %lf %lf %lf %lf %lf  %lf %lf  \n",p1[0],p1[1],p2[0],p2[1],p3[0],p3[1],Pt[0],Pt[1]);

  //  fprintf(stderr, "Test si le triangle %lf est dans le cercle circonscrit %d \n",TabVoisin[nTri][2],verif );

    //fprintf(stderr, "verif3 %d \n",verif );
    if(verif==1)
    {
      //on l'ajoute a Lc
      //on regarde si il n'est pas deja dans le tableau
      recherche=TabVoisin[nTri][2];
      for(int i=0;i<TailleLc;i++)
      {
        if(recherche==(*Lc)[i])
        {
          deja=1;
        }
      }
      if(deja==0)
      {
      TailleLc++;
      *Lc=realloc(*Lc,TailleLc*sizeof(int));
      (*Lc)[TailleLc-1]=TabVoisin[nTri][2];
      encore=1;
      }
    deja=0;
    }
    verif=0;
  }
  if(encore==1)
  {
  cmpt++;
  nTri=(*Lc)[cmpt]-1;
  }
}

  for (int i=0;i<nbTriangles; i++)
  {
    free(TabVoisin[i]);
  }
  free(TabVoisin);
  free(p1);
  free(p2);
  free(p3);
  return TailleLc;
}


int verif(int *aretecou,int *areteinit,int aretesTaille)
{
  int test_verif=0;
  for(int i=0;i<aretesTaille;i+=2)
  {
    if(areteinit[i]==aretecou[0] && areteinit[i+1]==aretecou[1])
    {
      test_verif++;
    }
    if(areteinit[i+1]==aretecou[0] && areteinit[i]==aretecou[1])
    {
      test_verif++;
    }
    if(areteinit[i+1]==aretecou[0] && areteinit[i+2]==aretecou[1])
    {
      test_verif++;
    }
    if(areteinit[i+2]==aretecou[0] && areteinit[i+1]==aretecou[1])
    {
      test_verif++;
    }
  }
  return test_verif;
}
//ordonne les points de la cavite crée
int testOrdo(int **Lc,int *TabTri,int nbTriangles,int TailleLc,int **arete_cavite2)
{
  //numero des triangles dont on connait que le point est dans le cercle circonscrit
 int nb_ar=6*TailleLc;
 int *ptcavite=NULL;
 int *aretecou=NULL;
 aretecou=calloc(2,sizeof(int));
 int *arete_cavite=NULL;
 arete_cavite=calloc(1,sizeof(int));
 //int *arete_cavite2=NULL;

 ptcavite=calloc(nb_ar,sizeof(int));
 int cmpt=0;
 int deja=0;
 int tailleCavite=0;

 for(int i=0;i<nb_ar;i+=6)
 {
   ptcavite[i]=TabTri[((*Lc)[cmpt]-1)*3];
   ptcavite[i+1]=TabTri[((*Lc)[cmpt]-1)*3+1];
   ptcavite[i+2]=TabTri[((*Lc)[cmpt]-1)*3+1];
   ptcavite[i+3]=TabTri[((*Lc)[cmpt]-1)*3+2];
   ptcavite[i+4]=TabTri[((*Lc)[cmpt]-1)*3+2];
   ptcavite[i+5]=TabTri[((*Lc)[cmpt]-1)*3];
   cmpt++;
 }

  for(int i=0;i<nb_ar;i+=2)
  {
    aretecou[0]=ptcavite[i];
    aretecou[1]=ptcavite[i+1];
    for(int j=0;j<nb_ar;j+=2)
    {
      if(i!=j)
      {
        if((aretecou[0]==ptcavite[j] && aretecou[1]==ptcavite[j+1]) || (aretecou[1]==ptcavite[j] && aretecou[0]==ptcavite[j+1]))
          {
                deja=1;
          }
        }
      }
      if(deja==0)
      {
        //mettre a jour aretecavite
        tailleCavite+=2;
        arete_cavite=realloc(arete_cavite,tailleCavite*sizeof(int));
        arete_cavite[tailleCavite-2]=aretecou[0];
        arete_cavite[tailleCavite-1]=aretecou[1];
      }
      deja=0;
  }//on a extrait les aretes de la cavite dans arete_cavite

  //on va maintenant ordonne les points de la cavite que l'on enregistre
  //fprintf(stderr, "taille de la cavite %d \n",tailleCavite );
  *arete_cavite2=realloc(*arete_cavite2,tailleCavite*sizeof(int));

  int rechercher=0;
  int modif=0;
  (*arete_cavite2)[0]=arete_cavite[0];
  (*arete_cavite2)[1]=arete_cavite[1];
  arete_cavite[0]=-1; //car on a deja ordonne cette arete
  arete_cavite[1]=-1;
  int i=2;

  cmpt=2; //on l'a deja remplit de deux valeurs
  while(cmpt<tailleCavite) //pas compris d'ou vient le -4
  {
    rechercher=(*arete_cavite2)[cmpt-1];
    //fprintf(stderr, "rechercher %d \n",rechercher );
    if((arete_cavite[i+1]>0 && arete_cavite[i]>0)) //si on a pas deja ajouter cette arete
    {
      if((rechercher==arete_cavite[i] || rechercher==arete_cavite[i+1]))
      {
      //on l'ajoute dans le bonne ordre
        if(rechercher==arete_cavite[i])
        {
          (*arete_cavite2)[cmpt]=arete_cavite[i];
          (*arete_cavite2)[cmpt+1]=arete_cavite[i+1];
        }
        else
        {
          (*arete_cavite2)[cmpt]=arete_cavite[i+1];
          (*arete_cavite2)[cmpt+1]=arete_cavite[i];
        }
        arete_cavite[i]=-1;
        arete_cavite[i+1]=-1;
        cmpt+=2; //ie qu'on ajouter 2 points pour une aretes
        i=0;
      }
      else
      {
      i+=2;
      }
    }
    else
    {
    i+=2;
    }
}
//affichage de la cavite
/*
for(int i=0;i<tailleCavite;i+=2)
{
fprintf(stderr, "cavite2 : %d %d \n",(*arete_cavite2)[i],(*arete_cavite2)[i+1] );
}*/
free(arete_cavite);
free(ptcavite);
free(aretecou);
return tailleCavite;
}

//le but est de mettre a  jour en mettant a -1 le triangle supprimé et de definir la cavite resultante
int Maj(int **Lc,int *TabTri,double *TabPt,int nbTriangles,double *Pt,int **areteCavite)
{
  int TailleLc=0;
  int ok=0;
  //fprintf(stderr, "nb t %d\n",nbTriangles );
 //on rempli Lc pour savoir qu'elle sont les triangles a supprimer
  TailleLc=LCirconscrit(Lc,TabTri,TabPt,nbTriangles,Pt); //numero des triangles dont on connait que le point est dans le cercle circonscrit
  /*for(int i=0;i<TailleLc;i++)
  {
    fprintf(stderr, "triangle a supprimer %d\n",(*Lc)[i]);
  }*/
  int nbpoints=3*TailleLc;
  int test1=0,test2=0,test3=0,nbneg=0,cmpt=0;
  cmpt=0;
  //enregistrement des sommets de la cavité
  int PtCavite=0;
  //int *areteCavite=NULL;
  //areteCavite=calloc(1,sizeof(int));
  PtCavite=testOrdo(Lc,TabTri,nbTriangles,TailleLc,areteCavite); //semble correcte a modifier son utilisation
  //les aretes de la cavite son dans areteCavite
  //int testOrdo(int **Lc,int *TabTri,int nbTriangles,int **ptcaviteOrdo,int TailleLc)
fprintf(stderr, "taille Lc %d %d %d  \n",TailleLc,(*Lc)[0],(*Lc)[TailleLc-1]);

  for(int i=0;i<TailleLc;i++)
  {
    TabTri[((*Lc)[i]-1)*3]=-1;
    TabTri[((*Lc)[i]-1)*3+1]=-1;
    TabTri[((*Lc)[i]-1)*3+2]=-1;
  }
  return PtCavite; //taille d'arete de la cavite
}
//Lc est le vecteurs represenatnts le numero des triangles supprimé
void newT(int **TabTri,double **TabPt,int **Tabarete,double *Pt)
{
  int i=0,TailleLc=0;
  int *Lc = NULL,*tabNewT=NULL;
  int nbPoints=0,nbTriangles=0;
  int cmpt=0;
  FILE *mesh;
  mesh= fopen("BoiteMesh.mesh","r"); //option de lecture seulement
  nbPoints=nbP(mesh);
  fseek(mesh,0,SEEK_SET); //on replace le curseur au début du
  nbTriangles=nbT(mesh);
  fclose(mesh);
  Lc=calloc(1,sizeof(int)); //indice j i

  //fprintf(stderr, "C p point %lf %lf\n",Pt[0],Pt[1] );
  TailleLc=LCirconscrit(&Lc,*TabTri,*TabPt,nbTriangles,Pt); //remplit Lc avec les numeros des triangles dont le cercle inscrit possede le point pT
  //fprintf(stderr, "ok \n");

  int *ptcaviteOrdo=NULL; //il faut l'allouer
  ptcaviteOrdo=calloc(1,sizeof(int));
  int taille_cavite=0;

  taille_cavite=Maj(&Lc,*TabTri,*TabPt,nbTriangles,Pt,&ptcaviteOrdo); //modifie la table des triangles et remplit les sommets de la cavite
  int nb_triangle_a_ajouter=taille_cavite/2; //vrai si le point a jouter n'est pas sur le bord
//fprintf(stderr, "tailleCavite %d \n",taille_cavite/2 );
  //Creation des nouveaux triangles
  tabNewT=calloc(3*nb_triangle_a_ajouter,sizeof(int));
  cmpt=0;
  int ici=0;
  for(i=0;i<3*nb_triangle_a_ajouter;i+=3)
  { //on alterne pour le choix de la position du nouveau point
    if(ici%2==0)
    {
    tabNewT[i]=ptcaviteOrdo[cmpt];
    tabNewT[i+1]=nbPoints+1; //numero du nouveau point a inserer
    tabNewT[i+2]=ptcaviteOrdo[cmpt+1];
    ici++;
    }
    else
    {
      tabNewT[i]=ptcaviteOrdo[cmpt];
      tabNewT[i+1]=ptcaviteOrdo[cmpt+1];
      tabNewT[i+2]=nbPoints+1; //on donne aux points le numero apres e dernier deja utilisé
      ici++;
    }
    cmpt+=2;
  }
  //affichage des nouveaux triangles
  /*
  for(i=0;i<3*(taille_cavite/2);i+=3)
  {
    fprintf(stderr, " T : %d %d %d \n", tabNewT[i],tabNewT[i+1],tabNewT[i+2]);
  }*/
  //fin de creation du tableau des nouveaux triangle

  *TabPt=realloc(*TabPt,(2*nbPoints+2)*sizeof(double)); // on ajoute les coordonnes du points dans la liste des points
  (*TabPt)[2*nbPoints]=Pt[0];
  (*TabPt)[2*nbPoints+1]=Pt[1];

  //on met maintenant a jour la tab des triangles
  cmpt=0;
  for(i=0;i<3*nbTriangles;i+=3)
  {
    if((*TabTri)[i]<0)
    {
      (*TabTri)[i]=tabNewT[cmpt];
      (*TabTri)[i+1]=tabNewT[cmpt+1];
      (*TabTri)[i+2]=tabNewT[cmpt+2];
      cmpt+=3;
    }
  }
  int restant=nb_triangle_a_ajouter-TailleLc;
//fprintf(stderr, "restant %d \n",restant );
*TabTri=realloc(*TabTri,3*(nbTriangles+restant)*sizeof(int) );
for(i=3*nbTriangles;i<3*(nbTriangles+restant);i+=3)
{
    (*TabTri)[i]=tabNewT[cmpt];
    (*TabTri)[i+1]=tabNewT[cmpt+1];
    (*TabTri)[i+2]=tabNewT[cmpt+2];
    cmpt+=3;
}
//creation du tableau d'arete
int *Tabarete_inter=NULL;
//int *Tabarete=NULL;
Tabarete_inter=calloc(6*(nbTriangles+restant),sizeof(int));
//Tabarete=calloc(2,sizeof(int));

cmpt=0;
for(i=0;i<6*(nbTriangles+restant);i+=6)
{
    Tabarete_inter[i]=(*TabTri)[cmpt];
    Tabarete_inter[i+1]=(*TabTri)[cmpt+1];
    Tabarete_inter[i+2]=(*TabTri)[cmpt+1];
    Tabarete_inter[i+3]=(*TabTri)[cmpt+2];
    Tabarete_inter[i+4]=(*TabTri)[cmpt+2];
    Tabarete_inter[i+5]=(*TabTri)[cmpt];
    cmpt+=3;

}
(*Tabarete[0])=Tabarete_inter[0];
(*Tabarete[1])=Tabarete_inter[1];
int TailleArete=2;
int *aretecou=calloc(2,sizeof(int));
int test=0;
for(i=2;i<6*(nbTriangles+restant);i+=2)
{
  aretecou[0]=Tabarete_inter[i];
  aretecou[1]=Tabarete_inter[i+1];
  for(int j=0;j<TailleArete;j+=2)
  {
    if((aretecou[0]==(*Tabarete)[j] && aretecou[1]==(*Tabarete)[j+1]) || (aretecou[0]==(*Tabarete)[j+1] &&  aretecou[1]==(*Tabarete)[j]))
    {
      //arete deja presente dans la liste
      test=1;
    }
  }
  if(test==0)
  {
    //l'arete courante n'est pas dans la liste
    TailleArete+=2;
    *Tabarete=realloc(*Tabarete,TailleArete*sizeof(int));
    (*Tabarete)[TailleArete-2]=aretecou[0];
    (*Tabarete)[TailleArete-1]=aretecou[1];
  }
  test=0;
}
//fprintf(stderr, "Voici le nombre d'arete %d \n",TailleArete/2 );
mesh= fopen("BoiteMesh.mesh","w+"); //option d'ecriture sur le fichier, il faut réecrire tous les points et triangles
fprintf(mesh, "MeshVersionFormatted 2\n");
fprintf(mesh, "\nDimension 2\n\n");
fprintf(mesh, "Vertices\n");
fprintf(mesh, "%d \n",nbPoints+1);
for(i=0;i<nbPoints*2+2;i+=2)
{
  fprintf(mesh, "%lf %lf %d \n",(*TabPt)[i],(*TabPt)[i+1],0);
}
fprintf(mesh, "\nEdges\n");
fprintf(mesh, "%d \n",TailleArete/2 );
for(i=0;i<TailleArete;i+=2)
{
  fprintf(mesh, "%d %d %d \n",(*Tabarete)[i],(*Tabarete)[i+1],1);
}
fprintf(mesh, "\nTriangles\n");
fprintf(mesh, "%d \n",nbTriangles+restant);//on prend 4 points en entree

for(i=0;i<(nbTriangles+restant)*3;i+=3)
{
  fprintf(mesh, "%d %d %d %d \n",(*TabTri)[i],(*TabTri)[i+1],(*TabTri)[i+2],1);
}
fprintf(mesh, "\nend");

fclose(mesh);
free(Lc);
free(tabNewT);
free(ptcaviteOrdo);
//free(Tabarete);
//fprintf(stderr, " ok\n");

}

void ajout_pt(int **TabTri,double **TabPt,int **Tabarete,double *TabP,double *Pt)
{
  int i = 0, TailleLc = 0;
  int *Lc = NULL, *tabNewT = NULL;
  int nbPoints = 0, nbTriangles = 0;
  int cmpt = 0;
  FILE *mesh;
  mesh = fopen("BoiteMesh_finie.mesh","r");
  nbPoints = nbP(mesh);
  //fprintf(stderr,"Voici le nombre de points actuel %d \n",nbPoints);
  fseek(mesh,0,SEEK_SET); //on replace le curseur au début du
  nbTriangles = nbT(mesh);
  fclose(mesh);
  Lc = calloc(1,sizeof(int));
  //fprintf(stderr, "C p point %lf %lf\n",Pt[0],Pt[1] );
  TailleLc = LCirconscrit(&Lc,*TabTri,*TabPt,nbTriangles,Pt); //remplit Lc avec les numeros des triangles dont le cercle inscrit possede le point pT
  fprintf(stderr, "LC ok \n");
//fprintf(stderr,"ok \n");
  int *ptcaviteOrdo = NULL; //il faut l'allouer
  ptcaviteOrdo = calloc(1,sizeof(int));
  int taille_cavite=0;
  taille_cavite = Maj(&Lc,*TabTri,*TabPt,nbTriangles,Pt,&ptcaviteOrdo); //modifie la table des triangles et remplit les sommets de la cavite7
  int nb_triangle_a_ajouter = taille_cavite/2;
//fprintf(stderr, "tailleCavite %d \n",taille_cavite/2 );
  //Creation des nouveaux triangles
  tabNewT = calloc(3*nb_triangle_a_ajouter,sizeof(int));
  cmpt = 0;
  int ici = 0;
  fprintf(stderr, "nombre de triangle a ajoter %d \n",nb_triangle_a_ajouter );
  if(nb_triangle_a_ajouter==1)
  {
    fprintf(stderr, "seulement 1 triangle\n" );
    if( ((TabP[2*(ptcaviteOrdo[0]-1)]+TabP[2*(ptcaviteOrdo[1]-1)])/2. == Pt[0]) && ((TabP[2*(ptcaviteOrdo[0]-1)+1]+TabP[2*(ptcaviteOrdo[1]-1)+1])/2. == Pt[1]))
    {
      tabNewT[0] = ptcaviteOrdo[0];
      tabNewT[1] = nbPoints+1; //numero du nouveau point a inserer
      tabNewT[2] = ptcaviteOrdo[2];

      tabNewT[3] = ptcaviteOrdo[2];
      tabNewT[4] = nbPoints+1; //numero du nouveau point a inserer
      tabNewT[5] = ptcaviteOrdo[1];
    }
    if( ((TabP[2*(ptcaviteOrdo[0]-1)]+TabP[2*(ptcaviteOrdo[2]-1)])/2. == Pt[0]) && ((TabP[2*(ptcaviteOrdo[0]-1)+1]+TabP[2*(ptcaviteOrdo[2]-1)+1])/2. == Pt[1]))
    {
      tabNewT[0] = nbPoints+1;
      tabNewT[1] = ptcaviteOrdo[1]; //numero du nouveau point a inserer
      tabNewT[2] = ptcaviteOrdo[0];

      tabNewT[3] = nbPoints+1;
      tabNewT[4] = ptcaviteOrdo[1]; //numero du nouveau point a inserer
      tabNewT[5] = ptcaviteOrdo[2];
    }
    if(( (TabP[2*(ptcaviteOrdo[1]-1)]+TabP[2*(ptcaviteOrdo[2]-1)])/2. == Pt[0]) && ((TabP[2*(ptcaviteOrdo[2]-1)+1]+TabP[2*(ptcaviteOrdo[1]-1)+1])/2. == Pt[1]))
    {
      tabNewT[0] = ptcaviteOrdo[2];
      tabNewT[1] = nbPoints+1; //numero du nouveau point a inserer
      tabNewT[2] = ptcaviteOrdo[0];

      tabNewT[3] = ptcaviteOrdo[0];
      tabNewT[4] = ptcaviteOrdo[1]; //numero du nouveau point a inserer
      tabNewT[5] =  nbPoints+1;
    }
  }
  else
  {
    for(i = 0; i < 3*nb_triangle_a_ajouter; i+=3)
    { //on alterne pour le choix de la position du nouveau point
      if((   (TabP[2*(ptcaviteOrdo[cmpt]-1)]+TabP[2*(ptcaviteOrdo[cmpt+1]-1)])/2. == Pt[0]) && ((TabP[2*(ptcaviteOrdo[cmpt]-1)+1]+TabP[2*(ptcaviteOrdo[cmpt+1]-1)+1])/2.==Pt[1]) )
      {
        tabNewT=realloc(tabNewT,3*(nb_triangle_a_ajouter-1)*sizeof(int));
        i-=3;
        nb_triangle_a_ajouter--;
      }
      else
      {
        if(ici%2 == 0)
        {
          tabNewT[i] = ptcaviteOrdo[cmpt];
          tabNewT[i+1] = nbPoints+1; //numero du nouveau point a inserer
          tabNewT[i+2] = ptcaviteOrdo[cmpt+1];
          ici++;
        }
        else
        {
          tabNewT[i] = ptcaviteOrdo[cmpt];
          tabNewT[i+1] = ptcaviteOrdo[cmpt+1];
          tabNewT[i+2] = nbPoints+1; //on donne aux points le numero apres e dernier deja utilisé
          ici++;
        }
      }
      cmpt+=2;
    }
  }
  //affichage des nouveaux triangles
  /*
  for(i = 0; i < 3*(taille_cavite/2); i+=3)
  {
    fprintf(stderr, " T : %d %d %d \n", tabNewT[i],tabNewT[i+1],tabNewT[i+2]);
  }*/
  //fin de creation du tableau des nouveaux triangle

  *TabPt = realloc(*TabPt,(2*nbPoints+2)*sizeof(double)); // on ajoute les coordonnes du points dans la liste des points
  (*TabPt)[2*nbPoints] = Pt[0];
  (*TabPt)[2*nbPoints+1] = Pt[1];

  //on met maintenant a jour la tab des triangles
  cmpt = 0;
  for(i = 0; i < 3*nbTriangles; i+=3)
  {
    if((*TabTri)[i] < 0)
    {
      (*TabTri)[i] = tabNewT[cmpt];
      (*TabTri)[i+1] = tabNewT[cmpt+1];
      (*TabTri)[i+2] = tabNewT[cmpt+2];
      cmpt+=3;
    }
  }
  int restant = nb_triangle_a_ajouter-TailleLc;
//fprintf(stderr, "restant %d \n",restant );
*TabTri = realloc(*TabTri,3*(nbTriangles+restant)*sizeof(int) );
for(i = 3*nbTriangles; i < 3*(nbTriangles+restant); i+=3)
{
    (*TabTri)[i] = tabNewT[cmpt];
    (*TabTri)[i+1] = tabNewT[cmpt+1];
    (*TabTri)[i+2] = tabNewT[cmpt+2];
    cmpt+=3;
}

//creation du tableau d'arete
int *Tabarete_inter = NULL;
//int *Tabarete=NULL;
Tabarete_inter = calloc(6*(nbTriangles+restant),sizeof(int));
//Tabarete=calloc(2,sizeof(int));

cmpt = 0;
for(i = 0; i < 6*(nbTriangles+restant); i+=6)
{
    Tabarete_inter[i] = (*TabTri)[cmpt];
    Tabarete_inter[i+1] = (*TabTri)[cmpt+1];
    Tabarete_inter[i+2] = (*TabTri)[cmpt+1];
    Tabarete_inter[i+3] = (*TabTri)[cmpt+2];
    Tabarete_inter[i+4] = (*TabTri)[cmpt+2];
    Tabarete_inter[i+5] = (*TabTri)[cmpt];
    cmpt+=3;

}
(*Tabarete[0]) = Tabarete_inter[0];
(*Tabarete[1]) = Tabarete_inter[1];
int TailleArete = 2;
int *aretecou = calloc(2,sizeof(int));
int test = 0;
for(i = 2; i < 6*(nbTriangles+restant); i+=2)
{
  aretecou[0] = Tabarete_inter[i];
  aretecou[1] = Tabarete_inter[i+1];
  for(int j = 0; j<TailleArete; j+=2)
  {
    if((aretecou[0]==(*Tabarete)[j] && aretecou[1]==(*Tabarete)[j+1]) || (aretecou[0]==(*Tabarete)[j+1] &&  aretecou[1]==(*Tabarete)[j]))
    {
      //arete deja presente dans la liste
      test=1;
    }
  }
  if(test==0)
  {
    //l'arete courante n'est pas dans la liste
    TailleArete+=2;
    *Tabarete=realloc(*Tabarete,TailleArete*sizeof(int));
    (*Tabarete)[TailleArete-2]=aretecou[0];
    (*Tabarete)[TailleArete-1]=aretecou[1];
  }
  test=0;
}
//fprintf(stderr, "Voici le nombre d'arete %d \n",TailleArete/2 );
mesh = fopen("BoiteMesh_finie.mesh","w+");
//FILE* mesh2;
//mesh2 = fopen("BoiteMesh_pt_ajout.mesh","w+"); //option d'ecriture sur le fichier, il faut réecrire tous les points et triangles
fprintf(mesh, "MeshVersionFormatted 2\n");
fprintf(mesh, "\nDimension 2\n\n");
fprintf(mesh, "Vertices\n");
fprintf(mesh, "%d \n",nbPoints+1);
for(i = 0; i < nbPoints*2 + 2; i+=2)
{
  fprintf(mesh, "%lf %lf %d \n",(*TabPt)[i],(*TabPt)[i+1],0);
}
fprintf(mesh, "\nEdges\n");
fprintf(mesh, "%d \n",TailleArete/2 );
for(i = 0; i < TailleArete; i+=2)
{
  fprintf(mesh, "%d %d %d \n",(*Tabarete)[i],(*Tabarete)[i+1],1);
}
fprintf(mesh, "\nTriangles\n");
fprintf(mesh, "%d \n",nbTriangles+restant);//on prend 4 points en entree

for(i = 0; i < (nbTriangles+restant)*3; i+=3)
{
  fprintf(mesh, "%d %d %d %d \n",(*TabTri)[i],(*TabTri)[i+1],(*TabTri)[i+2],1);
}
fprintf(mesh, "\nend");
fclose(mesh);

free(Lc);
free(tabNewT);
free(ptcaviteOrdo);
//free(Tabarete);
//fprintf(stderr, " ok\n");

}
