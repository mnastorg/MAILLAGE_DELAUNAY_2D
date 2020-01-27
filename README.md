# Rapport Technique de Maillage
## Création d'un mailleur de type Delaunay 2D

Ce github retrace l'étendu de notre travail sur la création d'un mailleur de type Delaunay 2D. 

Dans le dossier "REFS" vous trouverez les divers documents .pdf et références dont nous nous sommes inspirés pour mener 
à bien ce travail. Le répertoire "CODES", comme son nom l'indique, contient tous les fichiers nécessaires pour l'exécution 
de notre méthode, programmée dans le langage C. Suite à ce projet, nous sommes capables de : 

  - Lire des géométries à partir d'un fichier .mesh (maillage.c).
	
  - Créer la 'boite englobante' et les 2 premiers triangles pour initialiser l'algorithme (boite.c).
	
  - Ajouter les points de la géométrie initiale selon le critère du noyau de Delaunay (cercle.c). Ce fichier regroupe en particulier : 
	- Savoir si le point à ajouter est dans le cercle circonscrit d'un triangle.
	- Savoir dans quel triangle du maillage se trouve le point à ajouter.
	- Création du tableau des voisins.
	- Création d'une cavité (suppression des triangles où le point est dans leur cercle circonscit).
	- Mise à jour des tableaux (reconstruction des arêtes, triangles... à partir de la cavité créée).
	
  - Effectuer un forçage de la frontière, c'est à dire reconstruire proprement les éventuelles arêtes 
  de la frontière de la géométrie initiale non présentes après l'ajout des points un à un (basculement_arete.c).
	
  - Suppression des triangles composants la 'boite-englobante' (suppression.c).
	
  - Ajout de points sur la triangulation créée (méthode en cours de développement qui ne marche pas de manière optimale) 
  (ajout_point.c).
	
  - Création de l'histogramme de qualité des éléments de notre maillage. Il s'agit d'un fichier python utilisant 
  la bibliothèque matplotlib pour l'affichage du graphique). Il prend en entrée un fichier 'histogramme.txt' sortant 
  du fichier (qualite.c).
	
  - Ecriture du maillage dans un format .mesh.

Notre méthode semble être fonctionnelle pour des géométries de difficultés croissantes même si nous observons quelques 
limites pour des géométries plus complexes (ex. une géométrie avec plusieurs sous-domaines). 
L'affichage est possible en utilisant le logiciel MEDIT et nos fichiers BoiteMesh****.mesh.
Pour plus de détails sur l'implémentation des codes produits voir le fichier pdf "Détails_codes_techniques_de_maillage"
