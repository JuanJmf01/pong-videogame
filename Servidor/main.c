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
#include "variables/constantes.h"

/**
 * La funcion inicializa una serie de clientes estableciendo su socket y numero de partida en -1.
 */
void inicializarArregloClientes()
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        clients[i].socket = -1;
        clients[i].numeroDePartida = -1;
    }
}

/**
 * La funcion inicializa la posicion y velocidad de la pelota, asi como las puntuaciones para dos
 * jugadores en un juego.
 */
void inicializarPosicionBola()
{
    for (int i = 0; i < MAX_PARTIDAS; i++)
    {
        datosDeJuego[i].partida = i;
        datosDeJuego[i].posicion_bola_x = anchoPantalla / 2;
        datosDeJuego[i].posicion_bola_y = altoPantalla / 2;
        datosDeJuego[i].dx = velocidad_x;
        datosDeJuego[i].dy = velocidad_y;
        datosDeJuego[i].puntaje_j1 = 0.0;
        datosDeJuego[i].puntaje_j2 = 0.0;
    }
}



int main()
{
    inicializarConstantes();
    inicializarArregloClientes();
    inicializarPosicionBola();
    defineSocket();


    return 0;
}