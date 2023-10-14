// ManejarPelota/manejarPelota.c

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include <ncurses.h>

#include "manejarPelota.h"
#include "../variables/variablesCompartidas.h"
#include "../variables/constantes.h"

void *calcularPosicionBola(void *juegoDatos)
{
    initscr();            // Inicializar ncurses
    timeout(0);           // Establecer timeout para entrada sin bloqueo
    keypad(stdscr, TRUE); // Habilitar teclas especiales (como flechas)

    int isPaused = 0; // Variable para controlar si el programa está pausado o no

    struct DatosDeJuego *datos = (struct DatosDeJuego *)juegoDatos;
    // int numPosiciones = datos[0].numPosiciones;

    while (1)
    {
        if (!isPaused)
        {
            //float x = datos->posicion_bola_x;
            //float y = datos->posicion_bola_y;
            //float dx = datos->dx;
            //float dy = datos->dy;
            //float jugador1 = datos->raqueta_j1;
            //float jugador2 = datos->raqueta_j2;

            int posicionJugador1 = datos->partida * 2;

            datos->posicion_bola_x += datos->dx;
            datos->posicion_bola_y += datos->dy;

            // Colisiones
            // Choque con la parte inferior y superior
            if (datos->posicion_bola_y >= 720)
            {
                datos->dy *= -1;
            }

            if (datos->posicion_bola_y <= 0)
            {
                datos->dy *= -1;
            }

            if (datos->posicion_bola_x <= 0 + 50 + 10 && datos->posicion_bola_y >= datos->raqueta_j1 && datos->posicion_bola_y <= datos->raqueta_j1 + 120)
            {
                datos->dx *= -1;
                printf("POSICION R1: %f\n", datos->raqueta_j1);
                printf("POSICION R2: %f\n", datos->raqueta_j2);
                printf("X: %f - Y: %f\n\n", datos->posicion_bola_x, datos->posicion_bola_y);
                //isPaused = !isPaused;
            }

            if (datos->posicion_bola_x >= (960 - 50 - 10) && datos->posicion_bola_y >= datos->raqueta_j2 && datos->posicion_bola_y <= datos->raqueta_j2 + 120)
            {
                datos->dx *= -1;
                printf("POSICION R1: %f\n", datos->raqueta_j1);
                printf("POSICION R2: %f\n", datos->raqueta_j2);
                printf("X: %f - Y: %f\n\n", datos->posicion_bola_x, datos->posicion_bola_y);

                //isPaused = !isPaused;
            }

            if (datos->posicion_bola_x < 0 + 50)
            {
                // La bola salio por el lado izquierdo, reiniciar desde el centro hacia la derecha
                datos->posicion_bola_x = 960 / 2;
                datos->posicion_bola_y = 720 / 2;
                datos->dx *= -1;
            }

            if (datos->posicion_bola_x > 960 - 50)
            {
                // La bola salio por el lado derecho, reiniciar desde el centro hacia la izquierda
                datos->posicion_bola_x = 960 / 2;
                datos->posicion_bola_y = 720 / 2;
                datos->dx *= -1;
            }


            char buffer_jugador1[64];
            char buffer_jugador2[64];

            // Definimos nuevas variables 'x2' y 'dx2' para manejar vizualizacion en reflejo para jugador opuesto
            float x2 = 960 - datos->posicion_bola_x;
            float dx2 = -datos->dx;

            snprintf(buffer_jugador1, sizeof(buffer_jugador1), "POSICION_PELOTA:%f,%f,%f,%f", datos->posicion_bola_x, datos->posicion_bola_y, datos->dx, datos->dy);
            snprintf(buffer_jugador2, sizeof(buffer_jugador2), "POSICION_PELOTA:%f,%f,%f,%f", x2, datos->posicion_bola_y, dx2, datos->dy);

            for (int i = posicionJugador1; i <= posicionJugador1 + 1; i++)
            {
                int puertoReceptor = clients[i].client_port;
                // printf("POSICION JUGADOR 1 %d", posicionJugador1);

                char buffer[64];

                if (i % 2 == 0)
                {
                    // buffer = buffer_jugador1
                    strcpy(buffer, buffer_jugador1);
                }
                else
                {
                    // buffer = buffer_jugador2
                    strcpy(buffer, buffer_jugador2);
                }

                sendto(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&clients[i].client_addr, sizeof(clients[i].client_addr));
            }
            usleep(33333); // controlar la velocidad de actualizacion
        }
        int ch = getch();
        if (ch == 'S' || ch == 's')
        {
            isPaused = !isPaused;
        }

        refresh();
    }
    endwin();
}
