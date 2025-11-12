#include "juego.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//FUNCIONES DE INICIALIZACIÓN 

void inicializarJuego(Juego* juego) {
    inicializarTablero(&juego->tablero);
    juego->jugadorActual = NULL;
    juego->juegoTerminado = 0;
    juego->ganador = NULL;
    inicializarHistorial(&juego->historialMovimientos);
    inicializarRegistroJuego(&juego->registroJuego);
    obtenerFechaHoraActual(juego->fechaInicioJuego, sizeof(juego->fechaInicioJuego));
}

void iniciarNuevaPartida(Juego* juego) {
    inicializarTablero(&juego->tablero);
    juego->jugadorActual = &juego->jugador1;
    juego->juegoTerminado = 0;
    juego->ganador = NULL;
    inicializarHistorial(&juego->historialMovimientos);
    inicializarRegistroJuego(&juego->registroJuego);
    obtenerFechaHoraActual(juego->fechaInicioJuego, sizeof(juego->fechaInicioJuego));
    
    // Iniciar cronómetro del primer jugador
    iniciarCronometro(obtenerCronometro(&juego->jugador1));
}

void configurarJugadores(Juego* juego, const char* nombre1, const char* nombre2) {
    inicializarJugador(&juego->jugador1, nombre1, BLANCO);
    inicializarJugador(&juego->jugador2, nombre2, NEGRO);
    juego->jugadorActual = &juego->jugador1;
}

//GETTERS

Jugador* obtenerJugadorActual(const Juego* juego) {
    return juego->jugadorActual;
}

int estaJuegoTerminado(const Juego* juego) {
    return juego->juegoTerminado;
}

Jugador* obtenerGanador(const Juego* juego) {
    return juego->ganador;
}

//VISUALIZACIÓN

void mostrarTableroJuego(const Juego* juego) {
    mostrarTablero(&juego->tablero);
}

void mostrarEstadoJuego(const Juego* juego) {
    printf("\n========== ESTADO DEL JUEGO ==========\n");
    printf("Turno de: %s (%s)\n", 
           obtenerNombreJugador(juego->jugadorActual),
           obtenerNombreColorJugador(juego->jugadorActual));
    
    int piezasBlancas = contarPiezasActivasBlancas(&juego->tablero);
    int piezasNegras = contarPiezasActivasNegras(&juego->tablero);
    
    printf("\nPiezas en el tablero:\n");
    printf("  %s: %d piezas\n", obtenerNombreJugador(&juego->jugador1), piezasBlancas);
    printf("  %s: %d piezas\n", obtenerNombreJugador(&juego->jugador2), piezasNegras);
    
    // Mostrar tiempo
    char tiempoBuffer[50];
    obtenerTotalFormateado(obtenerCronometro(juego->jugadorActual), tiempoBuffer, sizeof(tiempoBuffer));
    printf("\nTiempo del turno actual: %s\n", tiempoBuffer);
    
    printf("======================================\n\n");
}

void mostrarEstadisticasJugadores(const Juego* juego) {
    int piezasBlancas = contarPiezasActivasBlancas(&juego->tablero);
    int piezasNegras = contarPiezasActivasNegras(&juego->tablero);
    
    mostrarEstadisticas(&juego->jugador1, piezasBlancas);
    mostrarEstadisticas(&juego->jugador2, piezasNegras);
}

void mostrarHistorialMovimientos(const Juego* juego) {
    mostrarHistorial(&juego->historialMovimientos);
}

//UTILIDADES

void cambiarJugador(Juego* juego) {
    // Detener cronómetro del jugador actual
    detenerCronometro(obtenerCronometro(juego->jugadorActual));
    
    // Cambiar al otro jugador
    if (juego->jugadorActual == &juego->jugador1) {
        juego->jugadorActual = &juego->jugador2;
    } else {
        juego->jugadorActual = &juego->jugador1;
    }
    
    // Iniciar cronómetro del nuevo jugador
    iniciarCronometro(obtenerCronometro(juego->jugadorActual));
}

void verificarFinJuego(Juego* juego) {
    int piezasBlancas = contarPiezasActivasBlancas(&juego->tablero);
    int piezasNegras = contarPiezasActivasNegras(&juego->tablero);
    
    // Verificar si algún jugador perdió todas sus piezas
    if (piezasBlancas == 0) {
        juego->juegoTerminado = 1;
        juego->ganador = &juego->jugador2;
        return;
    }
    
    if (piezasNegras == 0) {
        juego->juegoTerminado = 1;
        juego->ganador = &juego->jugador1;
        return;
    }
    
    // Verificar si el jugador actual tiene movimientos legales
    if (!tieneMovimientosLegales(juego, obtenerColorJugador(juego->jugadorActual))) {
        juego->juegoTerminado = 1;
        // El ganador es el otro jugador
        if (juego->jugadorActual == &juego->jugador1) {
            juego->ganador = &juego->jugador2;
        } else {
            juego->ganador = &juego->jugador1;
        }
    }
}

void obtenerDirecciones(const Pieza* pieza, Direccion direcciones[], int* numDirecciones) {
    *numDirecciones = 0;
    
    if (esDama(pieza)) {
        // Las damas se mueven en las 4 diagonales
        direcciones[(*numDirecciones)++] = (Direccion){-1, -1};
        direcciones[(*numDirecciones)++] = (Direccion){-1, 1};
        direcciones[(*numDirecciones)++] = (Direccion){1, -1};
        direcciones[(*numDirecciones)++] = (Direccion){1, 1};
    } else {
        // Las piezas normales se mueven según su color
        if (obtenerColor(pieza) == BLANCO) {
            // Blancas avanzan hacia abajo
            direcciones[(*numDirecciones)++] = (Direccion){1, -1};
            direcciones[(*numDirecciones)++] = (Direccion){1, 1};
        } else {
            // Negras avanzan hacia arriba
            direcciones[(*numDirecciones)++] = (Direccion){-1, -1};
            direcciones[(*numDirecciones)++] = (Direccion){-1, 1};
        }
    }
}

void limpiarBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int obtenerEntradaValida(const char* mensaje) {
    int valor;
    int resultado;
    
    while (1) {
        printf("%s", mensaje);
        resultado = scanf("%d", &valor);
        
        if (resultado == 1) {
            limpiarBufferEntrada();
            return valor;
        } else {
            printf("Entrada invalida. Por favor ingrese un numero.\n");
            limpiarBufferEntrada();
        }
    }
}

void obtenerNombresJugadores(char* nombre1, char* nombre2) {
    printf("\n=== CONFIGURACION DE JUGADORES ===\n");
    printf("Ingrese el nombre del Jugador 1 (Blancas): ");
    fgets(nombre1, 100, stdin);
    nombre1[strcspn(nombre1, "\n")] = 0;  // Remover salto de línea
    
    if (strlen(nombre1) == 0) {
        strcpy(nombre1, "Jugador 1");
    }
    
    printf("Ingrese el nombre del Jugador 2 (Negras): ");
    fgets(nombre2, 100, stdin);
    nombre2[strcspn(nombre2, "\n")] = 0;
    
    if (strlen(nombre2) == 0) {
        strcpy(nombre2, "Jugador 2");
    }
}

//VALIDACIÓN DE MOVIMIENTOS

int esMovimientoSimpleValido(Juego* juego, int filaOrigen, int colOrigen, int filaDestino, int colDestino) {
    // Verificar que las coordenadas estén dentro del tablero
    if (!esPosicionValida(filaOrigen, colOrigen) || !esPosicionValida(filaDestino, colDestino)) {
        return 0;
    }
    
    // Verificar que hay una pieza en el origen
    Pieza* pieza = obtenerPieza(&juego->tablero, filaOrigen, colOrigen);
    if (pieza == NULL) {
        return 0;
    }
    
    // Verificar que la pieza es del jugador actual
    if (obtenerColor(pieza) != obtenerColorJugador(juego->jugadorActual)) {
        return 0;
    }
    
    // Verificar que el destino está vacío
    if (!estaVacio(&juego->tablero, filaDestino, colDestino)) {
        return 0;
    }
    
    // Verificar que el destino es una casilla oscura
    if (!esCasillaOscura(filaDestino, colDestino)) {
        return 0;
    }
    
    int difFila = filaDestino - filaOrigen;
    int difCol = colDestino - colOrigen;
    
    // Verificar movimiento diagonal
    if (abs(difFila) != abs(difCol)) {
        return 0;
    }
    
    if (esDama(pieza)) {
        // Las damas pueden moverse cualquier distancia en diagonal
        int pasoFila = (difFila > 0) ? 1 : -1;
        int pasoCol = (difCol > 0) ? 1 : -1;
        
        // Verificar que no hay piezas en el camino
        int f = filaOrigen + pasoFila;
        int c = colOrigen + pasoCol;
        
        while (f != filaDestino && c != colDestino) {
            if (!estaVacio(&juego->tablero, f, c)) {
                return 0;  // Hay una pieza bloqueando el camino
            }
            f += pasoFila;
            c += pasoCol;
        }
        
        return 1;
    } else {
        // Las piezas normales solo pueden moverse una casilla
        if (abs(difFila) != 1 || abs(difCol) != 1) {
            return 0;
        }
        
        // Verificar dirección según el color
        if (obtenerColor(pieza) == BLANCO) {
            // Blancas solo avanzan hacia abajo (filas crecientes)
            if (difFila <= 0) {
                return 0;
            }
        } else {
            // Negras solo avanzan hacia arriba (filas decrecientes)
            if (difFila >= 0) {
                return 0;
            }
        }
        
        return 1;
    }
}

int esCapturaValida(Juego* juego, int filaOrigen, int colOrigen, int filaDestino, int colDestino) {
    if (!esPosicionValida(filaOrigen, colOrigen) || !esPosicionValida(filaDestino, colDestino)) {
        return 0;
    }
    
    Pieza* pieza = obtenerPieza(&juego->tablero, filaOrigen, colOrigen);
    if (pieza == NULL || obtenerColor(pieza) != obtenerColorJugador(juego->jugadorActual)) {
        return 0;
    }
    
    if (!estaVacio(&juego->tablero, filaDestino, colDestino)) {
        return 0;
    }
    
    int difFila = filaDestino - filaOrigen;
    int difCol = colDestino - colOrigen;
    
    if (abs(difFila) != abs(difCol) || abs(difFila) < 2) {
        return 0;
    }
    
    int pasoFila = (difFila > 0) ? 1 : -1;
    int pasoCol = (difCol > 0) ? 1 : -1;
    int piezasEnemigas = 0;
    int f = filaOrigen + pasoFila;
    int c = colOrigen + pasoCol;
    
    while (f != filaDestino) {
        Pieza* piezaEnCamino = obtenerPieza(&juego->tablero, f, c);
        if (piezaEnCamino != NULL) {
            if (obtenerColor(piezaEnCamino) != obtenerColor(pieza)) {
                piezasEnemigas++;
            } else {
                return 0;
            }
        }
        f += pasoFila;
        c += pasoCol;
    }
    
    if (!esDama(pieza) && piezasEnemigas != 1) return 0;
    if (piezasEnemigas < 1) return 0;
    
    return 1;
}

//REALIZAR MOVIMIENTO

int realizarMovimiento(Juego* juego, int filaOrigen, int colOrigen, int filaDestino, int colDestino) {
    // Verificar que el juego no ha terminado
    if (juego->juegoTerminado) {
        printf("El juego ya ha terminado.\n");
        return 0;
    }
    
    // Verificar que hay una pieza en el origen
    Pieza* pieza = obtenerPieza(&juego->tablero, filaOrigen, colOrigen);
    if (pieza == NULL) {
        printf("No hay ninguna pieza en la posicion de origen.\n");
        return 0;
    }
    
    // Verificar que la pieza pertenece al jugador actual
    if (obtenerColor(pieza) != obtenerColorJugador(juego->jugadorActual)) {
        printf("Esta pieza no te pertenece.\n");
        return 0;
    }
    
    // Verificar si hay capturas obligatorias
    Movimiento capturasObligatorias[MAX_MOVIMIENTOS_POSIBLES];
    int numCapturas = 0;
    encontrarTodasCapturas(juego, obtenerColorJugador(juego->jugadorActual), 
                          capturasObligatorias, &numCapturas);
    
    if (numCapturas > 0) {
        // Hay capturas obligatorias
        if (!esCapturaValida(juego, filaOrigen, colOrigen, filaDestino, colDestino)) {
            printf("Hay capturas obligatorias. Debes capturar.\n");
            return 0;
        }
    } else {
        // No hay capturas, verificar movimiento simple
        if (!esMovimientoSimpleValido(juego, filaOrigen, colOrigen, filaDestino, colDestino)) {
            printf("Movimiento invalido.\n");
            return 0;
        }
    }
    
    // Crear movimiento
    Movimiento movimiento;
    crearMovimiento(&movimiento, filaOrigen, colOrigen, filaDestino, colDestino, SIMPLE);
    
    // Si es captura, registrar piezas capturadas
    int difFila = filaDestino - filaOrigen;
    int difCol = colDestino - colOrigen;
    
    if (abs(difFila) > 1) {
        // Es una captura
        int pasoFila = (difFila > 0) ? 1 : -1;
        int pasoCol = (difCol > 0) ? 1 : -1;
        
        int f = filaOrigen + pasoFila;
        int c = colOrigen + pasoCol;
        
        while (f != filaDestino) {
            Pieza* piezaCapturada = obtenerPieza(&juego->tablero, f, c);
            
            if (piezaCapturada != NULL && obtenerColor(piezaCapturada) != obtenerColor(pieza)) {
                agregarPosicionCapturada(&movimiento, f, c);
                eliminarPieza(&juego->tablero, f, c);
                agregarPiezaCapturada(juego->jugadorActual);
            }
            
            f += pasoFila;
            c += pasoCol;
        }
    }
    
    // Realizar el movimiento
    moverPieza(&juego->tablero, filaOrigen, colOrigen, filaDestino, colDestino);
    
    // Verificar promoción
    pieza = obtenerPieza(&juego->tablero, filaDestino, colDestino);
    if (pieza != NULL && esDama(pieza) && obtenerTipo(pieza) == DAMA) {
        movimiento.promocionaADama = 1;
        agregarDamaCreada(juego->jugadorActual);
    }
    
    // Agregar movimiento al historial
    agregarMovimientoHistorial(&juego->historialMovimientos, &movimiento);
    
    // Incrementar contador de movimientos
    incrementarMovimientos(juego->jugadorActual);
    
    // Guardar en registro para CSV
    if (juego->registroJuego.numRegistros < MAX_REGISTROS_MOV) {
        RegistroMovimiento registro;
        registro.numeroTurno = juego->historialMovimientos.numMovimientos;
        strncpy(registro.nombreJugador, obtenerNombreJugador(juego->jugadorActual), 99);
        strncpy(registro.colorJugador, obtenerNombreColorJugador(juego->jugadorActual), 19);
        registro.movimiento = movimiento;
        registro.tiempoTurnoSegundos = obtenerSegundosTranscurridos(obtenerCronometro(juego->jugadorActual));
        registro.tiempoAcumuladoSegundos = obtenerTotalSegundos(obtenerCronometro(juego->jugadorActual));
        
        agregarRegistroMovimiento(&juego->registroJuego, &registro);
    }
    
    // Verificar si hay más capturas posibles con la misma pieza
    if (movimiento.numCapturas > 0) {
        Movimiento capturasAdicionales[MAX_MOVIMIENTOS_POSIBLES];
        int numCapturasAdicionales = 0;
        encontrarCapturasParaPieza(juego, filaDestino, colDestino, 
                                  capturasAdicionales, &numCapturasAdicionales);
        
        if (numCapturasAdicionales > 0) {
            printf("\n¡Puedes hacer capturas adicionales con esta pieza!\n");
            printf("Continua capturando desde (%d,%d)\n", filaDestino, colDestino);
            // No cambiar de jugador todavía
            return 1;
        }
    }
    
    // Cambiar de jugador
    cambiarJugador(juego);
    
    // Verificar si el juego ha terminado
    verificarFinJuego(juego);
    
    if (juego->juegoTerminado) {
        detenerCronometro(obtenerCronometro(&juego->jugador1));
        detenerCronometro(obtenerCronometro(&juego->jugador2));
        
        printf("\n\n");
        printf("========================================\n");
        printf("        ¡JUEGO TERMINADO!               \n");
        printf("========================================\n");
        printf("Ganador: %s (%s)\n", 
               obtenerNombreJugador(juego->ganador),
               obtenerNombreColorJugador(juego->ganador));
        printf("========================================\n\n");
    }
    
    return 1;
}
