#include "suppression.h"

//initialisation d'une FILE
//il faut toujours pouvoir initialser l'élément d'une structure
File *initialiser()
{
    File *file = malloc(sizeof(*file));
    file->premier = NULL;

    return file;
}

//affichage de file
void afficherFile(File *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    Element *element = file->premier;

    while (element != NULL)
    {
        printf("%d ", element->nombre);
        element = element->suivant;
    }

    printf("\n");
}


//fonction d'enfillage pour ajouter un element à la file
void enfiler(File *file, int numeroTriangle)
{
    Element *nouveau = malloc(sizeof(*nouveau));
    if (file == NULL || nouveau == NULL)
    {
      fprintf(stderr, "failure \n");
       exit(EXIT_FAILURE);
     }
    nouveau->nombre = numeroTriangle;
    nouveau->suivant = NULL;
//cas ou la file n'est pas vide
    if (file->premier != NULL)
    {
      //on se deplace sur toute la file pour aller jusqu'au dernier
        Element *elementActuel = file->premier;
        while (elementActuel->suivant != NULL)
        {
            elementActuel = elementActuel->suivant;
        } //lorsque que l'on tombe sur un suivant NULL c'est qu'on est arrivé au bout
        elementActuel->suivant = nouveau;
    }
    else // La file est vide, notre élément est le premier
    {
        file->premier = nouveau;
    }
}

//on va supprimer le dernier elements de la file
int defiler(File *file)
{
    int nombreDefiler = 0;
    if (file->premier != NULL)
    {
        Element *elementAEnlever = file->premier;
        nombreDefiler = elementAEnlever->nombre;
        file->premier = elementAEnlever->suivant;
        free(elementAEnlever);
    }
    return nombreDefiler; //retourne l'element supprimer de la fille
}

void suppression_triangles(int **TabTri,int **tabAr,double **tabP,double *tabP_init,FILE *mesh_init,FILE *mesh_delaunay,int nbEdges,int *tabE)
{
  //reconstruction d'un nouveau tableau avec des références
  int *tab_Triangles_ref=NULL;
  //il faut récupérer le nombre de triangles
  int nbTriangles=0;
  nbTriangles=nbT(mesh_delaunay);
  tab_Triangles_ref=calloc(4*nbTriangles,sizeof(int));
  fseek(mesh_delaunay,0,SEEK_SET);
  int nbPoints=nbP(mesh_delaunay);
  fseek(mesh_delaunay,0,SEEK_SET);
  int nb_arete_delaunay=nbE(mesh_delaunay);


  int cmpt=0;
  for(int i=0;i<4*nbTriangles;i+=4)
  {
    tab_Triangles_ref[i]=(*TabTri)[cmpt];
    tab_Triangles_ref[i+1]=(*TabTri)[cmpt+1];
    tab_Triangles_ref[i+2]=(*TabTri)[cmpt+2];
    tab_Triangles_ref[i+3]=0; // on met les references
    cmpt+=3;
  }
  //Construction de la tab des voisins
    double **TabVoisin = malloc(sizeof(*TabVoisin)*nbTriangles); //indice j i    double *p1=NULL,*p2=NULL,*p3=NULL;
    for (int i=0;i<nbTriangles; i++)
    {
      TabVoisin[i]=malloc(sizeof(**TabVoisin)*3);
    }
  rtabVoisin((*TabTri),TabVoisin,nbTriangles);//table des voisins avec valeurs nulles
  File  *maFile=initialiser();
  enfiler(maFile,1);
  //afficherFile(maFile); //test pour verifier la construction de la file
  int ref=1;
  //on ajoute le numero 1 comme premier element de la FILE
  //on va definir la zone de connexite associé au premier triangle
  int encore=0;
  int test=0;

while(encore==0)
  {
    tab_Triangles_ref[4*(maFile->premier->nombre-1)+3]=ref;
    while(maFile->premier !=NULL) // on s'arrete seulement lorsque que la fille est vide
    {
      //on regarde les voisins associé aux triangles courants
      //on va calculer l'arrete commune a chacune des voisins
      //fprintf(stderr, "Tab V %lf \n",TabVoisin[maFile->premier->nombre-1][0] );
      if(TabVoisin[maFile->premier->nombre-1][0]!=-1 && tab_Triangles_ref[4*((int)TabVoisin[maFile->premier->nombre-1][0]-1)+3]==0)
      {
        test=arete_comune(tab_Triangles_ref,maFile->premier->nombre,(int)TabVoisin[maFile->premier->nombre-1][0],nbEdges,tabE);
        if(test==1) //i.e que l'on a pas une arete qui se trouve être une arete de la geometrie
        {
          enfiler(maFile,(int)TabVoisin[maFile->premier->nombre-1][0]);
          tab_Triangles_ref[4*((int)TabVoisin[maFile->premier->nombre-1][0]-1)+3]=ref;
        }
      }
      if(TabVoisin[maFile->premier->nombre-1][1]!=-1 && tab_Triangles_ref[4*((int)TabVoisin[maFile->premier->nombre-1][1]-1)+3]==0)
      {
        test=arete_comune(tab_Triangles_ref,maFile->premier->nombre,(int)TabVoisin[maFile->premier->nombre-1][1],nbEdges,tabE);
        //il faut verifier que l'on a pas deja ajouter le triangles dans la file
        if(test==1) //i.e que l'on a pas une arete qui se trouve être une arete de la geometrie
        {
          enfiler(maFile,(int)TabVoisin[maFile->premier->nombre-1][1]);
          tab_Triangles_ref[4*((int)TabVoisin[maFile->premier->nombre-1][1]-1)+3]=ref;
        }
      }
      if(TabVoisin[maFile->premier->nombre-1][2]!=-1 && tab_Triangles_ref[4*((int)TabVoisin[maFile->premier->nombre-1][2]-1)+3]==0 )
      {
        test=arete_comune(tab_Triangles_ref,maFile->premier->nombre,(int)TabVoisin[maFile->premier->nombre-1][2],nbEdges,tabE);
        if(test==1) //i.e que l'on a pas une arete qui se trouve être une arete de la geometrie
        {
          enfiler(maFile,(int)TabVoisin[maFile->premier->nombre-1][2]);
          tab_Triangles_ref[4*((int)TabVoisin[maFile->premier->nombre-1][2]-1)+3]=ref;
        }
      }
      defiler(maFile); //on enleve l'element regarder et on regarde le suivant dans la file
    }
    fprintf(stderr, "ok \n");
   encore=ref_non_rempl(tab_Triangles_ref,nbTriangles,maFile); //on verifie ici si toutes les references sont differentes de 0
   ref++;
   //fprintf(stderr, "encore %d nb %d \n",encore,maFile->premier->nombre );
  //  si on trouve un triangle dont la ref est non nul il devient le premier element de la pile
}
//fin du coloriage des differentes zones
/*
//affichage de la tab avec reference
fprintf(stderr, "affichage de la tab avec reference \n");
for(int i=0;i<4*nbTriangles;i+=4)
{
  fprintf(stderr, "%d %d %d %d \n",tab_Triangles_ref[i],tab_Triangles_ref[i+1],tab_Triangles_ref[i+2],tab_Triangles_ref[i+3]);
}
*/

//on va enregistrer ce coloriage pour visualiser les resultats mais etape non necessaire pour la suppression des triangles


FILE *mesh_coloriage=NULL;
mesh_coloriage= fopen("BoiteMesh_colorie.mesh","w+"); //option d'ecriture sur le fichier, il faut réecrire tous les points et triangles
fprintf(mesh_coloriage, "MeshVersionFormatted 2\n");
fprintf(mesh_coloriage, "\nDimension 2\n\n");
fprintf(mesh_coloriage, "Vertices\n");
fprintf(mesh_coloriage, "%d \n",nbPoints);
for(int i=0;i<nbPoints*2;i+=2)
{
  fprintf(mesh_coloriage, "%lf %lf %d \n",(*tabP)[i],(*tabP)[i+1],0);
}
fprintf(mesh_coloriage, "\nEdges\n");
fprintf(mesh_coloriage, "%d \n",nb_arete_delaunay);
for(int i=0;i<2*nb_arete_delaunay;i+=2)
{
  fprintf(mesh_coloriage, "%d %d %d \n",(*tabAr)[i],(*tabAr)[i+1],1);
}
fprintf(mesh_coloriage, "\nTriangles\n");
fprintf(mesh_coloriage, "%d \n",nbTriangles);//on prend 4 points en entree

for(int i=0;i<4*nbTriangles;i+=4)
{
  fprintf(mesh_coloriage, "%d %d %d %d \n",tab_Triangles_ref[i],tab_Triangles_ref[i+1],tab_Triangles_ref[i+2],tab_Triangles_ref[i+3]);
}
fprintf(mesh_coloriage, "\nend");
fclose(mesh_coloriage);

//maintenant il s'agit d'un probleme d'identification de qu'elles sont les zones a supprimer
//il n'est pas claire de savoir laqu'elle est bien a supprimer lorsque l'on a plus de deux zones
//on considére pour l'instant deux zones
//on cherche la zone possédant le premier coins de la boite
int numero_zone=0;
numero_zone=recherche_zone2(tab_Triangles_ref,nbTriangles);
//fprintf(stderr, "Voici le numero de la zone boite  : %d \n",numero_zone);
int T=0,nv_nbTriangles=0;
if(ref<=2)
{
  //creation de la triangulation sans la boite ENGLOBANTE
  *TabTri=realloc(*TabTri,1*sizeof(int)); //on ve reremplir le tableau de triangle en le réécrasant
  T=0; //TAILLE ACTUEL DU TABLEAU DE TRIANGLE
  cmpt=0;
  for(int i=0;i<4*nbTriangles;i+=4)
  {
    if(tab_Triangles_ref[i+3]!=numero_zone)
    {
      T+=3;
      *TabTri=realloc(*TabTri,T*sizeof(int)); //on ve reremplir le tableau de triangle en le réécrasant
      (*TabTri)[cmpt]=tab_Triangles_ref[i]-4;
      (*TabTri)[cmpt+1]=tab_Triangles_ref[i+1]-4;
      (*TabTri)[cmpt+2]=tab_Triangles_ref[i+2]-4; //on enleve les 4 points du coins de la boite
      cmpt+=3;
    }
  }
  nv_nbTriangles=(cmpt/3);
  /*
  for(int i=0; i<T;i+=3)
  {
    fprintf(stderr, "%d %d %d \n",(*TabTri)[i],(*TabTri)[i+1],(*TabTri)[i+2]);
  }*/
  //il faut maintenant mettre à jour la table des points , en faite la table des points redevient la table des points d'origine
  *tabP=realloc(*tabP,2*(nbPoints-4)*sizeof(double));
  //fprintf(stderr, "nb p : %d \n",2*(nbPoints-4));
  for(int i=0;i<2*(nbPoints-4);i+=2)
  {
    (*tabP)[i]=tabP_init[i];
    (*tabP)[i+1]=tabP_init[i+1];

  }
}
else
{
  //il y a donc des zones internes a supprimer
  int zone_conserve=recherche_zone_conserver(tab_Triangles_ref,nbTriangles,numero_zone); //numero_zone est la zone de la boite externe que l'on a supprime
  //fprintf(stderr, "Voici le numero de la zone a conserver : %d \n",zone_conserve);
  //creation de la triangulation sans la boite ENGLOBANTE
  *TabTri=realloc(*TabTri,1*sizeof(int)); //on ve reremplir le tableau de triangle en le réécrasant
  T=0; //TAILLE ACTUEL DU TABLEAU DE TRIANGLE
  cmpt=0;
  for(int i=0;i<4*nbTriangles;i+=4)
  {
    if(tab_Triangles_ref[i+3]==zone_conserve)
    {
      T+=3;
      *TabTri=realloc(*TabTri,T*sizeof(int)); //on ve reremplir le tableau de triangle en le réécrasant
      (*TabTri)[cmpt]=tab_Triangles_ref[i]-4;
      (*TabTri)[cmpt+1]=tab_Triangles_ref[i+1]-4;
      (*TabTri)[cmpt+2]=tab_Triangles_ref[i+2]-4; //on enleve les 4 points du coins de la boite
      cmpt+=3;
    }
  }
  nbTriangles=cmpt/3;
  //fprintf(stderr, "Nouveau nombre de triangles %d \n", nbTriangles);
  nv_nbTriangles=(cmpt/3);
  //il faut maintenant mettre à jour la table des points , en faite la table des points redevient la table des points d'origine
  *tabP=realloc(*tabP,2*(nbPoints-4)*sizeof(double));
  //fprintf(stderr, "nb p : %d \n",2*(nbPoints-4));
  for(int i=0;i<2*(nbPoints-4);i+=2)
  {
    (*tabP)[i]=tabP_init[i];
    (*tabP)[i+1]=tabP_init[i+1];
  }
}

nb_arete_delaunay=modification_arete(TabTri,tabAr,nbTriangles); //on renvoi le nombre d'arete
FILE *mesh_suppression_boite=NULL;
mesh_suppression_boite= fopen("BoiteMesh_finie.mesh","w+"); //option d'ecriture sur le fichier, il faut réecrire tous les points et triangles
fprintf(mesh_suppression_boite, "MeshVersionFormatted 2\n");
fprintf(mesh_suppression_boite, "\nDimension 2\n\n");
fprintf(mesh_suppression_boite, "Vertices\n");
fprintf(mesh_suppression_boite, "%d \n",nbPoints-4);
for(int i=0;i<(nbPoints-4)*2;i+=2)
{
  fprintf(mesh_suppression_boite, "%lf %lf %d \n",(*tabP)[i],(*tabP)[i+1],0);
}
fprintf(mesh_suppression_boite, "\nEdges\n");
fprintf(mesh_suppression_boite, "%d \n",nb_arete_delaunay);
for(int i=0;i<2*nb_arete_delaunay;i+=2)
{
  fprintf(mesh_suppression_boite, "%d %d %d \n",(*tabAr)[i],(*tabAr)[i+1],1);
}
fprintf(mesh_suppression_boite, "\nTriangles\n");
fprintf(mesh_suppression_boite, "%d \n",nv_nbTriangles);//on prend 4 points en entree

for(int i=0;i<3*nv_nbTriangles;i+=3)
{
  fprintf(mesh_suppression_boite, "%d %d %d %d \n",(*TabTri)[i],(*TabTri)[i+1],(*TabTri)[i+2],0);
}
fprintf(mesh_suppression_boite, "\nend");
fclose(mesh_suppression_boite);

free(tab_Triangles_ref);
for (int i=0;i<nbTriangles; i++)
{
  free(TabVoisin[i]);
}
free(TabVoisin);

}



int ref_non_rempl(int *tab_Triangles_ref,int nbTriangles,File *maFile)
{
  int test=1;
  int i=0;
  int ref=0;
  //on verifie si une des references est encore nulle
  while(test==1 && i<4*nbTriangles)
  {

    ref=tab_Triangles_ref[i+3];
    if(ref==0)
    {
      //la reference n'a pas était changé
      test=0;
      enfiler(maFile,i/4+1); //numerotation des triangles
    }
    i+=4;
  }
  return test;
}

//prend deux triangles voisins et definis si leur arete commune est une arete sur le bord de la discretisation
int arete_comune(int *tab_Triangles_ref,int nbT1,int nbT2,int nbEdges,int *tabE)
{
  int *ar_cou1T1=calloc(2,sizeof(int));
  int *ar_cou2T1=calloc(2,sizeof(int));
  int *ar_cou3T1=calloc(2,sizeof(int));

  int *ar_cou1T2=calloc(2,sizeof(int));
  int *ar_cou2T2=calloc(2,sizeof(int));
  int *ar_cou3T2=calloc(2,sizeof(int));

  int *ar_commune=calloc(2,sizeof(int));

  ar_cou1T1[0]=tab_Triangles_ref[4*(nbT1-1)];
  ar_cou1T1[1]=tab_Triangles_ref[4*(nbT1-1)+1];
  ar_cou2T1[0]=tab_Triangles_ref[4*(nbT1-1)+1];
  ar_cou2T1[1]=tab_Triangles_ref[4*(nbT1-1)+2];
  ar_cou3T1[0]=tab_Triangles_ref[4*(nbT1-1)+2];
  ar_cou3T1[1]=tab_Triangles_ref[4*(nbT1-1)];

  ar_cou1T2[0]=tab_Triangles_ref[4*(nbT2-1)];
  ar_cou1T2[1]=tab_Triangles_ref[4*(nbT2-1)+1];
  ar_cou2T2[0]=tab_Triangles_ref[4*(nbT2-1)+1];
  ar_cou2T2[1]=tab_Triangles_ref[4*(nbT2-1)+2];
  ar_cou3T2[0]=tab_Triangles_ref[4*(nbT2-1)+2];
  ar_cou3T2[1]=tab_Triangles_ref[4*(nbT2-1)];

  if((ar_cou1T1[0]==ar_cou1T2[0] && ar_cou1T1[1]==ar_cou1T2[1]) || (ar_cou1T1[0]==ar_cou1T2[1] && ar_cou1T1[1]==ar_cou1T2[0] ))
  {
    //on a trouvé l'arete commune
    ar_commune[0]=ar_cou1T1[0];
    ar_commune[1]=ar_cou1T1[1];
  }
  if((ar_cou2T1[0]==ar_cou1T2[0] && ar_cou2T1[1]==ar_cou1T2[1]) || (ar_cou2T1[0]==ar_cou1T2[1] && ar_cou2T1[1]==ar_cou1T2[0]) )
  {
    //on a trouvé l'arete commune
    ar_commune[0]=ar_cou2T1[0];
    ar_commune[1]=ar_cou2T1[1];
  }
  if((ar_cou3T1[0]==ar_cou1T2[0] && ar_cou3T1[1]==ar_cou1T2[1]) || (ar_cou3T1[0]==ar_cou1T2[1] && ar_cou3T1[1]==ar_cou1T2[0]) )
  {
    //on a trouvé l'arete commune
    ar_commune[0]=ar_cou3T1[0];
    ar_commune[1]=ar_cou3T1[1];
  }


  if((ar_cou1T1[0]==ar_cou2T2[0] && ar_cou1T1[1]==ar_cou2T2[1]) || (ar_cou1T1[0]==ar_cou2T2[1] && ar_cou1T1[1]==ar_cou2T2[0] ))
  {
    //on a trouvé l'arete commune
    ar_commune[0]=ar_cou1T1[0];
    ar_commune[1]=ar_cou1T1[1];
  }
  if((ar_cou2T1[0]==ar_cou2T2[0] && ar_cou2T1[1]==ar_cou2T2[1]) || (ar_cou2T1[0]==ar_cou2T2[1] && ar_cou2T1[1]==ar_cou2T2[0]) )
  {
    //on a trouvé l'arete commune
    ar_commune[0]=ar_cou2T1[0];
    ar_commune[1]=ar_cou2T1[1];
  }
  if((ar_cou3T1[0]==ar_cou2T2[0] && ar_cou3T1[1]==ar_cou2T2[1]) || (ar_cou3T1[0]==ar_cou2T2[1] && ar_cou3T1[1]==ar_cou2T2[0]) )
  {
    //on a trouvé l'arete commune
    ar_commune[0]=ar_cou3T1[0];
    ar_commune[1]=ar_cou3T1[1];
  }

  if((ar_cou1T1[0]==ar_cou3T2[0] && ar_cou1T1[1]==ar_cou3T2[1]) || (ar_cou1T1[0]==ar_cou3T2[1] && ar_cou1T1[1]==ar_cou3T2[0] ))
  {
    //on a trouvé l'arete commune
    ar_commune[0]=ar_cou1T1[0];
    ar_commune[1]=ar_cou1T1[1];
  }
  if((ar_cou2T1[0]==ar_cou3T2[0] && ar_cou2T1[1]==ar_cou3T2[1]) || (ar_cou2T1[0]==ar_cou3T2[1] && ar_cou2T1[1]==ar_cou3T2[0]) )
  {
    //on a trouvé l'arete commune
    ar_commune[0]=ar_cou2T1[0];
    ar_commune[1]=ar_cou2T1[1];
  }
  if((ar_cou3T1[0]==ar_cou3T2[0] && ar_cou3T1[1]==ar_cou3T2[1]) || (ar_cou3T1[0]==ar_cou3T2[1] && ar_cou3T1[1]==ar_cou3T2[0]) )
  {
    //on a trouvé l'arete commune
    ar_commune[0]=ar_cou3T1[0];
    ar_commune[1]=ar_cou3T1[1];
  }
  int test=0;
  int cmpt=0;
  //fprintf(stderr, "T1 : %d, T2 : %d , arete commune %d %d \n",nbT1,nbT2,ar_commune[0],ar_commune[1]);
  //Maintenant que l'on a trouve l'arete commune on regare si c'est une arete du bord
  int *arete_bord=NULL;
  tabE[0]=1;
  arete_bord=calloc(2,sizeof(int));
  while(test==0 && cmpt<2*nbEdges)
  {
    arete_bord[0]=tabE[cmpt]+4;
    arete_bord[1]=tabE[cmpt+1]+4;
    //fprintf(stderr, "ar bord %d %d et %d %d  \n", arete_bord[0],arete_bord[1],ar_commune[0],ar_commune[1]);
    if( (arete_bord[0]==ar_commune[0] && arete_bord[1]==ar_commune[1]) || (arete_bord[1]==ar_commune[0] && arete_bord[0]==ar_commune[1]))
    {
      //on est bien sur une arete du bord du domaine
      //fprintf(stderr, "ici \n");
      test=1;
    }
    cmpt+=2;
  }
  free(ar_cou1T1);
  free(ar_cou2T1);
  free(ar_cou3T1);
  free(ar_cou1T2);
  free(ar_cou2T2);
  free(ar_cou3T2);
  free(ar_commune);
  free(arete_bord);
  if(test==0)
  {
    //l'arete n'est pas une arete du bord
    //fprintf(stderr, "%s\n",arete_bord[0],arete_bord[1] );
    return 1;
  }
  else
  {
    return 0;
  }
}

int recherche_zone2(int *tab_Triangles_ref,int nbTriangles)
{
//on va rechercher 1 dans la liste des rtriangles qui correspond à un point du coin de la boite et donc de la zone a supprimer
int stop=1;
int cmpt=0;
int ref_rechercher=0; //aucune ref n'est a zero
while(stop==1 && cmpt<4*nbTriangles)
{
  if(tab_Triangles_ref[cmpt]==1 || tab_Triangles_ref[cmpt+1]==1  || tab_Triangles_ref[cmpt+2]==1)
  {
    ref_rechercher=tab_Triangles_ref[cmpt+3];
    stop=0;
  }
  cmpt+=4;
}
return ref_rechercher;
}



int modification_arete(int **TabTri,int **tabAr,int nbTriangles)
{
  *tabAr=realloc(*tabAr,6*sizeof(int));
  //on va initialiser la table des aretes avec les 3 premieres aretes du premier Triangles
  (*tabAr)[0]=(*TabTri)[0];
  (*tabAr)[1]=(*TabTri)[1];
  (*tabAr)[2]=(*TabTri)[1];
  (*tabAr)[3]=(*TabTri)[2];
  (*tabAr)[4]=(*TabTri)[2];
  (*tabAr)[5]=(*TabTri)[0];

  int TailleArete=6;
  int *aretecou1=calloc(2,sizeof(int));
  int *aretecou2=calloc(2,sizeof(int));
  int *aretecou3=calloc(2,sizeof(int));

  int test1=0;
  int test2=0;
  int test3=0;

  for(int i=3;i<3*nbTriangles;i+=3)
  {
    aretecou1[0]=(*TabTri)[i];
    aretecou1[1]=(*TabTri)[i+1];

    aretecou2[0]=(*TabTri)[i+1];
    aretecou2[1]=(*TabTri)[i+2];

    aretecou3[0]=(*TabTri)[i+2];
    aretecou3[1]=(*TabTri)[i];

    for(int j=0;j<TailleArete;j+=2)
    {
      if((aretecou1[0]==(*tabAr)[j] && aretecou1[1]==(*tabAr)[j+1]) || (aretecou1[1]==(*tabAr)[j] && aretecou1[0]==(*tabAr)[j+1]) )
      {
        //arete deja presente dans la liste
        test1=1;
      }
    }
    if(test1==0)
    {
      //l'arete courante 1 n'est pas dans la liste
      TailleArete+=2;
      (*tabAr)=realloc(*tabAr,TailleArete*sizeof(int));
      (*tabAr)[TailleArete-2]=aretecou1[0];
      (*tabAr)[TailleArete-1]=aretecou1[1];
    }
    test1=0;

    for(int j=0;j<TailleArete;j+=2)
    {
      if( (aretecou2[0]==(*tabAr)[j] && aretecou2[1]==(*tabAr)[j+1]) || ( aretecou2[1]==(*tabAr)[j] && aretecou2[0]==(*tabAr)[j+1] ) )
      {
        //arete deja presente dans la liste
        test2=1;
      }
    }
    if(test2==0)
    {
      //l'arete courante 2 n'est pas dans la liste
      TailleArete+=2;
      (*tabAr)=realloc(*tabAr,TailleArete*sizeof(int));
      (*tabAr)[TailleArete-2]=aretecou2[0];
      (*tabAr)[TailleArete-1]=aretecou2[1];
    }


    test2=0;
    for(int j=0;j<TailleArete;j+=2)
    {
      if((aretecou3[0]==(*tabAr)[j] && aretecou3[1]==(*tabAr)[j+1]) || (aretecou3[1]==(*tabAr)[j] && aretecou3[0]==(*tabAr)[j+1]) )
      {
        //arete deja presente dans la liste
        test3=1;
      }
    }
    if(test3==0)
    {
      //l'arete courante 3 n'est pas dans la liste
      TailleArete+=2;
      (*tabAr)=realloc(*tabAr,TailleArete*sizeof(int));
      (*tabAr)[TailleArete-2]=aretecou3[0];
      (*tabAr)[TailleArete-1]=aretecou3[1];
    }
    test3=0;
  }


  free(aretecou1);
  free(aretecou2);
  free(aretecou3);

  return TailleArete/2.;
}

int recherche_zone_conserver(int *tab_Triangles_ref,int nbTriangles,int numero_zone)
{
  int stop=1;
  int cmpt=0;
  int ref_rechercher=0; //aucune ref n'est a zero
  while(stop==1 && cmpt<4*nbTriangles)
  {
    if( (tab_Triangles_ref[cmpt]==5 || tab_Triangles_ref[cmpt+1]==5  || tab_Triangles_ref[cmpt+2]==5) && tab_Triangles_ref[cmpt+3]!=numero_zone)
    {
      ref_rechercher=tab_Triangles_ref[cmpt+3];
      stop=0;
    }
    cmpt+=4;
  }
  return ref_rechercher;
}
