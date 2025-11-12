#ifndef MOVIMIENTO_H
#define MOVIMIENTO_H

// Tipos de movimiento
enum TipoMovimiento {
    SIMPLE,
    CAPTURA,
    CAPTURA_MULTIPLE,
    PROMOCION
};

// Estructura para almacenar una posición capturada
typedef struct {
    int fila;
    int columna;
} PosicionCapturada;

// Estructura que representa un movimiento
typedef struct {
    int filaOrigen;
    int colOrigen;
    int filaDestino;
    int colDestino;
    enum TipoMovimiento tipo;
    PosicionCapturada posicionesCapturadas[20];  // Máximo 20 capturas en un movimiento
    int numCapturas;
    int promocionaADama;  // 0 = no, 1 = sí
    int puntaje;
} Movimiento;

// Estructura para el historial de movimientos
typedef struct {
    Movimiento movimientos[500];  // Máximo 500 movimientos por partida
    int numMovimientos;
    int indiceActual;
} HistorialMovimientos;

// Funciones para movimientos
void inicializarMovimiento(Movimiento* mov);
void crearMovimiento(Movimiento* mov, int fOrigen, int cOrigen, int fDestino, int cDestino, enum TipoMovimiento tipo);
void agregarPosicionCapturada(Movimiento* mov, int fila, int columna);
void limpiarPosicionesCapturadas(Movimiento* mov);
int esCaptura(const Movimiento* mov);
int obtenerNumCapturas(const Movimiento* mov);
void obtenerTextoMovimiento(const Movimiento* mov, char* buffer, int tamBuffer);
int esMovimientoValido(const Movimiento* mov);

// Funciones para historial
void inicializarHistorial(HistorialMovimientos* historial);
void agregarMovimientoHistorial(HistorialMovimientos* historial, const Movimiento* mov);
void mostrarHistorial(const HistorialMovimientos* historial);
void limpiarHistorial(HistorialMovimientos* historial);

#endif
