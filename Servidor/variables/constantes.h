#ifndef CONSTANTES_H
#define CONSTANTES_H

// Direcciones y puertos
extern char ADRESS_IP[16];
extern char PORT[8];

// Escalabilidad
extern int MAX_CLIENTS;
extern int MAX_PARTIDAS;

// Pantalla del juego
extern int altoOriginal;
extern int anchoOriginal;
extern float nivelEscala;

extern int altoPantalla;
extern int anchoPantalla;

extern int anchoRaqueta;
extern int altoRaqueta;

extern int posicionHorizontalRaqueta1;

extern int diametroBola;
extern int radioBola;

extern int velocidad_x;
extern int velocidad_y;

void inicializarConstantes();

#endif
