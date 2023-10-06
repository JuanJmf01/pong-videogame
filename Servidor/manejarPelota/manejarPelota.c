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

#define ADRESS_IP "localhost"
#define PORT "3930"

#define ANCHO_PANTALLA 960
#define LARGO_PANTALLA 720

#define MAX_CLINTS 4

// ANCHO BOLA :
// ANCHO RAQUERA :
// ALTO RAQUETA :
// ALTO PANTALLA :
// ANCHO PANTALLA :

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

        x += dx;
        y += dy;


        // Colisiones
        // Choque con la parte inferior y superior
        if (y >= LARGO_PANTALLA)
        {
            dy *= -1;
        }

        if (y <= 0)
        {
            dy *= -1;
        }

        // Verificar colision con las raquetas
        if (x > (960 - 50 + 11 - 10) && (x - 11 < 960 - 10) &&
            (y + 11 > jugador2) && (y - 11 < jugador2 + 240 / 2))
        {
            dx *= -1;
        }
        if (x < (10 + 50 - 11) && (x + 11 > 10 + 10) &&
            (y + 11 > jugador1) && (y - 11 < jugador1 + 240 / 2))
        {
            dx *= -1;
        }


        if (x <= 0)
        {
            // La bola salio por el lado izquierdo, reiniciar desde el centro hacia la derecha
            x = 960 / 2;
            y = 720 / 2;
            dx *= 1;
        }

        if (x >= 960)
        {
            // La bola salio por el lado derecho, reiniciar desde el centro hacia la izquierda
            x = 960 / 2;
            y = 720 / 2;
            dx *= -1;
        }

        datos->posicion_bola_x = x;
        datos->posicion_bola_y = y;
        datos->dx = dx;
        datos->dy = dy;

        char buffer_jugador1[64];
        char buffer_jugador2[64];

        float x2 = ANCHO_PANTALLA - x;
        float dx2 = -dx;

        snprintf(buffer_jugador1, sizeof(buffer_jugador1), "POSICION_PELOTA:%f,%f,%f,%f", x, y, dx, dy);
        snprintf(buffer_jugador2, sizeof(buffer_jugador2), "POSICION_PELOTA:%f,%f,%f,%f", x2, y, dx2, dy);

        struct sockaddr_in client_addr;
        socklen_t addr_size = sizeof(client_addr);

        for (int i = 0; i < MAX_CLINTS; i++)
        {
            int puertoReceptor = clients[i].client_port;

            client_addr.sin_family = AF_UNSPEC;
            client_addr.sin_port = htons(puertoReceptor);
            inet_pton(AF_UNSPEC, ADRESS_IP, &(client_addr.sin_addr));

            char buffer[64];

            if (i == 0 || i == 2)
            {
                // buffer = buffer_jugador1
                strcpy(buffer, buffer_jugador1);
            }
            if (i == 1 || i == 3)
            {
                // buffer = buffer_jugador2
                strcpy(buffer, buffer_jugador2);
            }

            sendto(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
        }
        usleep(50000); // controlar la velocidad de actualizacion
    }
}
