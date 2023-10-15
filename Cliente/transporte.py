import select
import socket
import sys
import queue
import game



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



# Enviar posicion al servidor
def enviar_posicion_al_servidor(message):
    print("PRIMERA: ", message)
    client_socket.sendto(message.encode(), (SERVER_IP, SERVER_PORT))


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
                    game.actualizar_juego(data)
                    print("Actualizar juego")


