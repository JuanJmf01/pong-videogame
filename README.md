# Entrega1: Video Juego Pong
## Introduccion
En este proyecto se realizó un video juego pong en linea para que se pudiese jugar con multiples consolas unidas via internet, el proposito
de este fue aprender como por medio de sockets podriamos conectar un servidor en linea funcional
## Desarrollo
En el desarrollo de este proyecto se utilizaron dependencias como 
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
import socket
import select
import sys
import threading
import turtle
import queue
import pygame

Estas nos sirvieron para la realizacion correcta de todo nuestro proyecto, donde con estas mismas es como se logro realizar todo el proyecto.

En el proyecto hay 2 carpetas principales, el cliente y el servidor, dentro del cliente se encuentra el game y las funcionalidades del cliente, si se quiere hacer algun cambio en la interfaz del juego o de el jugador, se deberia de hacer dentro de esta carpeta
La carpeta del servidor es donde se guarda el servidor, los sockets de este, el manejo de la pelota y las constantes y variables del juego, si quieren realizar algun cambio al servidor, se deberia de hacer en esta carpeta y dependiendo del cambio se realiza en el archivo manejarPelota.c o en serverSocket.c


## Uso
Asegurate de tener los siguientes requisitos instalados:

- [GCC](https://gcc.gnu.org/) (para compilar el codigo)

Abre una terminal y actualiza la lista de paquetes disponibles para asegurarte de que este actualizada:
``` bash 
    sudo apt update
```

Instala GCC y otras herramientas esenciales de desarrollo utilizando el siguiente comando
```bash 
    sudo apt install build-essential
```

Verifica la instalacion nuevamente usando el comando:
```bash
    gcc --version
```

Para compilar el proyecto, utiliza el siguiente comando:

```bash
gcc main.c manejarPelota/manejarPelota.c servidor/serverSocket.c variables/variablesCompartidas.c variables/constantes.c -o main.out
```
Con el comando 
./main.out se inicializa el servidor y se crea un socket.
Con el comando 
python.game.py inicializamos el juego como cliente.

## Conclusiones
Se pudo concluir que: El proyecto era bastante retador por el poco uso y conocimiento que teniamos con respecto a los sockets y los protocolos, sin embargo 
logramos aprender el debido uso de un servidor en linea y como programar este mismo, identificamos el diferente uso de los sockets y como estos funcionan 
aprendimos la escalabilidad que pueden tener los servidores y como estos trabajan en base a esta misma, y comprendimos aun mas la importancia de la
sincronizacion y latencia en estos proyectos
## Referencias 
https://www.ponggame.org
https://www.programacion.com.py/noticias/sockets-en-c-parte-i-linux
https://www.tamos.com/htmlhelp/tg/int/es/udp_upstream_and_downstream_lo.htm
https://www.redeszone.net/tutoriales/internet/tcp-udp-caracteristicas-uso-diferencias/
https://stackoverflow.com/questions/4713582/simple-online-pong-game-network-synchronization
https://barcelonageeks.com/aplicacion-simple-de-cliente-servidor-en-c/
https://es.stackoverflow.com/questions/571255/codigo-de-c-funciona-diferente-en-servidor
