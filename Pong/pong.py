import turtle


#ventana 
wn = turtle.Screen()
wn.title ("Pong")
wn.bgcolor ("blue")
wn.setup(width = 800, height=600 )
wn.tracer(0)


#jugador 1
jugador1 = turtle.Turtle()
jugador1.speed(0)
jugador1.shape("square")
jugador1.color("green")
jugador1.penup()
jugador1.goto(-400,0)
jugador1.shapesize(stretch_wid=5, stretch_len=1)

#jugador2
jugador2 = turtle.Turtle()
jugador2.speed(0)
jugador2.shape("square")
jugador2.color("green")
jugador2.penup()
jugador2.goto(-400,0)
jugador2.shapesize(stretch_wid=5, stretch_len=1)


while True: 
    wn.update()
