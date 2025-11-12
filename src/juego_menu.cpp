#include "juego.h"
#include <stdio.h>
#include <string.h>

//MENÚS Y REGLAS

void mostrarMenu(Juego* juego) {
    int opcion;
    
    while (1) {
        printf("\n");
        printf("=========================================\n");
        printf("    DAMAS INTERNACIONALES - MENU         \n");
        printf("=========================================\n");
        printf("  1. Nueva Partida\n");
        printf("  2. Cargar Partida\n");
        printf("  3. Ver Reglas\n");
        printf("  4. Salir\n");
        printf("=========================================\n");
        
        opcion = obtenerEntradaValida("Seleccione una opcion: ");
        
        switch (opcion) {
            case 1: {
                char nombre1[100], nombre2[100];
                obtenerNombresJugadores(nombre1, nombre2);
                configurarJugadores(juego, nombre1, nombre2);
                iniciarNuevaPartida(juego);
                jugarPartida(juego);
                break;
            }
            case 2:
                mostrarMenuCargar(juego);
                if (!juego->juegoTerminado && juego->jugadorActual != NULL) {
                    jugarPartida(juego);
                }
                break;
            case 3:
                mostrarReglas();
                break;
            case 4:
                printf("\n Gracias por jugar!\n");
                return;
            default:
                printf("Opcion invalida.\n");
        }
    }
}

void mostrarReglas() {
    printf("\n");
    printf("=============================================================\n");
    printf("           REGLAS DE LAS DAMAS INTERNACIONALES               \n");
    printf("===========================================================\n\n");
    
    printf("OBJETIVO:\n");
    printf("  Capturar o bloquear todas las piezas del oponente.\n\n");
    
    printf("TABLERO:\n");
    printf("  - Tablero de 10x10 casillas\n");
    printf("  - Cada jugador comienza con 20 piezas\n");
    printf("  - Las piezas se colocan en las casillas oscuras\n\n");
    
    printf("MOVIMIENTOS:\n");
    printf("  - Las piezas normales se mueven en diagonal hacia adelante\n");
    printf("  - Las piezas normales avanzan una casilla por turno\n");
    printf("  - Las damas pueden moverse en cualquier direccion diagonal\n");
    printf("  - Las damas pueden moverse cualquier numero de casillas\n\n");
    
    printf("CAPTURAS:\n");
    printf("  - Se captura saltando sobre una pieza enemiga\n");
    printf("  - Las capturas son OBLIGATORIAS\n");
    printf("  - Se pueden hacer capturas multiples en un turno\n");
    printf("  - Las piezas pueden capturar hacia adelante y atras\n\n");
    
    printf("CORONACION:\n");
    printf("  - Una pieza se corona al alcanzar la ultima fila enemiga\n");
    printf("  - Las damas se representan con letras mayusculas (W/B)\n\n");
    
    printf("VICTORIA:\n");
    printf("  - Gana quien capture todas las piezas enemigas\n");
    printf("  - Gana quien deje al oponente sin movimientos validos\n\n");
    
    printf("===============================================================\n");
    printf("\nPresione Enter para continuar...");
    getchar();
}

//EXPORTACIÓN Y GUARDADO

void exportarJuegoACSV(Juego* juego) {
    char nombreArchivo[256];
    generarNombreArchivo(obtenerNombreJugador(&juego->jugador1),
                        obtenerNombreJugador(&juego->jugador2),
                        nombreArchivo, sizeof(nombreArchivo));
    
    // Llenar información del registro
    strncpy(juego->registroJuego.nombreJugador1, obtenerNombreJugador(&juego->jugador1), 99);
    strncpy(juego->registroJuego.nombreJugador2, obtenerNombreJugador(&juego->jugador2), 99);
    
    if (juego->ganador != NULL) {
        strncpy(juego->registroJuego.ganador, obtenerNombreJugador(juego->ganador), 99);
    } else {
        strcpy(juego->registroJuego.ganador, "En curso");
    }
    
    strncpy(juego->registroJuego.fecha, juego->fechaInicioJuego, 49);
    juego->registroJuego.totalMovimientos = juego->historialMovimientos.numMovimientos;
    juego->registroJuego.tiempoTotalJugador1 = obtenerTotalSegundos(obtenerCronometro(&juego->jugador1));
    juego->registroJuego.tiempoTotalJugador2 = obtenerTotalSegundos(obtenerCronometro(&juego->jugador2));
    
    if (exportarACSV(&juego->registroJuego, nombreArchivo)) {
        printf("\nPartida exportada exitosamente a: %s\n", nombreArchivo);
    } else {
        printf("\nError al exportar la partida.\n");
    }
}

//SERIALIZACIÓN SIMPLIFICADA 

char* serializarEstadoJuego(const Juego* juego, char* buffer, int tamBuffer) {
    int pos = 0;
    
    // Serializar información básica
    pos += snprintf(buffer + pos, tamBuffer - pos, "%s\n", obtenerNombreJugador(&juego->jugador1));
    pos += snprintf(buffer + pos, tamBuffer - pos, "%s\n", obtenerNombreJugador(&juego->jugador2));
    
    // Jugador actual (0 = jugador1, 1 = jugador2)
    int jugadorActualIdx = (juego->jugadorActual == &juego->jugador1) ? 0 : 1;
    pos += snprintf(buffer + pos, tamBuffer - pos, "%d\n", jugadorActualIdx);
    
    // Estadísticas
    pos += snprintf(buffer + pos, tamBuffer - pos, "%d %d %d\n",
                   obtenerPiezasCapturadas(&juego->jugador1),
                   obtenerTotalMovimientos(&juego->jugador1),
                   obtenerDamasCreadas(&juego->jugador1));
    
    pos += snprintf(buffer + pos, tamBuffer - pos, "%d %d %d\n",
                   obtenerPiezasCapturadas(&juego->jugador2),
                   obtenerTotalMovimientos(&juego->jugador2),
                   obtenerDamasCreadas(&juego->jugador2));
    
    // Tiempos
    pos += snprintf(buffer + pos, tamBuffer - pos, "%lld %lld\n",
                   obtenerTotalSegundos(obtenerCronometroConst(&juego->jugador1)),
                   obtenerTotalSegundos(obtenerCronometroConst(&juego->jugador2)));
    
    // Estado del tablero
    for (int i = 0; i < TAMANO_TABLERO; i++) {
        for (int j = 0; j < TAMANO_TABLERO; j++) {
            Pieza* pieza = juego->tablero.cuadricula[i][j];
            if (pieza == NULL) {
                pos += snprintf(buffer + pos, tamBuffer - pos, ". ");
            } else {
                char simbolo = obtenerSimbolo(pieza);
                pos += snprintf(buffer + pos, tamBuffer - pos, "%c ", simbolo);
            }
        }
        pos += snprintf(buffer + pos, tamBuffer - pos, "\n");
    }
    
    return buffer;
}

int deserializarEstadoJuego(Juego* juego, const char* datos) {
    char nombre1[100], nombre2[100];
    int jugadorActualIdx;
    int piezasCapJ1, movimientosJ1, damasJ1;
    int piezasCapJ2, movimientosJ2, damasJ2;
    long long tiempo1, tiempo2;
    
    const char* ptr = datos;
    
    // Leer nombres de jugadores
    sscanf(ptr, "%99[^\n]\n", nombre1);
    ptr = strchr(ptr, '\n') + 1;
    
    sscanf(ptr, "%99[^\n]\n", nombre2);
    ptr = strchr(ptr, '\n') + 1;
    
    // Leer jugador actual
    sscanf(ptr, "%d\n", &jugadorActualIdx);
    ptr = strchr(ptr, '\n') + 1;
    
    // Leer estadísticas jugador 1
    sscanf(ptr, "%d %d %d\n", &piezasCapJ1, &movimientosJ1, &damasJ1);
    ptr = strchr(ptr, '\n') + 1;
    
    // Leer estadísticas jugador 2
    sscanf(ptr, "%d %d %d\n", &piezasCapJ2, &movimientosJ2, &damasJ2);
    ptr = strchr(ptr, '\n') + 1;
    
    // Leer tiempos
    sscanf(ptr, "%lld %lld\n", &tiempo1, &tiempo2);
    ptr = strchr(ptr, '\n') + 1;
    
    // Configurar jugadores
    configurarJugadores(juego, nombre1, nombre2);
    
    // Restaurar estadísticas
    juego->jugador1.piezasCapturadas = piezasCapJ1;
    juego->jugador1.totalMovimientos = movimientosJ1;
    juego->jugador1.damasCreadas = damasJ1;
    juego->jugador1.cronometro.totalSegundos = tiempo1;
    
    juego->jugador2.piezasCapturadas = piezasCapJ2;
    juego->jugador2.totalMovimientos = movimientosJ2;
    juego->jugador2.damasCreadas = damasJ2;
    juego->jugador2.cronometro.totalSegundos = tiempo2;
    
    // Establecer jugador actual
    juego->jugadorActual = (jugadorActualIdx == 0) ? &juego->jugador1 : &juego->jugador2;
    
    // Limpiar tablero antes de reconstruirlo
    limpiarTablero(&juego->tablero);
    juego->tablero.numPiezasBlancas = 0;
    juego->tablero.numPiezasNegras = 0;
    
    // Leer estado del tablero
    for (int i = 0; i < TAMANO_TABLERO; i++) {
        for (int j = 0; j < TAMANO_TABLERO; j++) {
            char simbolo;
            sscanf(ptr, "%c ", &simbolo);
            ptr += 2; // avanzar carácter + espacio
            
            if (simbolo != '.') {
                Pieza* pieza = NULL;
                
                if (simbolo == 'w' || simbolo == 'W') {
                    // Pieza blanca
                    pieza = &juego->tablero.piezasBlancas[juego->tablero.numPiezasBlancas];
                    inicializarPieza(pieza, BLANCO, i, j);
                    if (simbolo == 'W') {
                        promover(pieza);
                    }
                    juego->tablero.numPiezasBlancas++;
                } else if (simbolo == 'b' || simbolo == 'B') {
                    // Pieza negra
                    pieza = &juego->tablero.piezasNegras[juego->tablero.numPiezasNegras];
                    inicializarPieza(pieza, NEGRO, i, j);
                    if (simbolo == 'B') {
                        promover(pieza);
                    }
                    juego->tablero.numPiezasNegras++;
                }
                
                if (pieza != NULL) {
                    juego->tablero.cuadricula[i][j] = pieza;
                }
            }
        }
        ptr++; 
    }
    
    printf("Partida cargada exitosamente!\n");
    printf("Jugadores: %s vs %s\n", nombre1, nombre2);
    printf("Turno de: %s\n", obtenerNombreJugador(juego->jugadorActual));
    
    return 1;
}

int guardarPartidaActual(Juego* juego, int slot) {
    char buffer[MAX_DATOS_GUARDADO];
    serializarEstadoJuego(juego, buffer, sizeof(buffer));
    
    if (!guardarPartida(slot, buffer)) {
        printf("Error al guardar la partida.\n");
        return 0;
    }
    
    // Guardar metadatos
    PartidaGuardada info;
    info.slot = slot;
    strncpy(info.nombreJugador1, obtenerNombreJugador(&juego->jugador1), 99);
    strncpy(info.nombreJugador2, obtenerNombreJugador(&juego->jugador2), 99);
    strncpy(info.jugadorActual, obtenerNombreJugador(juego->jugadorActual), 99);
    obtenerFechaHoraActual(info.fecha, sizeof(info.fecha));
    info.contadorMovimientos = juego->historialMovimientos.numMovimientos;
    info.tiempoJugador1 = obtenerTotalSegundos(obtenerCronometro(&juego->jugador1));
    info.tiempoJugador2 = obtenerTotalSegundos(obtenerCronometro(&juego->jugador2));
    
    guardarMetadatos(slot, &info);
    
    printf("\nPartida guardada exitosamente en el slot %d\n", slot);
    return 1;
}

int cargarPartidaDesdeSlot(Juego* juego, int slot) {
    char buffer[MAX_DATOS_GUARDADO];
    
    if (!cargarPartida(slot, buffer, sizeof(buffer))) {
        printf("Error al cargar la partida.\n");
        return 0;
    }
    
    if (!deserializarEstadoJuego(juego, buffer)) {
        printf("Error al deserializar la partida.\n");
        return 0;
    }
    
    // Inicializar estado del juego
    juego->juegoTerminado = 0;
    juego->ganador = NULL;
    inicializarHistorial(&juego->historialMovimientos);
    inicializarRegistroJuego(&juego->registroJuego);
    
    // Iniciar cronómetro del jugador actual
    iniciarCronometro(obtenerCronometro(juego->jugadorActual));
    
    return 1;
}

void mostrarMenuGuardar(Juego* juego) {
    printf("\n=== GUARDAR PARTIDA ===\n");
    printf("Seleccione un slot (1-%d): ", MAX_PARTIDAS_GUARDADAS);
    
    int slot = obtenerEntradaValida("");
    
    if (slot < 1 || slot > MAX_PARTIDAS_GUARDADAS) {
        printf("Slot invalido.\n");
        return;
    }
    
    if (slotExiste(slot)) {
        printf("Este slot ya contiene una partida.  Sobrescribir? (1=Si, 0=No): ");
        int confirmar = obtenerEntradaValida("");
        if (confirmar != 1) {
            printf("Guardado cancelado.\n");
            return;
        }
    }
    
    guardarPartidaActual(juego, slot);
}

void mostrarMenuCargar(Juego* juego) {
    PartidaGuardada partidas[MAX_PARTIDAS_GUARDADAS];
    int numPartidas;
    
    listarPartidasGuardadas(partidas, &numPartidas);
    
    printf("\n=== CARGAR PARTIDA ===\n");
    printf("Partidas guardadas:\n\n");
    
    int hayPartidas = 0;
    for (int i = 0; i < numPartidas; i++) {
        if (!partidas[i].estaVacio) {
            hayPartidas = 1;
            printf("Slot %d:\n", partidas[i].slot);
            printf("  Jugadores: %s vs %s\n", partidas[i].nombreJugador1, partidas[i].nombreJugador2);
            printf("  Turno actual: %s\n", partidas[i].jugadorActual);
            printf("  Movimientos: %d\n", partidas[i].contadorMovimientos);
            printf("  Fecha: %s\n\n", partidas[i].fecha);
        }
    }
    
    if (!hayPartidas) {
        printf("No hay partidas guardadas.\n");
        printf("\nPresione Enter para continuar...");
        getchar();
        return;
    }
    
    int slot = obtenerEntradaValida("Seleccione un slot para cargar (0 para cancelar): ");
    
    if (slot == 0) {
        return;
    }
    
    if (slot < 1 || slot > MAX_PARTIDAS_GUARDADAS) {
        printf("Slot invalido.\n");
        return;
    }
    
    if (!slotExiste(slot)) {
        printf("Este slot esta vacio.\n");
        return;
    }
    
    cargarPartidaDesdeSlot(juego, slot);
}