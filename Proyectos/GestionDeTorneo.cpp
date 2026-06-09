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

//Funciones de redimensionamiento

void redimensionarEquipos(SistemaDeportivo* s){
if(!(s->numEquipos == s->capacidadEquipos)){

return;
}

int nuevaCapacidad = s->capacidadEquipos * 2;
Equipo* nuevosEquipos = new Equipo[nuevaCapacidad];

for(int i = 0; i < s->numEquipos; i++){
nuevosEquipos[i] = s->equipos[i];

}

delete[] s->equipos;

s->equipos = nuevosEquipos;
s->capacidadEquipos = nuevaCapacidad;

}

void redimensionarJugadores(SistemaDeportivo* s){

if(!(s->numJugadores == s->capacidadJugadores)){

return;
}

int nuevaCapacidad = s->capacidadJugadores * 2;
Jugador* nuevosJugadores = new Jugador[nuevaCapacidad];

for(int i = 0; i < s->numJugadores; i++){
nuevosJugadores[i] = s->jugadores[i];

}

delete[] s->jugadores;

s->jugadores = nuevosJugadores;
s->capacidadJugadores = nuevaCapacidad;

}

void redimensionarPartidos(SistemaDeportivo* s){

if(!(s->numPartidos == s->capacidadPartidos)){

return;
}

int nuevaCapacidad = s->capacidadPartidos * 2;
Partido* nuevosPartidos = new Partido[nuevaCapacidad];

for(int i = 0; i < s->numPartidos; i++){
nuevosPartidos[i] = s->partidos[i];

}

delete[] s->partidos;

s->partidos = nuevosPartidos;
s->capacidadPartidos = nuevaCapacidad;

}




Equipo* agregarEquipo(SistemaDeportivo* s, const char* nombre, const char* ciudad, const char* entrenador){

    for(int i = 0; i < s->numEquipos; i++){ //Aqui verificamos si el nombre puesto de parametro ya existe, si existe, retornamos nullptr
       if (strcmp(s->equipos[i].nombre, nombre) == 0) {
    return nullptr;
}

    }

if(s->numEquipos == s->capacidadEquipos){
redimensionarEquipos(s);
}



int pos = s->numEquipos; //La siguiente posicion vacia 
s->equipos[pos].id = s->siguienteIdEquipo; //Primero hacemos que el equipo tenga su id autoincremental
s->siguienteIdEquipo++;



strcpy(s->equipos[pos].nombre, nombre);
strcpy(s->equipos[pos].city, ciudad);
strcpy(s->equipos[pos].entrenador, entrenador);


// Inicializamos todas las estadísticas deportivas en 0
    s->equipos[pos].puntos = 0;
    s->equipos[pos].victorias = 0;
    s->equipos[pos].empates = 0;
    s->equipos[pos].derrotas = 0;
    s->equipos[pos].puntosAFavor = 0;
    s->equipos[pos].puntosEnContra = 0;





}


int main() {
    
    
    return 0;
}