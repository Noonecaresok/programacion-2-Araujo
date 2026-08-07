#include "interfaz/Interfaz.hpp"
#include <windows.h> //Libreria para controlar la consola de Windows

int main() {
    //Obliga a la consola a usar codificacion UTF-8 para dibujar bien los bordes
    SetConsoleOutputCP(CP_UTF8);
    
    //Enciende el motor de la Interfaz y deja que ella tome el control.
    Interfaz::iniciar();
    
    return 0;
}