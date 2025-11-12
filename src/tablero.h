#ifndef TABLERO_H
#define TABLERO_H

#include "pieza.h"

#define TAMANO_TABLERO 10
#define MAX_PIEZAS 20

// Estructura que representa el tablero completo
typedef struct {
    Pieza* cuadricula[TAMANO_TABLERO][TAMANO_TABLERO];  // Matriz de punteros a piezas
    Pieza piezasBlancas[MAX_PIEZAS];  // Arreglo de piezas blancas
    Pieza piezasNegras[MAX_PIEZAS];   // Arreglo de piezas negras
    int numPiezasBlancas;             // Contador de piezas blancas
    int numPiezasNegras;              // Contador de piezas negras
} Tablero;

// Funciones para el tablero
void inicializarTablero(Tablero* tablero);
void limpiarTablero(Tablero* tablero);
int esCasillaOscura(int fila, int columna);
int esPosicionValida(int fila, int columna);
Pieza* obtenerPieza(Tablero* tablero, int fila, int columna);
void moverPieza(Tablero* tablero, int filaOrigen, int colOrigen, int filaDestino, int colDestino);
void eliminarPieza(Tablero* tablero, int fila, int columna);
void restaurarPieza(Tablero* tablero, int fila, int columna, Pieza* pieza);
void colocarPieza(Tablero* tablero, int fila, int columna, Pieza* pieza);
int estaVacio(Tablero* tablero, int fila, int columna);
int estaDentroLimites(int fila, int columna);
void mostrarTablero(const Tablero* tablero);

// Funciones auxiliares
int contarPiezasActivasBlancas(const Tablero* tablero);
int contarPiezasActivasNegras(const Tablero* tablero);

#endif