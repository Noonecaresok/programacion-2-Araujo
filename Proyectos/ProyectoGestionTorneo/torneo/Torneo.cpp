#include "Torneo.hpp"
#include <cstring>
#include <fstream>

using namespace std;

Torneo::Torneo() {
    strcpy(nombre, ""); strcpy(deporte, ""); strcpy(formato, "");
    strcpy(fechaInicio, ""); strcpy(fechaFin, "");
    fechaCreacion = time(0); fechaUltimaModificacion = time(0);
}

Torneo::Torneo(const char* _nombre, const char* _deporte, const char* _formato, const char* _inicio, const char* _fin) {
    strcpy(nombre, _nombre); strcpy(deporte, _deporte); strcpy(formato, _formato);
    strcpy(fechaInicio, _inicio); strcpy(fechaFin, _fin);
    fechaCreacion = time(0); fechaUltimaModificacion = time(0);
}

const char* Torneo::getNombre() const { return nombre; }

bool Torneo::leerTorneoActual(Torneo& t) {
    ifstream archivo("datos/torneo.bin", ios::binary);
    if (!archivo.is_open()) return false;
    archivo.read(reinterpret_cast<char*>(&t), sizeof(Torneo));
    archivo.close();
    return true;
}