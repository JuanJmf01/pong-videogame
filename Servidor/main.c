#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "variables/variablesCompartidas.h"
#include "manejarPelota/manejarPelota.h"
#include "servidor/serverSocket.h"


void inicializarArreglo()
{
    for (int i = 0; i < 2; i++)
    {
        clients[i].socket = -1;
    }
}

void inicializarPosicionBola()
{
    for (int i = 0; i < 1; i++)
    {
        datosDeJuego[i].posicion_bola_x = 0;
        datosDeJuego[i].posicion_bola_y = 0;
        datosDeJuego[i].dx = 5;
        datosDeJuego[i].dy = 5;
    }
}

int main()
{
    inicializarArreglo();
    inicializarPosicionBola();

    pthread_t hiloSocket, hiloDeBola;

    if (pthread_create(&hiloSocket, NULL, defineSocket, &datosDeJuego) != 0)
    {
        perror("Error");
        return 1;
    }

    if (pthread_create(&hiloDeBola, NULL, calcularPosicionBola, &datosDeJuego) != 0)
    {
        perror("Error");
        return 1;
    }

    pthread_join(hiloDeBola, NULL);
    pthread_join(hiloSocket, NULL);

    return 0;
}