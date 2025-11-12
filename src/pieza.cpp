#include "pieza.h"
#include <stdio.h>
#include <string.h>

// Inicializa una pieza con color y posición
void inicializarPieza(Pieza* pieza, enum ColorPieza c, int f, int col) {
    pieza->color = c;
    pieza->tipo = NORMAL;
    pieza->fila = f;
    pieza->columna = col;
    pieza->capturada = 0;
}

// Obtiene el color de la pieza
enum ColorPieza obtenerColor(const Pieza* pieza) {
    return pieza->color;
}

// Obtiene el tipo de la pieza
enum TipoPieza obtenerTipo(const Pieza* pieza) {
    return pieza->tipo;
}

// Obtiene la fila de la pieza
int obtenerFila(const Pieza* pieza) {
    return pieza->fila;
}

// Obtiene la columna de la pieza
int obtenerColumna(const Pieza* pieza) {
    return pieza->columna;
}

// Verifica si la pieza está capturada
int estaCapturada(const Pieza* pieza) {
    return pieza->capturada;
}

// Verifica si la pieza es una dama
int esDama(const Pieza* pieza) {
    return pieza->tipo == DAMA;
}

// Establece la posición de la pieza
void establecerPosicion(Pieza* pieza, int f, int col) {
    pieza->fila = f;
    pieza->columna = col;
}

// Promueve la pieza a dama
void promover(Pieza* pieza) {
    pieza->tipo = DAMA;
}

// Marca la pieza como capturada
void capturarPieza(Pieza* pieza) {
    pieza->capturada = 1;
}

// Desmarca la pieza como capturada (para deshacer movimientos)
void descapturarPieza(Pieza* pieza) {
    pieza->capturada = 0;
}

// Obtiene el símbolo de la pieza para mostrar en el tablero
char obtenerSimbolo(const Pieza* pieza) {
    if (pieza->color == BLANCO) {
        return pieza->tipo == DAMA ? 'W' : 'w';
    } else if (pieza->color == NEGRO) {
        return pieza->tipo == DAMA ? 'B' : 'b';
    }
    return ' ';
}

// Obtiene el nombre del color de la pieza
const char* obtenerNombreColor(const Pieza* pieza) {
    return pieza->color == BLANCO ? "Blancas" : "Negras";
}
