// ManejarPelota/manejarPelota.c

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "manejarPelota.h"
#include "../variables/variablesCompartidas.h"
#include "../variables/constantes.h"

void *calcularPosicionBola(void *juegoDatos)
{

    struct DatosDeJuego *datos = (struct DatosDeJuego *)juegoDatos;
    // int numPosiciones = datos[0].numPosiciones;

    while (1)
    {
        float x = datos->posicion_bola_x;
        float y = datos->posicion_bola_y;
        float dx = datos->dx;
        float dy = datos->dy;
        float jugador1 = datos->raqueta_j1;
        float jugador2 = datos->raqueta_j2;

        int posicionJugador1 = datos->partida * 2;

        x += dx;
        y += dy;

        // Colisiones
        // Choque con la parte inferior y superior
        if (y >= altoPantalla)
        {
            dy *= -1;
        }

        if (y <= 0)
        {
            dy *= -1;
        }

        // Verificar colision con las raquetas
        if (x > (anchoPantalla - posicionVerticalRaqueta1 + radioBola - 10) && (x - radioBola < anchoPantalla - 10) &&
            (y + radioBola > jugador2) && (y - radioBola < jugador2 + 240 / 2))
        {
            dx *= -1;
        }
        if (x < (10 + posicionVerticalRaqueta1 - radioBola) && (x + radioBola > 10 + 10) &&
            (y + radioBola > jugador1) && (y - radioBola < jugador1 + 240 / 2))
        {
            dx *= -1;
        }

        if (x <= 0)
        {
            // La bola salio por el lado izquierdo, reiniciar desde el centro hacia la derecha
            x = anchoPantalla / 2;
            y = altoPantalla / 2;
            dx *= -1;
        }

        if (x >= anchoPantalla)
        {
            // La bola salio por el lado derecho, reiniciar desde el centro hacia la izquierda
            x = anchoPantalla / 2;
            y = altoPantalla / 2;
            dx *= -1;
        }

        datos->posicion_bola_x = x;
        datos->posicion_bola_y = y;
        datos->dx = dx;
        datos->dy = dy;

        char buffer_jugador1[64];
        char buffer_jugador2[64];

        // Definimos nuevas variables 'x2' y 'dx2' para manejar vizualizacion en reflejo para jugador opuesto
        float x2 = anchoPantalla - x;
        float dx2 = -dx;

        snprintf(buffer_jugador1, sizeof(buffer_jugador1), "POSICION_PELOTA:%f,%f,%f,%f", x, y, dx, dy);
        snprintf(buffer_jugador2, sizeof(buffer_jugador2), "POSICION_PELOTA:%f,%f,%f,%f", x2, y, dx2, dy);

        struct sockaddr_in client_addr;
        socklen_t addr_size = sizeof(client_addr);

        for (int i = posicionJugador1; i <= posicionJugador1 + 1; i++)
        {
            int puertoReceptor = clients[i].client_port;
            // printf("POSICION JUGADOR 1 %d", posicionJugador1);

            client_addr.sin_family = AF_UNSPEC;
            client_addr.sin_port = htons(puertoReceptor);
            inet_pton(AF_UNSPEC, ADRESS_IP, &(client_addr.sin_addr));

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

            sendto(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&clients[i].client_addr, sizeof(clients->client_addr));
        }
        usleep(50000); // controlar la velocidad de actualizacion
    }
}
