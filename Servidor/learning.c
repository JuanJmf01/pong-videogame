#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <netdb.h>

// 93.184.216.34
// IPv4: 127.0.0.1 -> localhost
int main()
{

    struct ClientInfo
    {
        int socket; // El socket del cliente
        int client_port;
        // struct sockaddr_in address; // La dirección del cliente
    };

    // Arreglo para almacenar información de clientes
    struct ClientInfo clients[2];

    // Inicializar el arreglo de clientes en -1 para saber cual esta disponible para guardar informacion de un cliente
    for (int i = 0; i < 2; i++)
    {
        clients[i].socket = -1; // Inicializa todos los sockets a -1 (no utilizados)
    }

    int server_socket, client_socket;
    struct addrinfo hints, *res; // Utilizamos addrinfo para representar direcciones y nombres de host

    /* La línea `fd_set read_fds;` declara una variable `read_fds` de tipo `fd_set`. `fd_set` es un dato
    estructura utilizada por la función `select()` para que verifique si el socket esta listo para la lectura.
    En este caso, se utilizará `read_fds` para indicar qué sockets deben verificarse
    `select()` para ver si están listos para leer (recibir datos). */
    fd_set read_fds;
    int max_fd; // Descriptor máximo para select

    memset(&hints, 0, sizeof hints); // make sure the struct is empty

    // La estructura 'hints' se utiliza para especificar los criterios sobre como debe llenar la estructura '*res'
    hints.ai_family = AF_UNSPEC;    // AF_UNSPEC indica que se utilizará para comunicaciones en la capa de red utilizando direcciones IPv4 o IPv6
    hints.ai_socktype = SOCK_DGRAM; // SOCK_DGRAM se utiliza para especificar el tipo de socket que se va a crear tambien podemos utilizar SOCK_STREAM
    hints.ai_flags = AI_PASSIVE;    // AI_PASSIVE llena la IP por mi (mediante el host en el que se esta ejecutando)

    // getaddrinfo(host, puerto, criterios especificados en hints, puntero que se llena con informacion de direcciones resultantes)
    // Recordar que getaddrinfo llena la estructura del 'res' con la estructura de hints, el host y puerto proporcionados
    getaddrinfo("localhost", "3930", &hints, &res);

    // Asignamos a 'server_socket' un socket que contiene un dominio (ai_family) de tipo 'AF_UNSPEC'
    // le asignamos un tipo de socket,tipo el cual representa podemos utilizar SOCK_STREAM o SOCK_DGRAM
    // le asignamos un protocolo En latipo el cual representa mayoría de los casos, se establece en 0, lo que indica que el sistema operativo elegirá el protocolo apropiado basado en la combinación de familia de direcciones y tipo de socket
    server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (server_socket == -1)
    {
        perror("Error al crear el socket del servidor");
    }
    else
    {
        printf("Socket creado correctamente: -  %d  -\n", server_socket);
    }

    // Asignar dirección al socket del servidor (utilizamos bind al lado del servidor para aasignarle una direccion y poder conectar desde el cliente)
    // bind(descriptor de archivo envuelto por socket(), puntero a res que contiene puerto y direccion, longitud en bytes de esa direccion)
    if (bind(server_socket, res->ai_addr, res->ai_addrlen) != -1)
    {
        perror("Problemas al asignar direccion al socket");
    }
    else
    {
        printf("Direccion asignada al socket correctamente\n");
    }

    /* `FD_ZERO(&read_fds);` es una función que inicializa el conjunto `read_fds` para que esté vacío.*/
    FD_ZERO(&read_fds);

    /* `FD_SET(server_socket, &read_fds);` está agregando el descriptor de archivo `server_socket` al
        Conjunto `read_fds`. Esto se hace para indicar que se debe verificar el socket del servidor.
    */
    FD_SET(server_socket, &read_fds);
    /*ES proposito de 'max_fs' es ayudar a select() a saber cual es el valor maximo entre los descriptores de arvhivos (server_socket)
     * que se deben estar monitoreando ya que select debe saber cuantos descriptores de arvhivo va a monitorear
     */
    max_fd = server_socket;
    int clientesAdd = 0;
    
    while (1)
    {
        fd_set temp_fds = read_fds;
        /**Select monitorea los descriptores de archivos (sockets) en busca de eventos de lectura, es decir,
         * select comprueba si hay datos disponibles para ser leidos en los sockets especificados en 'read_fds.'
         * Si un socket esta listo para la lectura significa que hay datos pendientes en ese socket y select notificara al programa
         * para que permita realizar la operacion y no la bloquee
         * */
        if (select(max_fd + 1, &temp_fds, NULL, NULL, NULL) == -1)
        {
            perror("Error en select");
            exit(1);
        }

        for (int i = 0; i <= max_fd; i++)
        {
            if (FD_ISSET(i, &temp_fds))
            {
                if (i == server_socket)
                {
                    // Nuevo cliente
                    struct sockaddr_in client_addr;
                    socklen_t addr_size = sizeof(client_addr);
                    char buffer[1024];
                    ssize_t bytes_received;

                    bytes_received = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_size);
                    if (bytes_received <= 0)
                    {
                        perror("Error al recibir los datos");
                    }
                    else
                    {
                        buffer[bytes_received] = '\0';

                        char client_ip[INET_ADDRSTRLEN]; // Variable para almacenar la direccion IP como una cadena
                        int client_port;                 // Variable para almacenar el puerto

                        // Obten la direccion IP y el puerto
                        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
                        client_port = ntohs(client_addr.sin_port);

                        int free_slot = -1;
                        for (int i = 0; i < 2; i++)
                        {
                            if (clients[i].socket == -1)
                            {
                                free_slot = i;
                                break;
                            }
                        }

                        if (free_slot != -1)
                        {
                            // Guardar informacion del nuevo cliente en el arreglo clients
                            clients[free_slot].socket = i;
                            clients[free_slot].client_port = client_port;
                            clientesAdd++;
                            // printf("Cliente agregado: %d \n", clientesAdd);
                        }

                        if (clientesAdd == 2)
                        {
                            for (int i = 0; i < 2; i++)
                            {
                                printf("Mostrar informacion de los clientes: ");
                                int puertoCliente = clients[i].client_port;
                                printf("%d \n", puertoCliente);

                                int client_socket = i;
                                client_addr.sin_family = AF_UNSPEC;
                                client_addr.sin_port = htons(puertoCliente);
                                inet_pton(AF_UNSPEC, "localhost", &(client_addr.sin_addr));

                                sendto(server_socket, buffer, 8, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
                            }
                        }

                        // Imprime la información en variables separadas
                        printf("Cliente %s : %d dice: %s\n", client_ip, client_port, buffer);

                        if (strncmp(buffer, "apagar", 6) == 0)
                        {
                            close(server_socket);
                            exit(0);
                        }
                    }
                }
            }
        }

        // Aqui teniamos el recv para recibir mensajes de un cliente
    }

    close(server_socket);
    freeaddrinfo(res);

    return 0;
}