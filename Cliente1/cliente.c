#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

// 71692175

int main()
{
    int client_socket;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    // Recordar que getaddrinfo llena la estructura del res con la estructura de hints y el host y puerto proporcionados
    if (getaddrinfo("localhost", "3930", &hints, &res) != 0)
    {
        perror("Error en getaddrinfo");
        exit(EXIT_FAILURE);
    }

    client_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (client_socket == -1)
    {
        perror("Error al crear el socket del cliente");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Socket del cliente creado con éxito.\n");
    }

    if (connect(client_socket, res->ai_addr, res->ai_addrlen) == -1)
    {
        perror("Error al conectar con el servidor");
        close(client_socket);
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Conexión al servidor establecida con éxito.\n");
    }

    // Enviar un mensaje de registro al servidor
    if (sendto(client_socket, "clientConnect", 13, 0, res->ai_addr, res->ai_addrlen) != -1)
    {
        printf("Cnnfirmacion enviada");
    }
    else
    {
        perror("Error al enviar mensaje de confirmación");
        close(client_socket);
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    char message[20];
    char buffer[1024];
    fd_set read_fds;
    int max_fd;

    while (1)
    {

        // Nota: utilizamos STDIN_FILENO solo cuando sea necesario enviar datos por medio de la consola

        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        /* `FD_SET(client_socket, &read_fds);` está agregando el descriptor de archivo `client_socket` al conjunto
        de descriptores de archivos `read_fds`. Esto se hace en preparación para la función `select()`.
        que monitoreará los descriptores de archivos en `read_fds` para detectar cualquier actividad. Si hay
        actividad en `client_socket`, como datos entrantes, será detectada por `select()` y
        Se ejecutará el bloque de código correspondiente. */
        FD_SET(client_socket, &read_fds);

        /* `FD_SET(STDIN_FILENO, &read_fds);` está agregando el descriptor de archivo `STDIN_FILENO` (que
        representa el archivo de entrada estándar) al conjunto `read_fds`. Esto se hace para indicar que el
        El programa debe verificar si la entrada estándar (teclado) está lista para ser leída. */
        max_fd = (STDIN_FILENO > client_socket) ? STDIN_FILENO : client_socket;

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            perror("Error en select");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds))
        {

            // printf("Ingrese un mensaje: (prueba de contexto)");
            fgets(message, sizeof(message), stdin);

            ssize_t bytes_sent;

            // Enviar mensaje al servidor
            // bytes_sent = send(client_socket, message, sizeof(message), 0);
            bytes_sent = sendto(client_socket, message, strlen(message), 0, res->ai_addr, res->ai_addrlen);
            if (bytes_sent == -1)
            {
                perror("Error al enviar datos al servidor");
                close(client_socket);
                freeaddrinfo(res);
                exit(EXIT_FAILURE);
            }
            else
            {
                printf("Mensaje enviado al servor.\n");
            }
        }

        if (FD_ISSET(client_socket, &read_fds))
        {

            memset(buffer, 0, sizeof(buffer));

            // Esperar y recibir una respuesta del servidor
            ssize_t bytes_received = recvfrom(client_socket, buffer, sizeof(buffer), 0, NULL, NULL);
            if (bytes_received == -1)
            {
                perror("Error al recibir datos");
                close(client_socket);
            }

            // Mostrar la respuesta del servidor
            printf("Respuesta del servidor: %s\n", buffer);
        }
    }

    close(client_socket);

    return 0;
}