import socket
import select
import sys
import threading
import turtle

import queue


# Cola para intercambiar mensajes entre los hilos
cola_de_mensajes = queue.Queue()

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
                data_str = data.decode("utf-8")  # Decodificar data a str
                if data_str.startswith("POSICION_PELOTA:"):
                    cola_de_mensajes.put(data_str)
                else:
                    actualizar_juego(data)

        


def juego_grafica():
    while True:
        wn.update()
        # Actualiza la posicion de la bola

        if cola_de_mensajes.not_empty:
            mensaje = cola_de_mensajes.get()
            #print("GET COLA: ", mensaje)

            mensaje = mensaje[len("POSICION_PELOTA:"):]
            mensaje = mensaje.replace('\x00', '')
            partes = mensaje.split(',')
        
            if len(partes) == 4:
                # Obtener las partes como valores numéricos
                x, y, dx, dy = map(float, partes)

                # print("X: ", x)
                # print("Y: ", y)

                # Actualiza la posicion de la bola
                bola.setx(x)
                bola.sety(y)
                bola.dx = dx
                bola.dy = dy

            


            
# Como necesitamos ejecutar la parte grafica al mismo tiempo que tener en cuenta
# los mensajes de salida y entrada entonces creamos 2 hilps que se encarguen de ejecutar

# 1. Funcion de escucha para recibir_enviar_mms
hilo_cliente = threading.Thread(target=recibir_enviar_mms)
hilo_cliente.start()


# 2. Funcion de escucha de juego_grafica
hilo_grafico = threading.Thread(target=juego_grafica)
hilo_grafico.start()


turtle.done()


# Cerrar el socket