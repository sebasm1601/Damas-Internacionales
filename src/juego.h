#ifndef JUEGO_H
#define JUEGO_H

#include "tablero.h"
#include "jugador.h"
#include "movimiento.h"
#include "cronometro.h"
#include "exportador_csv.h"
#include "guardador_juego.h"

#define MAX_MOVIMIENTOS_POSIBLES 100

// Estructura para almacenar direcciones de movimiento
typedef struct {
    int dFila;
    int dColumna;
} Direccion;

// Estructura principal del juego
typedef struct {
    Tablero tablero;
    Jugador jugador1;
    Jugador jugador2;
    Jugador* jugadorActual;
    HistorialMovimientos historialMovimientos;
    int juegoTerminado;  // 0 = en curso, 1 = terminado
    Jugador* ganador;
    RegistroJuego registroJuego;
    char fechaInicioJuego[50];
} Juego;

// Funciones principales del juego
void inicializarJuego(Juego* juego);
void iniciarNuevaPartida(Juego* juego);
void configurarJugadores(Juego* juego, const char* nombre1, const char* nombre2);

// Getters
Jugador* obtenerJugadorActual(const Juego* juego);
int estaJuegoTerminado(const Juego* juego);
Jugador* obtenerGanador(const Juego* juego);

// Operaciones de juego
int realizarMovimiento(Juego* juego, int filaOrigen, int colOrigen, int filaDestino, int colDestino);
void mostrarTableroJuego(const Juego* juego);
void mostrarEstadoJuego(const Juego* juego);
void mostrarEstadisticasJugadores(const Juego* juego);

// Validación de movimientos
int esMovimientoSimpleValido(Juego* juego, int filaOrigen, int colOrigen, int filaDestino, int colDestino);
int esCapturaValida(Juego* juego, int filaOrigen, int colOrigen, int filaDestino, int colDestino);

// Búsqueda de capturas
int encontrarTodasCapturas(Juego* juego, enum ColorPieza jugador, Movimiento capturas[], int* numCapturas);
int encontrarCapturasParaPieza(Juego* juego, int fila, int columna, Movimiento capturas[], int* numCapturas);
void encontrarCapturasRecursivas(Juego* juego, int fila, int columna, Movimiento* movActual,
                                 Movimiento todasCapturas[], int* numCapturas,
                                 int visitado[TAMANO_TABLERO][TAMANO_TABLERO]);

// Utilidades
int tieneMovimientosLegales(Juego* juego, enum ColorPieza jugador);
void cambiarJugador(Juego* juego);
void verificarFinJuego(Juego* juego);
void obtenerDirecciones(const Pieza* pieza, Direccion direcciones[], int* numDirecciones);

// Menú y reglas
void mostrarMenu(Juego* juego);
void mostrarReglas();
void mostrarHistorialMovimientos(const Juego* juego);

// Funciones de exportación y guardado
void exportarJuegoACSV(Juego* juego);
int guardarPartidaActual(Juego* juego, int slot);
int cargarPartidaDesdeSlot(Juego* juego, int slot);
void mostrarMenuGuardar(Juego* juego);
void mostrarMenuCargar(Juego* juego);
char* serializarEstadoJuego(const Juego* juego, char* buffer, int tamBuffer);
int deserializarEstadoJuego(Juego* juego, const char* datos);

// Funciones auxiliares para el menú principal
void jugarPartida(Juego* juego);
int obtenerEntradaValida(const char* mensaje);
void limpiarBufferEntrada();
void obtenerNombresJugadores(char* nombre1, char* nombre2);

// Función principal que ejecuta todo el juego
void ejecutarJuego(Juego* juego);

#endif
