import pygame
import threading
from transporte import enviar_confirmacion, enviar_mensaje_al_servidor

# Inicializar Pygame
pygame.init()

# Configuracion de la ventana del juego
anchoOriginal, altoOriginal = 640, 480
nivelEscala = 1.5   
anchoPantalla, altoPantalla = int(
    anchoOriginal * nivelEscala), int(altoOriginal * nivelEscala)
pantalla = pygame.display.set_mode((anchoPantalla, altoPantalla))
pygame.display.set_caption("TelePong")

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

# Funciones para movimiento de jugador 1
def jugador1_sube():
    y = paletaJugador1.y
    y -= int(5 * nivelEscala)
    paletaJugador1.y = y
    paletaJugador1.y -= int(5 * nivelEscala)
    
    # Enviar mensaje al otro cliente
    enviar_mensaje_al_servidor(f"jugador1_up{y}")

def jugador1_baja():
    y = paletaJugador1.y
    y += int(5 * nivelEscala)
    paletaJugador1.y = y
    paletaJugador1.y += int(5 * nivelEscala)
    
    # Enviar mensaje al otro cliente
    enviar_mensaje_al_servidor(f"jugador1_down{y}")

# ...

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

        # ...

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

# Inicializa el cliente y comienza los hilos
if __name__ == "__main__":
    enviar_confirmacion()  # Envía la confirmación al servidor

    hilo_grafico = threading.Thread(target=juego_grafica)
    hilo_grafico.start()

    # Resto del código para configurar y ejecutar la aplicación del juego
