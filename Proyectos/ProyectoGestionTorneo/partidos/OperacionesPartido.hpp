#ifndef OPERACIONES_PARTIDO_HPP
#define OPERACIONES_PARTIDO_HPP

#include "Partido.hpp"

class OperacionesPartido {
public:
    static const char* RUTA_PARTIDOS;

    static int listarTodos(Partido resultados[], int maxResultados);
    static int listarPorEstado(const char* estado, Partido resultados[], int maxResultados);
    static int listarPorEquipo(int idEquipo, Partido resultados[], int maxResultados);
};

#endif