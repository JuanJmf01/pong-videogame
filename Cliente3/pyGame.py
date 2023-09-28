import turtle
import cliente

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



# Teclado para movimientos de jugador 1
wn.listen()
wn.onkeypress(jugador1_sube, "w")
wn.onkeypress(jugador1_baja, "s")




