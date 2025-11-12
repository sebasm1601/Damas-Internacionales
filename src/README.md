# Damas Internacionales

## Integrantes
Sebastian Andres Manotas Blanco


### Innovaciones

#### 1. Sistema de Cronómetro
**¿Para qué se implementó?**
Para llevar un registro preciso del tiempo que cada jugador toma en sus turnos y en total durante la partida.

**¿Por qué se consideró necesario?**
Permite analizar el desempeño de los jugadores y añade un elemento competitivo al juego. Es útil para partidas formales y torneos.

**¿Cómo se llevó a cabo?**
Se implementó usando la biblioteca `<time.h>` de C, creando una estructura `Cronometro` que registra el tiempo de inicio y fin de cada turno. El cronómetro se inicia automáticamente al comenzar el turno de un jugador y se detiene al cambiar de jugador.

#### 2. Historial de Movimientos
**¿Para qué se implementó?**
Para mantener un registro completo de todos los movimientos realizados durante la partida.

**¿Por qué se consideró necesario?**
Facilita el análisis de la partida, permite revisar jugadas pasadas y es esencial para la funcionalidad de exportación a CSV.

**¿Cómo se llevó a cabo?**
Se creó una estructura `HistorialMovimientos` que almacena un arreglo de hasta 500 movimientos. Cada movimiento incluye información sobre origen, destino, tipo (simple, captura, múltiple), y piezas capturadas.

#### 3. Exportación a CSV
**¿Para qué se implementó?**
Para generar un archivo CSV con el registro completo de la partida, incluyendo todos los movimientos, tiempos y estadísticas.

**¿Por qué se consideró necesario?**
Permite analizar partidas fuera del programa, compartir resultados y mantener un archivo histórico de las partidas jugadas. Es útil para estudiar estrategias y mejorar el juego.

**¿Cómo se llevó a cabo?**
Se implementó un sistema de exportación que genera archivos CSV con formato estándar, incluyendo: turno, jugador, color, coordenadas, tipo de movimiento, capturas, y tiempos. Los archivos se generan con nombres únicos basados en fecha y hora.

#### 4. Sistema de Guardado y Carga
**¿Para qué se implementó?**
Para permitir guardar partidas en progreso y reanudarlas posteriormente.

**¿Por qué se consideró necesario?**
Las partidas de damas pueden ser largas y no siempre se completan en una sesión. Esta funcionalidad permite a los jugadores pausar y continuar en otro momento.

**¿Cómo se llevó a cabo?**
Se implementó un sistema de 5 slots de guardado que serializa el estado completo del juego (tablero, jugadores, estadísticas, tiempos) en archivos. Se utilizan archivos separados para los datos del juego y los metadatos. El sistema incluye:
- Función `limpiarTablero()` para preparar la carga sin reinicializar piezas
- Deserialización completa que reconstruye el tablero pieza por pieza
- Preservación de posiciones, tipos (normal/dama), estadísticas y tiempos

#### 5. Estadísticas de Jugadores
**¿Para qué se implementó?**
Para llevar un registro detallado del desempeño de cada jugador durante la partida.

**¿Por qué se consideró necesario?**
Proporciona información valiosa sobre el juego, como piezas capturadas, damas creadas y número de movimientos. Esto añade profundidad al análisis de la partida.

**¿Cómo se llevó a cabo?**
Se agregaron campos a la estructura `Jugador` para almacenar estadísticas que se actualizan automáticamente durante el juego. Las estadísticas se pueden visualizar en cualquier momento durante la partida.



## Compilación

### Usando Make (Linux/Mac)
```bash
make
```

### Compilación Manual (Linux/Mac)
```bash
g++ -o damas main.cpp pieza.cpp tablero.cpp movimiento.cpp cronometro.cpp jugador.cpp exportador_csv.cpp guardador_juego.cpp juego.cpp juego_logica.cpp juego_menu.cpp juego_main.cpp -std=c++11
```

### En Windows
```bash
g++ -o damas.exe main.cpp pieza.cpp tablero.cpp movimiento.cpp cronometro.cpp jugador.cpp exportador_csv.cpp guardador_juego.cpp juego.cpp juego_logica.cpp juego_menu.cpp juego_main.cpp -std=c++11
```

O simplemente ejecutar el script:
```bash
compilar_windows.bat
```



