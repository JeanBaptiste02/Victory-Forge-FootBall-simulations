/**
 * \file tournoi.c
 * \brief Programme de simulation d'un tournoi de football avec des matchs
 * \authors DAMODARANE Jean-Baptiste
 * \version 1.0
 * \date 03/04/2023
*/

#include "./inc/ipcTools.h"

#define NB_EQUIPES 8 // nombre d'equipes dans le tournoi
#define NB_MATCHS 4 // nombre de matchs par tour


/**
 * @brief Permet de lire les noms des equipes du fichier
 * @param nom_equipes Le tableau de chaines de caracteres ayant les noms des equipes
 * @param file_equipes_nom Le nom du fichier contenant les noms des equipes
 * @throw Erreur si le fichier n'est pas accessible ou si le nombre d'equipes n'est pas respecte
*/
void lire_equipes(char nom_equipes[NB_EQUIPES][50], const char* file_equipes_nom) {
    FILE* equipe_file = fopen(file_equipes_nom, "r"); //ouvre le fichier en mode lecture
    if (equipe_file == NULL) {
        perror("Impossible d'ouvrir le fichier des équipes\n");
        exit(1);
    }
    int i = 0; //indice du tableau nom_equipes
    char ch; //caractere lu
    //tant que l'on n'a pas atteint la fin du fichier et que le nombre maximal d'equipes n'est pas atteint, on continu de lire chaque caractere
    while ((ch = fgetc(equipe_file)) != EOF && i < NB_EQUIPES) {
        if (ch == '\n') {
            nom_equipes[i][0] = '\0';
            i++;
        } else {
            strncat(nom_equipes[i], &ch, 1);
        }
    }
    fclose(equipe_file); //fermeture du fichier
    if (i != NB_EQUIPES) {
        printf("Le fichier des équipes est invalide (attendu %d équipes)\n", NB_EQUIPES);
        exit(1);
    }
}

/**
 * @brief Permet de compter le nombre de lignes du fichier texte
 * @param nom_fichier Le nom du fichier 
 * @throw Erreur si on n'arrive pas a ouvrir le fichier
 * @return Le nombre de lignes du fichier ou -1 si le fichier ne peut pas etre ouvert
*/
int compter_lignes(char* nom_fichier) {
    FILE* file = fopen(nom_fichier, "r");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", nom_fichier);
        return -1; //erreur 
    }
    int cpt_lignes = 1; // compteurs de lignes a 1 (premiere ligne)
    char ch;
    //tant que l'on n'a pas atteint la fin du fichier
    while(!feof(file)) {
        ch = fgetc(file); //pour lire le prochain caractere du fichier
        if(ch == '\n') { // si caractere = saut de ligne
            cpt_lignes++;
        }
    }
    fclose(file);
    return cpt_lignes;
}

/**
 * @brief Permet de charger les noms des equipes depuis le fichier et les stocke dans un tableau
 * @param nom_fichier Le nom du fichier
 * @param nom_equipes Le tableau de chaines de caracteres ayant les noms des equipes
 * @throw Erreur si le fichier n'est pas ouvert ou si il y a un nombre d'equipes inattendus
 * @return 0 si les noms ont ete charges ou 1 si non
*/
int charger_equipes(char *nom_fichier, char nom_equipes[NB_EQUIPES][50]) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        perror("Impossible d'ouvrir le fichier des équipes\n");
        return 1;
    }
    //pour chaque equipe a charger
    for (int i = 0; i < NB_EQUIPES; i++) {
        if (fgets(nom_equipes[i], 50, fichier) == NULL) { //lit une ligne du fichier et l'enregistre  dans le tableau nom_equipes
            perror("Le fichier des équipes est trop court\n");
            return 1;
        }
    }
    fclose(fichier);
    return 0;
}

/**
 * @brief Permet de verifier si le nombre de lignes du fichier est une puissance de 2
 * @param n nombre a verifier
 * @throw Erreur si n n'est pas une puissance de 2
 * @return 1 si n est une puissance de 2, sinon on retourne 0
*/
int est_puissance_de_2(int n) {
    if (n == 0) {
        return 0;
    }
    while (n != 1) {
        if (n % 2 != 0) {
            perror("Le nombre d'équipes n'est pas une puissance de 2\n\n");
            return 0;
        }
        n = n / 2;
    }
    return 1;
}

/**
 * @brief Permet de generer un score aleatoirement
*/
int gener_score_random() {
    return rand() % 100;
}

/**
 * @brief Permet de determiner le vainceur par rapport aux scores des 2 equipes
 * @param sc1 Le score de l'equipe 1
 * @param sc2 Le score de l'equipe 2
 * @param score1 Un pointeur vers int qui sera 1 si l'equipe1 gagne ou 0 sinon
 * @param score2 Un pointeur vers int qui sera 1 si l'equipe2 gagne ou 0 sinon
*/
void determiner_gagnant(int sc1, int sc2, int* score1, int* score2) {
    if (sc1 > sc2) {
        *score1 = 1; // équipe 1 gagne
        *score2 = 0;
    } else if (sc2 > sc1) {
        *score1 = 0;
        *score2 = 1; // équipe 2 gagne
    } else {
        *score1 = 0; // match nul
        *score2 = 0;
    }
}

/**
 * @brief Permet de simuler un match avec 2 scores aleatoires pour chaque equipe et determine le gagnant
 * @param arg Un pointeur vers la structure Match 
 * @return NULL
*/
void* simuler_match(void* arg) {
    Match* match = (Match*) arg;
    //appels de la fonction gener_score_random() qui genere les scores
    int sc1 = gener_score_random();
    int sc2 = gener_score_random();
    determiner_gagnant(sc1, sc2, &match->score1, &match->score2);
    pthread_exit(NULL);
}

/**
 * @brief Permet de calculer le score total et de verifier l'elimination d'une equipe specifique et de faire la mise à jour des scores
 * @param tour Un pointeur vers la structure Tour
 * @param total_score score total de l'equipe
 * @param i indice de l'equipe
*/
void calcul_score_verif_elimination(Tours* tour, int total_score, int i){
	for (int j = 0; j < NB_MATCHS; j++) { //on parcourt tous les matchs et on verifie la participation de chaque equipe dans chaque match
            if ((tour->matchs[j].team1 == i && tour->matchs[j].score1 == -1) || 
                (tour->matchs[j].team2 == i && tour->matchs[j].score2 == -1)) { //si le match n'a pas encore ete joue par l'equipe, alors on ne l'elimine pas
                break;//pour interromp la boucle
            } else if (tour->matchs[j].team1 == i) { //si l'equipe a participe au match et si elle est la 1ere equipe
                total_score += tour->matchs[j].score1; //score total de l'equipe mise a jour dans le match
            } else if (tour->matchs[j].team2 == i) { //si l'equipe a participe au match et si elle est la 2eme equipe
                total_score += tour->matchs[j].score2; //score total de l'equipe mise a jour dans le match
            }
        }
}

/**
 * @brief Permet de rechercher les equipes eliminees pour un tour donne et mettre a jour les scores des equipes qui restent
 * @param tour Un pointeur vers la structure Tour
 * @param num_tour Le numero du tour actuel
*/
void recherche_equipes_eliminees(Tours* tour, int num_tour){
	// Recherche des équipes éliminées
    int min_score = NB_MATCHS * 6; // Le score maximal possible
    int num_equipes_eliminees = 0;
    for (int i = 0; i < NB_EQUIPES; i++) {
        int total_score = 0;
        int est_eliminee = 0; //pour savoir si l'equipe en question est eliminee
        
        //appel de la fonction pour calculer le score total de l'equipe et verifie si elle est eliminee
		calcul_score_verif_elimination(tour, total_score, i);
		
        if (est_eliminee) {
            num_equipes_eliminees++;
            printf("Equipe %d éliminée au tour %d\n", i+1, num_tour);
            // pour enlever l'équipe eliminee des matchs des prochains tours
            for (int j = 0; j < NB_MATCHS; j++) {
                if (tour->matchs[j].team1 == i || tour->matchs[j].team2 == i) {
                    // équipe eliminee dans ce match est marque -1
                    tour->matchs[j].score1 = -1;
                    tour->matchs[j].score2 = -1;
                }
            }
        } else {
            if (total_score < min_score) { //si l'equipe n'est pas eliminee, on verifie le score minimal
                min_score = total_score;
                num_equipes_eliminees = 0;
            }
            if (total_score == min_score) { //si l'equipe a le même score que le score minimal
                num_equipes_eliminees++;
            }
        }
    }
}

/**
 * @brief Permet de simuler un tour avec les matchs qui vont avec ce tour
 * @param tour Un pointeur vers la structure Tour
 * @param num_tour Le numero du tour actuel
 * @param equipe_eliminee Le tableau des equipes deja eliminees
 * @param nom_equipes Le tableau ayant les noms des equipes
*/
void simuler_tour(Tours* tour, int num_tour, int equipe_eliminee[NB_EQUIPES], char nom_equipes[NB_EQUIPES][50]) {
    printf("\n\n\nDébut du tour %d : ------------------------------\n", num_tour);
    for (int i = 0; i < NB_MATCHS; i++) { //parcours des matchs d'un tour
        Match* match = &tour->matchs[i];
        int equipe1 = match->team1;
        int equipe2 = match->team2;
        if (equipe_eliminee[equipe1] || equipe_eliminee[equipe2]) { 
            // Si l'une des deux équipes a ete eliminee, alors il n'y a pas de match
            if (equipe_eliminee[equipe1]) {
                match->score1 = -1;
                match->score2 = 0;
            } else {
                match->score1 = 0;
                match->score2 = -1;
            }
            //affichage de message pour les matchs annules
			if (equipe_eliminee[equipe1]) {
				printf("\nLe match avec l'equipe %s a ete annule \n", nom_equipes[equipe2]);
			} else {
				printf("\nLe match avec l'equipe %s a ete annule \n", nom_equipes[equipe1]);
			}
            continue;
        }
        printf("\nMatch %d : %s - %s", i+1, nom_equipes[equipe1], nom_equipes[equipe2]);
        pthread_t thread; //lancement de thread pour simuler le match
        pthread_create(&thread, NULL, simuler_match, (void*) match);
        pthread_join(thread, NULL);
        printf("\nScore du match %d : %d - %d\n", i+1, match->score1, match->score2);
    }
    printf("\nFin du tour %d ------------------------------\n\n", num_tour);
    sleep(1);

    recherche_equipes_eliminees(tour, num_tour); 
}

/**
 * @brief Permet d'initialiser les equipes et les tours, puis simule les tours et ensuite met a jour les scores
 * @param tours Un tableau de strcutures Tours avec les tours du tournoi
 * @param nom_equipes Un tableau ayant les noms des equipes
 * @param scores Un tableau pour stocker les scores de chaque equipe
*/
void init_simuler_tours(Tours tours[NB_EQUIPES - 1], char nom_equipes[NB_EQUIPES][50], int scores[NB_EQUIPES]) {
    // Initialisation des équipes
    int mes_equipes[NB_EQUIPES];
    for (int i = 0; i < NB_EQUIPES; i++) {
        mes_equipes[i] = i + 1;
    }

    // Initialisation des tours
    for (int i = 0; i < NB_EQUIPES - 1; i++) {
        pthread_mutex_init(&tours[i].lock, NULL);
        for (int j = 0; j < NB_MATCHS; j++) {
            int team1 = mes_equipes[j];
            int team2 = mes_equipes[(j + i + 1) % NB_EQUIPES];
            tours[i].matchs[j] = (Match) {team1, team2, 0, 0};
        }
    }

    // Simulation des tours
    int equipe_eliminee[NB_EQUIPES] = {0};
    for (int i = 0; i < NB_EQUIPES - 1; i++) {
        Tours* tour = &tours[i];
        simuler_tour(tour, i+1, equipe_eliminee, nom_equipes);
        pthread_mutex_lock(&tour->lock); // pour avoir 1 seul thread a la fois

        for (int j = 0; j < NB_MATCHS; j++) {
            Match* match = &tour->matchs[j];
            scores[match->team1 - 1] += match->score1;
            scores[match->team2 - 1] += match->score2;
        }
		pthread_mutex_unlock(&tour->lock);
	}

	// Nettoyage
	for (int i = 0; i < NB_EQUIPES - 1; i++) {
		pthread_mutex_destroy(&tours[i].lock);
	}
}

/**
 * @brief Permet d'ecrire les resultats des matchs dans un fichier txt de sortie : scores des equipes lors des matchs de chaque tour et enfin le vaincueur
 * @param tours Un tableau ayant les tours du tournoi
 * @param nom_equipes Un tableau ayant les noms des equipes
 * @param scores Un tableau ayant les scores des equipes
 * @param nom_fichier_sortie Le nom du fichier de sortie
*/
void ecrire_resultats(Tours tours[], char nom_equipes[][50], int scores[], char* nom_fichier_sortie) {
    FILE* fptr;
    fptr = fopen(nom_fichier_sortie, "w"); //ouverture du fichier en ecriture

    if (fptr == NULL) {
        perror("Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }

    // Écrire les résultats des matchs
    for (int i = 0; i < NB_EQUIPES - 1; i++) {
        Tours* tour = &tours[i];

        fprintf(fptr, "Tour %d :\n", i+1);
        for (int j = 0; j < NB_MATCHS; j++) {
            Match* match = &tour->matchs[j];
            fprintf(fptr, "%s %d - %d %s\n", nom_equipes[match->team1 - 1], match->score1, match->score2, nom_equipes[match->team2 - 1]);
        }
        fprintf(fptr, "\n");
    }

    // Écrire les scores des équipes
    fprintf(fptr, "Scores finaux :\n");
    for (int i = 0; i < NB_EQUIPES; i++) {
        fprintf(fptr, "%s : %d\n", nom_equipes[i], scores[i]);
    }
    fprintf(fptr, "\n");

    // Écrire le gagnant final
    int max_score = scores[0];
    int gagnant = 1;
    for (int i = 1; i < NB_EQUIPES; i++) {
        if (scores[i] > max_score) {
            max_score = scores[i];
            gagnant = i+1;
        }
    }
    fprintf(fptr, "Le gagnant est %s avec un score de %d !\n", nom_equipes[gagnant-1], max_score);

    fclose(fptr);
}

/**
 * @brief Permet d'afficher les resultats finaux du tournoi (les scores finaux de chaque equipe et ainsi l'equipe gagnante) sur lignes de commandes
 * @param scores Le tableau contenant les scores de chaque equipe
 * @param nom_equipes Le tableau contenant le nom de chaque equipe
*/
void print_resultats_finaux_tournoi(int scores[NB_EQUIPES], char nom_equipes[NB_EQUIPES][50]){
	// Vérification des scores finaux
	int score_negatif = 0;
	for (int i = 0; i < NB_EQUIPES; i++) {
		if (scores[i] < 0) { // si un score negatif est detecte, alors on les mets a 0
			scores[i] = 0;
			score_negatif = 1;
		}
	}

	// Affichage des scores finaux
	printf("\nScores Finaux:\n");
	for (int i = 0; i < NB_EQUIPES; i++) {
		printf("\n%s -> %d\n", nom_equipes[i], scores[i]);
	}
	if (score_negatif) {
		printf("\nles scores ont ete verifies \n");
	}


	// Trouver l'indice de l'équipe gagnante et perdante
	int index_max = 0;
	for (int i = 1; i < NB_EQUIPES; i++) {
		if (scores[i] > scores[index_max]) {
			index_max = i;
		}
	   
	}

	// Afficher le nom de l'équipe gagnante et perdante
	printf("\nL'équipe gagnante est : %s\n", nom_equipes[index_max]);
	printf("Les équipes perdantes sont : \n\n");
	for (int i = 0; i < NB_EQUIPES; i++) {
		if (i != index_max) {
			printf(" \n\n%s", nom_equipes[i]);
		}
	}
    printf("\n\nLes resultats du tournoi ont ete exportes dans le fichier resultats.txt\n\n");
	printf("\n");	
}


/**
 * @brief Fonction principale du programme
 * @param argc nombre d'arguments passes en ligne de commande
 * @param argv tableau  ayant les arguments passes en ligne de commande
 * @return 0 si le programme est bien execute
*/
int main(int argc, char *argv[]) {

    char* file_equipes_nom = "equipes.txt";
    if (argc == 2) {
        file_equipes_nom = argv[1];
    }
	
	FILE* equipe_file = fopen(file_equipes_nom, "r");
	fclose(equipe_file);

    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));

    
	//verifie la puissance de 2 en parcourant chaque ligne du fichier
    int cpt_lignes = compter_lignes(file_equipes_nom);
	if (cpt_lignes == -1) {
		return 1;
	}
	if (!est_puissance_de_2(cpt_lignes)) {
		perror("Le nombre d'équipes dans le fichier doit être une puissance de 2.\n");
		return 1;
	}


    // Initialisation des scores des équipes
    int scores[NB_EQUIPES] = {0};

    // Chargement des noms des équipes depuis un fichier texte
    char nom_equipes[NB_EQUIPES][50];
	if (charger_equipes(file_equipes_nom, nom_equipes) != 0) {
		return 1;
	}
	
	Tours tours[NB_EQUIPES - 1];

    //initialisation et simulation des tours
	init_simuler_tours(tours, nom_equipes, scores);

	//affiche le resultat final sur le terminal
	print_resultats_finaux_tournoi(scores, nom_equipes);

	// Nettoyage
	for (int i = 0; i < NB_EQUIPES - 1; i++) {
		pthread_mutex_destroy(&tours[i].lock);
	}
	
	//appel de la fonction pour ecrire les resultats du tournoi dans un fichier texte de sorti
    ecrire_resultats(tours, nom_equipes, scores, "resultats.txt");

	return 0;

}