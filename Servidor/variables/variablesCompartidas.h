//variablesCompartidas.h

#ifndef VARIABLESCOMPARTIDAS_H
#define VARIABLESCOMPARTIDAS_H

#include <netinet/in.h>  // Agrega esta linea

extern int cliente;
extern int server_socket, client_socket;

struct DatosDeJuego
{
    float posicion_bola_x;
    float posicion_bola_y;
    float dx;
    float dy;
    float raqueta_j1;
    float raqueta_j2;
};

extern struct DatosDeJuego datosDeJuego[];

struct ClientInfo
{
    int socket;
    int client_port;
    /* La linea `struct sockaddr_in client_addr;` declara una variable `client_addr` de
    tipo `struct sockaddr_in`. Esta estructura se utiliza para almacenar la direccion del cliente.
    informacion, como la direccion IP y el numero de puerto. Se utiliza en funciones como
    `recvfrom()` y `sendto()` para especificar la direccion del cliente al recibir o
    enviar datos a traves de un socket de red. */
    struct sockaddr_in client_addr;
};

extern struct ClientInfo clients[];

#endif
