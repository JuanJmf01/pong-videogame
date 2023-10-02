int cliente;
int server_socket, client_socket;
#ifndef VARIABLESCOMPARTIDAS_H
#define VARIABLESCOMPARTIDAS_H

struct ClientInfo
{
    int socket;
    int client_port;
    /* La línea `struct sockaddr_in client_addr;` declara una variable `client_addr` de
    tipo `struct sockaddr_in`. Esta estructura se utiliza para almacenar la dirección del cliente.
    información, como la dirección IP y el número de puerto. Se utiliza en funciones como
    `recvfrom()` y `sendto()` para especificar la dirección del cliente al recibir o
    enviar datos a través de un socket de red. */
    struct sockaddr_in client_addr;
};

extern struct ClientInfo clients[2];


struct DatosDeJuego
{
    float posicion_bola_x;
    float posicion_bola_y;
    float dx;
    float dy;
    float raqueta_j1;
    float raqueta_j2;
};

struct DatosDeJuego datosDeJuego[1];


#endif
