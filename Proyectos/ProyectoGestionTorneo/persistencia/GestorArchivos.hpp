#ifndef GESTOR_ARCHIVOS_HPP
#define GESTOR_ARCHIVOS_HPP

#include <iostream>
#include <fstream>
#include "ArchivoHeader.hpp"

using namespace std;

class GestorArchivos {
private:
    //Método privado auxiliar para buscar la posición física (índice) de un registro
    template <typename T>
    static int buscarIndice(const char* ruta, int id) {
        ifstream archivo(ruta, ios::binary);
        if (!archivo.is_open()) return -1;

        ArchivoHeader h;
        archivo.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));

        T temp;
        for (int i = 0; i < h.cantidadRegistros; i++) {
            archivo.read(reinterpret_cast<char*>(&temp), sizeof(T));
            //Asume que la clase T tiene los metodos getId() y isEliminado()
            if (temp.getId() == id && !temp.isEliminado()) {
                archivo.close();
                return i;
            }
        }
        archivo.close();
        return -1;
    }

public:
    //Inicializa el archivo con un header si no existe
    template <typename T>
    static bool inicializarArchivo(const char* ruta) {
        ifstream validador(ruta, ios::binary);
        if (validador.good()) {
            validador.close();
            return true; 
        }
        ofstream creador(ruta, ios::binary);
        if (creador.fail()) return false;

        ArchivoHeader header = {0, 1, 0, 1}; 
        creador.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
        creador.close();
        return true;
    }

    //Lee el header de un archivo
    static ArchivoHeader leerHeader(const char* ruta) {
        ArchivoHeader header = {0, 1, 0, 1}; 
        ifstream archivo(ruta, ios::binary);
        if (archivo.is_open()) {
            archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
            archivo.close();
        }
        return header;
    }

    //Actualiza el header de un archivo
    static bool actualizarHeader(const char* ruta, ArchivoHeader header) {
        ofstream archivo(ruta, ios::binary | ios::in | ios::out); 
        if (!archivo.is_open()) return false;
        archivo.seekp(0, ios::beg); 
        archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
        archivo.close();
        return true;
    }

    //Guarda un nuevo registro (Funciona para Equipo, Jugador, Partido, etc.)
    template <typename T>
    static bool guardarRegistro(const char* ruta, T& entidad) {
        ArchivoHeader h = leerHeader(ruta);
        
        entidad.setId(h.proximoID); //Asignamos el ID correlativo

        ofstream archivo(ruta, ios::binary | ios::in | ios::out);
        if (!archivo.is_open()) return false;

        //Calculamos la posición: Header + (cantidadRegistros * sizeof(T))
        archivo.seekp(sizeof(ArchivoHeader) + (h.cantidadRegistros * sizeof(T)), ios::beg);
        archivo.write(reinterpret_cast<const char*>(&entidad), sizeof(T));
        archivo.close();

        h.cantidadRegistros++;
        h.registrosActivos++;
        h.proximoID++;
        return actualizarHeader(ruta, h);
    }

    //Obtiene un registro por su ID mediante Acceso Aleatorio
    template <typename T>
    static bool obtenerRegistroPorID(const char* ruta, int id, T& resultado) {
        int indice = buscarIndice<T>(ruta, id);
        if (indice == -1) return false;

        ifstream archivo(ruta, ios::binary);
        if (!archivo.is_open()) return false;

        int posicion = sizeof(ArchivoHeader) + (indice * sizeof(T));
        archivo.seekg(posicion, ios::beg);
        archivo.read(reinterpret_cast<char*>(&resultado), sizeof(T));
        archivo.close();
        return true;
    }

    //Sobrescribe un registro existente
    template <typename T>
    static bool actualizarRegistro(const char* ruta, const T& entidad) {
        int indice = buscarIndice<T>(ruta, entidad.getId());
        if (indice == -1) return false;

        ofstream archivo(ruta, ios::binary | ios::in | ios::out);
        if (!archivo.is_open()) return false;

        archivo.seekp(sizeof(ArchivoHeader) + (indice * sizeof(T)), ios::beg);
        archivo.write(reinterpret_cast<const char*>(&entidad), sizeof(T));
        archivo.close();
        return true;
    }
};

#endif