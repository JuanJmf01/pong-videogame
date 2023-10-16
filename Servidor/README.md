# Nombre del Proyecto
Pong Game - Un juego clásico de Pong implementado en C

## Descripcion corta del proyecto.

Este proyecto es una implementación del clásico juego de Pong en el lenguaje de programación C. Pong es un juego de mesa en el que dos jugadores controlan paletas y tratan de golpear una pelota de un lado al otro de la pantalla sin que la pelota salga del área de juego. El juego utiliza sockets para permitir que dos jugadores se conecten y  jueguen entre sí a través de la red.

## Requisitos Previos

Asegurate de tener los siguientes requisitos instalados:

- [GCC](https://gcc.gnu.org/) (para compilar el codigo)


## Instalacion de requisitos

Abre una tarminal y actualiza la lista de paquetes disponibles para asegurarte de que este actualizada:
``` bash 
    sudo apt update
```

Instala GCC y otras herramientas esenciales de desarrollo utilizando el siguiente comando
```bash 
    sudo apt install build-essential
```

Verifica la instalacion usando el comando:
```bash
    gcc --version
```



## Compilacion y ejecucion

Para compilar el proyecto, utiliza el siguiente comando:

```bash
gcc main.c manejarPelota/manejarPelota.c servidor/serverSocket.c variables/variablesCompartidas.c variables/constantes.c logger/logger.c -o main.out
```

Para ejecutar el proyecto

```bash
./main.out
```

