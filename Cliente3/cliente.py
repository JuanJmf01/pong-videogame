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


# Configuración de la parte gráfica del juego Pong
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


# Linea de Division
lineaDiv = turtle.Turtle()
lineaDiv.color("white")
lineaDiv.goto(0,-700)
lineaDiv.goto(0,700)



# Funciones para movimiento de jugador 1

def jugador1_sube():
    y = jugador1.ycor()
    y += 20
    jugador1.sety(y)

    # Envio de mms al cliente


def jugador1_baja():
    y = jugador1.ycor()
    y -= 20
    jugador1.sety(y)

# Funciones para movimiento de jugador 2



#Enviar posicion al servidor
#def enviar_posicion_al_servidor(mensaje):





# Teclado para movimientos de jugador 1
wn.listen()
wn.onkeypress(jugador1_sube, "w")
wn.onkeypress(jugador1_baja, "s")

# FUncion para enviar y recibir mensajes del servidor
def recibir_enviar_mms():
    while True:
         # Configurar la lista de sockets para select()
        # La línea `sockets_list = [sys.stdin, client_socket]` está creando una lista de sockets que seran
        # utilizados por la función `select()` para verificar si hay datos disponibles para leer en alguno de los sockets.
        sockets_list = [sys.stdin, client_socket]

        # select() es una función que permite a un programa monitorear múltiples descriptores de archivos (en este contexto, sockets) 
        # para determinar cuáles están listos para lectura, escritura o si se ha producido un error en alguno de ellos 
        # - read_sockets: sockets_listos_para_leer: La lista de sockets que están listos para lectura.
        # - _:            sockets_listos_para_escribir: La lista de sockets que están listos para escritura.
        # - _:            sockets_con_errores: La lista de sockets que tienen errores.
        # Utilizamos '_' ya que es una variable descarte (No se va utilizar)
        read_sockets, _, _ = select.select(sockets_list, [], [])
    
        for sock in read_sockets:
            if sock == client_socket:
                #print(read_sockets)
                # Datos recibidos del servidor
                data, server_address = client_socket.recvfrom(1024)
                print("Respuesta del servidor:", data.decode())
            elif sock == sys.stdin:
                #print(read_sockets)
                # Datos ingresados por el usuario
                message = sys.stdin.readline()
                client_socket.sendto(message.encode(), (SERVER_IP, SERVER_PORT))


def juego_grafica():
    while True:
        wn.update()


            
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

