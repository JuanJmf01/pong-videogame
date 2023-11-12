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

