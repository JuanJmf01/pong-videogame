#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "serverSocket.h"
#include "../variables/variablesCompartidas.h"
#include "../manejarPelota/manejarPelota.h"
#include "../variables/constantes.h"
#include "../logger/logger.h"

/**
 * La función "crearHiloPartida" crea un nuevo hilo para un juego y asigna jugadores al juego.
 *
 * @param partida El parámetro "partida" representa el número de juego o identificador del nuevo juego
 * siendo creado.
 * @param i El parámetro "i" representa el índice del cliente en la matriz "clientes".
 */
void crearHiloPartida(int partida, int i)
{
    printf("SE DEFINE PARTIDA EN: %d", partida);
    if (clients[i].numeroDePartida == -1 && clients[i + 1].numeroDePartida == -1)
    {
        if (i % 2 == 0)
        {
            clients[i].numeroDePartida = partida;
            clients[i + 1].numeroDePartida = partida;
        }
        else
        {
            clients[i].numeroDePartida = partida;
            clients[i - 1].numeroDePartida = partida;
        }

        // Crea un hilo para la nueva partida y pasa el puntero a la estructura de datos
        if (pthread_create(&hilos_partidas[partida], NULL, enviarPosicionBola, (void *)&datosDeJuego[partida]) != 0)
        {
            log_message(LOG_ERROR, "Error al crear el hilo de la partida");
        }
    }
}

void definirHiloPartida(int i)
{
    int partida = -1;
    if (i >= 2)
    {
        if (i % 2 == 0)
        {
            partida = i / 2;
            crearHiloPartida(partida, i);
        }
        else
        {
            partida = (i - 1) / 2;
            crearHiloPartida(partida, i);
        }
    }
    else
    {
        partida = 0;
        crearHiloPartida(partida, i);
    }
}

/**
 * La función "startGame" se utiliza para inicializar un juego entre dos clientes configurando
 *
 * @param client_addr El parámetro client_addr es una estructura sockaddr_in que representa la
 * información de dirección, incluida la dirección IP y el número de puerto.
 * @param posicionReceptor La variable "posicionReceptor" representa la posición del receptor
 * cliente en la matriz "clientes". Se utiliza para acceder a la información del cliente, como la dirección IP y
 * número de puerto.
 * @param puertoReceptor El parámetro "puertoReceptor" representa el número de puerto del receptor
 * cliente. Se utiliza para especificar el puerto al que se enviará el mensaje de verificación.
 * @param verificar_mensaje El parámetro mensaje_verificación es un puntero a un carácter constante
 *matriz (cadena) que contiene el mensaje a enviar al cliente.
 * @param i El parámetro "i" representa el índice del cliente en la matriz de clientes.
 */
void startGame(struct sockaddr_in client_addr, int posicionReceptor, int puertoReceptor, const char *verification_message, int i)
{

    int partida = clients[posicionReceptor].numeroDePartida;

    definirHiloPartida(i);

    float numero = atof(verification_message);
    if (i % 2 == 0)
    {
        datosDeJuego[partida].raqueta_j1 = numero;
    }
    else
    {
        datosDeJuego[partida].raqueta_j2 = numero;
    }
    printf("DIRECCION EMISOR : %s, PUERTO: %d \n", inet_ntoa(clients[i].client_addr.sin_addr), ntohs(clients[i].client_port));
    printf("DIRECCION RECEPTOR : %s, PUERTO: %d\n", inet_ntoa(clients[posicionReceptor].client_addr.sin_addr), ntohs(clients[posicionReceptor].client_port));
    printf("Cliente %d dice: %s\n", puertoReceptor, verification_message);
    sendto(server_socket, verification_message, strlen(verification_message), 0, (struct sockaddr *)&clients[posicionReceptor].client_addr, sizeof(clients[posicionReceptor].client_addr));
}

/**
 * La función "nuevoCliente" guarda información sobre un nuevo cliente, como su dirección IP, puerto y
 *nombre, en un array llamado "clientes".
 *
 * @param client_addr El parámetro `client_addr` es de tipo `struct sockaddr_in` y representa el
 * información de la dirección del cliente, incluida la dirección IP y el número de puerto.
 * @param nombre El parámetro "nombre" es un puntero a una matriz de caracteres que representa el nombre de
 * el cliente.
 */
void newClient(struct sockaddr_in client_addr, char *nombre)
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
            clients[i].nombre = nombre;
            char log_message_buffer[512]; // Tamaño adecuado según tus necesidades
            snprintf(log_message_buffer, sizeof(log_message_buffer), "Nuevo cliente conectado - Puerto: %d ; IP: %s ; Nombre: %s\n", client_port, client_ip, clients[i].nombre);
            log_message(LOG_INFO, log_message_buffer);
            printf("Nuevo cliente conectado - Puerto: %d ; IP: %s ; Nombre: %s\n\n", client_port, client_ip, clients[i].nombre);
            break;
        }
    }
}

void cancelarHilo(int posicion)
{
    int numeroPartida = clients[posicion].numeroDePartida;
    int resultado;
    char log_message_buffer[94];
    if (numeroPartida >= 0 && numeroPartida < 10)
    {
        resultado = pthread_cancel(hilos_partidas[numeroPartida]);

        if (resultado == 0)
        {
            snprintf(log_message_buffer, sizeof(log_message_buffer), "El hilo en la posición %d ha sido cancelado.\n", numeroPartida);
            log_message(LOG_INFO, log_message_buffer);
        }
        else
        {
            snprintf(log_message_buffer, sizeof(log_message_buffer), "Error al cancelar el hilo en la posición %d. Código de error: %d\n", numeroPartida, resultado);
            log_message(LOG_ERROR, log_message_buffer);
        }
    }
    else
    {
        // El índice está fuera de rango
        snprintf(log_message_buffer, sizeof(log_message_buffer), "Índice fuera de rango: %d\n", numeroPartida);
        log_message(LOG_WARNING, log_message_buffer);
    }
}

void conectTwoPlayers()
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

    while (1)
    {
        fd_set temp_fileDescriptor = read_fileDescriptors;
        if (select(max_fileDescriptor + 1, &temp_fileDescriptor, NULL, NULL, NULL) == -1)
        {
            log_message(LOG_ERROR, "Error en el socket");
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
                verification_message[bytes_received] = '\0';

                if (strncmp(verification_message, "clientConnect:", strlen("clientConnect:")) == 0)
                {
                    printf("MESSAGE %s\n", verification_message);

                    // Verificar que es un mensaje de confirmación
                    const char *delimiter = " ";
                    char *nombre = verification_message + strlen("clientConnect:");
                    newClient(client_addr, nombre);
                }
                else if (strncmp(verification_message, "desconection", strlen("desconection")) == 0)
                {
                    int puertoEmisorDesconectado = ntohs(client_addr.sin_port);
                    for (int i = 0; i < MAX_CLIENTS; i++)
                    {
                        int puertoEmisor_en_clients = clients[i].client_port;

                        if (puertoEmisor_en_clients == puertoEmisorDesconectado && puertoEmisor_en_clients != 0)
                        {
                            cancelarHilo(i);
                        }
                    }
                }
                else
                {
                    verification_message[bytes_received] = '\0';

                    int puertoEmisor_en_socket = ntohs(client_addr.sin_port);
                    char log_message_buffer[1050];

                    for (int i = 0; i < MAX_CLIENTS; i++)
                    {
                        int puertoEmisor_en_clients = clients[i].client_port;

                        if (puertoEmisor_en_socket == puertoEmisor_en_clients)
                        {
                            int puertoReceptor;
                            int valor_jugador;
                            int posicionReceptor;

                            if (i % 2 != 0)
                            {
                                puertoReceptor = clients[i - 1].client_port;
                                valor_jugador = clients[i - 1].socket;
                                posicionReceptor = i - 1;
                            }
                            else
                            {
                                puertoReceptor = clients[i + 1].client_port;
                                valor_jugador = clients[i + 1].socket;
                                posicionReceptor = i + 1;
                            }

                            if (valor_jugador != -1)
                            {
                                snprintf(log_message_buffer, sizeof(log_message_buffer), "Jugador %d dice: %s", puertoEmisor_en_socket, verification_message);

                                log_message(LOG_INFO, log_message_buffer);
                                startGame(client_addr, posicionReceptor, puertoReceptor, verification_message, i);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void defineSocket()
{
    // struct DatosDeJuego *datos = (struct DatosDeJuego *)juegoDatos;

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
        log_message(LOG_ERROR, "Error al crear el socket del servidor");
    }

    /* La funcion `bind()` se utiliza para asociar un socket con una direccion y un numero de puerto especificos. En
    En este caso, esta vinculando el `server_socket` a la direccion y el puerto especificados en el `res`.
    Esto permite que el servidor escuche las conexiones entrantes en esa direccion y puerto. */
    if (bind(server_socket, res->ai_addr, res->ai_addrlen) != -1)
    {
        log_message(LOG_ERROR, "Server socket creado correctamente\n");
        conectTwoPlayers();
    }

    freeaddrinfo(res);
}