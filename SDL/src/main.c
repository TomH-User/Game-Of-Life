#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include "fonctions.h"

#define WINDOW_WIDTH 1000   // Si modification, doit également être modifié dans le fichier "fonction.h"
#define WINDOW_HEIGHT 600   // Si modification, doit également être modifié dans le fichier "fonction.h"
#define RESOLUTION 6        // Si modification, doit également être modifié dans le fichier "fonction.h"
#define COLONNE (WINDOW_WIDTH / RESOLUTION)
#define LIGNE (WINDOW_HEIGHT / RESOLUTION)

int main (int argc, char **argv) {

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    //  Création du tableau qui contiendra de manière aléatoire des 0 et des 1
    int **tableau = Creation_tableau (COLONNE, LIGNE);   

// -------------------------------------------------- Initialisation SDL, fenêtre et rendu  -------------------------------------------------------------------
    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
        SDL_Gestion_erreur("Impossible d'initialiser la SDL");
    
    //  Création de la fenêtre
    window = SDL_CreateWindow("Jeu de la Vie",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (window == NULL)
        SDL_Gestion_erreur("Impossible de creer la fenetre");
    
    //  Création du rendu
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
        SDL_Gestion_erreur("Impossible de creer le rendue");

// ------------------------------------------------- Affichage Jeu de la vie et gestion des évènements --------------------------------------------------------------------

    // Affichage du menu
    Affichage_Menu (window, renderer);
    SDL_RenderPresent(renderer);

    //  Affichage du tableau initiale
    Affichage (tableau, COLONNE, LIGNE, renderer);
    
    //  Démarrage de la gestion des évènements
    SDL_bool program_launched = SDL_TRUE;

    while (program_launched) {

        SDL_Event event;
        SDL_WaitEvent (&event);

            switch (event.type) {
                
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_SPACE) {                       // Affichage du tableau suivant si appui sur la touche espace
                        tableau = Tableau_suivant (tableau, COLONNE, LIGNE);
                        Affichage (tableau, COLONNE, LIGNE, renderer);
                        break;
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) {                 // Sortie du jeu si appui sur la touche échap
                        program_launched = SDL_FALSE;
                        break;
                    }
                    else 
                        break;

                case SDL_QUIT:
                    program_launched = SDL_FALSE;
                    break;

                default: break;
            }

        SDL_Delay(20);     // GESTION DE LA VITESSE DE PROPAGATION. Choix du temps d'attente (en milisecondes) entre chaque affichage
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(tableau);

    return EXIT_SUCCESS;
}


/* 
   Ligne de commande à écrire pour la compilation du programme :
    gcc src/*.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2  

   Pour éxécuter le programme : 
    bin\prog.exe                                                            
*/