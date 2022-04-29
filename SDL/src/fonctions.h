#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600
#define RESOLUTION 6
#define COLONNE (WINDOW_WIDTH / RESOLUTION)
#define LIGNE (WINDOW_HEIGHT / RESOLUTION)


void SDL_Gestion_erreur (const char *message);

int **Creation_tableau (int colonne, int ligne);

int Compter_nombre_de_voisins (int **tableau, int x, int y, int colonne, int ligne);

int **Tableau_suivant (int **tableau, int colonne, int ligne);

void Affichage (int **tableau, int colonne, int ligne, SDL_Renderer *renderer);

void Affichage_Menu (SDL_Window *window, SDL_Renderer *renderer);

#endif