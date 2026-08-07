#include "OperacionesEquipo.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include <cstring>
#include <cctype>
#include <fstream>

using namespace std;

//Definimos la ruta del archivo
const char* OperacionesEquipo::RUTA_EQUIPOS = "datos/equipos.bin";

bool OperacionesEquipo::eliminarEquipo(int id) {
    Equipo equipo;
    //Buscamos el equipo usando nuestro Template de GestorArchivos
    if (!GestorArchivos::obtenerRegistroPorID<Equipo>(RUTA_EQUIPOS, id, equipo)) {
        return false; //El equipo no existe
    }
    
    //REGLA DE NEGOCIO: No se puede eliminar si ya tiene partidos asociados
    if (equipo.getCantidadPartidos() > 0) {
        return false; 
    }

    //Aplicamos el borrado lógico a través del setter
    equipo.setEliminado(true);

    //Guardamos los cambios en el disco
    return GestorArchivos::actualizarRegistro<Equipo>(RUTA_EQUIPOS, equipo);
}

int OperacionesEquipo::buscarPorNombre(const char* subcadena, Equipo resultados[], int maxResultados) {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_EQUIPOS);
    Equipo temp;
    int encontrados = 0;
    
    //Convertimos la búsqueda a minúsculas para que no importe cómo escriba el usuario
    char sub[100];
    for(int j = 0; subcadena[j] != '\0'; j++) sub[j] = tolower(subcadena[j]);
    sub[strlen(subcadena)] = '\0';

    ifstream archivo(RUTA_EQUIPOS, ios::binary);
    if (!archivo.is_open()) return 0;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros && encontrados < maxResultados; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Equipo));
        
        if (!temp.isEliminado()) {
            char cad[100];
            const char* nombreOriginal = temp.getNombre();
            for(int j = 0; nombreOriginal[j] != '\0'; j++) cad[j] = tolower(nombreOriginal[j]);
            cad[strlen(nombreOriginal)] = '\0';
            
            //Si la subcadena está dentro del nombre, lo agregamos a los resultados
            if (strstr(cad, sub) != nullptr) {
                resultados[encontrados] = temp;
                encontrados++;
            }
        }
    }
    archivo.close();
    return encontrados;
}

int OperacionesEquipo::listarTodos(Equipo resultados[], int maxResultados) {
    ArchivoHeader h = GestorArchivos::leerHeader(RUTA_EQUIPOS);
    Equipo temp;
    int encontrados = 0;

    ifstream archivo(RUTA_EQUIPOS, ios::binary);
    if (!archivo.is_open()) return 0;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros && encontrados < maxResultados; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Equipo));
        if (!temp.isEliminado()) {
            resultados[encontrados] = temp;
            encontrados++;
        }
    }
    archivo.close();
    return encontrados;
}