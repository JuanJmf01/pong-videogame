# Entrega1: Video Juego Pong
## Introduccion
En este proyecto se realizó un video juego pong en linea para que se pudiese jugar con multiples consolas unidas via internet, el proposito
de este fue aprender como por medio de sockets podriamos conectar un servidor en linea funcional
## Desarrollo

A modo breve de explicacion de las funcionalidades, en game.py es donde se inicializa y se definen todas las variables del juego basicas, el tamaño de la pantalla, raquetas y pelota aca tambien se definen las teclas de movimiento de los jugadores y el socket de las mismas.
En manejarPelota es donde tenemos desde el servidor la actualizacion constante de la pelota, sus fisicas y como esta actua cuando choca con la pared, con el jugador o cuando el jugador no le da a la bola, aca tambien se controla el delay que tiene la misma y la tasa de refresco del servidor para que sea mas optimo.
En serverSocket se maneja ya toda la parte del servidor-cliente aca se maneja el control de las partidas y de los clientes que ingresan donde se maneja que por partida solo reciba un maximo de 2 clientes y si llega un tercero este no podria jugar porque haria falta de otro cliente para poder ingresar a la partida, esto se refleja en un ciclo donde por partida solo recibe 2 clientes y si llegan otros 2 clientes este ciclo creara otro hilo y unira los dos clientes a este nuevo hilo, tambien se manejan los sockets del servidor y de los datos del juego.
En constantes se inicializan las variables ya declaradas en los clientes pero ahora en el servidor para que ambos clientes observen la misma pantalla y en variablesCompartidas se inicializan las variables en comun del cliente y del servidor.


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
```bash
./main.out
```
Se inicializa el servidor y se crea un socket, donde se pueden unir dos clientes.

Con el comando 
```bash
python.game.py 
```
 inicializamos el juego como cliente.


## Conclusiones
Se pudo concluir que: El proyecto era bastante retador por el poco uso y conocimiento que teniamos con respecto a los sockets y los protocolos, sin embargo 
logramos aprender el debido uso de un servidor en linea y como programar este mismo, identificamos el diferente uso de los sockets y como estos funcionan 
aprendimos la escalabilidad que pueden tener los servidores y como estos trabajan en base a esta misma, y comprendimos aun mas la importancia de la
sincronizacion y latencia en estos proyectos
A nivel de UDP aprendimos a 

## Referencias 
https://www.ponggame.org
https://www.programacion.com.py/noticias/sockets-en-c-parte-i-linux
https://www.tamos.com/htmlhelp/tg/int/es/udp_upstream_and_downstream_lo.htm
https://www.redeszone.net/tutoriales/internet/tcp-udp-caracteristicas-uso-diferencias/
https://stackoverflow.com/questions/4713582/simple-online-pong-game-network-synchronization
https://barcelonageeks.com/aplicacion-simple-de-cliente-servidor-en-c/
https://es.stackoverflow.com/questions/571255/codigo-de-c-funciona-diferente-en-servidor
