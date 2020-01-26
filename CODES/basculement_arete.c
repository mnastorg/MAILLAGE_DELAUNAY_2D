#include "basculement_arete.h"

void basculement(int *TabTri,int *Tabarete,double *TabPt,FILE* mesh_init,FILE* mesh_delaunay)
{
  int nb_arete_init=0,nb_arete_delaunay=0;
  int pt1=0,pt2=0,nbTriangles=0;
  int *Liste_Tri1= NULL;
  Liste_Tri1 = calloc(1,sizeof(int));
  int *Liste_Tri2=NULL;
  Liste_Tri2 = calloc(1,sizeof(int));
  int Taille_liste_Tri1=0,Taille_liste_Tri2=0;
  int *TriangleAChanger=calloc(2,sizeof(int));
  int *aretecou=calloc(2,sizeof(int));
  //FILE *mesh_delaunay=NULL;
  //FILE *mesh_init=NULL;
  //on recupere le nombre d'arete des deux fichier de maillage

  //mesh_init=fopen("trou.mesh","r");
  int nP = 0, nT = 0;
  nP = nbP(mesh_init);
  fseek(mesh_init,0,SEEK_SET);
  nT = nbT(mesh_init);
  fseek(mesh_init,0,SEEK_SET);
  nb_arete_init = nbE(mesh_init);
  double *tabP = calloc(2.*nP,sizeof(double)); //+1 par rapport au nb d'éléments
  int *Tab_arete_init = calloc(2.*nb_arete_init,sizeof(int));
  int *tabT = calloc(3.*nT,sizeof(int));
  fseek(mesh_init,0,SEEK_SET);
  remplissage(mesh_init,tabP,tabT,Tab_arete_init);
  //fclose(mesh_init);


  //mesh_delaunay=fopen("BoiteMesh.mesh","r");
  nb_arete_delaunay=nbE(mesh_delaunay);
  fseek(mesh_delaunay,0,SEEK_SET);
  nbTriangles=nbT(mesh_delaunay);
  fseek(mesh_delaunay,0,SEEK_SET);
  int nbPoints=nbP(mesh_delaunay);
  //fclose(mesh_delaunay);

//on fait les modifs pour avoir un probleme
/*Tabarete[82]=7;
Tabarete[83]=1;
TabTri[3*nbTriangles-1]=16;
TabTri[3*nbTriangles-2]=7;
TabTri[3*nbTriangles-3]=1;
TabTri[3*nbTriangles-4]=7;
TabTri[3*nbTriangles-5]=5;
TabTri[3*nbTriangles-6]=1;*/
//on creer un probleme qui n'existe pas on l'enregistre pour montrer la comparaison a la fin
//dans le cas d'un vrai probleme cette partie d'ecriture de fichier et a supprimer
/*FILE *mesh2=NULL;
mesh2= fopen("BoiteMesh_problem.mesh","w+"); //option d'ecriture sur le fichier, il faut réecrire tous les points et triangles
fprintf(mesh2, "MeshVersionFormatted 2\n");
fprintf(mesh2, "\nDimension 2\n\n");
fprintf(mesh2, "Vertices\n");
fprintf(mesh2, "%d \n",nbPoints);
for(int i=0;i<nbPoints*2;i+=2)
{
  fprintf(mesh2, "%lf %lf %d \n",TabPt[i],TabPt[i+1],0);
}
fprintf(mesh2, "\nEdges\n");
fprintf(mesh2, "%d \n",nb_arete_delaunay);
for(int i=0;i<2*nb_arete_delaunay;i+=2)
{
  fprintf(mesh2, "%d %d %d \n",Tabarete[i],Tabarete[i+1],1);
}
fprintf(mesh2, "\nTriangles\n");
fprintf(mesh2, "%d \n",nbTriangles);//on prend 4 points en entree

for(int i=0;i<3*nbTriangles;i+=3)
{
  fprintf(mesh2, "%d %d %d %d \n",TabTri[i],TabTri[i+1],TabTri[i+2],1);
}
fprintf(mesh2, "\nend");
fclose(mesh2); //fin de la partie a enlever dans le cas d'un vraie basculement
*/
for (int i=0; i<2*nb_arete_init;i+=2)
{
  Tab_arete_init[i]+=4;
  Tab_arete_init[i+1]+=4;
}

  //on doit verifier que toutes les aretes de Tab_arete_init sont dans Tabarete
  int ok=0;
  for(int i=0;i<2*nb_arete_init ; i+=2)
  {
    ok=0;
    for(int j=0 ; j<2*nb_arete_delaunay;j+=2)
    { //FAIRE ATTENTON AU DECALAGE D'INDICE SUR LA NUMEROTATION DES POINTS
      if(Tabarete[j]==Tab_arete_init[i] && Tabarete[j+1]==Tab_arete_init[i+1])
      {
        ok=1;
      }
      if(Tabarete[j]==Tab_arete_init[i+1] && Tabarete[j+1]==Tab_arete_init[i])
      {
        ok=1;
      }
      //fprintf(stderr, "test %d %d %d %d \n",Tabarete[j],Tabarete[j+1],Tab_arete_init[i],Tab_arete_init[i+1]);
    }
    if(ok==0) //alors l'arete n'a pas etait trouve
    {
      //fprintf(stderr, "init %d %d \n",Tab_arete_init[i],Tab_arete_init[i+1]);
      pt1=Tab_arete_init[i];
      pt2=Tab_arete_init[i+1];

      //on cherche tous les triangles possédant ce point
      for(int k=0;k<3*nbTriangles;k+=3)
      {
        if(TabTri[k]==pt1 || TabTri[k+1]==pt1 || TabTri[k+2]==pt1)
        {
          //on l'ajoute a la liste des triangles
          Taille_liste_Tri1++;
          Liste_Tri1=realloc(Liste_Tri1,Taille_liste_Tri1*sizeof(int));
          Liste_Tri1[Taille_liste_Tri1-1]=k/3+1; //numero du triangle
        }
      }
      //fprintf(stderr, "tailleliste %d\n",Taille_liste_Tri1 );
      //on cherche tous les triangles possédant ce point
      for(int k=0;k<3*nbTriangles;k+=3)
      {
        //fprintf(stderr, "tab Tri %d %d %d \n", TabTri[k],TabTri[k+1],TabTri[k+2]);
        if(TabTri[k]==pt2 || TabTri[k+1]==pt2 || TabTri[k+2]==pt2)
        {
          //on l'ajoute a la liste des triangles
          Taille_liste_Tri2++;
          Liste_Tri2=realloc(Liste_Tri2,Taille_liste_Tri2*sizeof(int));
          Liste_Tri2[Taille_liste_Tri2-1]=k/3+1; //numero du triangle
        }
      }
      //fprintf(stderr, "tailleliste %d\n",Taille_liste_Tri2 );
    }
    //on cherche l'arete commune entre les deux triangles
    //on modifie les triangles pour creer le probleme associe aux aretes

//fprintf(stderr, "taille Liste 2 : %d\n", Taille_liste_Tri2);
    int test=0;
    int taille=0;
    /*
    for(int y=0;y<Taille_liste_Tri1;y++)
    {
      fprintf(stderr, "Liste 1 %d \n", Liste_Tri1[y]);
    }*/
    while(test==0 & ok==0)
    {
      //il faut identifier l'arete que l'on recherche
      if(pt1==TabTri[3*(Liste_Tri1[taille]-1)])
      {
        aretecou[0]=TabTri[3*(Liste_Tri1[taille]-1)+1];
        aretecou[1]=TabTri[3*(Liste_Tri1[taille]-1)+2];
      }
      else if(pt1==TabTri[3*(Liste_Tri1[taille]-1)+1])
      {
        aretecou[0]=TabTri[3*(Liste_Tri1[taille]-1)];
        aretecou[1]=TabTri[3*(Liste_Tri1[taille]-1)+2];
      }
      else
      {
        aretecou[0]=TabTri[3*(Liste_Tri1[taille]-1)];
        aretecou[1]=TabTri[3*(Liste_Tri1[taille]-1)+1];
      }
    //  fprintf(stderr, "%d %d %d \n",pt1,aretecou[0],aretecou[1] );
      //on cherche cette arete dans la seconde liste
      for(int t=0;t<Taille_liste_Tri2;t++)
      {
          if( (aretecou[0]==TabTri[3*(Liste_Tri2[t]-1)] && aretecou[1]==TabTri[3*(Liste_Tri2[t]-1)+1]) || (aretecou[0]==TabTri[3*(Liste_Tri2[t]-1)+1] && aretecou[1]==TabTri[3*(Liste_Tri2[t]-1)]))
          {
            //on est sur la meme arete
            test=1;
            //fprintf(stderr, "ici 1 \n");
            //on enregistre les deux triangles2
            TriangleAChanger[0]=Liste_Tri1[taille];
            TriangleAChanger[1]=Liste_Tri2[t];
          }
          if((aretecou[0]==TabTri[3*(Liste_Tri2[t]-1)+1] && aretecou[1]==TabTri[3*(Liste_Tri2[t]-1)+2]) || (aretecou[0]==TabTri[3*(Liste_Tri2[t]-1)+2] && aretecou[1]==TabTri[3*(Liste_Tri2[t]-1)]))
          {
            //on est sur la meme arete
            test=1;
            //fprintf(stderr, "ici 2 \n");
            //on enregistre les deux triangles2
            TriangleAChanger[0]=Liste_Tri1[taille];
            TriangleAChanger[1]=Liste_Tri2[t];
          }
          if((aretecou[0]==TabTri[3*(Liste_Tri2[t]-1)+2] && aretecou[1]==TabTri[3*(Liste_Tri2[t]-1)]) || (aretecou[0]==TabTri[3*(Liste_Tri2[t]-1)] && aretecou[1]==TabTri[3*(Liste_Tri2[t]-1)+2]))
          {
            //on est sur la meme arete
            test=1;
            //fprintf(stderr, "ici 3 : %d %d t %d \n",TabTri[3*(Liste_Tri2[t]-1)+2],TabTri[3*(Liste_Tri2[t]-1)],Liste_Tri2[t]);

            //on enregistre les deux triangles2
            TriangleAChanger[0]=Liste_Tri1[taille];
            TriangleAChanger[1]=Liste_Tri2[t];
          }
      }
      taille++;
    }
    //fprintf(stderr, "Triangle a changer %d %d \n",TriangleAChanger[0],TriangleAChanger[1]);
    //creation des deux nouveaux triangles2 on remplace ceux que l'on a modifie
    if(TriangleAChanger[0]!=0 && TriangleAChanger[1]!=0)
    {
      TabTri[3*(TriangleAChanger[0]-1)]=pt2;
      TabTri[3*(TriangleAChanger[0]-1)+1]= aretecou[0];
      TabTri[3*(TriangleAChanger[0]-1)+2]= pt1;
      TabTri[3*(TriangleAChanger[1]-1)]=pt1;
      TabTri[3*(TriangleAChanger[1]-1)+1]=pt2;
      TabTri[3*(TriangleAChanger[1]-1)+2]=aretecou[1];
      //fprintf(stderr, "ok \n");
      //on modifie la tab des aretes
      for(int k=0;k<2*nb_arete_delaunay;k+=2)
      {
        if((Tabarete[k]==aretecou[0] && Tabarete[k+1]==aretecou[1]) && (Tabarete[k]==aretecou[1] && Tabarete[k+1]==aretecou[0]))
        {
          Tabarete[k]=pt2;
          Tabarete[k+1]=pt1;
        }
      }
    }
TriangleAChanger[0]=0;
TriangleAChanger[1]=0;
ok=0;
Taille_liste_Tri1=0;
Taille_liste_Tri2=0;
}

//on reecrit dans un nouveau fichier la correction faite
FILE *mesh=NULL;
mesh= fopen("BoiteMesh_modifie.mesh","w+"); //option d'ecriture sur le fichier, il faut réecrire tous les points et triangles
fprintf(mesh, "MeshVersionFormatted 2\n");
fprintf(mesh, "\nDimension 2\n\n");
fprintf(mesh, "Vertices\n");
fprintf(mesh, "%d \n",nbPoints);
for(int i=0;i<nbPoints*2;i+=2)
{
  fprintf(mesh, "%lf %lf %d \n",TabPt[i],TabPt[i+1],0);
}
fprintf(mesh, "\nEdges\n");
fprintf(mesh, "%d \n",nb_arete_delaunay);
for(int i=0;i<2*nb_arete_delaunay;i+=2)
{
  fprintf(mesh, "%d %d %d \n",Tabarete[i],Tabarete[i+1],1);
}
fprintf(mesh, "\nTriangles\n");
fprintf(mesh, "%d \n",nbTriangles);//on prend 4 points en entree

for(int i=0;i<3*nbTriangles;i+=3)
{
  fprintf(mesh, "%d %d %d %d \n",TabTri[i],TabTri[i+1],TabTri[i+2],1);
}
fprintf(mesh, "\nend");
fclose(mesh);

free(tabP);
free(Tab_arete_init);
free(tabT);
free(Liste_Tri1);
free(Liste_Tri2);
free(TriangleAChanger);
free(aretecou);
}
