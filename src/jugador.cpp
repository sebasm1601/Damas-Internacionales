#include "jugador.h"
#include <stdio.h>
#include <string.h>

// Inicializa un jugador
void inicializarJugador(Jugador* jugador, const char* nombre, enum ColorPieza color) {
    strncpy(jugador->nombre, nombre, MAX_NOMBRE - 1);
    jugador->nombre[MAX_NOMBRE - 1] = '\0';
    jugador->color = color;
    jugador->piezasCapturadas = 0;
    jugador->totalMovimientos = 0;
    jugador->damasCreadas = 0;
    inicializarCronometro(&jugador->cronometro);
}

// Obtiene el nombre del jugador
const char* obtenerNombreJugador(const Jugador* jugador) {
    return jugador->nombre;
}

// Obtiene el color del jugador
enum ColorPieza obtenerColorJugador(const Jugador* jugador) {
    return jugador->color;
}

// Obtiene las piezas capturadas por el jugador
int obtenerPiezasCapturadas(const Jugador* jugador) {
    return jugador->piezasCapturadas;
}

// Obtiene el total de movimientos del jugador
int obtenerTotalMovimientos(const Jugador* jugador) {
    return jugador->totalMovimientos;
}

// Obtiene las damas creadas por el jugador
int obtenerDamasCreadas(const Jugador* jugador) {
    return jugador->damasCreadas;
}

// Establece el nombre del jugador
void establecerNombre(Jugador* jugador, const char* nuevoNombre) {
    strncpy(jugador->nombre, nuevoNombre, MAX_NOMBRE - 1);
    jugador->nombre[MAX_NOMBRE - 1] = '\0';
}

// Incrementa el contador de movimientos
void incrementarMovimientos(Jugador* jugador) {
    jugador->totalMovimientos++;
}

// Agrega una pieza capturada
void agregarPiezaCapturada(Jugador* jugador) {
    jugador->piezasCapturadas++;
}

// Agrega una dama creada
void agregarDamaCreada(Jugador* jugador) {
    jugador->damasCreadas++;
}

// Muestra las estadísticas del jugador
void mostrarEstadisticas(const Jugador* jugador, int piezasActivas) {
    char tiempoBuffer[50];
    obtenerTotalFormateado(&jugador->cronometro, tiempoBuffer, sizeof(tiempoBuffer));
    
    printf("\n=== Estadisticas de %s ===\n", jugador->nombre);
    printf("Color: %s\n", obtenerNombreColorJugador(jugador));
    printf("Piezas activas: %d\n", piezasActivas);
    printf("Piezas capturadas del oponente: %d\n", jugador->piezasCapturadas);
    printf("Movimientos realizados: %d\n", jugador->totalMovimientos);
    printf("Damas coronadas: %d\n", jugador->damasCreadas);
    printf("Tiempo total: %s\n", tiempoBuffer);
    printf("===============================\n");
}

// Reinicia las estadísticas del jugador
void reiniciarEstadisticas(Jugador* jugador) {
    jugador->piezasCapturadas = 0;
    jugador->totalMovimientos = 0;
    jugador->damasCreadas = 0;
    reiniciarCronometro(&jugador->cronometro);
}

// Obtiene el nombre del color del jugador
const char* obtenerNombreColorJugador(const Jugador* jugador) {
    return jugador->color == BLANCO ? "Blancas" : "Negras";
}

// Obtiene el puntero al cronómetro del jugador
Cronometro* obtenerCronometro(Jugador* jugador) {
    return &jugador->cronometro;
}

// Obtiene el puntero const al cronómetro del jugador
const Cronometro* obtenerCronometroConst(const Jugador* jugador) {
    return &jugador->cronometro;
}