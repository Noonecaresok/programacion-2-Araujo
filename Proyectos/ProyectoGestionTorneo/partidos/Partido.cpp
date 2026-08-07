#include "Partido.hpp"
#include <cstring>

Partido::Partido() {
    id = 0; idEquipoLocal = 0; idEquipoVisitante = 0;
    strcpy(fecha, ""); strcpy(estado, "PROGRAMADO"); strcpy(descripcion, "");
    golesLocal = 0; golesVisitante = 0; numGoles = 0;
    eliminado = false; fechaCreacion = time(0); fechaUltimaModificacion = time(0);
}

Partido::Partido(int _idLocal, int _idVisitante, const char* _fecha, const char* _desc) {
    id = 0; 
    idEquipoLocal = _idLocal; 
    idEquipoVisitante = _idVisitante;
    strcpy(fecha, _fecha); 
    strcpy(estado, "PROGRAMADO"); 
    strcpy(descripcion, _desc);
    golesLocal = 0; golesVisitante = 0; numGoles = 0;
    eliminado = false; fechaCreacion = time(0); fechaUltimaModificacion = time(0);
}

int Partido::getId() const { return id; }
int Partido::getIdEquipoLocal() const { return idEquipoLocal; }
int Partido::getIdEquipoVisitante() const { return idEquipoVisitante; }
const char* Partido::getFecha() const { return fecha; }
const char* Partido::getEstado() const { return estado; }
const char* Partido::getDescripcion() const { return descripcion; }
int Partido::getGolesLocal() const { return golesLocal; }
int Partido::getGolesVisitante() const { return golesVisitante; }
int Partido::getNumGoles() const { return numGoles; }
bool Partido::isEliminado() const { return eliminado; }
Gol Partido::getGol(int indice) const { return goles[indice]; }

void Partido::setId(int _id) { this->id = _id; }
void Partido::setEstado(const char* _estado) { 
    strcpy(this->estado, _estado); 
    actualizarFechaModificacion();
}
void Partido::setEliminado(bool estado) {
    this->eliminado = estado;
    actualizarFechaModificacion();
}
void Partido::actualizarFechaModificacion() { this->fechaUltimaModificacion = time(0); }

//El partido registra sus propios goles internamente
bool Partido::registrarGol(int _idJugador, int _minuto, const char* _equipoTipo) {
    if (numGoles >= 22) return false; // Tope máximo
    
    goles[numGoles] = Gol(_idJugador, _minuto, _equipoTipo);
    numGoles++;
    
    if (strcmp(_equipoTipo, "LOCAL") == 0) golesLocal++;
    else if (strcmp(_equipoTipo, "VISITANTE") == 0) golesVisitante++;
    
    actualizarFechaModificacion();
    return true;
}

void Partido::finalizarPartido() {
    strcpy(this->estado, "JUGADO");
    actualizarFechaModificacion();
}

size_t Partido::obtenerTamano() { return sizeof(Partido); }