import select
import socket
import sys
import threading
import pygame
import queue


SERVER_IP = "54.208.204.28"
SERVER_PORT = 3930

cola_de_mensajes = queue.Queue()

# Crear un socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


# Enviar un mensaje de confirmacion al servidor
try:
    client_socket.sendto(b'clientConnect', (SERVER_IP, SERVER_PORT))
    print("Confirmacion enviada")
except:
    print("Error al enviar la confirmacion")
    
# Inicializar Pygame
pygame.init()

# Configuracion de la ventana del juego
anchoOriginal, altoOriginal = 640, 480
nivelEscala = 1.5   
anchoPantalla, altoPantalla = int(
    anchoOriginal * nivelEscala), int(altoOriginal * nivelEscala)
pantalla = pygame.display.set_mode((anchoPantalla, altoPantalla))
pygame.display.set_caption("TelePong")

# print("ANCHO PANTALLA: ", anchoPantalla) #960
# print("Alto PANTALLA: ", altoPantalla)  #720

# Colores
black = (0, 0, 0)
white = (255, 255, 255)

# Palos
anchoRaqueta, altoRaqueta = 10, int(80 * nivelEscala)
paletaJugador1 = pygame.Rect(
    50, altoPantalla // 2 - altoRaqueta // 2, anchoRaqueta, altoRaqueta)
paletaJugador2 = pygame.Rect(anchoPantalla - 50 - anchoRaqueta,
                             altoPantalla // 2 - altoRaqueta // 2, anchoRaqueta, altoRaqueta)

# Bola
bola = pygame.Rect(anchoPantalla // 2 - int(7.5 * nivelEscala), altoPantalla //
                   2 - int(7.5 * nivelEscala), int(15 * nivelEscala), int(15 * nivelEscala))
velocidadBola_x = int(7 * nivelEscala)
velocidadBola_y = int(7 * nivelEscala)

# print("ALTO RAQUETA: ", altoRaqueta) #120
# print("ANCHO RAQUETA: ", anchoRaqueta) #10
# print("ALEJAMIENTO RAQUETA: ", paletaJugador1) # <rect(50, 300, 10, 120)>
# print("BOLA: ", bola) # <rect(469, 349, 22, 22)>


# Funciones para movimiento de jugador 1
def jugador1_sube():
    print("MENSAJE SUBIR")
    y = paletaJugador1.y
    y -= int(5 * nivelEscala)
    paletaJugador1.y = y
    paletaJugador1.y -= int(5 * nivelEscala)

    # Enviar mensaje al otro cliente
    enviar_posicion_al_servidor(f"jugador1_up{y}")


def jugador1_baja():
    print("MENSAJE BAJAR")
    y = paletaJugador1.y
    y += int(5 * nivelEscala)
    paletaJugador1.y = y
    paletaJugador1.y += int(5 * nivelEscala)

    # Enviar mensaje al otro cliente
    enviar_posicion_al_servidor(f"jugador1_down{y}")


# Enviar posicion al servidor
def enviar_posicion_al_servidor(message):
    print("PRIMERA: ", message)
    client_socket.sendto(message.encode(), (SERVER_IP, SERVER_PORT))


# Actualizar interfaz grafica
def actualizar_juego(data):
    mensaje = data.decode()
    print("Esto llega del otro cliente", mensaje)

    if mensaje.startswith("jugador1_up"):
        # Obtener la nueva posición y actualizar al jugador 2
        nueva_posicion = int(mensaje[len("jugador1_up"):])
        paletaJugador2.y = nueva_posicion

    elif mensaje.startswith("jugador1_down"):
        # Obtener la nueva posición y actualizar al jugador 2
        nueva_posicion = int(mensaje[len("jugador1_down"):])
        paletaJugador2.y = nueva_posicion


# Función para el bucle del juego gráfico
def juego_grafica():
    global velocidadBola_x, velocidadBola_y

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

        llave = pygame.key.get_pressed()    
        if llave[pygame.K_w] and paletaJugador1.centery > 0:
            jugador1_sube()

        if llave[pygame.K_s] and paletaJugador1.centery < altoPantalla:
            jugador1_baja()

        if not cola_de_mensajes.empty():
            mensaje = cola_de_mensajes.get()

            #print("GET COLA: ", mensaje)

            mensaje = mensaje[len("POSICION_PELOTA:"):]
            mensaje = mensaje.replace('\x00', '')
            partes = mensaje.split(',')

            if len(partes) == 4:
                # Obtener las partes como valores numéricos
                x, y, dx, dy = map(float, partes)

                # Actualiza la posicion de la bola usando las nuevas coordenadas
                bola.x = x
                bola.y = y
                # Actualiza las velocidades de la bola
                velocidadBola_x = dx
                velocidadBola_y = dy


        # Dibujar todo en la pantalla
        pantalla.fill(black)
        pygame.draw.rect(pantalla, white, paletaJugador1)
        pygame.draw.rect(pantalla, white, paletaJugador2)
        pygame.draw.ellipse(pantalla, white, bola)
        pygame.draw.aaline(pantalla, white, (anchoPantalla //
                           2, 0), (anchoPantalla // 2, altoPantalla))

        pygame.display.flip()

        # Control de velocidad
        pygame.time.Clock().tick(30)

# FUncion para enviar y recibir mensajes del servidor


def recibir_enviar_mms():
    while True:
        # Configurar la lista de sockets para select()
        # La linea `sockets_list = [sys.stdin, client_socket]` esta creando una lista de sockets que seran
        # utilizados por la funcion `select()` para verificar si hay datos disponibles para leer en alguno de los sockets.
        sockets_list = [sys.stdin, client_socket]

        # select() es una funcion que permite a un programa monitorear multiples descriptores de archivos (en este contexto, sockets)
        # para determinar cuales estan listos para lectura, escritura o si se ha producido un error en alguno de ellos
        # - read_sockets: sockets_listos_para_leer: La lista de sockets que estan listos para lectura.
        # - _:            sockets_listos_para_escribir: La lista de sockets que estan listos para escritura.
        # - _:            sockets_con_errores: La lista de sockets que tienen errores.
        # Utilizamos '_' ya que es una variable descarte (No se va utilizar)
        read_sockets, _, _ = select.select(sockets_list, [], [])

        for sock in read_sockets:
            if sock == client_socket:
                # print(read_sockets)
                # Datos recibidos del servidor
                data, server_address = client_socket.recvfrom(1024)
                data_str = data.decode("utf-8")  # Decodificar data a str
                if data_str.startswith("POSICION_PELOTA:"):
                    cola_de_mensajes.put(data_str)
                else:
                    actualizar_juego(data)
                    print("Actualizar juego")


# 1. Funcion de escucha de juego_grafica
hilo_grafico = threading.Thread(target=juego_grafica)
hilo_grafico.start()


# 2. Funcion de escucha para recibir_enviar_mms
hilo_cliente = threading.Thread(target=recibir_enviar_mms)
hilo_cliente.start()
