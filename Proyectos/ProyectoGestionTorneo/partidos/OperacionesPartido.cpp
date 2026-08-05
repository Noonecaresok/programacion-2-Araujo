#include "OperacionesPartido.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include <cstring>
#include <fstream>

using namespace std;

const char* OperacionesPartido::RUTA_PARTIDOS = "datos/partidos.bin";

int OperacionesPartido::listarTodos(Partido resultados[], int maxResultados) {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_PARTIDOS);
    Partido temp;
    int encontrados = 0;

    ifstream archivo(RUTA_PARTIDOS, ios::binary);
    if (!archivo.is_open()) return 0;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros && encontrados < maxResultados; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Partido));
        if (!temp.isEliminado()) {
            resultados[encontrados] = temp;
            encontrados++;
        }
    }
    archivo.close();
    return encontrados;
}

int OperacionesPartido::listarPorEstado(const char* estado, Partido resultados[], int maxResultados) {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_PARTIDOS);
    Partido temp;
    int encontrados = 0;

    ifstream archivo(RUTA_PARTIDOS, ios::binary);
    if (!archivo.is_open()) return 0;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros && encontrados < maxResultados; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Partido));
        if (!temp.isEliminado() && strcmp(temp.getEstado(), estado) == 0) {
            resultados[encontrados] = temp;
            encontrados++;
        }
    }
    archivo.close();
    return encontrados;
}

int OperacionesPartido::listarPorEquipo(int idEquipo, Partido resultados[], int maxResultados) {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_PARTIDOS);
    Partido temp;
    int encontrados = 0;

    ifstream archivo(RUTA_PARTIDOS, ios::binary);
    if (!archivo.is_open()) return 0;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros && encontrados < maxResultados; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Partido));
        if (!temp.isEliminado() && (temp.getIdEquipoLocal() == idEquipo || temp.getIdEquipoVisitante() == idEquipo)) {
            resultados[encontrados] = temp;
            encontrados++;
        }
    }
    archivo.close();
    return encontrados;
}