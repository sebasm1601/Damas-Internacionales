#include "tablero.h"
#include <stdio.h>
#include <stdlib.h>

// Inicializa el tablero con las piezas en posición inicial
void inicializarTablero(Tablero* tablero) {
    // Inicializar contadores
    tablero->numPiezasBlancas = 0;
    tablero->numPiezasNegras = 0;
    
    // Limpiar cuadricula
    for (int i = 0; i < TAMANO_TABLERO; i++) {
        for (int j = 0; j < TAMANO_TABLERO; j++) {
            tablero->cuadricula[i][j] = NULL;
        }
    }
    
    // Colocar piezas blancas en las primeras 4 filas
    for (int fila = 0; fila < 4; fila++) {
        for (int col = 0; col < TAMANO_TABLERO; col++) {
            if (esCasillaOscura(fila, col)) {
                Pieza* pieza = &tablero->piezasBlancas[tablero->numPiezasBlancas];
                inicializarPieza(pieza, BLANCO, fila, col);
                tablero->cuadricula[fila][col] = pieza;
                tablero->numPiezasBlancas++;
            }
        }
    }
    
    // Colocar piezas negras en las últimas 4 filas
    for (int fila = 6; fila < TAMANO_TABLERO; fila++) {
        for (int col = 0; col < TAMANO_TABLERO; col++) {
            if (esCasillaOscura(fila, col)) {
                Pieza* pieza = &tablero->piezasNegras[tablero->numPiezasNegras];
                inicializarPieza(pieza, NEGRO, fila, col);
                tablero->cuadricula[fila][col] = pieza;
                tablero->numPiezasNegras++;
            }
        }
    }
}

// Limpia solo la cuadrícula del tablero sin tocar las piezas
void limpiarTablero(Tablero* tablero) {
    // Solo limpiar la cuadricula, no tocar las piezas
    for (int i = 0; i < TAMANO_TABLERO; i++) {
        for (int j = 0; j < TAMANO_TABLERO; j++) {
            tablero->cuadricula[i][j] = NULL;
        }
    }
}

// Verifica si una casilla es oscura (jugable)
int esCasillaOscura(int fila, int columna) {
    return (fila + columna) % 2 == 1;
}

// Verifica si una posición está dentro de los límites del tablero
int esPosicionValida(int fila, int columna) {
    return fila >= 0 && fila < TAMANO_TABLERO && columna >= 0 && columna < TAMANO_TABLERO;
}

// Obtiene la pieza en una posición específica
Pieza* obtenerPieza(Tablero* tablero, int fila, int columna) {
    if (!esPosicionValida(fila, columna)) {
        return NULL;
    }
    return tablero->cuadricula[fila][columna];
}

// Mueve una pieza de una posición a otra
void moverPieza(Tablero* tablero, int filaOrigen, int colOrigen, int filaDestino, int colDestino) {
    if (!esPosicionValida(filaOrigen, colOrigen) || !esPosicionValida(filaDestino, colDestino)) {
        return;
    }
    
    Pieza* pieza = tablero->cuadricula[filaOrigen][colOrigen];
    if (pieza != NULL) {
        // Mover la pieza
        tablero->cuadricula[filaDestino][colDestino] = pieza;
        tablero->cuadricula[filaOrigen][colOrigen] = NULL;
        
        // Actualizar posición de la pieza
        establecerPosicion(pieza, filaDestino, colDestino);
        
        // Verificar si debe ser promovida a dama
        if ((obtenerColor(pieza) == BLANCO && filaDestino == TAMANO_TABLERO - 1) ||
            (obtenerColor(pieza) == NEGRO && filaDestino == 0)) {
            if (!esDama(pieza)) {
                promover(pieza);
            }
        }
    }
}

// Elimina una pieza del tablero (la captura)
void eliminarPieza(Tablero* tablero, int fila, int columna) {
    if (!esPosicionValida(fila, columna)) {
        return;
    }
    
    Pieza* pieza = tablero->cuadricula[fila][columna];
    if (pieza != NULL) {
        capturarPieza(pieza);
        tablero->cuadricula[fila][columna] = NULL;
    }
}

// Restaura una pieza en el tablero (para deshacer movimientos)
void restaurarPieza(Tablero* tablero, int fila, int columna, Pieza* pieza) {
    if (!esPosicionValida(fila, columna) || pieza == NULL) {
        return;
    }
    
    tablero->cuadricula[fila][columna] = pieza;
    descapturarPieza(pieza);
    establecerPosicion(pieza, fila, columna);
}

// Coloca una pieza en una posición específica
void colocarPieza(Tablero* tablero, int fila, int columna, Pieza* pieza) {
    if (!esPosicionValida(fila, columna) || pieza == NULL) {
        return;
    }
    
    tablero->cuadricula[fila][columna] = pieza;
    establecerPosicion(pieza, fila, columna);
}

// Verifica si una casilla está vacía
int estaVacio(Tablero* tablero, int fila, int columna) {
    return esPosicionValida(fila, columna) && tablero->cuadricula[fila][columna] == NULL;
}

// Verifica si una posición está dentro de los límites
int estaDentroLimites(int fila, int columna) {
    return esPosicionValida(fila, columna);
}

// Muestra el tablero en la consola
void mostrarTablero(const Tablero* tablero) {
    printf("\n   ");
    
    // Mostrar números de columna
    for (int i = 0; i < TAMANO_TABLERO; i++) {
        printf(" %d ", i);
    }
    printf("\n");
    
    // Mostrar filas del tablero
    for (int fila = 0; fila < TAMANO_TABLERO; fila++) {
        printf(" %d ", fila);
        
        for (int col = 0; col < TAMANO_TABLERO; col++) {
            printf("[");
            
            if (tablero->cuadricula[fila][col] != NULL) {
                // Mostrar símbolo de la pieza
                printf("%c", obtenerSimbolo(tablero->cuadricula[fila][col]));
            } else if (esCasillaOscura(fila, col)) {
                // Casilla oscura vacía
                printf(".");
            } else {
                // Casilla clara
                printf(" ");
            }
            
            printf("]");
        }
        printf("\n");
    }
    
    printf("\nLeyenda: w=blanca, W=Dama blanca, b=negra, B=Dama negra, .=casilla jugable\n");
}

// Cuenta las piezas blancas activas
int contarPiezasActivasBlancas(const Tablero* tablero) {
    int contador = 0;
    for (int i = 0; i < tablero->numPiezasBlancas; i++) {
        if (!estaCapturada(&tablero->piezasBlancas[i])) {
            contador++;
        }
    }
    return contador;
}

// Cuenta las piezas negras activas
int contarPiezasActivasNegras(const Tablero* tablero) {
    int contador = 0;
    for (int i = 0; i < tablero->numPiezasNegras; i++) {
        if (!estaCapturada(&tablero->piezasNegras[i])) {
            contador++;
        }
    }
    return contador;
}