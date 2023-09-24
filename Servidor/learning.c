#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>

#define ADRESS_IP "localhost"
#define PORT "3930"

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

struct ClientInfo clients[3];

void startGame(int server_socket, int clientesAdd, fd_set temp_fileDescriptor, int max_fileDescriptor)
{
    if (clientesAdd == 3)
    {
        printf("Estoy listo para recibir mensajes\n");

        for (int i = 0; i <= max_fileDescriptor; i++)
        {

            if (FD_ISSET(i, &temp_fileDescriptor))
            {
                struct sockaddr_in client_addr;
                socklen_t addr_size = sizeof(client_addr);
                char buffer[1024];
                ssize_t bytes_received;

                /* ´recvfrom´ se utiliza para recibir datos de un cliente usando la función `recvfrom()`. Dentro de esta funcion,
                la estructura ´client_addr´ se llenara con la informacion del cliente que envio los datos al igual que la estructura ´addr_size´*/
                bytes_received = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_size);
                if (bytes_received == -1)
                {
                    perror("Error al recibir los datos");
                }

                buffer[bytes_received] = '\0';

                int puertoEmisor = ntohs(client_addr.sin_port);
                for (int i = 0; i < 3; i++)
                {
                    int puertoReceptor = clients[i].client_port;

                    if (puertoReceptor != puertoEmisor)
                    {
                        client_addr.sin_family = AF_UNSPEC;
                        client_addr.sin_port = htons(puertoReceptor);
                        inet_pton(AF_UNSPEC, ADRESS_IP, &(client_addr.sin_addr));

                        sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
                        printf("Cliente %d dice: %s\n", puertoReceptor, buffer);
                        break;
                    }
                }

                // for (int i = 0; i < 2; i++)
                // {
                //     int puertoReceptor = clients[i].client_port;
                //     // printf("%d \n", puertoReceptor);

                //     client_addr.sin_family = AF_UNSPEC;
                //     client_addr.sin_port = htons(puertoReceptor);
                //     inet_pton(AF_UNSPEC, ADRESS_IP, &(client_addr.sin_addr));

                //     sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, sizeof(client_addr)); // Imprime la información en variables separadas
                //     printf("Cliente %d dice: %s\n", puertoReceptor, buffer);
                // }
            }
        }
    }
}

void newClient(int server_socket, struct sockaddr_in client_addr)
{
    char client_ip[INET_ADDRSTRLEN]; // Variable para almacenar la direccion IP como una cadena
    int client_port;                 // Variable para almacenar el puerto

    // Obten la direccion IP y el puerto
    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    client_port = ntohs(client_addr.sin_port);

    for (int i = 0; i < 3; i++)
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

void conectTwoPlayers(int server_socket)
{

    /* La línea `fd_set read_fileDescriptors;` declara una variable `read_fileDescriptors` de tipo `fd_set`. `fd_set` es un dato
    estructura utilizada por la función `select()` para que verifique si el socket esta listo para la lectura.
    En este caso, se utilizará `read_fileDescriptors` para indicar qué sockets deben verificarse por
    `select()` para ver si están listos para leer (recibir datos). */
    fd_set read_fileDescriptors;

    int max_fileDescriptor;

    /* `FD_ZERO(&read_fileDescriptors);` es una función que inicializa el conjunto `read_fileDescriptors` para que esté vacío.*/
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
        printf("SERVER SOCKET: %d \n", server_socket);
        fd_set temp_fileDescriptor = read_fileDescriptors;

        if (select(max_fileDescriptor + 1, &temp_fileDescriptor, NULL, NULL, NULL) == -1)
        {
            perror("Error en select");
        }

        /* El bucle `for` itera sobre los descriptores de archivo desde 0 hasta `max_fileDescriptor`. Se
        se usa para verificar cada descriptor de archivo para ver si está listo para leer usando `FD_ISSET`
        macro. */
        for (int i = 0; i <= max_fileDescriptor; i++)
        {
            /* La sentencia `if` comprueba si el socket del servidor está listo para lectura y si el
             el número de clientes conectados es inferior a 2. */
            if (FD_ISSET(server_socket, &temp_fileDescriptor) && clientesAdd < 3)
            {

                struct sockaddr_in client_addr;
                socklen_t addr_size = sizeof(client_addr);
                char verification_message[1024];
                if (recvfrom(server_socket, verification_message, sizeof(verification_message), 0, (struct sockaddr *)&client_addr, &addr_size) != -1)
                {
                    if (strncmp(verification_message, "clientConnect", strlen("clientConnect")) == 0)
                    {
                        // printf("Mensaje del cliente: %s\n", verification_message);
                        newClient(server_socket, client_addr);
                        clientesAdd++;
                    }
                }
            }
        }

        startGame(server_socket, clientesAdd, temp_fileDescriptor, max_fileDescriptor);
    }
}

int defineSocket()
{
    int server_socket, client_socket;
    struct addrinfo hints, *res; // Utilizamos addrinfo para representar direcciones y nombres de host

    // make sure the struct is empty
    memset(&hints, 0, sizeof hints);

    // La estructura 'hints' se utiliza para especificar los criterios para la direccion
    hints.ai_family = AF_UNSPEC; // AF_UNSPEC indica que se utilizará para comunicaciones en la capa de red utilizando direcciones IPv4 o IPv6
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // AI_PASSIVE llena la IP por mi (mediante el host en el que se esta ejecutando)

    /*La función `getaddrinfo()` se utiliza para obtener la información de la dirección IP 'ADRESS_IP'.
    y el puerto 'PORT'. El parámetro `hints` se utiliza para especificar los criterios para la dirección.
    Información, como la familia de direcciones (IPv4 o IPv6) y el tipo de socket (en este caso,
    `SOCK_DGRAM` para UDP). La información de dirección resultante se almacena en la estructura `res`. */
    getaddrinfo(ADRESS_IP, PORT, &hints, &res);

    /* Creacion de un socket para el servidor.*/
    server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (server_socket == -1)
    {
        perror("Error al crear el socket del servidor");
    }

    /* La función `bind()` se utiliza para asociar un socket con una dirección y un numero de puerto específicos. En
    En este caso, está vinculando el `server_socket` a la dirección y el puerto especificados en el `res`.
    Esto permite que el servidor escuche las conexiones entrantes en esa dirección y puerto. */
    if (bind(server_socket, res->ai_addr, res->ai_addrlen) != -1)
    {
        printf("Socket creado correctamente. \n\n");
        conectTwoPlayers(server_socket);
    }

    freeaddrinfo(res);

    return server_socket;
}

void inicializarArreglo()
{
    for (int i = 0; i < 3; i++)
    {
        clients[i].socket = -1;
    }
}

int main()
{
    inicializarArreglo();
    defineSocket();

    return 0;
}