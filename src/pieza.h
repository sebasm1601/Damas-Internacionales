#ifndef PIEZA_H
#define PIEZA_H

// Enumeraciones para tipo y color de pieza
enum TipoPieza { NORMAL, DAMA };
enum ColorPieza { BLANCO, NEGRO, NINGUNO };

// Estructura que representa una pieza
typedef struct {
    enum ColorPieza color;
    enum TipoPieza tipo;
    int fila;
    int columna;
    int capturada;  // 0 = no capturada, 1 = capturada
} Pieza;

// Funciones para manejar piezas
void inicializarPieza(Pieza* pieza, enum ColorPieza c, int f, int col);
enum ColorPieza obtenerColor(const Pieza* pieza);
enum TipoPieza obtenerTipo(const Pieza* pieza);
int obtenerFila(const Pieza* pieza);
int obtenerColumna(const Pieza* pieza);
int estaCapturada(const Pieza* pieza);
int esDama(const Pieza* pieza);
void establecerPosicion(Pieza* pieza, int f, int col);
void promover(Pieza* pieza);
void capturarPieza(Pieza* pieza);
void descapturarPieza(Pieza* pieza);
char obtenerSimbolo(const Pieza* pieza);
const char* obtenerNombreColor(const Pieza* pieza);

#endif
