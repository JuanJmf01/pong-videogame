import socket
import select
import sys
import threading
import turtle


SERVER_IP = "localhost"
SERVER_PORT = 3930

# Crear un socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Enviar un mensaje de confirmacion al servidor
try:
    client_socket.sendto(b'clientConnect', (SERVER_IP, SERVER_PORT))
    print("Confirmacion enviada")
except: 
    print("Error al enviar la confirmacion")


# Configuracion de la parte grafica del juego Pong
# Ventana 
wn = turtle.Screen()
wn.title ("Pong")
wn.bgcolor ("black")
wn.setup(width = 900, height=700)
wn.tracer(0)



# Interfaz para jugador 1 
jugador1 = turtle.Turtle()
jugador1.speed(0)
jugador1.shape("square")
jugador1.color("white")
jugador1.penup()
jugador1.goto(-400,0)
jugador1.shapesize(stretch_wid=4, stretch_len=0.5)



# Interfaz para jugador 2 
jugador2 = turtle.Turtle()
jugador2.speed(0)
jugador2.shape("square")
jugador2.color("white")
jugador2.penup()
jugador2.goto(400,0)
jugador2.shapesize(stretch_wid=4, stretch_len=0.5)


# Linea de Division
lineaDiv = turtle.Turtle()
lineaDiv.color("white")
lineaDiv.goto(0,-700)
lineaDiv.goto(0,700)


#bola
bola = turtle.Turtle()
bola.speed(0)
bola.shape("circle")
bola.color("white")
bola.penup()
bola.goto(0,0)



# Velocidad de la bola
bola.dx = 0.2  
bola.dy = 0.2




# Funciones para movimiento de jugador 1
def jugador1_sube():
    y = jugador1.ycor()
    y += 20
    #print(y)
    jugador1.sety(y)

    # Envio de mms al cliente
    enviar_posicion_al_servidor("jugador1_up" + str(y))

def jugador1_baja():
    y = jugador1.ycor()
    y -= 20

    jugador1.sety(y)
    enviar_posicion_al_servidor("jugador1_down" + str(y))


# Funciones para movimiento de jugador 2
def jugador2_sube():
    y = jugador2.ycor()
    y += 20
    jugador2.sety(y)


def jugador2_baja():
    y = jugador2.ycor()
    y -= 20

    jugador2.sety(y)


#Enviar posicion al servidor
def enviar_posicion_al_servidor(message):
    print("PRIMERA: ", message)
    client_socket.sendto(message.encode(), (SERVER_IP, SERVER_PORT))


# Actualizar interfaz grafica 
def actualizar_juego(data):
    menssage = data.decode()
    print("Esto llega del otro cliente", menssage)
    if "jugador1_up" in menssage:
        jugador2_sube()
    elif "jugador1_down" in menssage:
        jugador2_baja() 
    elif "jugador2_up" in menssage:
        jugador1_sube()
    elif "jugador2_down" in menssage:
        jugador1_baja()




# Teclado para movimientos de jugador 1
wn.listen()
wn.onkeypress(jugador1_sube, "w")
wn.onkeypress(jugador1_baja, "s")

# Teclado para movimientos de jugador 2
wn.onkeypress(jugador2_sube, "Up")
wn.onkeypress(jugador2_baja, "Down")

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
                #print(read_sockets)
                # Datos recibidos del servidor
                data, server_address = client_socket.recvfrom(1024)
                actualizar_juego(data)
            elif sock == sys.stdin:
                #print(read_sockets)
                # Datos ingresados por el usuario
                message = sys.stdin.readline()
                client_socket.sendto(message.encode(), (SERVER_IP, SERVER_PORT))


def juego_grafica():
    while True:
        wn.update()
        # Actualiza la posicion de la bola
        x = bola.xcor()
        y = bola.ycor()
        x += bola.dx
        y += bola.dy
        bola.setx(x)
        bola.sety(y)

        # Detectar choque con la pared superior
        if y > 350:
            bola.sety(350)  # Ajusta la posicion para evitar que la bola se salga de la ventana
            bola.dy *= -1  # Invierte la direccion vertical

        # Detectar choque con la pared inferior 
        if y < -350:
            bola.sety(-345)
            bola.dy *= -1
        
        # Detectar choque con la pared izquierda
        if x < -440:
            bola.goto(0, 0)  # Vuelve a colocar la bola en el centro
            bola.dx *= -1     # Invierte la direccion horizontal

        # Detectar choque con la pared derecha
        if x > 440:
            bola.goto(0, 0)  # Vuelve a colocar la bola en el centro
            bola.dx *= -1     # Invierte la direccion horizontal

           #Detectar choque con el jugador 1
        if (x > 390) and (y < jugador2.ycor() + 50) and (y > jugador2.ycor() - 50):
            bola.setx(390)  # Ajusta la posicion para evitar que la bola se solape con el jugador
            bola.dx *= -1  # Invierte la direccion horizontal

           #Detectar choque con el jugador 2
        if (x < -390) and (y < jugador1.ycor() + 50) and (y > jugador1.ycor() - 50):
            bola.setx(-390)
            bola.dx *= -1



            
# Como necesitamos ejecutar la parte grafica al mismo tiempo que tener en cuenta
# los mensajes de salida y entrada entonces creamos 2 hilps que se encarguen de ejecutar

# 1. Inicia la funcion de escucha para recibir_enviar_mms
hilo_cliente = threading.Thread(target=recibir_enviar_mms)
hilo_cliente.start()


# 2. Inicia la funcion de escucha de juego_grafica
hilo_grafico = threading.Thread(target=juego_grafica)
hilo_grafico.start()


turtle.done()


# Cerrar el socket

