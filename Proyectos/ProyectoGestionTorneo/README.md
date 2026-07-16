# Sistema de Gestión de Torneos Deportivos (Proyecto 1)

**Asignatura:** Programación 2  
**Profesor:** Ing. Victor Kneider  
**Trimestre:** 2026-B  

## Descripción del Proyecto
Este proyecto es un sistema en C++ diseñado para administrar un torneo deportivo de cualquier deporte, pero se programo pensando en Futbol. Permite la gestión completa (CRUD) de Equipos, Jugadores y Partidos, calculando automáticamente las estadísticas deportivas y generando una tabla de posiciones dinámica.

## Instrucciones de Compilación y Ejecución
El proyecto está escrito en C++ estándar y utiliza la biblioteca `<windows.h>` para renderizar correctamente los caracteres UTF-8 en la consola de Windows.

**Para compilar (usando g++ en consola):**
\`\`\`bash
g++ GestionDeTorneo.cpp -o GestionDeTorneo
\`\`\`

**Para ejecutar:**
\`\`\`bash
.\GestionDeTorneo
\`\`\`

## Arquitectura del Sistema
El sistema se construyó bajo un estricto modelo de **dos capas**:
1. **Capa de Lógica:** Funciones que operan directamente sobre la memoria (`SistemaDeportivo*`). Retornan punteros, arrays de punteros o booleanos. **No contienen interacciones de E/S (`cin`/`cout`).**
2. **Capa de Presentación:** Funciones que actúan como interfaz gráfica (`menús`). Se encargan de validar las entradas del usuario, invocar a la capa de lógica, interpretar los punteros retornados, liberar la memoria de los arrays temporales (`delete[]`) e imprimir los resultados en pantalla.

## Gestión de Memoria Dinámica
Se implementó una gestión manual del *Heap*. 
- Los arreglos de Equipos, Jugadores y Partidos inicializan con una capacidad de 4 elementos.
- Cuentan con un sistema de **redimensionamiento automático** que duplica la capacidad (`capacidad * 2`) y migra los datos cuando el arreglo se llena, liberando el bloque de memoria anterior.
- Las funciones de búsqueda múltiple (ej. `listarJugadores`) retornan arrays dinámicos de punteros (`Jugador**`) del tamaño exacto de los resultados, los cuales son liberados posteriormente por la capa de presentación para evitar fugas de memoria.

## Funcionalidades Implementadas
- **Equipos:** Registro, búsqueda por subcadena, actualización de datos básicos, eliminación segura (solo si no tienen partidos) y generación de Tabla de Posiciones ordenada por múltiples criterios (Burbuja).
- **Jugadores:** Registro con validación cruzada (cédulas y dorsales únicos por equipo), búsqueda y listados filtrados.
- **Partidos:** Programación de encuentros (validando disponibilidad), registro de resultados con alteración automática de estadísticas (`PTS, G, E, D, GF, GC`), y cancelación de partidos con **reversión matemática** de las estadísticas.