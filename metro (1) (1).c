/*Binome : DJIOJO Igor et LEMAITRE Antoine
tout le bout de code suivant vérifie juste le fonctionnement de nos différentes fonctions ecrite jusqu'à présent
nous allons procéder comme suit:
- recuperer toutes les données (objectif de la sceances du 17/05)
- pouvoir les manipuler à notre guise 
- ecrire un algorithme de recherche du plus court chemin juste pour une ligne
ensuite pour 3 lignes puis éssayer de généraliser l'algorithme(objectif de la sceance du 24/05 et du 22/05)
- biensûr les taches ne sont pas énnumérées */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 256
#define M 800
typedef char STRING[SIZE];

typedef struct _STATION /*definition de la structure des metros*/
{
	STRING nom;
	STRING nom_metro;
	double tps_prev;
	double tps_next;
	struct _STATION *parent;
	double poids;
}STATION;

typedef struct _LIGNE
{
	STATION station;
	struct _LIGNE *next;
	struct _LIGNE *prev;
}LIGNE;

typedef struct _METRO
{
	LIGNE *head;
	LIGNE *tail;
	int nb_station;
}METRO;

METRO *readligne(FILE *file, int *nb_ligne);
METRO *readAllLigne(METRO *m);
void affichage_de_ligne();
void add_fin(METRO *m,LIGNE *l);
void add_debut(METRO *m,LIGNE *l);
LIGNE *recherche_station(METRO *m, char *str);
void suprimer_station(LIGNE *ligne);
STATION stationFromLine(char *line);
METRO *creation_reserve(METRO *m);
METRO *recherche_itineraire(char *depart, char *arrive);
void metro();

int main(int argc, char *argv[])
{
	metro();
	getchar();
	return 0;
}

void metro()
{
	int i = 0;
	char a[SIZE];
	char d[SIZE];
	METRO *solution;
	solution = (METRO *)malloc(M * sizeof(METRO));
	printf("Entrez votre adresse de depart : ");
	fgets(a, SIZE, stdin);
	while (a[i] != '\0')
	{
		if (a[i] == '\n')
			a[i] = '\0';
		i++;
	}
	i=0;
	printf("Entrez votre adresse d'arriver : ");
	fgets(d, SIZE, stdin);
	while (d[i] != '\0')
	{
		if (d[i] == '\n')
			d[i] = '\0';
		i++;
	}
	solution = recherche_itineraire(a, d);
	STATION *tmp = &(solution->tail->station);
	while (tmp->parent != NULL)
	{
		printf("%s\n", tmp->nom);
		tmp = tmp->parent;
	}
	printf("%s\n", tmp->nom);
}

METRO *recherche_itineraire(char *depart, char *arrive)
{
	int good = 0;
	int relier = 0;
	int trouver = 0;
	int sorti = 0;
	int vrai = 0;
	int P = 1000;
	METRO *r;
	METRO *m;

	int nb = 0;
	FILE *file;
	STRING line = "";
	file = fopen("metro.csv", "r");

	LIGNE *intermediaire;
	intermediaire = (LIGNE *)malloc(SIZE * sizeof(LIGNE));
	//initialisation de la liste itineraire vide
	METRO *itineraire;
	m = (METRO *)calloc(M ,sizeof(METRO));
	itineraire = (METRO *)calloc(M,sizeof(METRO));
	r = readAllLigne(m);
	if (file == NULL) { printf("Error : file don't exist"); exit(1); }
	else
		m = readligne(file, &nb);
	fclose(file);
	for (int i = 0; i < nb; i++)
	{
		m[i].tail = m[i].head;
		while (m[i].tail->next != NULL)
		{
			if (m[i].tail->prev == NULL)
			{
				m[i].tail->station.tps_prev = 0;
				m[i].tail->station.tps_next = m[i].tail->next->station.tps_next - m[i].tail->station.tps_next;
			}
			else if (m[i].tail->next == NULL)
			{
				m[i].tail->station.tps_next = 0;
				m[i].tail->station.tps_prev = m[i].tail->station.tps_next - m[i].tail->prev->station.tps_next;
			}
			else
			{
				if (m[i].tail->next->station.tps_next < 0)
				{
					m[i].tail->station.tps_next = m[i].tail->next->station.tps_prev - m[i].tail->station.tps_prev;
					m[i].tail->station.tps_prev = m[i].tail->station.tps_next - m[i].tail->prev->station.tps_next;
				}
				else if (m[i].tail->station.tps_next < 0)
				{
					m[i].tail->station.tps_next = m[i].tail->next->station.tps_next + m[i].tail->station.tps_next;
					m[i].tail->station.tps_prev = m[i].tail->next->station.tps_prev - m[i].tail->station.tps_prev;
				}
				else
				{
					m[i].tail->station.tps_next = m[i].tail->next->station.tps_next - m[i].tail->station.tps_next;
					m[i].tail->station.tps_prev = m[i].tail->station.tps_next - m[i].tail->prev->station.tps_next;
				}
			}
			m[i].tail = m[i].tail->next;
		}
	}

	//initialisation du poids et du pointeur parent de chaque éléments de la réserve 

	r->tail = r->head;
	while (r->tail->next != NULL)
	{
		r->tail->station.poids = P;
		r->tail->station.parent = (STATION *)malloc(M * sizeof(STATION));
		if (r->tail->next->next == NULL)
		{
			r->tail->next->station.poids = P;
			r->tail->next->station.parent = (STATION *)malloc(M * sizeof(STATION));
		}
		r->tail = r->tail->next;
	}
	//recherche de la station de départ dans la réserve et l'affecter un poids null puis l'enlever
	r->tail = r->head;
	while (r->tail->next != NULL && vrai == 0)
	{
		if (strcmp(r->tail->station.nom, depart) == 0 && r->tail->prev != NULL && r->tail->next != NULL)
		{
			r->tail->next->prev = r->tail->prev;
			r->tail->prev->next = r->tail->next;
			r->tail->station.poids = 0;
			intermediaire = r->tail;
			r->tail = intermediaire->next;
			vrai = 1;
		}
		else if(strcmp(r->tail->station.nom, depart) == 0 && r->tail->prev == NULL && r->tail->next != NULL)
		{
			r->tail->next->prev = r->tail->prev;
			r->head = r->tail->next;
			r->tail->station.poids = 0;
			intermediaire = r->tail;
			vrai = 1;
		}
		else if (r->tail->next->next == NULL)
		{
			if (strcmp(r->tail->next->station.nom, depart) == 0)
			{
				r->tail->next->station.poids = 0;
				intermediaire = r->tail->next;
				r->tail->next = NULL;
				vrai = 1;
			}
		}
		if(r->tail->next != NULL)
			r->tail = r->tail->next;
	}
	//insérer (par poids croissant) à nouveau la station dans la reserve
	vrai = 0;
	r->tail = r->head;
	while (r->tail->next != NULL)
	{
		if (r->tail->prev == NULL)
		{
			r->tail->prev = intermediaire;
			intermediaire->next = r->tail;
			r->tail->prev->prev = NULL;
			r->head = r->tail->prev;
			intermediaire = NULL;
		}
		r->tail = r->tail->next;
	}
	//tant que la reserve n'est pas vide l'algorithme s'exécute comme suit
	while (r->head != NULL)
	{
		//enlever la station en tete de la reserve
		intermediaire = r->head;
		r->head = intermediaire->next;
		r->head->prev = NULL;
		intermediaire->next = NULL;
		//insérer cette station dans l'itineraire à la fin
		if (vrai == 0)
		{
			itineraire->tail = intermediaire;
			itineraire->head = intermediaire;
			itineraire->head->station.parent = NULL;
			vrai = 1;
		}
		else
		{
			itineraire->tail->next = intermediaire;
			itineraire->tail->next->prev = itineraire->tail;
			itineraire->tail = intermediaire;
			itineraire->tail->next = NULL;
		}


		/**********************************************/
		if (itineraire->head->next != NULL)
		{
			good = 0;
			itineraire->tail = itineraire->head->next;
			while (itineraire->tail != NULL && good == 0)
			{
				relier = 0;
				if (itineraire->tail->prev != NULL && itineraire->tail->next != NULL)
				{
					intermediaire = itineraire->head;
					while (strcmp(intermediaire->station.nom, itineraire->tail->next->station.nom) != 0 && relier == 0)
					{
						if (strcmp(intermediaire->station.nom, itineraire->tail->station.parent->nom) == 0)
						{
							itineraire->tail->station.parent = &(intermediaire->station);
							relier = 1;
						}
						intermediaire = intermediaire->next;
					}
					itineraire->tail = itineraire->tail->next;
				}
				else if (itineraire->tail->next == NULL)
				{
					intermediaire = itineraire->head;
					while (intermediaire != NULL && relier == 0)
					{
						if (strcmp(intermediaire->station.nom, itineraire->tail->station.parent->nom) == 0)
						{
							itineraire->tail->station.parent = &(intermediaire->station);
							relier = 1;
							good = 1;
						}
						intermediaire = intermediaire->next;
					}
				}

			}
		}
		/*******************************************************/
		//sorti = 1;
		//est-ce qu'il sagit de la station d'arrivée ou pas
		if (strcmp(itineraire->tail->station.nom, arrive) == 0)
			return itineraire;
		else
		{
			for (int k = 0; k < nb; k++)
			{
				sorti = 0;
				m[k].tail = m[k].head;
				while (m[k].tail != NULL && sorti == 0)
				{
					if (strcmp(m[k].tail->station.nom, itineraire->tail->station.nom) == 0)
					{
						if (m[k].tail->next == NULL)
							sorti = 1;
						else
						{
							// calcul du nouveaunpoids et on l'affecte au poid de la station suivante
							if (strcmp(m[k].tail->next->station.nom, depart) == 0)
							{
								sorti = 1;
							}
							else
							{
								m[k].tail->next->station.poids = m[k].tail->station.tps_next + itineraire->tail->station.poids;
								//chercher la station suivante dans la reserve
								r->tail = r->head;
								trouver = 0;
								while (r->tail != NULL && trouver == 0)
								{
									if (strcmp(r->tail->station.nom, m[k].tail->next->station.nom) == 0)
									{
										if (r->tail->station.poids > m[k].tail->next->station.poids)
										{
											//enlever de la reserve
											if (r->tail->next != NULL && r->tail->prev != NULL)
											{
												r->tail->next->prev = r->tail->prev;
												r->tail->prev->next = r->tail->next;
												intermediaire = r->tail;
												r->tail = intermediaire->next;
												intermediaire->next = NULL;
												intermediaire->prev = NULL;
												//mettre à jour son nouveau poid
												intermediaire->station.poids = m[k].tail->next->station.poids;							
												//affecter à son parent l'adresse de la station actuelle
												intermediaire->station.parent = &(m[k].tail->station);// incompatible
												//inserer dans la reserve par poids croissant
												r->tail = r->head;
												while (r->tail != NULL && trouver == 0 )
												{
													if (r->tail->prev != NULL)
													{
														if (r->tail->station.poids >= intermediaire->station.poids && r->tail->prev->station.poids < intermediaire->station.poids)
														{
															intermediaire->next = r->tail;
															intermediaire->prev = r->tail->prev;
															r->tail->prev->next = intermediaire;
															r->tail->prev = intermediaire;
															trouver = 1;
														}
													}
													else if (r->tail->prev == NULL)
													{
														if (r->tail->station.poids >= intermediaire->station.poids)
														{
															intermediaire->next = r->tail;
															r->tail->prev = intermediaire;
															r->head = intermediaire;
															trouver = 1;
														}
													}
													r->tail = r->tail->next;
												}
											}
											else if (r->tail->prev == NULL)
											{
												r->tail->next->prev = NULL;
												intermediaire = r->tail;
												r->tail = intermediaire->next;
												r->head = intermediaire->next;
												intermediaire->next = NULL;
												//mettre à jour son nouveau poid
												intermediaire->station.poids = m[k].tail->next->station.poids;
												//affecter à son parent l'adresse de la station actuelle
												intermediaire->station.parent = &(m[k].tail->station);// incompatible
												//inserer dans la reserve par poids croissant
												r->tail = r->head;
												while (r->tail != NULL && trouver == 0)
												{
													if (r->tail->prev != NULL)
													{
														if (r->tail->station.poids >= intermediaire->station.poids && r->tail->prev->station.poids <= intermediaire->station.poids)
														{
															intermediaire->next = r->tail;
															intermediaire->prev = r->tail->prev;
															r->tail->prev->next = intermediaire;
															r->tail->prev = intermediaire;
															trouver = 1;
														}
													}
													else if (r->tail->prev == NULL)
													{
														if (r->tail->station.poids >= intermediaire->station.poids)
														{
															intermediaire->next = r->tail;
															r->tail->prev = intermediaire;
															r->head = intermediaire;
															trouver = 1;
														}
													}
													r->tail = r->tail->next;
												}
											}
											else if (r->tail->next == NULL)
											{
												intermediaire = r->tail;
												r->tail->prev->next = NULL;
												r->tail = NULL;
												intermediaire->prev = NULL;
												//mettre à jour son nouveau poid
												intermediaire->station.poids = m[k].tail->next->station.poids;
												//affecter à son parent l'adresse de la station actuelle
												intermediaire->station.parent = &(m[k].tail->station);// incompatible
												//inserer dans la reserve par poids croissant
												r->tail = r->head;
												while (r->tail != NULL && trouver == 0)
												{
													if (r->tail->prev != NULL)
													{
														if (r->tail->station.poids >= intermediaire->station.poids && r->tail->prev->station.poids <= intermediaire->station.poids)
														{
															intermediaire->next = r->tail;
															intermediaire->prev = r->tail->prev;
															r->tail->prev->next = intermediaire;
															r->tail->prev = intermediaire;
															trouver = 1;
														}
													}
													else if (r->tail->prev == NULL)
													{
														if (r->tail->station.poids >= intermediaire->station.poids)
														{
															intermediaire->next = r->tail;
															r->tail->prev = intermediaire;
															r->head = intermediaire;
															trouver = 1;
														}
													}
													r->tail = r->tail->next;
												}
											}
										}
									}
									if (r->tail != NULL)
										r->tail = r->tail->next;
									//lasuite
								}
								sorti = 1;
							}		
						}
					}
					m[k].tail = m[k].tail->next;
					//lasuite
				}
			}
		}
		//r->head = r->head->next;
	}
}

void suprimer_station(LIGNE *ligne)
{
	ligne->next->prev = ligne->prev;
	ligne->prev->next = ligne->next;
}

METRO *creation_reserve(METRO *m)
{
	METRO *reserve;
	reserve = (METRO *)malloc(M * sizeof(METRO));
	

	return reserve;
}

void add_debut(METRO *m, LIGNE *l)
{
	if (m->head != NULL)
	{
		m->head = m->tail;
		while (m->head->prev != NULL)
			m->head = m->head->prev;
		if (m->head->prev == NULL)
		{
			m->head->prev = l;
			m->head->prev->next = m->head;
			m->head = l;
			m->head->prev = NULL;
		}
	}
	else
	{
		m->head = l;
		m->head->next = NULL;
		m->head->prev = NULL;
		m->tail = l;
	}
	m->nb_station = m->nb_station + 1;
}
void add_fin(METRO *m,LIGNE *l)
{
	if (m->tail != NULL)
	{
		m->tail = m->head;
		while (m->tail->next != NULL)
			m->tail = m->tail->next;
		if (m->tail->next == NULL)
		{
			m->tail->next = l;
			m->tail->next->prev = m->tail;
			m->tail = l;
			m->tail->next = NULL;
		}
	}
	else
	{
		m->tail = l;
		m->tail->next = NULL;
		m->tail->prev = NULL;
		m->head = l;
	}
	m->nb_station = m->nb_station + 1;
}

LIGNE *recherche_station(METRO *m, char *str)
{
	m->tail = m->head;
	while (m->tail != NULL)
	{
		if (m->tail->station.nom == str)
			return m->tail;
		m->tail = m->tail->next;
	}
}

METRO * readAllLigne(METRO *m)
{
	METRO *reserve;
	int x;
	int nb=0;
	reserve = (METRO *)malloc(M * sizeof(METRO));
	FILE *file;
	STRING line = "";
	file = fopen("metro.csv", "r");
	if (file == NULL) { printf("Error : file don't exist"); exit(1); }
	else
		m = readligne(file, &nb);
	fclose(file);
	for (int i = 0; i < nb; i++)
	{
		m[i].tail = m[i].head;
		while (m[i].tail->next != NULL)
		{
			if (m[i].tail->prev == NULL)
			{
				m[i].tail->station.tps_prev = 0;
				m[i].tail->station.tps_next = m[i].tail->next->station.tps_next - m[i].tail->station.tps_next;
			}
			else if (m[i].tail->next == NULL)
			{
				m[i].tail->station.tps_next = 0;
				m[i].tail->station.tps_prev = m[i].tail->station.tps_next - m[i].tail->prev->station.tps_next;
			}
			else
			{
				if (m[i].tail->next->station.tps_next < 0)
				{
					m[i].tail->station.tps_next = m[i].tail->next->station.tps_prev - m[i].tail->station.tps_prev;
					m[i].tail->station.tps_prev = m[i].tail->station.tps_next - m[i].tail->prev->station.tps_next;
				}
				else if (m[i].tail->station.tps_next < 0)
				{
					m[i].tail->station.tps_next = m[i].tail->next->station.tps_next + m[i].tail->station.tps_next;
					m[i].tail->station.tps_prev = m[i].tail->next->station.tps_prev - m[i].tail->station.tps_prev;
				}
				else
				{
					m[i].tail->station.tps_next = m[i].tail->next->station.tps_next - m[i].tail->station.tps_next;
					m[i].tail->station.tps_prev = m[i].tail->station.tps_next - m[i].tail->prev->station.tps_next;
				}
			}
			m[i].tail = m[i].tail->next;
		}
	}
	reserve->head = m[0].head;
	reserve->tail = m[0].tail;
	for (int i = 1; i < nb; i++)
	{
		m[i].tail = m[i].head;
		while (m[i].tail != NULL)
		{
			x = 0;
			reserve->tail = reserve->head;
			while (reserve->tail->next != NULL && x == 0)
			{
				
				if (strcmp(m[i].tail->station.nom, reserve->tail->station.nom) == 0 )
				{
					x = 1;
				}
				if (reserve->tail->next->next == NULL && x == 0)
				{
					if (strcmp(m[i].tail->station.nom,reserve->tail->next->station.nom) == 0)
						x = 1;
				}
				reserve->tail = reserve->tail->next;
			}
			
			if (x == 0)
			{
				reserve->tail->next = m[i].tail;
				reserve->tail->next->prev = reserve->tail;
				reserve->tail = m[i].tail;
				m[i].tail = m[i].tail->next;
				reserve->tail->next = NULL;
			}
			else
				m[i].tail = m[i].tail->next;
		}
	}
	return reserve;
}


STATION stationFromLine(char *line)
{
	STATION station = { "", 0, 0 };
	char *ptr = strtok(line, ":");
	int n = 0;
	strncpy(station.nom_metro, ptr, SIZE);
		/* Skip the first fields */
		for (n = 0; n < 2; n++)
			ptr = strtok(NULL, ":");
		/* Copy the station name */
		if (ptr != NULL)
			strncpy(station.nom, ptr, SIZE);
		/* Skip some fields */
		for (n = 0; n < 3; n++)
			ptr = strtok(NULL, ":");
		if (ptr != NULL)
		{
			if (ptr[0] == '*')
				station.tps_next = -1 * atof(&ptr[1]);
			else
				station.tps_next = atof(ptr);
			ptr = strtok(NULL, ":");
			if (ptr[0] == '*')
				station.tps_prev = -1 * atof(&ptr[1]);
			else
				station.tps_prev = atof(ptr);
		}
	return station;
}

METRO *readligne(FILE *file,int *nb_ligne)
{
	STRING line = "";
	METRO *tab_metro;
	tab_metro = (METRO *)calloc(M ,sizeof(METRO));
	int n=0,i=0;
	while (fgets(line, SIZE, file) != NULL)
	{
		if (n < 13) {}
		else if (n > 13 && n < 794) {
			if (line[0] != '\0')    /* Avoid the last line */
			{
				if (line[0] == '#')
				{
					i++;
					tab_metro[i].head = NULL;
					tab_metro[i].tail = NULL;
					tab_metro[i].nb_station = 0;
				}
				else
				{
					
					STATION station = { "", 0, 0 };
					station = stationFromLine(line);
					LIGNE *ligne;
					ligne = (LIGNE *)malloc(SIZE * sizeof(LIGNE));
					ligne->station = station;// FromLine(line);
					if (tab_metro[i].head == NULL && tab_metro[i].tail == NULL && tab_metro[i].nb_station == 0) {
						ligne->prev = NULL;
						tab_metro[i].head = ligne;
						tab_metro[i].tail = ligne;
						tab_metro[i].tail->next = NULL;
						tab_metro[i].nb_station++;
					}
					else {
						tab_metro[i].tail->next = ligne;
						ligne->prev = tab_metro[i].tail;
						tab_metro[i].tail = ligne;
						tab_metro[i].tail->next = NULL;
						tab_metro[i].nb_station++;
					}
				}
			}
		}
		n++;
	}
	*nb_ligne = i;
	return tab_metro;
}

void affichage_de_ligne()
{
	char *ptr;
	int n = 0;
	FILE *file;
	STRING line = "";
	file = fopen("metro.csv", "r");
	if (file == NULL) { printf("Error : file don't exist"); exit(1); }
	while (fgets(line, SIZE, file) != NULL)
	{
		if (n < 62) {}
		else if (n > 64 && n < 66) {
			ptr = strtok(line, ":");
			printf("%s\n", &ptr[4]);
			ptr = strtok(NULL, ":");
			printf("direction %s, ", ptr);
			ptr = strtok(NULL, ":");
			printf("destination %s, ", ptr);
			ptr = strtok(NULL, ":");
			printf("coord Est %s, ", ptr);
			ptr = strtok(NULL, ":");
			printf("coord Nord %s, ", ptr);
			ptr = strtok(NULL, ":");
			printf("heure de départ %s, ", ptr);
			ptr = strtok(NULL, ":");
			printf("heure de passage du dernier metro %s \n\n ", ptr);
		}
		n++;
	}
	fclose(file);
}