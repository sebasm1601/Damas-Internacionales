#include "juego.h"
#include <stdio.h>
#include <stdlib.h>

//BUCLE PRINCIPAL DEL JUEGO

void jugarPartida(Juego* juego) {
    while (!juego->juegoTerminado) {
        // Limpiar pantalla 
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif
        
        // Mostrar tablero
        mostrarTableroJuego(juego);
        mostrarEstadoJuego(juego);
        
        // Verificar si hay capturas obligatorias
        Movimiento capturasObligatorias[MAX_MOVIMIENTOS_POSIBLES];
        int numCapturas = 0;
        encontrarTodasCapturas(juego, obtenerColorJugador(juego->jugadorActual),
                              capturasObligatorias, &numCapturas);
        
        if (numCapturas > 0) {
            printf("*** HAY CAPTURAS OBLIGATORIAS ***\n");
            printf("Debes realizar una captura.\n\n");
        }
        
        // Menú de opciones durante el juego
        printf("Opciones:\n");
        printf("  1. Realizar movimiento\n");
        printf("  2. Ver historial\n");
        printf("  3. Ver estadisticas\n");
        printf("  4. Guardar partida\n");
        printf("  5. Exportar a CSV\n");
        printf("  6. Rendirse\n");
        printf("  7. Volver al menu principal\n\n");
        
        int opcion = obtenerEntradaValida("Seleccione una opcion: ");
        
        switch (opcion) {
            case 1: {
                // Realizar movimiento
                printf("\nIngrese coordenadas del movimiento:\n");
                
                int filaOrigen = obtenerEntradaValida("Fila origen (0-9): ");
                int colOrigen = obtenerEntradaValida("Columna origen (0-9): ");
                int filaDestino = obtenerEntradaValida("Fila destino (0-9): ");
                int colDestino = obtenerEntradaValida("Columna destino (0-9): ");
                
                if (realizarMovimiento(juego, filaOrigen, colOrigen, filaDestino, colDestino)) {
                    if (juego->juegoTerminado) {
                        // Mostrar tablero final
                        #ifdef _WIN32
                        system("cls");
                        #else
                        system("clear");
                        #endif
                        
                        mostrarTableroJuego(juego);
                        mostrarEstadisticasJugadores(juego);
                        
                        printf("\nDesea exportar la partida a CSV? (1=Si, 0=No): ");
                        int exportar = obtenerEntradaValida("");
                        if (exportar == 1) {
                            exportarJuegoACSV(juego);
                        }
                        
                        printf("\nPresione Enter para continuar...");
                        getchar();
                    }
                } else {
                    printf("\nPresione Enter para continuar...");
                    getchar();
                }
                break;
            }
            
            case 2:
                // Ver historial
                mostrarHistorialMovimientos(juego);
                printf("\nPresione Enter para continuar...");
                getchar();
                break;
            
            case 3:
                // Ver estadísticas
                mostrarEstadisticasJugadores(juego);
                printf("\nPresione Enter para continuar...");
                getchar();
                break;
            
            case 4:
                // Guardar partida
                mostrarMenuGuardar(juego);
                printf("\nPresione Enter para continuar...");
                getchar();
                break;
            
            case 5:
                // Exportar a CSV
                exportarJuegoACSV(juego);
                printf("\nPresione Enter para continuar...");
                getchar();
                break;
            
            case 6: {
                // Rendirse
                printf("\n Esta seguro de que desea rendirse? (1=Si, 0=No): ");
                int confirmar = obtenerEntradaValida("");
                
                if (confirmar == 1) {
                    juego->juegoTerminado = 1;
                    
                    // El ganador es el otro jugador
                    if (juego->jugadorActual == &juego->jugador1) {
                        juego->ganador = &juego->jugador2;
                    } else {
                        juego->ganador = &juego->jugador1;
                    }
                    
                    detenerCronometro(obtenerCronometro(&juego->jugador1));
                    detenerCronometro(obtenerCronometro(&juego->jugador2));
                    
                    printf("\n%s se ha rendido.\n", obtenerNombreJugador(juego->jugadorActual));
                    printf("Ganador: %s\n", obtenerNombreJugador(juego->ganador));
                    
                    printf("\nPresione Enter para continuar...");
                    getchar();
                }
                break;
            }
            
            case 7: {
                // Volver al menú principal
                printf("\n Desea guardar la partida antes de salir? (1=Si, 0=No): ");
                int guardar = obtenerEntradaValida("");
                
                if (guardar == 1) {
                    mostrarMenuGuardar(juego);
                }
                
                // Detener cronómetros
                detenerCronometro(obtenerCronometro(&juego->jugador1));
                detenerCronometro(obtenerCronometro(&juego->jugador2));
                
                return;
            }
            
            default:
                printf("Opcion invalida.\n");
                printf("\nPresione Enter para continuar...");
                getchar();
                break;
        }
    }
}

//FUNCIÓN PRINCIPAL

void ejecutarJuego(Juego* juego) {
    // Inicializar el juego
    inicializarJuego(juego);
    
    // Mostrar mensaje de bienvenida
    printf("\n");
    printf("=========================================================\n");
    printf("                                                         \n");
    printf("          DAMAS INTERNACIONALES                          \n");
    printf("          International Draughts                         \n");
    printf("                                                         \n");
    printf("          Tablero 10x10                                  \n");
    printf("          20 piezas por jugador                          \n");
    printf("                                                         \n");
    printf("=========================================================\n");
    printf("\n");
    printf("Presione Enter para continuar...");
    getchar();
    
    // Mostrar menú principal
    mostrarMenu(juego);
}