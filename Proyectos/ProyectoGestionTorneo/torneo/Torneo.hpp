#ifndef TORNEO_HPP
#define TORNEO_HPP

#include <ctime>

class Torneo {
private:
    char nombre[100];
    char deporte[50];
    char formato[20];
    char fechaInicio[11];
    char fechaFin[11];
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    Torneo();
    Torneo(const char* _nombre, const char* _deporte, const char* _formato, const char* _inicio, const char* _fin);

    const char* getNombre() const;
    
    //Método estático de utilidad para leer el torneo único
    static bool leerTorneoActual(Torneo& t);
};

#endif