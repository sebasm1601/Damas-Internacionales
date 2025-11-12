#ifndef JUGADOR_H
#define JUGADOR_H

#include "pieza.h"
#include "cronometro.h"

#define MAX_NOMBRE 100

// Estructura que representa un jugador
typedef struct {
    char nombre[MAX_NOMBRE];
    enum ColorPieza color;
    int piezasCapturadas;
    int totalMovimientos;
    int damasCreadas;
    Cronometro cronometro;
} Jugador;

// Funciones para jugadores
void inicializarJugador(Jugador* jugador, const char* nombre, enum ColorPieza color);
const char* obtenerNombreJugador(const Jugador* jugador);
enum ColorPieza obtenerColorJugador(const Jugador* jugador);
int obtenerPiezasCapturadas(const Jugador* jugador);
int obtenerTotalMovimientos(const Jugador* jugador);
int obtenerDamasCreadas(const Jugador* jugador);
void establecerNombre(Jugador* jugador, const char* nuevoNombre);
void incrementarMovimientos(Jugador* jugador);
void agregarPiezaCapturada(Jugador* jugador);
void agregarDamaCreada(Jugador* jugador);
void mostrarEstadisticas(const Jugador* jugador, int piezasActivas);
void reiniciarEstadisticas(Jugador* jugador);
const char* obtenerNombreColorJugador(const Jugador* jugador);
Cronometro* obtenerCronometro(Jugador* jugador);
const Cronometro* obtenerCronometroConst(const Jugador* jugador);

#endif
