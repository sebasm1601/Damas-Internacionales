#ifndef EXPORTADOR_CSV_H
#define EXPORTADOR_CSV_H

#include "movimiento.h"

#define MAX_NOMBRE_ARCHIVO 256
#define MAX_REGISTROS_MOV 500

// Estructura para un registro de movimiento con tiempo
typedef struct {
    int numeroTurno;
    char nombreJugador[100];
    char colorJugador[20];
    Movimiento movimiento;
    long long tiempoTurnoSegundos;
    long long tiempoAcumuladoSegundos;
} RegistroMovimiento;

// Estructura para el registro completo del juego
typedef struct {
    char nombreJugador1[100];
    char nombreJugador2[100];
    char ganador[100];
    char fecha[50];
    int totalMovimientos;
    long long tiempoTotalJugador1;
    long long tiempoTotalJugador2;
    RegistroMovimiento registros[MAX_REGISTROS_MOV];
    int numRegistros;
} RegistroJuego;

// Funciones para exportar a CSV
int exportarACSV(const RegistroJuego* juego, const char* nombreArchivo);
void generarNombreArchivo(const char* jugador1, const char* jugador2, char* buffer, int tamBuffer);
void obtenerFechaHoraActual(char* buffer, int tamBuffer);
void formatearCoordenada(int fila, int columna, char* buffer, int tamBuffer);

// Funciones auxiliares para el registro de juego
void inicializarRegistroJuego(RegistroJuego* registro);
void agregarRegistroMovimiento(RegistroJuego* registro, const RegistroMovimiento* movimiento);

#endif
