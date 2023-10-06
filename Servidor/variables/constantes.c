#include "constantes.h"

// Manejo de direcciones y puertos
char ADRESS_IP[16] = "localhost";
char PORT[8] = "3930";

// Escalabilidad
int MAX_CLIENTS = 4;
int MAX_PARTIDAS;

/* Configuracion de la ventana del juego. Las variables que se defienen en la ventana
del juego depenten totalmente del nivel estala. EL nivel escala es utilizado para fines practicos*/

int altoOriginal = 480;
int anchoOriginal = 640;
float nivelEscala = 1.0;
int anchoRaqueta = 10;

int posicionVerticalRaqueta1 = 50;

int altoPantalla;
int anchoPantalla;
int altoRaqueta;
int posicionVerticalRaqueta2;
int diametroBola;
int radioBola;

void inicializarConstantes()
{
    // Escalabilidad
    MAX_PARTIDAS = MAX_CLIENTS / 2;

    // Ventana del juego
    altoPantalla = (int)(nivelEscala * altoOriginal);
    anchoPantalla = (int)(nivelEscala * anchoOriginal);

    altoRaqueta = (int)(80 * nivelEscala);

    posicionVerticalRaqueta2 = anchoPantalla - 50;
    diametroBola = (int)(15 * nivelEscala);
    radioBola = (int)(diametroBola / 2);
}