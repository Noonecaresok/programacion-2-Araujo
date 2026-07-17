# Proyecto 2: Sistema de Gestión de Torneos

Sistema de gestión deportiva desarrollado en C++ utilizando **persistencia de datos mediante archivos binarios** y **acceso aleatorio**.

## 1. Estructuras y Tamaños (sizeof)
El sistema utiliza estructuras de tamaño fijo para garantizar el acceso directo a memoria en disco:

| Estructura | Tamaño (Bytes) |
| :--- | :--- |
| `ArchivoHeader` | 16 |
| `Equipo` | 540 |
| `Jugador` | 280 |
| `Partido` | 1500 |

## 2. Fórmulas de Acceso Aleatorio
Para leer o escribir un registro específico sin recorrer el archivo, se utiliza la siguiente fórmula:

> `posicion = sizeof(ArchivoHeader) + (indice * sizeof(Estructura))`

*   **`sizeof(ArchivoHeader)`**: Salta la metadata inicial.
*   **`indice`**: Posición lógica del registro (0, 1, 2...).
*   **`sizeof(Estructura)`**: Tamaño en bytes de la entidad.

## 3. Manual de Usuario
*   **Gestión de Equipos:** Registro, búsqueda, actualización y borrado lógico.
*   **Gestión de Jugadores:** Registro validando unicidad de cédula y dorsal.
*   **Gestión de Partidos:** Programación y registro de resultados con actualización automática de estadísticas (Partidos, Equipos y Jugadores).
*   **Reportes:** Tablas de posiciones ordenadas y Top 10 de goleadores (lectura directa desde archivo).
*   **Mantenimiento:** Verificación de integridad referencial y creación de respaldos (Backups) automáticos.

## 4. Instrucciones de Ejecución
1. **Compilar:** 
   `g++ GestionDeTorneo.cpp -o Torneo.exe`
2. **Ejecutar:** 
   `./Torneo.exe`

---
**Autor:** David Araujo | **Asignatura:** Programación 2 | **Profesor:** Ing. Victor Kneider