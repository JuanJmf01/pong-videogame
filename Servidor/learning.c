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
    // struct sockaddr_in address;
};

struct ClientInfo clients[2];

void startGame(int server_socket, int clientesAdd, fd_set temp_fds, int max_fd)
{
    if (clientesAdd == 2)
    {
        printf("Estoy listo para recibir mensajes\n");

        for (int i = 0; i <= max_fd; i++)
        {

            if (FD_ISSET(i, &temp_fds))
            {

                struct sockaddr_in client_addr;
                socklen_t addr_size = sizeof(client_addr);
                char buffer[1024];
                ssize_t bytes_received;

                bytes_received = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_size);
                if (bytes_received <= 0)
                {
                    perror("Error al recibir los datos");
                }

                buffer[bytes_received] = '\0';

                for (int i = 0; i < 2; i++)
                {
                    int puertoCliente = clients[i].client_port;
                    // printf("%d \n", puertoCliente);

                    client_addr.sin_family = AF_UNSPEC;
                    client_addr.sin_port = htons(puertoCliente);
                    inet_pton(AF_UNSPEC, ADRESS_IP, &(client_addr.sin_addr));

                    sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, sizeof(client_addr)); // Imprime la información en variables separadas
                    printf("Cliente %d dice: %s\n", puertoCliente, buffer);
                }
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

    int emptySpace = -1;
    for (int i = 0; i < 2; i++)
    {
        if (clients[i].socket == -1)
        {
            emptySpace = i;
            // Guardar informacion del nuevo cliente en el arreglo clients
            clients[emptySpace].socket = server_socket;
            clients[emptySpace].client_port = client_port;
            printf("PUERTO: %d ; ", client_port);
            printf("IP: %s \n", client_ip);
            break;
        }
    }
}

void conectTwoPlayers(int server_socket)
{

    /* La línea `fd_set read_fds;` declara una variable `read_fds` de tipo `fd_set`. `fd_set` es un dato
  estructura utilizada por la función `select()` para que verifique si el socket esta listo para la lectura.
  En este caso, se utilizará `read_fds` para indicar qué sockets deben verificarse
  `select()` para ver si están listos para leer (recibir datos). */
    fd_set read_fds;

    int max_fd; // Descriptor máximo para select

    /* `FD_ZERO(&read_fds);` es una función que inicializa el conjunto `read_fds` para que esté vacío.*/
    FD_ZERO(&read_fds);

    /* `FD_SET(server_socket, &read_fds);` está agregando el descriptor de archivo `server_socket` al
        Conjunto `read_fds`. Esto se hace para indicar que se debe verificar el socket del servidor.
    */
    FD_SET(server_socket, &read_fds);

    /* Es proposito de 'max_fs' es ayudar a select() a saber cual es el valor maximo entre los descriptores de arvhivos (server_socket)
     * que se deben estar monitoreando ya que select debe saber cuantos descriptores de arvhivo va a monitorear
     */
    max_fd = server_socket;
    int clientesAdd = 0;

    while (1)
    {
        fd_set temp_fds = read_fds;

        if (select(max_fd + 1, &temp_fds, NULL, NULL, NULL) == -1)
        {
            perror("Error en select");
           
        }

        // Iteramos sobre los sockets (descriptores de archivos)
        for (int i = 0; i <= max_fd; i++)
        {
            if (FD_ISSET(server_socket, &temp_fds) && clientesAdd < 2)
            {
                struct sockaddr_in client_addr;
                socklen_t addr_size = sizeof(client_addr);
                char verification_message[1024];
                ssize_t bytesConfirmacion = recvfrom(server_socket, verification_message, sizeof(verification_message), 0, (struct sockaddr *)&client_addr, &addr_size);

                if (bytesConfirmacion == -1)
                {
                    perror("Error al recibir datos");
                    continue;
                }
                // printf("MENSAJE ANTES: %s \n", verification_message);
                if (strncmp(verification_message, "clientConnect", strlen("clientConnect")) == 0)
                {
                    printf("Mensaje del cliente: %s\n", verification_message);
                    newClient(server_socket, client_addr);
                    clientesAdd++;
                }
            }
        }

        startGame(server_socket, clientesAdd, temp_fds, max_fd);
    }
}

int defineSocket()
{
    int server_socket, client_socket;
    struct addrinfo hints, *res; // Utilizamos addrinfo para representar direcciones y nombres de host

    memset(&hints, 0, sizeof hints); // make sure the struct is empty

    // La estructura 'hints' se utiliza para especificar los criterios sobre como debe llenar la estructura '*res'
    hints.ai_family = AF_UNSPEC;    // AF_UNSPEC indica que se utilizará para comunicaciones en la capa de red utilizando direcciones IPv4 o IPv6
    hints.ai_socktype = SOCK_DGRAM; // SOCK_DGRAM se utiliza para especificar el tipo de socket que se va a crear tambien podemos utilizar SOCK_STREAM
    hints.ai_flags = AI_PASSIVE;    // AI_PASSIVE llena la IP por mi (mediante el host en el que se esta ejecutando)

    // getaddrinfo(host, puerto, criterios especificados en hints, puntero que se llena con informacion de direcciones resultantes)
    // Recordar que getaddrinfo llena la estructura del 'res' con la estructura de hints, el host y puerto proporcionados
    getaddrinfo(ADRESS_IP, PORT, &hints, &res);

    // Asignamos a 'server_socket' un socket que contiene un dominio (ai_family) de tipo 'AF_UNSPEC'
    // le asignamos un tipo de socket,tipo el cual representa podemos utilizar SOCK_STREAM o SOCK_DGRAM
    // le asignamos un protocolo En latipo el cual representa mayoría de los casos, se establece en 0, lo que indica que el sistema operativo elegirá el protocolo apropiado basado en la combinación de familia de direcciones y tipo de socket
    server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (server_socket == -1)
    {
        perror("Error al crear el socket del servidor");
    }

    // Asignar dirección al socket del servidor (utilizamos bind al lado del servidor para aasignarle una direccion y poder conectar desde el cliente)
    // bind(descriptor de archivo envuelto por socket(), puntero a res que contiene puerto y direccion, longitud en bytes de esa direccion)
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
    for (int i = 0; i < 2; i++)
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