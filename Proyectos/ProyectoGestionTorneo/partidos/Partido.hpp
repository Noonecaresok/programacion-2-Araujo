#ifndef PARTIDO_HPP
#define PARTIDO_HPP

#include "Gol.hpp"
#include <ctime>
#include <cstddef>

class Partido {
private:
    // Mismo orden exacto del struct original
    int  id;
    int  idEquipoLocal;
    int  idEquipoVisitante;
    char fecha[11];
    char estado[12];       // "PROGRAMADO", "JUGADO", "CANCELADO"
    char descripcion[200];

    int  golesLocal;
    int  golesVisitante;

    Gol  goles[22];        // ¡COMPOSICIÓN! Un arreglo de objetos Gol
    int  numGoles;

    bool   eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    Partido();
    Partido(int _idLocal, int _idVisitante, const char* _fecha, const char* _desc);

    // Getters
    int getId() const;
    int getIdEquipoLocal() const;
    int getIdEquipoVisitante() const;
    const char* getFecha() const;
    const char* getEstado() const;
    const char* getDescripcion() const;
    int getGolesLocal() const;
    int getGolesVisitante() const;
    int getNumGoles() const;
    Gol getGol(int indice) const; // Para leer un gol específico
    bool isEliminado() const;

    // Setters
    void setId(int _id);
    void setEstado(const char* _estado);
    void setEliminado(bool estado);
    void actualizarFechaModificacion();

    // Métodos de Negocio (El partido se gestiona a sí mismo)
    bool registrarGol(int _idJugador, int _minuto, const char* _equipoTipo);
    void finalizarPartido();

    static size_t obtenerTamano();
};

#endif