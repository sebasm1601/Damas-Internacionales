#ifndef CRONOMETRO_H
#define CRONOMETRO_H

#include <time.h>

// Estructura para el cronómetro
typedef struct {
    time_t tiempoInicio;
    time_t tiempoFin;
    long long totalSegundos;
    int enEjecucion;  // 0 = detenido, 1 = en ejecución
} Cronometro;

// Funciones para el cronómetro
void inicializarCronometro(Cronometro* crono);
void iniciarCronometro(Cronometro* crono);
void detenerCronometro(Cronometro* crono);
void reiniciarCronometro(Cronometro* crono);
long long obtenerSegundosTranscurridos(const Cronometro* crono);
long long obtenerTotalSegundos(const Cronometro* crono);
void obtenerTiempoFormateado(const Cronometro* crono, char* buffer, int tamBuffer);
void obtenerTotalFormateado(const Cronometro* crono, char* buffer, int tamBuffer);
int estaEnEjecucion(const Cronometro* crono);
void formatearTiempo(long long segundos, char* buffer, int tamBuffer);

#endif
