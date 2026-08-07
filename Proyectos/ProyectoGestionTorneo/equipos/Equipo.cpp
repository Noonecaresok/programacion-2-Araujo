#include "Equipo.hpp"
#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

//CONSTRUCTORES
//Constructor por defecto: Inicializa todo en cero o vacío
Equipo::Equipo() {
    id = 0;
    strcpy(nombre, "");
    strcpy(ciudad, "");
    strcpy(entrenador, "");
    puntos = 0;
    victorias = 0;
    empates = 0;
    derrotas = 0;
    puntosAFavor = 0;
    puntosEnContra = 0;
    cantidadPartidos = 0;
    for(int i = 0; i < 50; i++) partidosIDs[i] = 0;
    eliminado = false;
    fechaCreacion = time(0);
    fechaUltimaModificacion = time(0);
}

//Constructor parametrizado: Ideal para cuando el usuario registra uno nuevo
Equipo::Equipo(const char* _nombre, const char* _city, const char* _entrenador) {
    id = 0; //Se asignará luego desde el GestorArchivos
    strcpy(nombre, _nombre);
    strcpy(ciudad, _city);
    strcpy(entrenador, _entrenador);
    puntos = 0;
    victorias = 0;
    empates = 0;
    derrotas = 0;
    puntosAFavor = 0;
    puntosEnContra = 0;
    cantidadPartidos = 0;
    for(int i = 0; i < 50; i++) partidosIDs[i] = 0;
    eliminado = false;
    fechaCreacion = time(0);
    fechaUltimaModificacion = time(0);
}

//GETTERS (Simplemente retornan el valor)
int Equipo::getId() const { return id; }
const char* Equipo::getNombre() const { return nombre; }
const char* Equipo::getCity() const { return ciudad; }
const char* Equipo::getEntrenador() const { return entrenador; }

int Equipo::getPuntos() const { return puntos; }
int Equipo::getVictorias() const { return victorias; }
int Equipo::getEmpates() const { return empates; }
int Equipo::getDerrotas() const { return derrotas; }
int Equipo::getPuntosAFavor() const { return puntosAFavor; }
int Equipo::getPuntosEnContra() const { return puntosEnContra; }

int Equipo::getCantidadPartidos() const { return cantidadPartidos; }
bool Equipo::isEliminado() const { return eliminado; }
time_t Equipo::getFechaCreacion() const { return fechaCreacion; }
time_t Equipo::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

//SETTERS (Con validación y actualización automática de fecha)
void Equipo::setId(int _id) { 
    this->id = _id; 
}

bool Equipo::setNombre(const char* _nombre) {
    if (strlen(_nombre) == 0 || strlen(_nombre) >= 100) return false; // Validación
    strcpy(this->nombre, _nombre);
    actualizarFechaModificacion();
    return true;
}

bool Equipo::setCity(const char* _city) {
    if (strlen(_city) == 0 || strlen(_city) >= 100) return false;
    strcpy(this->ciudad, _city);
    actualizarFechaModificacion();
    return true;
}

bool Equipo::setEntrenador(const char* _entrenador) {
    if (strlen(_entrenador) == 0 || strlen(_entrenador) >= 100) return false;
    strcpy(this->entrenador, _entrenador);
    actualizarFechaModificacion();
    return true;
}

void Equipo::setEliminado(bool estado) {
    this->eliminado = estado;
    actualizarFechaModificacion();
}

void Equipo::actualizarFechaModificacion() {
    this->fechaUltimaModificacion = time(0);
}

//MÉTODOS DE NEGOCIO 
bool Equipo::registrarPartido(int idPartido) {
    if (cantidadPartidos >= 50) return false; //Tope del arreglo alcanzado
    partidosIDs[cantidadPartidos] = idPartido;
    cantidadPartidos++;
    actualizarFechaModificacion();
    return true;
}

void Equipo::registrarResultado(int golesFavor, int golesContra) {
    this->puntosAFavor += golesFavor;
    this->puntosEnContra += golesContra;

    if (golesFavor > golesContra) {
        this->puntos += 3;
        this->victorias++;
    } else if (golesFavor == golesContra) {
        this->puntos += 1;
        this->empates++;
    } else {
        this->derrotas++;
    }
    actualizarFechaModificacion();
}

//MÉTODOS DE PRESENTACIÓN
void Equipo::mostrarBasico() const {
    cout << "ID: " << setw(2) << id << " | " << left << setw(28) << nombre 
         << " | Ciudad: " << ciudad << endl;
}

void Equipo::mostrarDetalle() const {
    cout << "\n--- DATOS DEL EQUIPO ---" << endl;
    cout << "ID: " << id << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Ciudad: " << ciudad << endl;
    cout << "Entrenador: " << entrenador << endl;
    cout << "Estadisticas:" << endl;
    cout << " Puntos: " << puntos << " | V: " << victorias << " | E: " << empates << " | D: " << derrotas << endl;
    cout << " GF: " << puntosAFavor << " | GC: " << puntosEnContra << " | Partidos: " << cantidadPartidos << endl;
    cout << "------------------------\n" << endl;
}

//MÉTODO ESTÁTICO
size_t Equipo::obtenerTamano() {
    return sizeof(Equipo);
}