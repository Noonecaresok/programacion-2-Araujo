#include "Jugador.hpp"
#include <cstring>

Jugador::Jugador() {
    id = 0; idEquipo = 0; edad = 0; numeroDorsal = 0;
    golesAnotados = 0; tarjetasAmarillas = 0; tarjetasRojas = 0;
    strcpy(nombre, ""); strcpy(cedula, ""); strcpy(posicion, "");
    eliminado = false; fechaCreacion = time(0); fechaUltimaModificacion = time(0);
}

Jugador::Jugador(int _idEquipo, const char* _nombre, const char* _cedula, const char* _posicion, int _edad, int _dorsal) {
    id = 0; 
    idEquipo = _idEquipo;
    strcpy(nombre, _nombre);
    strcpy(cedula, _cedula);
    strcpy(posicion, _posicion);
    edad = _edad;
    numeroDorsal = _dorsal;
    golesAnotados = 0; tarjetasAmarillas = 0; tarjetasRojas = 0;
    eliminado = false; fechaCreacion = time(0); fechaUltimaModificacion = time(0);
}

int Jugador::getId() const { return id; }
int Jugador::getIdEquipo() const { return idEquipo; }
const char* Jugador::getNombre() const { return nombre; }
const char* Jugador::getCedula() const { return cedula; }
const char* Jugador::getPosicion() const { return posicion; }
int Jugador::getEdad() const { return edad; }
int Jugador::getNumeroDorsal() const { return numeroDorsal; }
int Jugador::getGolesAnotados() const { return golesAnotados; }
bool Jugador::isEliminado() const { return eliminado; }

void Jugador::setId(int _id) { this->id = _id; }
void Jugador::setIdEquipo(int _idEquipo) { this->idEquipo = _idEquipo; actualizarFechaModificacion(); }

bool Jugador::setNombre(const char* _nombre) {
    if (strlen(_nombre) == 0) return false;
    strcpy(this->nombre, _nombre);
    actualizarFechaModificacion();
    return true;
}

bool Jugador::setPosicion(const char* _posicion) {
    strcpy(this->posicion, _posicion);
    actualizarFechaModificacion();
    return true;
}

bool Jugador::setEdad(int _edad) {
    if (_edad < 14 || _edad > 50) return false; // Regla de negocio encapsulada
    this->edad = _edad;
    actualizarFechaModificacion();
    return true;
}

bool Jugador::setNumeroDorsal(int _dorsal) {
    if (_dorsal < 1 || _dorsal > 99) return false;
    this->numeroDorsal = _dorsal;
    actualizarFechaModificacion();
    return true;
}

void Jugador::setEliminado(bool estado) {
    this->eliminado = estado;
    actualizarFechaModificacion();
}

void Jugador::actualizarFechaModificacion() { this->fechaUltimaModificacion = time(0); }

void Jugador::registrarGol() {
    this->golesAnotados++;
    actualizarFechaModificacion();
}

size_t Jugador::obtenerTamano() { return sizeof(Jugador); }