#include "constantes.h"

// Manejo de direcciones y puertos
char ADRESS_IP[16] = "localhost";
char PORT[8] = "3930";

// Escalabilidad
int MAX_CLIENTS = 20;
int MAX_PARTIDAS;

/* Configuracion de la ventana del juego. Las variables que se defienen en la ventana
del juego depenten totalmente del nivel estala. EL nivel escala es utilizado para fines practicos*/

int altoOriginal = 480;
int anchoOriginal = 640;
float nivelEscala = 1.5;
int anchoRaqueta = 10;

int posicionHorizontalRaqueta1 = 50;

int altoPantalla; 
int anchoPantalla;
int altoRaqueta; 
int diametroBola;
int radioBola; 
int velocidad_x;
int velocidad_y;

void inicializarConstantes()
{
    // Escalabilidad
    MAX_PARTIDAS = MAX_CLIENTS / 2;

    // Ventana del juego
    altoPantalla = (int)(nivelEscala * altoOriginal);
    anchoPantalla = (int)(nivelEscala * anchoOriginal);

    altoRaqueta = (int)(80 * nivelEscala);

    diametroBola = (int)(15 * nivelEscala);
    radioBola = (int)(diametroBola / 2);

    velocidad_x = (int)(8 * nivelEscala);
    velocidad_y = (int)(8 * nivelEscala);
}