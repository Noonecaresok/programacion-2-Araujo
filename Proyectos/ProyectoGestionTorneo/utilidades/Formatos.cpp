#include "Formatos.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

void Formatos::leerEntero(int &num) {
    bool valido = false;
    do {
        cin >> num;
        if (cin.fail()) {
            cout << "ERROR: Entrada invalida. Ingrese un numero entero: ";
            cin.clear();
            cin.ignore(1000, '\n');
        } else {
            valido = true;
        }
    } while (!valido);
}

void Formatos::leerCadena(char* destino, int maxLongitud) {
    do {
        if (cin.peek() == '\n') cin.ignore();
        cin.getline(destino, maxLongitud);
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
        }
        if (strlen(destino) == 0) {
            cout << "ERROR: El campo no puede estar vacio. Intente de nuevo: ";
        }
    } while (strlen(destino) == 0);
}

void Formatos::pausar() {
    cout << "\nPresione ENTER para continuar...";
    if (cin.peek() == '\n') cin.ignore();
    cin.get();
}

void Formatos::limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}