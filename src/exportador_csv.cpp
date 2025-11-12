#include "exportador_csv.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// Inicializa un registro de juego
void inicializarRegistroJuego(RegistroJuego* registro) {
    memset(registro->nombreJugador1, 0, sizeof(registro->nombreJugador1));
    memset(registro->nombreJugador2, 0, sizeof(registro->nombreJugador2));
    memset(registro->ganador, 0, sizeof(registro->ganador));
    memset(registro->fecha, 0, sizeof(registro->fecha));
    registro->totalMovimientos = 0;
    registro->tiempoTotalJugador1 = 0;
    registro->tiempoTotalJugador2 = 0;
    registro->numRegistros = 0;
}

// Agrega un registro de movimiento
void agregarRegistroMovimiento(RegistroJuego* registro, const RegistroMovimiento* movimiento) {
    if (registro->numRegistros < MAX_REGISTROS_MOV) {
        registro->registros[registro->numRegistros] = *movimiento;
        registro->numRegistros++;
    }
}

// Formatea una coordenada
void formatearCoordenada(int fila, int columna, char* buffer, int tamBuffer) {
    snprintf(buffer, tamBuffer, "(%d,%d)", fila, columna);
}

// Obtiene la fecha y hora actual
void obtenerFechaHoraActual(char* buffer, int tamBuffer) {
    time_t ahora = time(NULL);
    struct tm* tiempoInfo = localtime(&ahora);
    strftime(buffer, tamBuffer, "%Y-%m-%d %H:%M:%S", tiempoInfo);
}

// Genera un nombre de archivo para el CSV
void generarNombreArchivo(const char* jugador1, const char* jugador2, char* buffer, int tamBuffer) {
    time_t ahora = time(NULL);
    struct tm* tiempoInfo = localtime(&ahora);
    
    char fecha[50];
    strftime(fecha, sizeof(fecha), "%Y%m%d_%H%M%S", tiempoInfo);
    
    snprintf(buffer, tamBuffer, "Damas_%s_vs_%s_%s.csv", jugador1, jugador2, fecha);
}

// Exporta el juego a un archivo CSV
int exportarACSV(const RegistroJuego* juego, const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "w");
    
    if (archivo == NULL) {
        printf("Error: No se pudo crear el archivo %s\n", nombreArchivo);
        return 0;
    }
    
    // Escribir encabezado
    fprintf(archivo, "Turno;Jugador;Color;Desde;Hasta;Tipo;Capturas;Tiempo Turno;Tiempo Acumulado\n");
    
    // Escribir cada registro
    for (int i = 0; i < juego->numRegistros; i++) {
        const RegistroMovimiento* registro = &juego->registros[i];
        
        // Número de turno
        fprintf(archivo, "%d;", registro->numeroTurno);
        
        // Nombre del jugador
        fprintf(archivo, "%s;", registro->nombreJugador);
        
        // Color del jugador
        fprintf(archivo, "%s;", registro->colorJugador);
        
        // Coordenada origen
        char coordOrigen[20];
        formatearCoordenada(registro->movimiento.filaOrigen, 
                           registro->movimiento.colOrigen, 
                           coordOrigen, sizeof(coordOrigen));
        fprintf(archivo, "%s;", coordOrigen);
        
        // Coordenada destino
        char coordDestino[20];
        formatearCoordenada(registro->movimiento.filaDestino, 
                           registro->movimiento.colDestino, 
                           coordDestino, sizeof(coordDestino));
        fprintf(archivo, "%s;", coordDestino);
        
        // Tipo de movimiento
        const char* tipoStr;
        switch (registro->movimiento.tipo) {
            case SIMPLE:
                tipoStr = "Simple";
                break;
            case CAPTURA:
                tipoStr = "Captura";
                break;
            case CAPTURA_MULTIPLE:
                tipoStr = "Captura Multiple";
                break;
            case PROMOCION:
                tipoStr = "Coronacion";
                break;
            default:
                tipoStr = "Desconocido";
        }
        fprintf(archivo, "%s;", tipoStr);
        
        // Número de capturas
        fprintf(archivo, "%d;", registro->movimiento.numCapturas);
        
        // Tiempo del turno
        fprintf(archivo, "%llds;", registro->tiempoTurnoSegundos);
        
        // Tiempo acumulado
        fprintf(archivo, "%llds", registro->tiempoAcumuladoSegundos);
        
        fprintf(archivo, "\n");
    }
    
    fclose(archivo);
    return 1;
}
