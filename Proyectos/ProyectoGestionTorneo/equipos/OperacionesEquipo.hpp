#ifndef OPERACIONES_EQUIPO_HPP
#define OPERACIONES_EQUIPO_HPP

#include "Equipo.hpp"

class OperacionesEquipo {
public:
    //Ruta fija del archivo para no estar escribiéndola a cada rato
    static const char* RUTA_EQUIPOS;

    //Métodos de negocio 
    static bool eliminarEquipo(int id);
    static int buscarPorNombre(const char* subcadena, Equipo resultados[], int maxResultados);
    static int listarTodos(Equipo resultados[], int maxResultados);
};

#endif