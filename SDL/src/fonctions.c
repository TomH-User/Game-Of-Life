#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include "fonctions.h"


void SDL_Gestion_erreur (const char *message) {
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit (EXIT_FAILURE);
}

int **Creation_tableau (int colonne, int  ligne) {
    int i, j;

    // Allocation Dynamique tableau 2 dimensions 
    int **tableau = (int**) malloc (sizeof(int*) * colonne);
    if (tableau == NULL) {
        printf("Erreur lors de l'allocation dynamique");
        exit(EXIT_FAILURE);
    }
    for (i=0; i < colonne; i++)
        tableau [i] = (int*) malloc (sizeof(int) * ligne);

    // Remplissage aléatoire du tableau avec des 1 et des 0
    srand(time(NULL));
    for (i = 0; i < colonne; i++) {
        for (j = 0; j < ligne; j++) {
            tableau [i][j] = rand() % 2;
        }
    }
   return tableau;
}

void Affichage (int **tableau, int colonne, int ligne, SDL_Renderer *renderer) {
    int i, j;
    SDL_Rect rectangle1, rectangle2;
    rectangle1.x = 0;
    rectangle1.y = 0;
    rectangle1.w = WINDOW_WIDTH;
    rectangle1.h = WINDOW_HEIGHT;

    // Initialisation de la Couleur = blanc
    if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
        SDL_Gestion_erreur("Impossible de changer la couleur pour le rendu");
    
    // Dessin du fond d'écran grâce à l'insertion du rectangle1
    if (SDL_RenderFillRect(renderer, &rectangle1) != 0)
        SDL_Gestion_erreur("Impossible de dessiner un rectangle1");

    // Changement de la couleur  = noir
    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE) != 0)
        SDL_Gestion_erreur("Impossible de changer la couleur pour le rendu");

    // Dessin des cases vivantes grâce à l'insertion des rectangle2
     for (i = 0; i < COLONNE; i++) {
        for (j = 0; j < LIGNE; j++) {
            if (tableau [i][j] == 1) {              // Si la cellule est vivante, on dessine un rectangle2
                rectangle2.x = i * RESOLUTION;
                rectangle2.y = j * RESOLUTION;
                rectangle2.w = RESOLUTION - 1;      // On soustrait -1 afin de faire apparaître des bordures entre les cellules
                rectangle2.h = RESOLUTION - 1;
                if (SDL_RenderFillRect(renderer, &rectangle2) != 0)
                    SDL_Gestion_erreur("Impossible de dessiner un rectangle2");
            }
        }
    }
}

int **Tableau_suivant (int **tableau, int colonne, int ligne) {
   
    int i, j, etat_cellule, nb_de_voisins;
    int **tableau_suivant = Creation_tableau (COLONNE, LIGNE);

    for (i = 0; i < colonne; i++) {
        for (j = 0; j < ligne; j++) {

                // Pour chaque celulle on enregistre son état et son nombre de voisins
                etat_cellule = tableau[i][j];   
                nb_de_voisins = Compter_nombre_de_voisins (tableau, i, j, colonne, ligne);

                // On définit ainsi l'état de la case dans le tableau_suivant selon les règles du Jeu de la Vie
                if (etat_cellule == 0 && nb_de_voisins == 3)
                    tableau_suivant [i][j] = 1;
                else if (etat_cellule == 1 && (nb_de_voisins < 2 || nb_de_voisins > 3))
                    tableau_suivant [i][j] = 0;
                else 
                    tableau_suivant [i][j] = etat_cellule;
        }
    }
    return tableau_suivant;
}

int Compter_nombre_de_voisins (int **tableau, int x, int y, int colonne, int ligne) {
    int i, j, col, lin, somme_voisins = 0;
        
    for (i = -1; i < 2; i++) {
        for (j = -1; j < 2; j++) {
            col = (x + i + colonne) % colonne ;      //  L'utilisation de l'opérateur modulo permet de gérer le cas des cellules situées aux bords.   
            lin = (y + j + ligne) % ligne ;          //  Cela nous permet de considérer le tableau comme un tableau circulaire.                   
            somme_voisins += tableau [col][lin] ;
        }
    }
    somme_voisins -= tableau [x][y]; // On ne comptabilise pas l'état de la celulle elle même dans la somme des voisins.
    return somme_voisins;
}

void Menu_error (SDL_Window *window, SDL_Renderer *renderer, const char *message) {
    SDL_DestroyRenderer (renderer);
    SDL_DestroyWindow (window);
    SDL_Gestion_erreur(message);
}

void Affichage_Menu (SDL_Window *window, SDL_Renderer *renderer) {
    
    SDL_Rect rectangle;
    rectangle.x = (WINDOW_WIDTH/2) - 210; // Image centré
    rectangle.y = (WINDOW_HEIGHT/2) - 63;
    SDL_Surface *image = NULL;
    SDL_Texture *texture = NULL;
    
    image = SDL_LoadBMP("src/Menu_start3.bmp"); // Chargement de l'image
    // Gestion erreur
    if (image == NULL) {
        Menu_error (window, renderer, "Impossible de charger l'image");
    }

    texture = SDL_CreateTextureFromSurface(renderer, image); // Chargement de la texture
    SDL_FreeSurface(image);
    // Gestion erreurs
    if (texture == NULL) {
        Menu_error (window, renderer, "Impossible de créer la texture");
    }   
    if (SDL_QueryTexture (texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0) {
        Menu_error (window, renderer, "Impossible de charger la texture");
    }
    if (SDL_RenderCopy (renderer, texture, NULL, &rectangle) != 0) {
        Menu_error (window, renderer, "Impossible d'afficher la texture");
    }

    
    SDL_bool program_launched = SDL_TRUE;

    while (program_launched) {  // Affichage de l'image Menu 
        SDL_RenderPresent(renderer);
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            
            switch (event.type) {

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_SPACE)
                        program_launched = SDL_FALSE;
            
                case SDL_QUIT:
                    program_launched = SDL_FALSE;
                    break;

                default:
                    break;
            }
        }
    }
    SDL_DestroyTexture (texture);
}