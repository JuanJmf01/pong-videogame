import turtle


#ventana 
wn = turtle.Screen()
wn.title ("Pong")
wn.bgcolor ("black")
wn.setup(width = 800, height=600 )
wn.tracer(0)


#jugador 1
jugador1 = turtle.Turtle()
jugador1.speed(0)
jugador1.shape("square")
jugador1.color("white")
jugador1.penup()
jugador1.goto(-400,0)
jugador1.shapesize(stretch_wid=5, stretch_len=1)

#jugador2
jugador2 = turtle.Turtle()
jugador2.speed(0)
jugador2.shape("square")
jugador2.color("white")
jugador2.penup()
jugador2.goto(400,0)
jugador2.shapesize(stretch_wid=5, stretch_len=1)

#bola
bola = turtle.Turtle()
bola.speed(0)
bola.shape("circle")
bola.color("white")
bola.penup()
bola.goto(0,0)
bola.dx = 0.5
bola.dy = 0.5


#Linea Division
lineaDiv = turtle.Turtle()
lineaDiv.color("white")
lineaDiv.goto(0,-700)
lineaDiv.goto(0,700)



#funciones 

def jugador1_sube():
    y = jugador1.ycor()
    y += 20
    jugador1.sety(y)

def jugador1_baja():
    y = jugador1.ycor()
    y -= 20
    jugador1.sety(y)

def jugador1_der():
    x = jugador1.xcor()
    x += 20
    jugador1.setx(x)

def jugador1_izq():
    x = jugador1.xcor()
    x -= 20
    jugador1.setx(x)

#teclao
wn.listen()
wn.onkeypress(jugador1_sube, "w")
wn.onkeypress(jugador1_baja, "s")
wn.onkeypress(jugador1_der, "d")
wn.onkeypress(jugador1_izq, "a")



while True: 
    wn.update()
    bola.setx(bola.xcor() + bola.dx)
    bola.sety(bola.ycor() + bola.dy)
    



    #Bordes Base
    if bola.ycor() > 360:
        bola.dy *= -1
    if bola.ycor() < -360:
        bola.dy *= -1



    #Bordes Laterales    
    if bola.xcor() > 450:
        bola.goto(0,0)
        bola.dx *= -1
    if bola.xcor() < -450:
        bola.goto(0,0)
        bola.dx *= -1


    #fisicas

    if ((bola.xcor() > 390 and bola.xcor() < 400)
        and (bola.ycor() < jugador2.ycor() +50
        and bola.ycor() > jugador2.ycor()-50)):
         bola.dx *= -1

    if ((bola.xcor() < -390 and bola.xcor() > -400)
        and (bola.ycor() < jugador1.ycor() +50
        and bola.ycor() > jugador1.ycor()-50)):
         bola.dx *= -1   