import select
import socket
import sys
import threading
import queue

SERVER_IP = "54.208.204.28"
SERVER_PORT = 3930

cola_de_mensajes = queue.Queue()

# Crear un socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Enviar un mensaje de confirmación al servidor
def enviar_confirmacion():
    try:
        client_socket.sendto(b'clientConnect', (SERVER_IP, SERVER_PORT))
        print("Confirmación enviada")
    except:
        print("Error al enviar la confirmación")

# Función para enviar mensajes al servidor
def enviar_mensaje_al_servidor(message):
    client_socket.sendto(message.encode(), (SERVER_IP, SERVER_PORT))

# Función para recibir y enviar mensajes del servidor
def recibir_enviar_mensajes():
    while True:
        # Configurar la lista de sockets para select()
        sockets_list = [sys.stdin, client_socket]
        
        # select() es una función que permite monitorear múltiples descriptores de archivos (en este caso, sockets)
        read_sockets, _, _ = select.select(sockets_list, [], [])
        
        for sock in read_sockets:
            if sock == client_socket:
                data, server_address = client_socket.recvfrom(1024)
                data_str = data.decode("utf-8")
                if data_str.startswith("POSICION_PELOTA:"):
                    cola_de_mensajes.put(data_str)
                else:
                    print("Actualizar juego:", data_str)

# Hilo para recibir y enviar mensajes del servidor
hilo_cliente = threading.Thread(target=recibir_enviar_mensajes)
hilo_cliente.start()
