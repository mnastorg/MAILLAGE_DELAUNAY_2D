#include "basculement_niveau2.h"

void basculement(int *TabTri,int *Tabarete,double *TabPt,FILE* mesh_init,FILE* mesh_delaunay)
{
  int nb_arete_init=0,nb_arete_delaunay=0;
  int pt1=0,pt2=0,nbTriangles=0;
  int *Liste_ar_inter= NULL;
  Liste_ar_inter = calloc(1,sizeof(int));
  int Taille_liste=0;
  int *TriangleAChanger=calloc(2,sizeof(int));
  int *aretecou=calloc(2,sizeof(int));
  //FILE *mesh_delaunay=NULL;
  //FILE *mesh_init=NULL;
  //on recupere le nombre d'arete des deux fichier de maillage

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
  int t1=0,t2=0,t3=0,t4=0,t5=0;
int ici=0;
int svg1=0,svg2=0,svg3=0;

  nb_arete_delaunay=nbE(mesh_delaunay);
  fseek(mesh_delaunay,0,SEEK_SET);
  nbTriangles=nbT(mesh_delaunay);
  fseek(mesh_delaunay,0,SEEK_SET);
  int nbPoints=nbP(mesh_delaunay);


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
      fprintf(stderr, "init %d %d \n",Tab_arete_init[i],Tab_arete_init[i+1]);

      pt1=Tab_arete_init[i]-1;
      pt2=Tab_arete_init[i+1]-1;
      double U1=0,U2=0,V1=0,V2=0,W1x=0,W1y=0,W2x=0,W2y=0,W3x=0,W3y=0,W4x=0,W4y=0;
      U1=TabPt[2*pt2]-TabPt[2*pt1];
      U2=TabPt[2*pt2+1]-TabPt[2*pt1+1];
      //on cherche tous les triangles possédant ce point
      for(int k=0;k<3*nbTriangles;k+=3)
      {

        svg1=TabTri[k+1];
        svg2=TabTri[k];
        svg3=TabTri[k+2];

          V1=TabPt[2*(svg1-1)]-TabPt[2*(TabTri[k]-1)]; //A' et B' segment du triangle
          V2=TabPt[2*(svg1-1)+1]-TabPt[2*(TabTri[k]-1)+1];

          W1x=TabPt[2*(svg1-1)]-TabPt[2*pt1];
          W1y=TabPt[2*(svg1-1)+1]-TabPt[2*pt1+1];

          W2x=TabPt[2*(TabTri[k]-1)]-TabPt[2*pt1];
          W2y=TabPt[2*(TabTri[k]-1)+1]-TabPt[2*pt1+1];

          W3x=TabPt[2*pt2]-TabPt[2*(TabTri[k]-1)];
          W3y=TabPt[2*pt2+1]-TabPt[2*(TabTri[k]-1)+1];

          W4x=TabPt[2*pt1]-TabPt[2*(TabTri[k]-1)];
          W4y=TabPt[2*pt1+1]-TabPt[2*(TabTri[k]-1)+1];


          t1=U1*V2-V1*U2; //produit vectoriel AB A'B'
          t2=U1*W1y-W1x*U2; // AB AB'
          t3=U1*W2y-W2x*U2; //Ab AA'
          t4 = V1*W3y-W3x*V2; //A'B' A'B
          t5 = V1*W4y-W4x*V2;  // A'B' A'A


          if(t1!=0 && t2*t3<=0 && t4*t5<=0 && svg1!=Tab_arete_init[i] && svg1!=Tab_arete_init[i+1] && TabTri[k]!=Tab_arete_init[i] && TabTri[k]!=Tab_arete_init[i+1])
          {
          //on fait le basculement entre les deux triangles possédant cette arete
          int Tr1=0;
          int Tr2=0;
          for(int kk=0;kk<3*nbTriangles;kk+=3)
          {
            if(TabTri[kk]==svg1 || TabTri[kk+1]==svg1 || TabTri[kk+2]==svg1 )
            {
              if(TabTri[kk]==TabTri[k] || TabTri[kk+1]==TabTri[k] || TabTri[kk+2]==TabTri[k] )
              {
                if(Tr1==0)
                {
                  Tr1=kk/3+1;
                }
                else
                {
                  Tr2=kk/3+1;
                }

              }
            }
          }
          fprintf(stderr, " tr %d %d \n",Tr1,Tr2 );
          fprintf(stderr, "tr 1 %d %d %d %d %d \n",TabTri[3*(Tr1-1)],TabTri[3*(Tr1-1)+1],TabTri[3*(Tr1-1)+2],svg1,TabTri[k] );
          fprintf(stderr, "tr 2 %d %d %d %d %d \n",TabTri[3*(Tr2-1)],TabTri[3*(Tr2-1)+1],TabTri[3*(Tr2-1)+2] ,svg1,TabTri[k]);
            //on a deux triangles on va chercher a identifier l'arete commune

          //on a identifie les deux triangles a switcher
          //identifier l'arete commune
          if( (svg1==TabTri[3*(Tr1-1)] && TabTri[3*(Tr1-1)+1]==TabTri[k]) || (TabTri[k]==TabTri[3*(Tr1-1)] && TabTri[3*(Tr1-1)+1]==svg1))
          {
            pt1=TabTri[3*(Tr1-1)+2]-1;
          }
          else if( (svg1==TabTri[3*(Tr1-1)+2] && TabTri[3*(Tr1-1)+1]==TabTri[k]) || (TabTri[k]==TabTri[3*(Tr1-1)+2] && TabTri[3*(Tr1-1)+1]==svg1))
          {
            pt1=TabTri[3*(Tr1-1)]-1;
          }
          else
          {
            pt1=TabTri[3*(Tr1-1)+1]-1;
          }

          if( (svg1==TabTri[3*(Tr2-1)] && TabTri[3*(Tr2-1)+1]==TabTri[k]) || (TabTri[k]==TabTri[3*(Tr2-1)] && TabTri[3*(Tr2-1)+1]==svg1))
          {
            pt2=TabTri[3*(Tr2-1)+2]-1;
          }
          else if( (svg1==TabTri[3*(Tr2-1)+2] && TabTri[3*(Tr2-1)+1]==TabTri[k]) || (TabTri[k]==TabTri[3*(Tr2-1)+2] && TabTri[3*(Tr2-1)+1]==svg1))
          {
            pt2=TabTri[3*(Tr2-1)]-1;
          }
          else
          {
            pt2=TabTri[3*(Tr2-1)+1]-1;
          }


            TabTri[3*(Tr1-1)]=pt2+1;
            TabTri[3*(Tr1-1)+1]= svg1;
            TabTri[3*(Tr1-1)+2]= pt1+1;


            TabTri[3*(Tr2-1)]=pt1+1;
            TabTri[3*(Tr2-1)+1]=pt2+1;
            TabTri[3*(Tr2-1)+2]=svg2;
            fprintf(stderr, "pt1 pt 2 %d %d \n",pt1,pt2 );
            fprintf(stderr, "Nv triangle 1 %d %d %d \n",TabTri[3*(Tr1-1)],TabTri[3*(Tr1-1)+1],TabTri[3*(Tr1-1)+2] );
            fprintf(stderr, "Nv triangle 2 %d %d %d \n",TabTri[3*(Tr2-1)],TabTri[3*(Tr2-1)+1],TabTri[3*(Tr2-1)+2] );
            //fprintf(stderr, "ok \n");
            //on modifie la tab des aretes
            for(int kk=0;kk<2*nb_arete_delaunay;kk+=2)
            {
              if((Tabarete[kk]==svg1 && Tabarete[kk+1]==svg2) && (Tabarete[kk]==svg2 && Tabarete[kk+1]==svg1))
              {
              Tabarete[kk]=pt2+1;
              Tabarete[kk+1]=pt1+1;
              }
            }

            pt1=Tab_arete_init[i]-1;
            pt2=Tab_arete_init[i+1]-1;
          }
          //pour l'autre arete du triangle
          V1=TabPt[2*(svg3-1)]-TabPt[2*(svg1-1)]; //A' et B' segment du triangle
          V2=TabPt[2*(svg3-1)+1]-TabPt[2*(svg1-1)+1];

          W1x=TabPt[2*(svg2-1)]-TabPt[2*pt1];
          W1y=TabPt[2*(svg2-1)+1]-TabPt[2*pt1+1];

          W2x=TabPt[2*(svg1-1)]-TabPt[2*pt1];
          W2y=TabPt[2*(svg1-1)+1]-TabPt[2*pt1+1];

          W3x=TabPt[2*pt2]-TabPt[2*(svg1-1)];
          W3y=TabPt[2*pt2+1]-TabPt[2*(svg1-1)+1];

          W4x=TabPt[2*pt1]-TabPt[2*(svg1-1)];
          W4y=TabPt[2*pt1+1]-TabPt[2*(svg1-1)+1];


           t1=U1*V2-V1*U2; //produit vectoriel AB A'B'
           t2=U1*W1y-W1x*U2; // AB AB'
           t3=U1*W2y-W2x*U2; //Ab AA'
           t4 = V1*W3y-W3x*V2; //A'B' A'B
           t5 = V1*W4y-W4x*V2;  // A'B' A'A

          if(t1!=0 && t2*t3<=0 && t4*t5<=0 &&  svg1!=Tab_arete_init[i] && svg1!=Tab_arete_init[i+1] && svg3!=Tab_arete_init[i] && svg3!=Tab_arete_init[i+1])
          {
          //on fait le basculement entre les deux triangles possédant cette arete
          int Tr1=0;
          int Tr2=0;
          for(int kk=0;kk<3*nbTriangles;kk+=3)
          {
            if(TabTri[kk]==svg3 || TabTri[kk+1]==svg3 || TabTri[kk+2]==svg3 )
            {
              if(TabTri[kk]==svg1 || TabTri[kk+1]==svg1 || TabTri[kk+2]==svg1 )
              {
                if(Tr1==0)
                {
                  Tr1=kk/3+1;
                }
                else
                {
                  Tr2=kk/3+1;
                }

              }
            }
          }
          //on a identifie les deux triangles a switcher
          //identifier l'arete commune
          fprintf(stderr, " tr2 %d %d \n",Tr1,Tr2 );
          fprintf(stderr, "tr 1 %d %d %d %d %d \n",TabTri[3*(Tr1-1)],TabTri[3*(Tr1-1)+1],TabTri[3*(Tr1-1)+2],svg1,TabTri[k] );
          fprintf(stderr, "tr 2 %d %d %d %d %d \n",TabTri[3*(Tr2-1)],TabTri[3*(Tr2-1)+1],TabTri[3*(Tr2-1)+2] ,svg1,TabTri[k]);
          if( (svg1==TabTri[3*(Tr1-1)] && TabTri[3*(Tr1-1)+1]==svg3) || (svg3==TabTri[3*(Tr1-1)] && TabTri[3*(Tr1-1)+1]==svg1))
          {
            pt1=TabTri[3*(Tr1-1)+2]-1;
          }
          else if( (svg1==TabTri[3*(Tr1-1)+2] && TabTri[3*(Tr1-1)+1]==svg3) || (svg3==TabTri[3*(Tr1-1)+2] && TabTri[3*(Tr1-1)+1]==svg1))
          {
            pt1=TabTri[3*(Tr1-1)]-1;
          }
          else
          {
            pt1=TabTri[3*(Tr1-1)+1]-1;
          }

          if( (svg1==TabTri[3*(Tr2-1)] && TabTri[3*(Tr2-1)+1]==svg3) || (svg3==TabTri[3*(Tr2-1)] && TabTri[3*(Tr2-1)+1]==svg1))
          {
            pt2=TabTri[3*(Tr2-1)+2]-1;
          }
          else if( (svg1==TabTri[3*(Tr2-1)+2] && TabTri[3*(Tr2-1)+1]==svg3) || (svg3==TabTri[3*(Tr2-1)+2] && TabTri[3*(Tr2-1)+1]==svg1))
          {
            pt2=TabTri[3*(Tr2-1)]-1;
          }
          else
          {
            pt2=TabTri[3*(Tr2-1)+1]-1;
          }


            TabTri[3*(Tr1-1)]=pt2+1;
            TabTri[3*(Tr1-1)+1]=svg3;
            TabTri[3*(Tr1-1)+2]= pt1+1;


            TabTri[3*(Tr2-1)]=pt1+1;
            TabTri[3*(Tr2-1)+1]=pt2+1;
            TabTri[3*(Tr2-1)+2]=svg1;
            fprintf(stderr, "Nv triangle 1 %d %d %d \n",TabTri[3*(Tr1-1)],TabTri[3*(Tr1-1)+1],TabTri[3*(Tr1-1)+2] );
            fprintf(stderr, "Nv triangle 2 %d %d %d \n",TabTri[3*(Tr2-1)],TabTri[3*(Tr2-1)+1],TabTri[3*(Tr2-1)+2] );

            //fprintf(stderr, "ok \n");
            //on modifie la tab des aretes
            for(int kk=0;kk<2*nb_arete_delaunay;kk+=2)
            {
              if((Tabarete[kk]==svg3 && Tabarete[kk+1]==svg1) && (Tabarete[kk]==svg1 && Tabarete[kk+1]==svg3))
              {
              Tabarete[kk]=pt2+1;
              Tabarete[kk+1]=pt1+1;
              }
            }

            pt1=Tab_arete_init[i]-1;
            pt2=Tab_arete_init[i+1]-1;
          }
          //pour l'autre arete du triangle
          V1=TabPt[2*(svg2-1)]-TabPt[2*(svg3-1)]; //A' et B' segment du triangle
          V2=TabPt[2*(svg2-1)+1]-TabPt[2*(svg3-1)+1];

          W1x=TabPt[2*(svg2-1)]-TabPt[2*pt1];
          W1y=TabPt[2*(svg2-1)+1]-TabPt[2*pt1+1];

          W2x=TabPt[2*(svg3-1)]-TabPt[2*pt1];
          W2y=TabPt[2*(svg3-1)+1]-TabPt[2*pt1+1];

          W3x=TabPt[2*pt2]-TabPt[2*(svg3-1)];
          W3y=TabPt[2*pt2+1]-TabPt[2*(svg3-1)+1];

          W4x=TabPt[2*pt1]-TabPt[2*(svg3-1)];
          W4y=TabPt[2*pt1+1]-TabPt[2*(svg3-1)+1];


           t1=U1*V2-V1*U2; //produit vectoriel AB A'B'
           t2=U1*W1y-W1x*U2; // AB AB'
           t3=U1*W2y-W2x*U2; //Ab AA'
           t4 = V1*W3y-W3x*V2; //A'B' A'B
           t5 = V1*W4y-W4x*V2;  // A'B' A'A

          if(t1!=0 && t2*t3<=0 && t4*t5<=0 &&  svg2!=Tab_arete_init[i] && svg2!=Tab_arete_init[i+1] && svg3!=Tab_arete_init[i] && svg3!=Tab_arete_init[i+1])
          {

          //on fait le basculement entre les deux triangles possédant cette arete
          int Tr1=0;
          int Tr2=0;

          for(int kk=0;kk<3*nbTriangles;kk+=3)
          {
            if(TabTri[kk]==svg2 || TabTri[kk+1]==svg2 || TabTri[kk+2]==svg2 )
            {
              if(TabTri[kk]==svg3 || TabTri[kk+1]==svg3 || TabTri[kk+2]==svg3 )
              {
                if(Tr1==0)
                {
                  Tr1=kk/3+1;
                }
                else
                {
                  Tr2=kk/3+1;
                }

              }
            }
          }
          //on a identifie les deux triangles a switcher
          //identifier l'arete commune
          fprintf(stderr, "tr 1 %d %d %d %d %d \n",TabTri[3*(Tr1-1)],TabTri[3*(Tr1-1)+1],TabTri[3*(Tr1-1)+2],svg1,svg2 );
          fprintf(stderr, "tr 2 %d %d %d %d %d \n",TabTri[3*(Tr2-1)],TabTri[3*(Tr2-1)+1],TabTri[3*(Tr2-1)+2] ,svg1,svg2);
          if( (svg3==TabTri[3*(Tr1-1)] && TabTri[3*(Tr1-1)+1]==svg2) || (svg2==TabTri[3*(Tr1-1)] && TabTri[3*(Tr1-1)+1]==svg3))
          {
            pt1=TabTri[3*(Tr1-1)+2]-1;
          }
          else if( (svg3==TabTri[3*(Tr1-1)+2] && TabTri[3*(Tr1-1)+1]==svg2) || (svg2==TabTri[3*(Tr1-1)+2] && TabTri[3*(Tr1-1)+1]==svg3))
          {
            pt1=TabTri[3*(Tr1-1)]-1;
          }
          else
          {
            pt1=TabTri[3*(Tr1-1)+1]-1;
          }

          if( (svg3==TabTri[3*(Tr2-1)] && TabTri[3*(Tr2-1)+1]==svg2) || (svg2==TabTri[3*(Tr2-1)] && TabTri[3*(Tr2-1)+1]==svg3))
          {
            pt2=TabTri[3*(Tr2-1)+2]-1;
          }
          else if( (svg3==TabTri[3*(Tr2-1)+2] && TabTri[3*(Tr2-1)+1]==svg2) || (svg2==TabTri[3*(Tr2-1)+2] && TabTri[3*(Tr2-1)+1]==svg3))
          {
            pt2=TabTri[3*(Tr2-1)]-1;
          }
          else
          {
            pt2=TabTri[3*(Tr2-1)+1]-1;
          }

            TabTri[3*(Tr1-1)]=pt2+1;
            TabTri[3*(Tr1-1)+1]= svg2;
            TabTri[3*(Tr1-1)+2]= pt1+1;

            TabTri[3*(Tr2-1)]=pt1+1;
            TabTri[3*(Tr2-1)+1]=pt2+1;
            TabTri[3*(Tr2-1)+2]=svg3;

            fprintf(stderr, "Nv triangle 1 %d %d %d \n",TabTri[3*(Tr1-1)],TabTri[3*(Tr1-1)+1],TabTri[3*(Tr1-1)+2] );
            fprintf(stderr, "Nv triangle 2 %d %d %d \n",TabTri[3*(Tr2-1)],TabTri[3*(Tr2-1)+1],TabTri[3*(Tr2-1)+2] );
            //fprintf(stderr, "ok \n");
            //on modifie la tab des aretes
            for(int kk=0;kk<2*nb_arete_delaunay;kk+=2)
            {
              if((Tabarete[kk]==svg2 && Tabarete[kk+1]==svg3) && (Tabarete[kk]==svg3 && Tabarete[kk+1]==svg2))
              {
              Tabarete[kk]=pt2+1;
              Tabarete[kk+1]=pt1+1;
              }
            }
            pt1=Tab_arete_init[i]-1;
            pt2=Tab_arete_init[i+1]-1;
          }

        }//fin boucle sur triangles

      }
    }
    //on reecrit dans un nouveau fichier la correction faite
    FILE *mesh=NULL;
    mesh= fopen("BoiteMesh_modifie_basculementnvx2.mesh","w+"); //option d'ecriture sur le fichier, il faut réecrire tous les points et triangles
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
}
