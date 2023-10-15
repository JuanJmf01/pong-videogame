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

void definirPosicionBola(struct DatosDeJuego *datos)
{
    float x = datos->posicion_bola_x;
    float y = datos->posicion_bola_y;
    float dx = datos->dx;
    float dy = datos->dy;
    float jugador1 = datos->raqueta_j1;
    float jugador2 = datos->raqueta_j2;

    x += dx;
    y += dy;

    // Choque con la parte inferior y superior de la pantalla
    if (y >= altoPantalla)
    {
        dy *= -1;
    }

    if (y <= 0)
    {
        dy *= -1;
    }

    // CHoque con la raqueta del jugador1
    if (x <= 0 + posicionHorizontalRaqueta1 + 10 && y >= jugador1 && y <= jugador1 + altoRaqueta)
    {
        dx *= -1;
    }

    // Choque con la raqueta del jugador2
    if (x >= (anchoPantalla - posicionHorizontalRaqueta1 - 10) && y >= jugador2 && y <= jugador2 + altoRaqueta)
    {
        dx *= -1;
    }

    // Choque con el lado izquierdo de la pantalla
    if (x < 0 + posicionHorizontalRaqueta1)
    {
        x = anchoPantalla / 2;
        y = altoPantalla / 2;
        dx *= -1;
    }

    // Choque con el lado derecho de la pantalla
    if (x > anchoPantalla - posicionHorizontalRaqueta1)
    {
        x = anchoPantalla / 2;
        y = altoPantalla / 2;
        dx *= -1;
    }

    datos->posicion_bola_x = x;
    datos->posicion_bola_y = y;
    datos->dx = dx;
    datos->dy = dy;
}

void *enviarPosicionBola(void *juegoDatos)
{

    struct DatosDeJuego *datos = (struct DatosDeJuego *)juegoDatos;
    // int numPosiciones = datos[0].numPosiciones;

    while (1)
    {
        int posicionJugador1 = datos->partida * 2;

        float x = datos->posicion_bola_x;
        float y = datos->posicion_bola_y;
        float dx = datos->dx;
        float dy = datos->dy;
        definirPosicionBola(datos);

        char buffer_jugador1[64];
        char buffer_jugador2[64];

        // Definimos nuevas variables 'x2' y 'dx2' para manejar vizualizacion en reflejo para jugador opuesto
        float x2 = anchoPantalla - x;
        float dx2 = -dx;

        snprintf(buffer_jugador1, sizeof(buffer_jugador1), "POSICION_PELOTA:%f,%f,%f,%f", x, y, dx, dy);
        snprintf(buffer_jugador2, sizeof(buffer_jugador2), "POSICION_PELOTA:%f,%f,%f,%f", x2, y, dx2, dy);

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
        usleep(66666); // controlar la velocidad de actualizacion
    }
}
