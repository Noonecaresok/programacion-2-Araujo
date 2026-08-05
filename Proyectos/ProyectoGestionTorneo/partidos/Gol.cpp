#include "Gol.hpp"
#include <cstring>

Gol::Gol() {
    idJugador = 0;
    minuto = 0;
    strcpy(equipo, "");
}

Gol::Gol(int _idJugador, int _minuto, const char* _equipo) {
    idJugador = _idJugador;
    minuto = _minuto;
    strcpy(equipo, _equipo);
}

int Gol::getIdJugador() const { return idJugador; }
int Gol::getMinuto() const { return minuto; }
const char* Gol::getEquipo() const { return equipo; }

void Gol::setIdJugador(int _idJugador) { this->idJugador = _idJugador; }
void Gol::setMinuto(int _minuto) { this->minuto = _minuto; }
void Gol::setEquipo(const char* _equipo) { strcpy(this->equipo, _equipo); }