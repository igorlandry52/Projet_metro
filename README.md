# Projet_metro
Le but dans ce projet de pouvoir à partir d'une station de départ de pouvoir aller à une station d'arriver en île de france (choisi au préable par l'utilisateur) en utilisant le plus court trajet possible. Donc, pour cela nous avons utlisés l'algorithme de DIJKSTRA pour la recherche du plus court chemin. Et donc à notre disposition , nous avons un fichier sur lequel etait repertorié toutes les stations de metros, RER, grande Ligne ainsi que le temps séparant deux stations consécutives.

L'algorithme de Dijkstra permet de résoudre un problème algorithmique : le problème du plus court chemin. Ce problème a plusieurs variantes. La plus simple est la suivante : étant donné un graphe non-orienté, dont les arêtes sont munies de poids, et deux sommets de ce graphe, trouver un chemin entre les deux sommets dans le graphe, de poids minimum. L'algorithme de Dijkstra permet de résoudre un problème plus général : le graphe peut être orienté, et l'on peut désigner un unique sommet, et demander d'avoir la liste des plus courts chemins pour tous les autres nœuds du graphe.

L'algorithme prend en entrée un graphe orienté pondéré par des réels positifs et un sommet source. Il s'agit de construire progressivement un sous-graphe dans lequel sont classés les différents sommets par ordre croissant de leur distance minimale au sommet de départ. La distance correspond à la somme des poids des arcs empruntés.

Au départ, on considère que les distances de chaque sommet au sommet de départ sont infinies, sauf pour le sommet de départ pour lequel la distance est nulle. Le sous-graphe de départ est l'ensemble vide.

Au cours de chaque itération, on choisit en dehors du sous-graphe un sommet de distance minimale et on l'ajoute au sous-graphe. Ensuite, on met à jour les distances des sommets voisins de celui ajouté. La mise à jour s'opère comme suit : la nouvelle distance du sommet voisin est le minimum entre la distance existante et celle obtenue en ajoutant le poids de l'arc entre sommet voisin et sommet ajouté à la distance du sommet ajouté.

On continue ainsi jusqu'à épuisement des sommets (ou jusqu'à sélection du sommet d'arrivée).
