#include "juego.h"
#include <stdio.h>
#include <string.h>

//BÚSQUEDA DE CAPTURAS

int encontrarTodasCapturas(Juego* juego, enum ColorPieza jugador, Movimiento capturas[], int* numCapturas) {
    *numCapturas = 0;
    
    // Buscar en todas las piezas del jugador
    Pieza* piezas;
    int numPiezas;
    
    if (jugador == BLANCO) {
        piezas = juego->tablero.piezasBlancas;
        numPiezas = juego->tablero.numPiezasBlancas;
    } else {
        piezas = juego->tablero.piezasNegras;
        numPiezas = juego->tablero.numPiezasNegras;
    }
    
    for (int i = 0; i < numPiezas; i++) {
        if (!estaCapturada(&piezas[i])) {
            int fila = obtenerFila(&piezas[i]);
            int col = obtenerColumna(&piezas[i]);
            encontrarCapturasParaPieza(juego, fila, col, capturas, numCapturas);
        }
    }
    
    return (*numCapturas > 0);
}

int encontrarCapturasParaPieza(Juego* juego, int fila, int columna, Movimiento capturas[], int* numCapturas) {
    int numInicial = *numCapturas;
    
    Pieza* pieza = obtenerPieza(&juego->tablero, fila, columna);
    if (pieza == NULL) {
        return 0;
    }
    
    Direccion direcciones[4];
    int numDirecciones;
    obtenerDirecciones(pieza, direcciones, &numDirecciones);
    
    // Matriz para marcar casillas visitadas durante la búsqueda recursiva
    int visitado[TAMANO_TABLERO][TAMANO_TABLERO];
    memset(visitado, 0, sizeof(visitado));
    
    // Probar cada dirección
    for (int d = 0; d < numDirecciones; d++) {
        int dFila = direcciones[d].dFila;
        int dCol = direcciones[d].dColumna;
        
        if (esDama(pieza)) {
            // Las damas pueden saltar a cualquier distancia
            for (int dist = 2; dist < TAMANO_TABLERO; dist++) {
                int filaDestino = fila + (dFila * dist);
                int colDestino = columna + (dCol * dist);
                
                if (!esPosicionValida(filaDestino, colDestino)) {
                    break;
                }
                
                if (esCapturaValida(juego, fila, columna, filaDestino, colDestino)) {
                    Movimiento mov;
                    crearMovimiento(&mov, fila, columna, filaDestino, colDestino, CAPTURA);
                    
                    // Encontrar las piezas capturadas
                    int f = fila + dFila;
                    int c = columna + dCol;
                    while (f != filaDestino) {
                        Pieza* piezaEnCamino = obtenerPieza(&juego->tablero, f, c);
                        if (piezaEnCamino != NULL && obtenerColor(piezaEnCamino) != obtenerColor(pieza)) {
                            agregarPosicionCapturada(&mov, f, c);
                        }
                        f += dFila;
                        c += dCol;
                    }
                    
                    // Buscar capturas adicionales recursivamente
                    encontrarCapturasRecursivas(juego, filaDestino, colDestino, &mov, 
                                               capturas, numCapturas, visitado);
                }
                
                // Si hay una pieza en el camino, no podemos ir más lejos
                if (!estaVacio(&juego->tablero, filaDestino, colDestino)) {
                    break;
                }
            }
        } else {
            // Piezas normales solo saltan 2 casillas
            int filaDestino = fila + (dFila * 2);
            int colDestino = columna + (dCol * 2);
            
            if (esPosicionValida(filaDestino, colDestino) && 
                esCapturaValida(juego, fila, columna, filaDestino, colDestino)) {
                
                Movimiento mov;
                crearMovimiento(&mov, fila, columna, filaDestino, colDestino, CAPTURA);
                
                // La pieza capturada está en el medio
                int filaCapturada = fila + dFila;
                int colCapturada = columna + dCol;
                agregarPosicionCapturada(&mov, filaCapturada, colCapturada);
                
                // Buscar capturas adicionales recursivamente
                encontrarCapturasRecursivas(juego, filaDestino, colDestino, &mov,
                                           capturas, numCapturas, visitado);
            }
        }
    }
    
    return (*numCapturas > numInicial);
}

void encontrarCapturasRecursivas(Juego* juego, int fila, int columna, Movimiento* movActual,
                                 Movimiento todasCapturas[], int* numCapturas,
                                 int visitado[TAMANO_TABLERO][TAMANO_TABLERO]) {
    
    // Marcar esta posición como visitada
    visitado[fila][columna] = 1;
    
    Pieza* pieza = obtenerPieza(&juego->tablero, movActual->filaOrigen, movActual->colOrigen);
    if (pieza == NULL) {
        visitado[fila][columna] = 0;
        return;
    }
    
    Direccion direcciones[4];
    int numDirecciones;
    
    // Determinar direcciones basándose en si la pieza sería dama en esta posición
    int seraDama = esDama(pieza);
    if (!seraDama) {
        // Verificar si se convierte en dama al llegar a fila
        if ((obtenerColor(pieza) == BLANCO && fila == TAMANO_TABLERO - 1) ||
            (obtenerColor(pieza) == NEGRO && fila == 0)) {
            seraDama = 1;
        }
    }
    
    if (seraDama) {
        numDirecciones = 4;
        direcciones[0] = (Direccion){-1, -1};
        direcciones[1] = (Direccion){-1, 1};
        direcciones[2] = (Direccion){1, -1};
        direcciones[3] = (Direccion){1, 1};
    } else {
        obtenerDirecciones(pieza, direcciones, &numDirecciones);
        // Las piezas normales pueden capturar también hacia atrás
        if (obtenerColor(pieza) == BLANCO) {
            direcciones[numDirecciones++] = (Direccion){-1, -1};
            direcciones[numDirecciones++] = (Direccion){-1, 1};
        } else {
            direcciones[numDirecciones++] = (Direccion){1, -1};
            direcciones[numDirecciones++] = (Direccion){1, 1};
        }
    }
    
    int encontroCaptura = 0;
    
    // Probar capturas desde esta posición
    for (int d = 0; d < numDirecciones; d++) {
        int dFila = direcciones[d].dFila;
        int dCol = direcciones[d].dColumna;
        
        // Simular que la pieza está en la nueva posición
        Tablero tableroTemp = juego->tablero;
        
        // Aplicar el movimiento actual temporalmente
        for (int i = 0; i < movActual->numCapturas; i++) {
            int fCap = movActual->posicionesCapturadas[i].fila;
            int cCap = movActual->posicionesCapturadas[i].columna;
            tableroTemp.cuadricula[fCap][cCap] = NULL;
        }
        
        int maxDist = seraDama ? TAMANO_TABLERO : 2;
        
        for (int dist = 2; dist <= maxDist; dist++) {
            int filaDestino = fila + (dFila * dist);
            int colDestino = columna + (dCol * dist);
            
            if (!esPosicionValida(filaDestino, colDestino)) {
                break;
            }
            
            if (visitado[filaDestino][colDestino]) {
                continue;
            }
            
            // Verificar si hay una captura válida
            int hayEnemigo = 0;
            int f = fila + dFila;
            int c = columna + dCol;
            int filaEnemigo = -1, colEnemigo = -1;
            
            while (f != filaDestino) {
                Pieza* piezaEnCamino = tableroTemp.cuadricula[f][c];
                
                if (piezaEnCamino != NULL) {
                    if (obtenerColor(piezaEnCamino) != obtenerColor(pieza)) {
                        if (!hayEnemigo) {
                            hayEnemigo = 1;
                            filaEnemigo = f;
                            colEnemigo = c;
                        } else {
                            // Hay más de una pieza en el camino
                            hayEnemigo = 0;
                            break;
                        }
                    } else {
                        // Pieza propia en el camino
                        hayEnemigo = 0;
                        break;
                    }
                }
                
                f += dFila;
                c += dCol;
            }
            
            if (hayEnemigo && tableroTemp.cuadricula[filaDestino][colDestino] == NULL) {
                // Captura válida encontrada
                encontroCaptura = 1;
                
                // Crear nuevo movimiento con esta captura adicional
                Movimiento nuevoMov = *movActual;
                nuevoMov.filaDestino = filaDestino;
                nuevoMov.colDestino = colDestino;
                agregarPosicionCapturada(&nuevoMov, filaEnemigo, colEnemigo);
                
                // Buscar más capturas recursivamente
                encontrarCapturasRecursivas(juego, filaDestino, colDestino, &nuevoMov,
                                           todasCapturas, numCapturas, visitado);
            }
            
            if (!seraDama && dist >= 2) {
                break;  // Piezas normales solo intentan distancia 2
            }
            
            if (tableroTemp.cuadricula[filaDestino][colDestino] != NULL) {
                break;  // Hay una pieza bloqueando
            }
        }
    }
    
    // Si no encontramos más capturas, guardar el movimiento actual
    if (!encontroCaptura && *numCapturas < MAX_MOVIMIENTOS_POSIBLES) {
        todasCapturas[*numCapturas] = *movActual;
        (*numCapturas)++;
    }
    
    // Desmarcar posición
    visitado[fila][columna] = 0;
}

//VERIFICACIÓN DE MOVIMIENTOS LEGALES

int tieneMovimientosLegales(Juego* juego, enum ColorPieza jugador) {
    // Primero verificar si hay capturas obligatorias
    Movimiento capturas[MAX_MOVIMIENTOS_POSIBLES];
    int numCapturas = 0;
    
    if (encontrarTodasCapturas(juego, jugador, capturas, &numCapturas)) {
        return 1;  // Hay capturas disponibles
    }
    
    // Si no hay capturas, verificar movimientos simples
    Pieza* piezas;
    int numPiezas;
    
    if (jugador == BLANCO) {
        piezas = juego->tablero.piezasBlancas;
        numPiezas = juego->tablero.numPiezasBlancas;
    } else {
        piezas = juego->tablero.piezasNegras;
        numPiezas = juego->tablero.numPiezasNegras;
    }
    
    for (int i = 0; i < numPiezas; i++) {
        if (!estaCapturada(&piezas[i])) {
            int fila = obtenerFila(&piezas[i]);
            int col = obtenerColumna(&piezas[i]);
            
            Direccion direcciones[4];
            int numDirecciones;
            obtenerDirecciones(&piezas[i], direcciones, &numDirecciones);
            
            for (int d = 0; d < numDirecciones; d++) {
                if (esDama(&piezas[i])) {
                    // Damas pueden moverse cualquier distancia
                    for (int dist = 1; dist < TAMANO_TABLERO; dist++) {
                        int filaDestino = fila + (direcciones[d].dFila * dist);
                        int colDestino = col + (direcciones[d].dColumna * dist);
                        
                        if (!esPosicionValida(filaDestino, colDestino)) {
                            break;
                        }
                        
                        if (estaVacio(&juego->tablero, filaDestino, colDestino)) {
                            return 1;  // Movimiento válido encontrado
                        } else {
                            break;  // Pieza bloqueando
                        }
                    }
                } else {
                    // Piezas normales solo 1 casilla
                    int filaDestino = fila + direcciones[d].dFila;
                    int colDestino = col + direcciones[d].dColumna;
                    
                    if (esPosicionValida(filaDestino, colDestino) &&
                        estaVacio(&juego->tablero, filaDestino, colDestino)) {
                        return 1;  // Movimiento válido encontrado
                    }
                }
            }
        }
    }
    
    return 0;  // No hay movimientos legales
}
