#include "Validaciones.hpp"
#include <cstring>

bool Validaciones::validarFechaFormato(const char* fecha) {
    if (strlen(fecha) != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (fecha[i] < '0' || fecha[i] > '9') return false;
    }
    return true;
}