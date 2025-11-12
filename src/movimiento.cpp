#include "movimiento.h"
#include <stdio.h>
#include <string.h>

// Inicializa un movimiento vacío
void inicializarMovimiento(Movimiento* mov) {
    mov->filaOrigen = -1;
    mov->colOrigen = -1;
    mov->filaDestino = -1;
    mov->colDestino = -1;
    mov->tipo = SIMPLE;
    mov->numCapturas = 0;
    mov->promocionaADama = 0;
    mov->puntaje = 0;
}

// Crea un movimiento con parámetros específicos
void crearMovimiento(Movimiento* mov, int fOrigen, int cOrigen, int fDestino, int cDestino, enum TipoMovimiento tipo) {
    mov->filaOrigen = fOrigen;
    mov->colOrigen = cOrigen;
    mov->filaDestino = fDestino;
    mov->colDestino = cDestino;
    mov->tipo = tipo;
    mov->numCapturas = 0;
    mov->promocionaADama = 0;
    mov->puntaje = 0;
}

// Agrega una posición capturada al movimiento
void agregarPosicionCapturada(Movimiento* mov, int fila, int columna) {
    if (mov->numCapturas < 20) {
        mov->posicionesCapturadas[mov->numCapturas].fila = fila;
        mov->posicionesCapturadas[mov->numCapturas].columna = columna;
        mov->numCapturas++;
        
        // Actualizar tipo de movimiento
        if (mov->numCapturas == 1) {
            mov->tipo = CAPTURA;
        } else if (mov->numCapturas > 1) {
            mov->tipo = CAPTURA_MULTIPLE;
        }
    }
}

// Limpia las posiciones capturadas
void limpiarPosicionesCapturadas(Movimiento* mov) {
    mov->numCapturas = 0;
    if (mov->tipo == CAPTURA || mov->tipo == CAPTURA_MULTIPLE) {
        mov->tipo = SIMPLE;
    }
}

// Verifica si el movimiento es una captura
int esCaptura(const Movimiento* mov) {
    return mov->tipo == CAPTURA || mov->tipo == CAPTURA_MULTIPLE;
}

// Obtiene el número de capturas del movimiento
int obtenerNumCapturas(const Movimiento* mov) {
    return mov->numCapturas;
}

// Genera una representación en texto del movimiento
void obtenerTextoMovimiento(const Movimiento* mov, char* buffer, int tamBuffer) {
    snprintf(buffer, tamBuffer, "(%d,%d) -> (%d,%d)", 
             mov->filaOrigen, mov->colOrigen, 
             mov->filaDestino, mov->colDestino);
    
    if (esCaptura(mov)) {
        char temp[50];
        snprintf(temp, sizeof(temp), " [Captura %d pieza(s)]", mov->numCapturas);
        strncat(buffer, temp, tamBuffer - strlen(buffer) - 1);
    }
    
    if (mov->promocionaADama) {
        strncat(buffer, " [Coronacion]", tamBuffer - strlen(buffer) - 1);
    }
}

// Verifica si el movimiento es válido
int esMovimientoValido(const Movimiento* mov) {
    // Verificar límites
    if (mov->filaOrigen < 0 || mov->filaOrigen >= 10 || 
        mov->colOrigen < 0 || mov->colOrigen >= 10) {
        return 0;
    }
    if (mov->filaDestino < 0 || mov->filaDestino >= 10 || 
        mov->colDestino < 0 || mov->colDestino >= 10) {
        return 0;
    }
    
    // Verificar que origen y destino no sean iguales
    if (mov->filaOrigen == mov->filaDestino && mov->colOrigen == mov->colDestino) {
        return 0;
    }
    
    return 1;
}

// Inicializa el historial de movimientos
void inicializarHistorial(HistorialMovimientos* historial) {
    historial->numMovimientos = 0;
    historial->indiceActual = -1;
}

// Agrega un movimiento al historial
void agregarMovimientoHistorial(HistorialMovimientos* historial, const Movimiento* mov) {
    if (historial->numMovimientos < 500) {
        // Si no estamos al final del historial, eliminar movimientos futuros
        if (historial->indiceActual < historial->numMovimientos - 1) {
            historial->numMovimientos = historial->indiceActual + 1;
        }
        
        // Agregar el movimiento
        historial->movimientos[historial->numMovimientos] = *mov;
        historial->numMovimientos++;
        historial->indiceActual++;
    }
}

// Muestra el historial de movimientos
void mostrarHistorial(const HistorialMovimientos* historial) {
    if (historial->numMovimientos == 0) {
        printf("No hay movimientos en el historial.\n");
        return;
    }
    
    printf("\n=== Historial de Movimientos ===\n");
    printf("%-6s %-20s %-15s\n", "Turno", "Movimiento", "Tipo");
    printf("--------------------------------------------\n");
    
    for (int i = 0; i < historial->numMovimientos; i++) {
        const Movimiento* mov = &historial->movimientos[i];
        char textoMov[100];
        obtenerTextoMovimiento(mov, textoMov, sizeof(textoMov));
        
        const char* tipoStr;
        switch (mov->tipo) {
            case SIMPLE:
                tipoStr = "Simple";
                break;
            case CAPTURA:
                tipoStr = "Captura";
                break;
            case CAPTURA_MULTIPLE:
                tipoStr = "Cap. Multiple";
                break;
            case PROMOCION:
                tipoStr = "Coronacion";
                break;
            default:
                tipoStr = "Desconocido";
        }
        
        printf("%-6d %-20s %-15s", i + 1, textoMov, tipoStr);
        
        if (i == historial->indiceActual) {
            printf(" <- Actual");
        }
        
        printf("\n");
    }
    
    printf("================================\n");
}

// Limpia el historial
void limpiarHistorial(HistorialMovimientos* historial) {
    historial->numMovimientos = 0;
    historial->indiceActual = -1;
}
