# Entrega1: Video Juego Pong
## Introduccion
En este proyecto se realizó un video juego pong en linea para que se pudiese jugar con multiples consolas unidas via internet, el proposito
de este fue aprender como por medio de sockets podriamos conectar un servidor en linea funcional
## Desarrollo

A modo breve de explicacion de las funcionalidades, en game.py es donde se inicializa y se definen todas las variables del juego basicas, el tamaño de la pantalla, raquetas y pelota aca tambien se definen las teclas de movimiento de los jugadores y el socket de las mismas, tambien la pantalla donde se colocan el apodo los jugadores y el tablero de puntaje
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
Se pudo concluir que: El proyecto era bastante retador por el poco uso y conocimiento que teniamos con respecto a los sockets y los protocolos, sin embargo. 
Logramos aprender el debido uso de un servidor en linea y como programar este mismo, identificamos el diferente uso de los sockets y como estos funcionan para permitir la comunicación en tiempo real entre el servidor y los clientes. 
La elección de protocolos y técnicas adecuadas, en UDP es crucial para garantizar una comunicación eficiente y receptiva en un juego en línea.
Aprendimos la escalabilidad que pueden tener los servidores y como estos trabajan en base a esta misma, y comprendimos aun mas la importancia de la
sincronizacion y latencia en estos proyectos.
A nivel de UDP aprendimos a manejarlo y utilizar su buena comunicacion multicast que estos tienen, aunque llegamos a presentar problemas con la perdida de paquetes, logramos optimizar estas y gestionarla de forma que no afectase la experiencia del usuario como nos pudo afectar en un momento y aprendimos mas de como estos protocolos suelen afectar un proyecto en base cuando no se optimiza completamente.
A nivel de encolamiento pudimos concluir que aunque logramos que no hubiera un encolamiento en el juego y tuviera falencias cuando habian mas de 2 clientes, comprendimos el importante uso de una buena optimizacion a nivel del servidor en la nube donde todo estuviese correctamente organizado y no presentara estos problemas en un futuro.
Se pudo concluir que la implementación de la comunicación en tiempo real entre clientes y servidores es un desafío técnico importante. La elección de protocolos y técnicas adecuadas para transmitir datos de manera eficiente y segura fue esencial para el éxito del juego en línea.
 Aprendimos que en un juego en línea, la optimización del rendimiento es crucial. Esto implica evitar repeticiones innecesarias de cálculos, minimizar la sobrecarga de la red y garantizar que el código sea lo más eficiente posible.

## Referencias 
https://www.ponggame.org
https://www.programacion.com.py/noticias/sockets-en-c-parte-i-linux
https://www.tamos.com/htmlhelp/tg/int/es/udp_upstream_and_downstream_lo.htm
https://www.redeszone.net/tutoriales/internet/tcp-udp-caracteristicas-uso-diferencias/
https://stackoverflow.com/questions/4713582/simple-online-pong-game-network-synchronization
https://barcelonageeks.com/aplicacion-simple-de-cliente-servidor-en-c/
https://es.stackoverflow.com/questions/571255/codigo-de-c-funciona-diferente-en-servidor
