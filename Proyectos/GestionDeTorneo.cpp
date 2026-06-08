#include <iostream>
#include <iomanip>
#include <cstring> 

using namespace std;

//Structs 

struct Torneo {
    char nombre[100];
    char deporte[50];
    char formato[20];
    char fechaInicio[11];
    char fechaFin[11];
};

struct Equipo {
    int  id;
    char nombre[100];
    char city[100]; // Evitamos conflictos de palabras reservadas usando un nombre limpio
    char entrenador[100];
    int  puntos;
    int  victorias;
    int  empates;
    int  derrotas;
    int  puntosAFavor;
    int  puntosEnContra;
    char fechaRegistro[11];
};

struct Jugador {
    int  id;
    int  idEquipo;
    char nombre[100];
    char cedula[20];
    char posicion[20];
    int  edad;
    int  numeroDorsal;
    char fechaRegistro[11];
};

struct Partido {
    int  id;
    int  idEquipoLocal;
    int  idEquipoVisitante;
    int  puntosLocal;
    int  puntosVisitante;
    char fecha[11];
    char estado[12];
    char descripcion[200];
};

struct SistemaDeportivo {
    Torneo torneo;
    
    Equipo* equipos;
    int      numEquipos;
    int      capacidadEquipos;

    Jugador* jugadores;
    int      numJugadores;
    int      capacidadJugadores;

    Partido* partidos;
    int      numPartidos;
    int      capacidadPartidos;

    int siguienteIdEquipo;
    int siguienteIdJugador;
    int siguienteIdPartido;
};


 
//Funciones de validacion (Por ahora)

void leerEntero(int &num) {
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

void leerCadena(char* destino, int maxLongitud) {
    // Limpia buffers residuales si existen antes de leer texto real
    if (cin.peek() == '\n') cin.ignore();
    
    cin.getline(destino, maxLongitud);
    
    // Si el usuario ingreso mas texto del permitido, limpia el exceso
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
    }
}


//Funciones del proyecto

void inicializarSistema(SistemaDeportivo* s, Torneo torneo){
/*
Con esta funcion inicializamos el sistema, de manera que podamos manipular correctamente todo accediendo solo con s.
sin este struct sistemaDeportivo, seria un dolor de cabeza ya que todas las variables y arrays estarian sueltos
pero teniendo esto con un solo s. podemos acceder a todo su contenido.

Esta funcion se encarga entonces de inicializar el sistema, con todos sus valores predeterminados.
*/


//Atributos de equipos

//El torneo lo enlazamos con el sistema deportivo
s->torneo = torneo;
//Inicializamos los demas atributos de sistema deportivo, con 4 de capacidad y 0 equipos anotados inicialmente
s->capacidadEquipos = 4;
s->numEquipos = 0;
//Ahora creamos un array dinamico de equipos, con la capacidad inicial.
s->equipos = new Equipo[s->capacidadEquipos];

//Atributos de jugadores

s->capacidadJugadores = 4; //Inicializamos en capacidad 4 igual
s->numJugadores = 0; //0 iniciales
s->jugadores = new Jugador[s->capacidadJugadores];

//Atributos de partidos

s->capacidadPartidos = 4;
s->numPartidos = 0;
s->partidos = new Partido[s->capacidadPartidos];

//Ids (Todon inicializan en 1)

s->siguienteIdEquipo = 1;
s->siguienteIdJugador = 1;
s->siguienteIdPartido = 1;

}

void liberarSistema(SistemaDeportivo* s){

    delete[] s->equipos;
    s->equipos = nullptr;

s->capacidadEquipos = 4;
s->numEquipos = 0;

    delete[] s->partidos;
    s->partidos = nullptr;

s->capacidadPartidos = 4;
s->numPartidos = 0;

    delete[] s->jugadores;
    s->jugadores = nullptr;

    s->capacidadJugadores = 4; //Inicializamos en capacidad 4 igual
s->numJugadores = 0; //0 iniciales

    s->siguienteIdEquipo = 1;
s->siguienteIdJugador = 1;
s->siguienteIdPartido = 1;


}


int main() {
    
    
    return 0;
}