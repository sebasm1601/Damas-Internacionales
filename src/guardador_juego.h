#ifndef GUARDADOR_JUEGO_H
#define GUARDADOR_JUEGO_H

#define MAX_PARTIDAS_GUARDADAS 5
#define DIRECTORIO_GUARDADOS "guardados"
#define MAX_DATOS_GUARDADO 50000

// Estructura para información de una partida guardada
typedef struct {
    int slot;
    char nombreJugador1[100];
    char nombreJugador2[100];
    char jugadorActual[100];
    char fecha[50];
    int contadorMovimientos;
    long long tiempoJugador1;
    long long tiempoJugador2;
    char nombreArchivo[256];
    int estaVacio;  // 0 = tiene datos, 1 = vacío
} PartidaGuardada;

// Funciones para guardar/cargar partidas
int guardarPartida(int slot, const char* datosJuego);
int cargarPartida(int slot, char* datosJuego, int tamBuffer);
int eliminarPartida(int slot);
int slotExiste(int slot);
PartidaGuardada obtenerInfoPartida(int slot);
void listarPartidasGuardadas(PartidaGuardada partidas[], int* numPartidas);

// Funciones para metadatos
int guardarMetadatos(int slot, const PartidaGuardada* info);
PartidaGuardada cargarMetadatos(int slot);

// Funciones auxiliares
void asegurarDirectorioGuardados();
void obtenerNombreArchivoSlot(int slot, char* buffer, int tamBuffer);
void obtenerNombreArchivoMetadatos(int slot, char* buffer, int tamBuffer);

#endif
