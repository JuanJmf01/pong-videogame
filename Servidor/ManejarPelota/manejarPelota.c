//ManejarPelota/manejarPelota.c

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "manejarPelota.h"
#include "../variablesCompartidas.h"


#define ADRESS_IP "localhost"
#define PORT "3930"



void *calcularPosicionBola(void *juegoDatos)
{
    struct DatosDeJuego *datos = (struct DatosDeJuego *)juegoDatos;
    // int numPosiciones = datos[0].numPosiciones;

    while (1)
    {
        if (cliente == 2)
        {
            float x = datos[0].posicion_bola_x;
            float y = datos[0].posicion_bola_y;
            float dx = datos[0].dx;
            float dy = datos[0].dy;
            float jugador1 = datos[0].raqueta_j1;
            float jugador2 = datos[0].raqueta_j2;

            x += dx;
            y += dy;

            // printf("MIS POSICIONES: %f : %f,\n", x, y);

            // Detectar choque con la pared superior
            if (y > 350)
            {
                y = 350;
                dy *= -1;
            }

            // Detecta rchoque con la pared inferior
            if (y < -350)
            {
                y = -350;
                dy *= -1;
            }

            // Detectar choque con la pared derecha
            if (x > 440)
            {
                x = 0;
                y = 0;
                dx *= -1;
            }

            // Detectar choque con la pared izquierda
            if (x < -440)
            {
                x = 0;
                y = 0;
                dx *= -1;
            }

            // Choque con la raqueta del jugador 2
            if (x > 390 && y < jugador1 + 50 && y > jugador1 - 50)
            {
                x = 390;
                dx *= -1;
            }

            // Choque con la raqueta del jugador 1
            if (x < -390 && y < jugador2 + 50 && y > jugador2 - 50)
            {
                x = -390;
                dx *= -1;
            }

            datos[0].posicion_bola_x = x;
            datos[0].posicion_bola_y = y;
            datos[0].dx = dx;
            datos[0].dy = dy;

            char buffer_jugador1[64];
            char buffer_jugador2[64];

            snprintf(buffer_jugador1, sizeof(buffer_jugador1), "POSICION_PELOTA:%f,%f,%f,%f", x, y, dx, dy);
            snprintf(buffer_jugador2, sizeof(buffer_jugador2), "POSICION_PELOTA:%f,%f,%f,%f", -x, y, dx, dy);

            struct sockaddr_in client_addr;
            socklen_t addr_size = sizeof(client_addr);

            for (int i = 0; i < 2; i++)
            {
                int puertoReceptor = clients[i].client_port;

                client_addr.sin_family = AF_UNSPEC;
                client_addr.sin_port = htons(puertoReceptor);
                inet_pton(AF_UNSPEC, ADRESS_IP, &(client_addr.sin_addr));

                char buffer[64];

                if (i == 0)
                {
                    // buffer = buffer_jugador1
                    strcpy(buffer, buffer_jugador1);
                }
                if (i == 1)
                {
                    // buffer = buffer_jugador2
                    strcpy(buffer, buffer_jugador2);
                }

                sendto(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
            }
        }
        usleep(50000); // controlar la velocidad de actualización
    }

}
