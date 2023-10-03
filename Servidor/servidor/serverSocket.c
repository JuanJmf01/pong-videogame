#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "serverSocket.h"
#include "../variables/variablesCompartidas.h"

#define ADRESS_IP "localhost"
#define PORT "3930"

#define MAX_CLIENTS 4

void startGame(int clientesAdd, ssize_t bytes_received, struct sockaddr_in client_addr, struct DatosDeJuego *datos)
{
}

void newClient(struct sockaddr_in client_addr)
{
    char client_ip[INET_ADDRSTRLEN]; // Variable para almacenar la direccion IP como una cadena
    int client_port;                 // Variable para almacenar el puerto

    // Obten la direccion IP y el puerto
    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    client_port = ntohs(client_addr.sin_port);

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].socket == -1)
        {
            // Guardar informacion del nuevo cliente en el arreglo clients
            clients[i].socket = server_socket;
            clients[i].client_port = client_port;
            clients[i].client_addr = client_addr;
            printf("PUERTO: %d ; IP: %s \n", client_port, client_ip);
            break;
        }
    }
}

void conectTwoPlayers(struct DatosDeJuego *datos)
{

    /* La linea `fd_set read_fileDescriptors;` declara una variable `read_fileDescriptors` de tipo `fd_set`. `fd_set` es un dato
    estructura utilizada por la funcion `select()` para que verifique si el socket esta listo para la lectura.
    En este caso, se utilizara `read_fileDescriptors` para indicar que sockets deben verificarse por
    `select()` para ver si estan listos para leer (recibir datos). */
    fd_set read_fileDescriptors;

    int max_fileDescriptor;

    /* `FD_ZERO(&read_fileDescriptors);` es una funcion que inicializa el conjunto `read_fileDescriptors` para que este vacio.*/
    FD_ZERO(&read_fileDescriptors);

    /* `FD_SET(server_socket, &read_fileDescriptors);` esta agregando el descriptor de archivo `server_socket` al
    conjunto `read_fileDescriptors`. Esto se hace para indicar que se debe verificar el socket del servidor.*/
    FD_SET(server_socket, &read_fileDescriptors);

    /* Es proposito de 'max_fileDescriptor' es ayudar a select() a saber cual es el valor maximo entre los descriptores de arvhivos (server_socket)
    que se deben estar monitoreando ya que select debe saber cuantos descriptores de arvhivo va a monitorear */
    max_fileDescriptor = server_socket;
    int clientesAdd = 0;

    while (1)
    {
        fd_set temp_fileDescriptor = read_fileDescriptors;

        if (select(max_fileDescriptor + 1, &temp_fileDescriptor, NULL, NULL, NULL) == -1)
        {
            perror("Error en select");
        }

        /* El bucle `for` itera sobre los descriptores de archivo desde 0 hasta `max_fileDescriptor`. Se
        se usa para verificar cada descriptor de archivo para ver si esta listo para leer usando `FD_ISSET`
        macro. */
        for (int i = 0; i <= max_fileDescriptor; i++)
        {
            /* La sentencia `if` comprueba si el socket del servidor esta listo para lectura*/
            if (FD_ISSET(server_socket, &temp_fileDescriptor))
            {

                struct sockaddr_in client_addr;
                socklen_t addr_size = sizeof(client_addr);
                char verification_message[1024];
                ssize_t bytes_received;

                bytes_received = recvfrom(server_socket, verification_message, sizeof(verification_message), 0, (struct sockaddr *)&client_addr, &addr_size);

                if (strncmp(verification_message, "clientConnect", strlen("clientConnect")) == 0)
                {
                    printf("Mensaje del cliente: %s\n", verification_message);
                    newClient(client_addr);
                    clientesAdd++;
                    cliente++;
                }
                else
                {
                    // startGame(clientesAdd, bytes_received, client_addr, datos);
                    verification_message[bytes_received] = '\0';

                    printf("Mensaje del cliente: %s\n", verification_message);
                    // startGame(clientesAdd, temp_fileDescriptor, max_fileDescriptor, datos);

                    int tienePareja = 0;
                    int puertoEmisor = ntohs(client_addr.sin_port);

                    for (int i = 0; i < MAX_CLIENTS; i++)
                    {
                        if (i % 2 != 0)
                        {
                            int valor = clients[i-1].socket;
                            printf("ESTE ES EL VALOR: %d \n", valor);
                        }
                    }

                    if (clientesAdd == 2)
                    {

                        for (int i = 0; i < 2; i++)
                        {
                            int puertoReceptor = clients[i].client_port;

                            if (puertoReceptor != puertoEmisor)
                            {
                                client_addr.sin_family = AF_UNSPEC;
                                client_addr.sin_port = htons(puertoReceptor);
                                inet_pton(AF_UNSPEC, ADRESS_IP, &(client_addr.sin_addr));

                                // Actualizar posicion de raqueta en 'datosDeJuego'
                                if (strncmp(verification_message, "jugador1_up", strlen("jugador1_up")) == 0)
                                {
                                    if (i == 0)
                                    {
                                        datos[0].raqueta_j1 += 20;
                                    }
                                    else if (i == 1)
                                    {
                                        datos[0].raqueta_j2 += 20;
                                    }
                                }
                                if (strncmp(verification_message, "jugador1_down", strlen("jugador1_down")) == 0)
                                {
                                    if (i == 0)
                                    {
                                        datos[0].raqueta_j1 -= 20;
                                    }
                                    else if (i == 1)
                                    {
                                        datos[0].raqueta_j2 -= 20;
                                    }
                                }

                                printf("MI NUEVA POSICION 1: %f\n", datos[0].raqueta_j1);
                                printf("MI NUEVA POSICION 2: %f\n", datos[0].raqueta_j2);

                                sendto(server_socket, verification_message, strlen(verification_message), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
                                printf("Cliente %d dice: %s\n", puertoReceptor, verification_message);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void *defineSocket(void *juegoDatos)
{
    struct DatosDeJuego *datos = (struct DatosDeJuego *)juegoDatos;

    struct addrinfo hints, *res; // Utilizamos addrinfo para representar direcciones y nombres de host

    // make sure the struct is empty
    memset(&hints, 0, sizeof hints);

    // La estructura 'hints' se utiliza para especificar los criterios para la direccion
    hints.ai_family = AF_UNSPEC; // AF_UNSPEC indica que se utilizara para comunicaciones en la capa de red utilizando direcciones IPv4 o IPv6
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // AI_PASSIVE llena la IP por mi (mediante el host en el que se esta ejecutando)

    /*La funcion `getaddrinfo()` se utiliza para obtener la informacion de la direccion IP 'ADRESS_IP'.
    y el puerto 'PORT'. El parametro `hints` se utiliza para especificar los criterios para la direccion.
    Informacion, como la familia de direcciones (IPv4 o IPv6) y el tipo de socket (en este caso,
    `SOCK_DGRAM` para UDP). La informacion de direccion resultante se almacena en la estructura `res`. */
    getaddrinfo(ADRESS_IP, PORT, &hints, &res);

    /* Creacion de un socket para el servidor.*/
    server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (server_socket == -1)
    {
        perror("Error al crear el socket del servidor");
    }

    /* La funcion `bind()` se utiliza para asociar un socket con una direccion y un numero de puerto especificos. En
    En este caso, esta vinculando el `server_socket` a la direccion y el puerto especificados en el `res`.
    Esto permite que el servidor escuche las conexiones entrantes en esa direccion y puerto. */
    if (bind(server_socket, res->ai_addr, res->ai_addrlen) != -1)
    {
        printf("Socket creado correctamente. \n\n");
        conectTwoPlayers(datos);
    }

    freeaddrinfo(res);
}