# TelePong - Implementación del Juego


## Descripción
    Este proyecto es una implementacion del clasico juego de Pong. Pong es un juego de mesa en el que dos jugadores controlan paletas y tratan de golpear una pelota de un lado al otro de la pantalla sin que la pelota salga del area de juego. Teleping sockets y protololo UDP para permitir que dos jugadores se conecten y  jueguen entre si a traves de la red. Para mas informacion ver Documentacion.docx


## Estructura del Proyecto
- **`/Server`**: Contiene la implementación del servidor en C.
- **`/Client`**: Contiene la implementación del cliente en Python.
- **`/Protocol`**: Detalles sobre el protocolo de comunicación definido para el intercambio de información.


## Detalles Importantes

### Protocolo de Comunicación
    El juego utiliza el protocolo UDP para la transmisión de datos entre el cliente y el servidor. Se ha diseñado un protocolo que define la estructura y el manejo de los mensajes intercambiados.


### Concurrencia
    La concurrencia en el servidor se maneja utilizando hilos para el movimiento y la lógica de la pelota. Esto permite el correcto funcionamiento del juego para múltiples jugadores simultáneos.


### Funcionalidades Clave
-   **Registro de Usuarios**: Los usuarios pueden registrarse en el servidor proporcionando un apodo.
-    **Mantenimiento del Estado del Juego**: El servidor es responsable de mantener el estado del juego, procesar la información de los jugadores y notificar los eventos a todos los clientes conectados.
-   **Log de Eventos**: Se implementó un sistema de logging que imprime por consola y guarda en un archivo todos los eventos relevantes del juego.

- **Cliente**: La ejecución del cliente se realiza utilizando el código Python proporcionado en la carpeta `Client`.


### Compilación
    El código del servidor se compila utilizando `gcc`, siguiendo las instrucciones proporcionadas en el archivo readme.md del servidor. Mientras que el cliente se compila siguiento las instrucciones proporcionadas en el archivo readme.md del cliente


## Integrantes del Equipo
- Juan Jose Muñoz
- Pedro Cardenas
- Andres Rua


