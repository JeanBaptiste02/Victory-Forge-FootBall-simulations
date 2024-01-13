/**
 * \file ipcTools.h
 * \brief Fichier contenant les declarations des fonctions, des variables, des constantes et les structures de donnees
 * \authors DAMODARANE Jean-Baptiste
 * \version 1.0
 * \date 03/04/2023
*/

#ifndef __ipcTools_h 
#define __ipcTools_h 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>

#include <pthread.h>

#define NB_EQUIPES 8 // nombre d'equipes dans le tournoi
#define NB_MATCHS 4 // nombre de matchs par tour

/**
 * @brief structure de donnees d'un match entre 2 equipes avec leurs scores et le temps
*/
typedef struct {
    int team1; // équipe 1
    int team2; // équipe 2
    int score1; // score de l'équipe 1
    int score2; // score de l'équipe 2
} Match;

/**
 * @brief structure de données d'un tour ayant des matchs
*/
typedef struct {
    pthread_mutex_t lock; // mutex pour synchroniser l'acces aux matchs
    Match matchs[NB_MATCHS]; // tableau pour stcoker les matchs
} Tours;

/**
 * Prototypes
*/
void lire_equipes(char nom_equipes[NB_EQUIPES][50], const char* file_equipes_nom);

int compter_lignes(char* nom_fichier);

int charger_equipes(char *nom_fichier, char nom_equipes[NB_EQUIPES][50]);

int is_power_of_2(int n);

int gener_score_random();

void determiner_gagnant(int sc1, int sc2, int* score1, int* score2);

void* simuler_match(void* arg);

void calcul_score_verif_elimination(Tours* tour, int total_score, int i);

void recherche_equipes_eliminees(Tours* tour, int num_tour);

void simuler_tour(Tours* tour, int num_tour, int equipe_eliminee[NB_EQUIPES], char nom_equipes[NB_EQUIPES][50]);

void init_simuler_tours(Tours tours[NB_EQUIPES - 1], char nom_equipes[NB_EQUIPES][50], int scores[NB_EQUIPES]);

void ecrire_resultats(Tours tours[], char nom_equipes[][50], int scores[], char* nom_fichier_sortie);

void print_resultats_finaux_tournoi(int scores[NB_EQUIPES], char nom_equipes[NB_EQUIPES][50]);

#endif