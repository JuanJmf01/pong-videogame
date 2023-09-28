import socket
import select
import sys

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


# Cerrar el socket


