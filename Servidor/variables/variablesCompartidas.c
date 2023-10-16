#include "variablesCompartidas.h"
#include "constantes.h"

int server_socket;
pthread_t hilos_partidas[30];



struct DatosDeJuego datosDeJuego[30];
struct ClientInfo clients[60];
