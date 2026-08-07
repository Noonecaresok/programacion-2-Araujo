#ifndef OPERACIONES_JUGADOR_HPP
#define OPERACIONES_JUGADOR_HPP

#include "Jugador.hpp"

class OperacionesJugador {
public:
    static const char* RUTA_JUGADORES;

    static int listarTodos(Jugador resultados[], int maxResultados);
    static int listarPorEquipo(int idEquipo, Jugador resultados[], int maxResultados);
    static int buscarPorNombre(const char* subcadena, Jugador resultados[], int maxResultados);
    
    // Reglas críticas de negocio
    static int validarUnicidad(const char* cedula, int idEquipo, int dorsal);
    static bool eliminarJugador(int id);
};

#endif