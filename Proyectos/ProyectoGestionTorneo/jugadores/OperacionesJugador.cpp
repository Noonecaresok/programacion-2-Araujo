#include "OperacionesJugador.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include <cstring>
#include <cctype>
#include <fstream>

using namespace std;

const char* OperacionesJugador::RUTA_JUGADORES = "datos/jugadores.bin";

int OperacionesJugador::listarTodos(Jugador resultados[], int maxResultados) {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_JUGADORES);
    Jugador temp;
    int encontrados = 0;

    ifstream archivo(RUTA_JUGADORES, ios::binary);
    if (!archivo.is_open()) return 0;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros && encontrados < maxResultados; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Jugador));
        if (!temp.isEliminado()) {
            resultados[encontrados] = temp;
            encontrados++;
        }
    }
    archivo.close();
    return encontrados;
}

int OperacionesJugador::listarPorEquipo(int idEquipo, Jugador resultados[], int maxResultados) {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_JUGADORES);
    Jugador temp;
    int encontrados = 0;

    ifstream archivo(RUTA_JUGADORES, ios::binary);
    if (!archivo.is_open()) return 0;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros && encontrados < maxResultados; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Jugador));
        if (!temp.isEliminado() && temp.getIdEquipo() == idEquipo) {
            resultados[encontrados] = temp;
            encontrados++;
        }
    }
    archivo.close();
    return encontrados;
}

int OperacionesJugador::buscarPorNombre(const char* subcadena, Jugador resultados[], int maxResultados) {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_JUGADORES);
    Jugador temp;
    int encontrados = 0;
    
    char sub[100];
    for(int j = 0; subcadena[j] != '\0'; j++) sub[j] = tolower(subcadena[j]);
    sub[strlen(subcadena)] = '\0';

    ifstream archivo(RUTA_JUGADORES, ios::binary);
    if (!archivo.is_open()) return 0;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros && encontrados < maxResultados; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Jugador));
        if (!temp.isEliminado()) {
            char cad[100];
            const char* nombreOrig = temp.getNombre();
            for(int j = 0; nombreOrig[j] != '\0'; j++) cad[j] = tolower(nombreOrig[j]);
            cad[strlen(nombreOrig)] = '\0';
            
            if (strstr(cad, sub) != nullptr) {
                resultados[encontrados] = temp;
                encontrados++;
            }
        }
    }
    archivo.close();
    return encontrados;
}

int OperacionesJugador::validarUnicidad(const char* cedula, int idEquipo, int dorsal) {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_JUGADORES);
    Jugador temp;
    ifstream archivo(RUTA_JUGADORES, ios::binary);
    if (!archivo.is_open()) return 0; 

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Jugador));
        if (!temp.isEliminado()) {
            if (strcmp(temp.getCedula(), cedula) == 0) return 1; // 1 = Cédula repetida
            if (temp.getIdEquipo() == idEquipo && temp.getNumeroDorsal() == dorsal) return 2; // 2 = Dorsal repetido en el equipo
        }
    }
    archivo.close();
    return 0; // Todo OK
}

bool OperacionesJugador::eliminarJugador(int id) {
    Jugador j;
    if (!GestorArchivos::obtenerRegistroPorID<Jugador>(RUTA_JUGADORES, id, j)) return false;
    
    j.setEliminado(true);
    return GestorArchivos::actualizarRegistro<Jugador>(RUTA_JUGADORES, j);
}