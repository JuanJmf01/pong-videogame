import sys
import threading
import pygame
import random
import transporte


# Inicializar Pygame
pygame.init()


# Configuracion de la ventana del juego
anchoOriginal, altoOriginal = 640, 480
nivelEscala = 1.5
anchoPantalla, altoPantalla = int(
    anchoOriginal * nivelEscala), int(altoOriginal * nivelEscala)
pantalla = pygame.display.set_mode((anchoPantalla, altoPantalla))
pygame.display.set_caption("TelePong")


# Define colores
color_fondo_caja = (0, 0, 0)  # Fondo negro
color_borde_caja = (0, 255, 0)  # Bordes verdes
color_texto = (0, 255, 0)  # Texto en verde

# Definir la ruta de la fuente personalizada (reemplaza con la ubicación de tu fuente)
ruta_fuente = "PressStart2P-Regular.ttf"

# Crear una fuente Pygame
# Ajusta el tamaño de la fuente según tus preferencias
fuente = pygame.font.Font(ruta_fuente, 15)


# Colores
black = (0, 0, 0)
white = (255, 255, 255)
green = (0, 255, 0)

# Fuentes
fuente = pygame.font.Font("PressStart2P-Regular.ttf", 15)
fuente2 = pygame.font.Font("PressStart2P-Regular.ttf", 25)


# Titulo "Pong"
titulo = fuente.render("Pong", True, white)
titulo_rect = titulo.get_rect(center=(anchoOriginal // 2, 50))

# Campo de entrada de nombre
input_rect = pygame.Rect(
    anchoOriginal // 4, altoOriginal // 1.7, anchoOriginal // 2, 32)
input_rect.centerx = pantalla.get_rect().centerx  # Centra horizontalmente
color_activo = pygame.Color('green')
color_inactivo = pygame.Color('dodgerblue2')
color = color_inactivo
nombre = ""
texto = fuente.render(nombre, True, color)
texto_rect = texto.get_rect(center=input_rect.center)

# Mensaje Bienvenido
mensajeBienvenida = fuente2.render(
    'Bienvenido a TelePong! ', True, pygame.Color('green'))
mensajeBienvenida_rect = mensajeBienvenida.get_rect()
mensajeBienvenida_rect.centerx = pantalla.get_rect().centerx  # Centra horizontalmente
mensajeBienvenida_rect.top = altoOriginal // 4  # Define la posición vertical

# Mensaje
mensaje = fuente.render('Ingresa tu apodo para continuar',
                        True, pygame.Color('green'))
mensaje_rect = mensaje.get_rect()
mensaje_rect.centerx = pantalla.get_rect().centerx  # Centra horizontalmente
mensaje_rect.top = altoOriginal // 2  # Define la posición vertical

# Botón "Play"
boton = pygame.Rect(anchoOriginal // 4, altoOriginal *
                    3 / 4, anchoOriginal // 2, 50)
boton.centerx = pantalla.get_rect().centerx  # Centra horizontalmente
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

# Calcular caida de bolitas desde arriba hacia bajo
clock = pygame.time.Clock()
lista_coordenadas = []
for i in range(60):
    x = random.randint(0, anchoPantalla)
    y = random.randint(0, altoPantalla)
    lista_coordenadas.append([x, y])

pygame.display.set_caption("TELEPONG")

# Propiedades de la bola de pantalla de inicio
ball_radius = 20
ball_x = anchoPantalla // 2
ball_y = altoPantalla // 2
ball_speed_x = 3
ball_speed_y = 3

# Funciones para movimiento de jugador 1
def jugador1_sube():
    print("MENSAJE SUBIR")
    y = paletaJugador1.y
    y -= int(5 * nivelEscala)
    paletaJugador1.y = y
    paletaJugador1.y -= int(5 * nivelEscala)

    # Enviar mensaje al otro cliente
    transporte.enviar_posicion_al_servidor(f"{y}")


def jugador1_baja():
    print("MENSAJE BAJAR")
    y = paletaJugador1.y
    y += int(5 * nivelEscala)
    paletaJugador1.y = y
    paletaJugador1.y += int(5 * nivelEscala)

    # Enviar mensaje al otro cliente
    transporte.enviar_posicion_al_servidor(f"{y}")


# Actualizar interfaz grafica
def actualizar_juego(data):
    mensaje = data.decode()
    print("Esto llega del otro cliente", mensaje)

    paletaJugador2.y = int(mensaje)





def juego_inicial():
    global entrada_activa, nombre, ball_x, ball_y, ball_speed_x, ball_speed_y
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
                    transporte.enviaConfirmacionServidor(nombre)
                    juego_grafica()

                    break
                else:
                    print("Ingresa un nombre para continuar")
        pantalla.fill(black)
        for coordenada in lista_coordenadas:
            pygame.draw.circle(pantalla, green, coordenada, 2)
            coordenada[1] += 1
            if (coordenada[1] > altoPantalla):
                coordenada[1] = 0

        # Mueve la bola
        ball_x += ball_speed_x
        ball_y += ball_speed_y

        # Rebote en el eje X
        if ball_x < 0 or ball_x > anchoPantalla:
            ball_speed_x *= -1

        # Rebote en el eje Y
        if ball_y < 0 or ball_y > altoPantalla:
            ball_speed_y *= -1

        pygame.draw.circle(pantalla, white, (ball_x, ball_y), ball_radius)

        pantalla.blit(mensajeBienvenida, mensajeBienvenida_rect)
        pantalla.blit(mensaje, mensaje_rect)

        # Dibujar el campo de entrada y el boton "Play"
        pygame.draw.rect(pantalla, color, input_rect, 2)
        pantalla.blit(texto, texto_rect)
        pygame.draw.rect(pantalla, white, boton)
        pantalla.blit(texto_boton, texto_boton_rect)

        pygame.display.flip()
        clock.tick(30)


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

        if not transporte.cola_de_mensajes.empty():
            mensaje = transporte.cola_de_mensajes.get()

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
                puntaje_jugador1 = int(puntaje1)
                puntaje_jugador2 = int(puntaje2)

                if puntaje_jugador1 == 5:
                    mensaje_ganador = fuente.render(
                        "Jugador 1 ganó!", True, white)
                    mensaje_ganador_rect = mensaje_ganador.get_rect(
                        center=(anchoOriginal // 2, altoOriginal // 2))
                    pantalla.blit(mensaje_ganador, mensaje_ganador_rect)
                    mensaje = "desconection"
                    transporte.enviar_posicion_al_servidor(f"{mensaje}")
                    pygame.display.flip()
                    pygame.time.wait(3000)
                    pygame.quit()
                    sys.exit()
                elif puntaje_jugador2 == 5:
                    mensaje_ganador = fuente.render(
                        "Jugador 2 ganó!", True, white)
                    mensaje_ganador_rect = mensaje_ganador.get_rect(
                        center=(anchoOriginal // 2, altoOriginal // 2))
                    pantalla.blit(mensaje_ganador, mensaje_ganador_rect)
                    pygame.display.flip()
                    pygame.time.wait(3000)
                    pygame.quit()
                    sys.exit()

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


# 1. Funcion de escucha de juego_grafica
hilo_grafico = threading.Thread(target=juego_inicial)
hilo_grafico.start()


# 2. Funcion de escucha para recibir_enviar_mms
hilo_cliente = threading.Thread(target=transporte.recibir_enviar_mms)
hilo_cliente.start()
