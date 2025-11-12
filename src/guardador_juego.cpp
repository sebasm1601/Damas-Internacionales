#include "guardador_juego.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

// Asegura que existe el directorio de guardados
void asegurarDirectorioGuardados() {
    #ifdef _WIN32
    system("mkdir guardados 2>nul");
    #else
    system("mkdir -p guardados");
    #endif
}

// Obtiene el nombre de archivo para un slot
void obtenerNombreArchivoSlot(int slot, char* buffer, int tamBuffer) {
    snprintf(buffer, tamBuffer, "%s/slot_%d.sav", DIRECTORIO_GUARDADOS, slot);
}

// Obtiene el nombre de archivo de metadatos para un slot
void obtenerNombreArchivoMetadatos(int slot, char* buffer, int tamBuffer) {
    snprintf(buffer, tamBuffer, "%s/slot_%d.meta", DIRECTORIO_GUARDADOS, slot);
}

// Verifica si un slot existe
int slotExiste(int slot) {
    if (slot < 1 || slot > MAX_PARTIDAS_GUARDADAS) {
        return 0;
    }
    
    char nombreArchivo[256];
    obtenerNombreArchivoSlot(slot, nombreArchivo, sizeof(nombreArchivo));
    
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo != NULL) {
        fclose(archivo);
        return 1;
    }
    return 0;
}

// Guarda una partida en un slot
int guardarPartida(int slot, const char* datosJuego) {
    if (slot < 1 || slot > MAX_PARTIDAS_GUARDADAS) {
        return 0;
    }
    
    asegurarDirectorioGuardados();
    
    char nombreArchivo[256];
    obtenerNombreArchivoSlot(slot, nombreArchivo, sizeof(nombreArchivo));
    
    FILE* archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        return 0;
    }
    
    fprintf(archivo, "%s", datosJuego);
    fclose(archivo);
    
    return 1;
}

// Carga una partida desde un slot
int cargarPartida(int slot, char* datosJuego, int tamBuffer) {
    if (slot < 1 || slot > MAX_PARTIDAS_GUARDADAS) {
        return 0;
    }
    
    char nombreArchivo[256];
    obtenerNombreArchivoSlot(slot, nombreArchivo, sizeof(nombreArchivo));
    
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        return 0;
    }
    
    // Leer todo el archivo
    int pos = 0;
    int c;
    while ((c = fgetc(archivo)) != EOF && pos < tamBuffer - 1) {
        datosJuego[pos++] = c;
    }
    datosJuego[pos] = '\0';
    
    fclose(archivo);
    return 1;
}

// Elimina una partida guardada
int eliminarPartida(int slot) {
    if (slot < 1 || slot > MAX_PARTIDAS_GUARDADAS) {
        return 0;
    }
    
    char nombreArchivo[256];
    char nombreMeta[256];
    
    obtenerNombreArchivoSlot(slot, nombreArchivo, sizeof(nombreArchivo));
    obtenerNombreArchivoMetadatos(slot, nombreMeta, sizeof(nombreMeta));
    
    remove(nombreArchivo);
    remove(nombreMeta);
    
    return 1;
}

// Guarda los metadatos de una partida
int guardarMetadatos(int slot, const PartidaGuardada* info) {
    char nombreArchivo[256];
    obtenerNombreArchivoMetadatos(slot, nombreArchivo, sizeof(nombreArchivo));
    
    FILE* archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        return 0;
    }
    
    fprintf(archivo, "%s\n", info->nombreJugador1);
    fprintf(archivo, "%s\n", info->nombreJugador2);
    fprintf(archivo, "%s\n", info->jugadorActual);
    fprintf(archivo, "%s\n", info->fecha);
    fprintf(archivo, "%d\n", info->contadorMovimientos);
    fprintf(archivo, "%lld\n", info->tiempoJugador1);
    fprintf(archivo, "%lld\n", info->tiempoJugador2);
    
    fclose(archivo);
    return 1;
}

// Carga los metadatos de una partida
PartidaGuardada cargarMetadatos(int slot) {
    PartidaGuardada info;
    info.slot = slot;
    info.estaVacio = 1;
    
    if (!slotExiste(slot)) {
        return info;
    }
    
    char nombreArchivo[256];
    obtenerNombreArchivoMetadatos(slot, nombreArchivo, sizeof(nombreArchivo));
    
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        return info;
    }
    
    fgets(info.nombreJugador1, sizeof(info.nombreJugador1), archivo);
    fgets(info.nombreJugador2, sizeof(info.nombreJugador2), archivo);
    fgets(info.jugadorActual, sizeof(info.jugadorActual), archivo);
    fgets(info.fecha, sizeof(info.fecha), archivo);
    
    // Remover saltos de línea
    info.nombreJugador1[strcspn(info.nombreJugador1, "\n")] = 0;
    info.nombreJugador2[strcspn(info.nombreJugador2, "\n")] = 0;
    info.jugadorActual[strcspn(info.jugadorActual, "\n")] = 0;
    info.fecha[strcspn(info.fecha, "\n")] = 0;
    
    fscanf(archivo, "%d\n", &info.contadorMovimientos);
    fscanf(archivo, "%lld\n", &info.tiempoJugador1);
    fscanf(archivo, "%lld\n", &info.tiempoJugador2);
    
    obtenerNombreArchivoSlot(slot, info.nombreArchivo, sizeof(info.nombreArchivo));
    info.estaVacio = 0;
    
    fclose(archivo);
    return info;
}

// Obtiene la información de una partida guardada
PartidaGuardada obtenerInfoPartida(int slot) {
    return cargarMetadatos(slot);
}

// Lista todas las partidas guardadas
void listarPartidasGuardadas(PartidaGuardada partidas[], int* numPartidas) {
    *numPartidas = 0;
    
    for (int i = 1; i <= MAX_PARTIDAS_GUARDADAS; i++) {
        PartidaGuardada info = cargarMetadatos(i);
        info.slot = i;
        partidas[*numPartidas] = info;
        (*numPartidas)++;
    }
}
