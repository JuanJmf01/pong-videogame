#include "variablesCompartidas.h"
#include "constantes.h"

int server_socket;
pthread_t hilos_partidas[10];



struct DatosDeJuego datosDeJuego[10];
struct ClientInfo clients[20];
