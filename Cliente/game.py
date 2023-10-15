import select
import socket
import sys
import threading
import pygame
import queue

SERVER_IP = "localhost"
SERVER_PORT = 3930

cola_de_mensajes = queue.Queue(maxsize=5)

# Crear un socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


# Enviar un mensaje de confirmacion al servidor
def enviaConfirmacionServidor(nombre):
    try:
        print("Nombre ingresado:", nombre)
        mensaje = 'clientConnect:' + nombre
        client_socket.sendto(mensaje.encode(), (SERVER_IP, SERVER_PORT))
        print("Confirmacion enviada: ", mensaje)

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

ventana = pygame.display.set_mode((anchoOriginal, altoOriginal))
pygame.display.set_caption("Pong")

# Define colores
color_fondo_caja = (0, 0, 0)  # Fondo negro
color_borde_caja = (0, 255, 0)  # Bordes verdes
color_texto = (0, 255, 0)  # Texto en verde

# Definir la ruta de la fuente personalizada (reemplaza con la ubicación de tu fuente)
ruta_fuente = "PressStart2P-Regular.ttf"

# Crear una fuente Pygame
fuente = pygame.font.Font(ruta_fuente, 15)  # Ajusta el tamaño de la fuente según tus preferencias


# Colores
black = (0, 0, 0)
white = (255, 255, 255)
green = (0, 255, 0)

# Fuentes
fuente = pygame.font.Font("PressStart2P-Regular.ttf", 15)

# Titulo "Pong"
titulo = fuente.render("Pong", True, white)
titulo_rect = titulo.get_rect(center=(anchoOriginal // 2, 50))

# Campo de entrada de nombre
input_rect = pygame.Rect(
    anchoOriginal // 4, altoOriginal // 2, anchoOriginal // 2, 32)
color_activo = pygame.Color('green')
color_inactivo = pygame.Color('dodgerblue2')
color = color_inactivo
nombre = ""
texto = fuente.render(nombre, True, color)
texto_rect = texto.get_rect(center=input_rect.center)

# Mensaje
mensaje = fuente.render('Ingresa tu apodo para continuar', True, pygame.Color('green'))
mensaje_rect = mensaje.get_rect()
mensaje_rect.center = (anchoOriginal // 2, altoOriginal // 4)

# Boton "Play"
boton = pygame.Rect(anchoOriginal // 4, altoOriginal *
                    3 / 4, anchoOriginal // 2, 50)
texto_boton = fuente.render("Play", True, black)
texto_boton_rect = texto_boton.get_rect(center=boton.center)

# Variables para los puntajes iniciales
puntaje_jugador1 = 0
puntaje_jugador2 = 0

# Textos de puntaje (inicialmente vacios)
texto_puntaje_jugador1 = fuente.render('', True, white)
texto_puntaje_jugador2 = fuente.render('', True, white)

# Posiciones de los textos de puntaje
pos_puntaje_jugador1 = (anchoPantalla/10, 20)
pos_puntaje_jugador2 = (anchoPantalla - (anchoPantalla/2.5), 20)

# Palos
anchoRaqueta, altoRaqueta = 10, int(80 * nivelEscala)
paletaJugador1 = pygame.Rect(
    50, altoPantalla // 2 - altoRaqueta // 2, anchoRaqueta, altoRaqueta)
paletaJugador2 = pygame.Rect(anchoPantalla - 50 - anchoRaqueta,
                             altoPantalla // 2 - altoRaqueta // 2, anchoRaqueta, altoRaqueta)

# Bola
bola = pygame.Rect(anchoPantalla // 2 - int(7.5 * nivelEscala), altoPantalla //
                   2 - int(7.5 * nivelEscala), int(15 * nivelEscala), int(15 * nivelEscala))
velocidadBola_x = int(8 * nivelEscala)
velocidadBola_y = int(8 * nivelEscala)

entrada_activa = False

# Funciones para movimiento de jugador 1


def jugador1_sube():
    print("MENSAJE SUBIR")
    y = paletaJugador1.y
    y -= int(5 * nivelEscala)
    paletaJugador1.y = y
    paletaJugador1.y -= int(5 * nivelEscala)

    # Enviar mensaje al otro cliente
    enviar_posicion_al_servidor(f"{y}")


def jugador1_baja():
    print("MENSAJE BAJAR")
    y = paletaJugador1.y
    y += int(5 * nivelEscala)
    paletaJugador1.y = y
    paletaJugador1.y += int(5 * nivelEscala)

    # Enviar mensaje al otro cliente
    enviar_posicion_al_servidor(f"{y}")


# Enviar posicion al servidor
def enviar_posicion_al_servidor(message):
    print("PRIMERA: ", message)
    client_socket.sendto(message.encode(), (SERVER_IP, SERVER_PORT))


# Actualizar interfaz grafica
def actualizar_juego(data):
    mensaje = data.decode()
    print("Esto llega del otro cliente", mensaje)

    paletaJugador2.y = int(mensaje)


def juego_inicial():
    global entrada_activa, nombre
    color = color_inactivo
    texto = fuente.render(nombre, True, color)
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN:
                if input_rect.collidepoint(event.pos):
                    entrada_activa = not entrada_activa
                else:
                    entrada_activa = False
                color = color_activo if entrada_activa else color_inactivo
            if event.type == pygame.KEYDOWN:
                if entrada_activa:
                    if event.key == pygame.K_RETURN:
                        # Aqui puedes manejar la accion cuando se presiona "Enter"
                        print("Nombre ingresado:", nombre)
                    elif event.key == pygame.K_BACKSPACE:
                        nombre = nombre[:-1]
                    else:
                        nombre += event.unicode
                    texto = fuente.render(nombre, True, color)
            # Evento para el boton "Play"
            if event.type == pygame.MOUSEBUTTONDOWN and boton.collidepoint(event.pos) and nombre:
                if nombre is not None:
                    enviaConfirmacionServidor(nombre)
                    juego_grafica()

                    break
                else:
                    print("Ingresa un nombre para continuar")
        
        pantalla.blit(mensaje, mensaje_rect)            

        # Dibujar el campo de entrada y el boton "Play"
        pygame.draw.rect(pantalla, color, input_rect, 2)
        pantalla.blit(texto, texto_rect)
        pygame.draw.rect(pantalla, white, boton)
        pantalla.blit(texto_boton, texto_boton_rect)

        pygame.display.flip()


# Funcion para el bucle del juego grafico
def juego_grafica():
    global velocidadBola_x, velocidadBola_y, puntaje_jugador1, puntaje_jugador2

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

            print("GET COLA: ", mensaje)

            mensaje = mensaje[len("GAME:"):]
            mensaje = mensaje.replace('\x00', '')
            partes = mensaje.split(',')

            if len(partes) == 6:
                # Obtener las partes como valores numericos
                x, y, dx, dy, puntaje1, puntaje2 = map(float, partes)

                # Actualiza la posicion de la bola usando las nuevas coordenadas
                bola.x = x
                bola.y = y
                # Actualiza las velocidades de la bola
                velocidadBola_x = dx
                velocidadBola_y = dy

                # Actualiza los puntajes
                puntaje_jugador1 = str(puntaje1)
                puntaje_jugador2 = str(puntaje2)


        # Dibujar todo en la pantalla
        pantalla.fill(black)
        pygame.draw.rect(pantalla, white, paletaJugador1)
        pygame.draw.rect(pantalla, white, paletaJugador2)
        pygame.draw.ellipse(pantalla, white, bola)
        pygame.draw.aaline(pantalla, white, (anchoPantalla //
                           2, 0), (anchoPantalla // 2, altoPantalla))

        # Actualizar los textos de puntaje
        texto_puntaje_jugador1 = fuente.render(
            f'Jugador 1: {puntaje_jugador1}', True, white)
        texto_puntaje_jugador2 = fuente.render(
            f'Jugador 2: {puntaje_jugador2}', True, white)

        # Dibujar los textos de puntaje en la pantalla
        pantalla.blit(texto_puntaje_jugador1, pos_puntaje_jugador1)
        pantalla.blit(texto_puntaje_jugador2, pos_puntaje_jugador2)

        pygame.display.flip()

        # Control de velocidad
        pygame.time.Clock().tick(15)


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
                if data_str.startswith("GAME:"):
                    try:
                        cola_de_mensajes.put_nowait(data_str)
                    except queue.Full:
                        # La cola esta llena, eliminar el mensaje mas antiguo
                        cola_de_mensajes.get_nowait()  # Eliminar el mensaje mas antiguo
                        cola_de_mensajes.put_nowait(data_str)
                else:
                    actualizar_juego(data)
                    print("Actualizar juego")


# 1. Funcion de escucha de juego_grafica
hilo_grafico = threading.Thread(target=juego_inicial)
hilo_grafico.start()


# 2. Funcion de escucha para recibir_enviar_mms
hilo_cliente = threading.Thread(target=recibir_enviar_mms)
hilo_cliente.start()
