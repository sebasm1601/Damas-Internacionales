#include "cronometro.h"
#include <stdio.h>
#include <string.h>

// Inicializa el cronómetro
void inicializarCronometro(Cronometro* crono) {
    crono->totalSegundos = 0;
    crono->enEjecucion = 0;
    crono->tiempoInicio = 0;
    crono->tiempoFin = 0;
}

// Inicia el cronómetro
void iniciarCronometro(Cronometro* crono) {
    if (!crono->enEjecucion) {
        crono->tiempoInicio = time(NULL);
        crono->enEjecucion = 1;
    }
}

// Detiene el cronómetro
void detenerCronometro(Cronometro* crono) {
    if (crono->enEjecucion) {
        crono->tiempoFin = time(NULL);
        long long duracion = (long long)difftime(crono->tiempoFin, crono->tiempoInicio);
        crono->totalSegundos += duracion;
        crono->enEjecucion = 0;
    }
}

// Reinicia el cronómetro
void reiniciarCronometro(Cronometro* crono) {
    crono->totalSegundos = 0;
    crono->enEjecucion = 0;
    crono->tiempoInicio = 0;
    crono->tiempoFin = 0;
}

// Obtiene los segundos transcurridos en la sesión actual
long long obtenerSegundosTranscurridos(const Cronometro* crono) {
    if (crono->enEjecucion) {
        time_t ahora = time(NULL);
        return (long long)difftime(ahora, crono->tiempoInicio);
    }
    return 0;
}

// Obtiene el total de segundos acumulados
long long obtenerTotalSegundos(const Cronometro* crono) {
    long long total = crono->totalSegundos;
    if (crono->enEjecucion) {
        total += obtenerSegundosTranscurridos(crono);
    }
    return total;
}

// Formatea el tiempo en texto
void formatearTiempo(long long segundos, char* buffer, int tamBuffer) {
    long long horas = segundos / 3600;
    long long minutos = (segundos % 3600) / 60;
    long long segs = segundos % 60;
    
    if (horas > 0) {
        snprintf(buffer, tamBuffer, "%lldh %lldm %llds", horas, minutos, segs);
    } else if (minutos > 0) {
        snprintf(buffer, tamBuffer, "%lldm %llds", minutos, segs);
    } else {
        snprintf(buffer, tamBuffer, "%llds", segs);
    }
}

// Obtiene el tiempo transcurrido formateado
void obtenerTiempoFormateado(const Cronometro* crono, char* buffer, int tamBuffer) {
    formatearTiempo(obtenerSegundosTranscurridos(crono), buffer, tamBuffer);
}

// Obtiene el tiempo total formateado
void obtenerTotalFormateado(const Cronometro* crono, char* buffer, int tamBuffer) {
    formatearTiempo(obtenerTotalSegundos(crono), buffer, tamBuffer);
}

// Verifica si el cronómetro está en ejecución
int estaEnEjecucion(const Cronometro* crono) {
    return crono->enEjecucion;
}
