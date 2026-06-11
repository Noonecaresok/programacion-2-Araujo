#include <iostream>
#include <iomanip>
#include <cstring> 
#include <windows.h>


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
if(!(s->numEquipos == s->capacidadEquipos)){ //Si el numero de equipos no es igual a la capacidad (Es decir no esta lleno), Return.

return;
}
//Ampliamos por dos la capacidad 
int nuevaCapacidad = s->capacidadEquipos * 2;
Equipo* nuevosEquipos = new Equipo[nuevaCapacidad];//Creamos una copia con mas capacidad

for(int i = 0; i < s->numEquipos; i++){//Migramos todos los equipos al nuevo array con mas capacidad
nuevosEquipos[i] = s->equipos[i];

}

delete[] s->equipos;//Eliminamos el viejo

s->equipos = nuevosEquipos;//Igualamos al nuevo
s->capacidadEquipos = nuevaCapacidad;//Actualizamos la capacidad

}

//Exactamente lo mismo que redimensionarEquipos, pero cambiandolo a jugadores
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

//Exactamente lo mismo que redimensionarEquipos, pero cambiandolo a partidos
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




//En esta funcion de logica, agregamos la funcionalidad de agregar un equipo al sistema
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


//Inicializamos todas las estadisticas deportivas en 0
    s->equipos[pos].puntos = 0;
    s->equipos[pos].victorias = 0;
    s->equipos[pos].empates = 0;
    s->equipos[pos].derrotas = 0;
    s->equipos[pos].puntosAFavor = 0;
    s->equipos[pos].puntosEnContra = 0;

//Ponemos la fecha de registro de este equipo, en una fecha especifica
strcpy(s->equipos[pos].fechaRegistro, "2026-06-08");

//Ahora con todo listo incrementamos el numero de equipos actuales
s->numEquipos++;
return &(s->equipos[pos]); //Retornamos la direccion de memoria del equipo

}

Equipo* buscarEquipoPorID(SistemaDeportivo* s, int id){

for(int i = 0; i < s->numEquipos; i++){

if (s->equipos[i].id == id){ //Si el id del equipo de esta iteracion es igual al buscado, se encontro.
    
return &(s->equipos[i]);
}

}
//Si salio del for es que nunca encontro el id, asi que retorna nullptr.
return nullptr;

}

Equipo** buscarEquiposPorNombre(SistemaDeportivo* s, const char* subcadena, int* cantidad){
*cantidad = 0; //Reiniciamos el valor de cantidad a su predeterminado, 0


    for(int i = 0; i < s->numEquipos; i++){ //En este for mediante la funcion strstr buscamos las coincidencias, primero su cantidad

    if(strstr(s->equipos[i].nombre, subcadena) != nullptr){ 
       *cantidad+=1;
    }

    }

Equipo** busqueda = new Equipo*[*cantidad]; //Gracias a la cantidad podemos crear un array dinamico con la nueva cantidad, en este array almacenaremos las coincidencias

int pos = 0;
for(int i = 0; i < s->numEquipos; i++){ //Ahora buscamos nuevamente las coincidencias y las almacenamos en el array dinamico que creamos
if(strstr(s->equipos[i].nombre, subcadena) != nullptr){
      busqueda[pos] = &(s->equipos[i]);
      pos++;
    }


}

return busqueda;

}

//Esta funcion devuelve una copia de los equipos activos, en forma de array dinamico, este array dinamico contiene la direccion de memoria de cada equipo
Equipo** listarEquipos(SistemaDeportivo* s, int* cantidad){
if(s->numEquipos == 0){ //Si hay 0 equipos retornamos nullptr
    return nullptr;
}

*cantidad = s->numEquipos; //Ahora cantidad sera igual al numero de equipos

Equipo** lista = new Equipo*[*cantidad]; //Creamos una nuevo array dinamico temporal para guardar la lista de equipos actuales

for(int i = 0; i < s->numEquipos; i++){
lista[i] = &(s->equipos[i]); //Y copiamos la direccion en la lista, de cada equipo
}

return lista; //Retornamos la lista
}

Equipo** generarTablaPosiciones(SistemaDeportivo* s, int* cantidad){
    if(s->numEquipos == 0){ 
        return nullptr;
    }

    *cantidad = s->numEquipos; //Cantidad de equipos en el sistema para la presentacion

    Equipo** tabla = new Equipo*[*cantidad]; //Array dinamico de punteros para ordenar los equipos

    for(int i = 0; i < s->numEquipos; i++){
        tabla[i] = &(s->equipos[i]); //Guardamos la direccion de memoria de cada equipo
    }

    Equipo* auxiliar = nullptr;

    for(int a = 0; a < s->numEquipos - 1; a++){
        for(int b = 0; b < s->numEquipos - a - 1; b++){
            
            int difLocal = tabla[b]->puntosAFavor - tabla[b]->puntosEnContra;
            int difVisitante = tabla[b+1]->puntosAFavor - tabla[b+1]->puntosEnContra;

            bool debeIntercambiar = false;

            //Primero verificamos por puntos totales
            if (tabla[b]->puntos < tabla[b + 1]->puntos) {
                debeIntercambiar = true;
            }
            //Si empatan en puntos, verificamos por diferencia de goles
            else if (tabla[b]->puntos == tabla[b + 1]->puntos && difLocal < difVisitante) {
                debeIntercambiar = true;
            }
            //Si tambien empatan en diferencia, verificamos por goles anotados
            else if (tabla[b]->puntos == tabla[b + 1]->puntos && difLocal == difVisitante && tabla[b]->puntosAFavor < tabla[b + 1]->puntosAFavor) {
                debeIntercambiar = true;
            }

            if (debeIntercambiar) { //Si se cumple el orden inverso, hacemos el cambio de posicion
                auxiliar = tabla[b];
                tabla[b] = tabla[b + 1];
                tabla[b + 1] = auxiliar;
            }
        }
    }

    return tabla; 
}

bool actualizarEquipo(SistemaDeportivo* s, int id, Equipo equipoActualizado){
    for(int i = 0; i < s->numEquipos; i++){
        if(s->equipos[i].id == id){ //Si encontramos el id buscado, actualizamos sus datos basicos
            strcpy(s->equipos[i].nombre, equipoActualizado.nombre);
            strcpy(s->equipos[i].city, equipoActualizado.city);
            strcpy(s->equipos[i].entrenador, equipoActualizado.entrenador);
            return true;
        }
    }
    return false; //Si sale del for es porque el id no existe
}

bool eliminarEquipo(SistemaDeportivo* s, int id){
    for(int i = 0; i < s->numPartidos; i++){ // Verificamos si el equipo tiene partidos locales o de visitante asignados
        if(s->partidos[i].idEquipoLocal == id || s->partidos[i].idEquipoVisitante == id){
            return false; //Si tiene partidos no se puede eliminar
        }
    }

    for(int i = 0; i < s->numEquipos; i++){
        if(s->equipos[i].id == id){ //Buscamos el id del equipo que queremos borrar
            for(int j = i; j < s->numEquipos - 1; j++){ //Movemos los demas elementos una posicion a la izquierda para cubrir el espacio
                s->equipos[j] = s->equipos[j + 1];
            }
            s->numEquipos--; //Restamos 1 al contador total de equipos
            return true;
        }
    }
    return false; //No se encontro el id
}






//Agrega un jugador al equipo indicado
Jugador* agregarJugador(SistemaDeportivo* s, int idEquipo, const char* nombre, const char* cedula, const char* posicion, int edad, int numeroDorsal){
    
    //Primero verificamos que el equipo ingresado exista realmente en el sistema
    Equipo* equipoValido = buscarEquipoPorID(s, idEquipo);
    if(equipoValido == nullptr){
        return nullptr; //Si el equipo no existe, abortamos
    }

    //Verificamos que la cedula no este duplicada y el dorsal no se repita en ese mismo equipo
    for(int i = 0; i < s->numJugadores; i++){
        if(strcmp(s->jugadores[i].cedula, cedula) == 0){
            return nullptr; //Cedula repetida en el sistema, retornamos nulo
        }
        if(s->jugadores[i].idEquipo == idEquipo && s->jugadores[i].numeroDorsal == numeroDorsal){
            return nullptr; //Dorsal ya usado en este equipo, retornamos nullptr
        }
    }

    if(s->numJugadores == s->capacidadJugadores){
        redimensionarJugadores(s);
    }

    int pos = s->numJugadores; //La siguiente posicion vacia
    
    s->jugadores[pos].id = s->siguienteIdJugador; //Asignamos ID autoincremental
    s->siguienteIdJugador++;

    //Guardamos todos los datos usando strcpy para el texto y asignamos directamente los numeros
    s->jugadores[pos].idEquipo = idEquipo;
    strcpy(s->jugadores[pos].nombre, nombre);
    strcpy(s->jugadores[pos].cedula, cedula);
    strcpy(s->jugadores[pos].posicion, posicion);
    s->jugadores[pos].edad = edad;
    s->jugadores[pos].numeroDorsal = numeroDorsal;
    
    //Ponemos la fecha fija del torneo igual que en equipos
    strcpy(s->jugadores[pos].fechaRegistro, "2026-06-08"); 

    s->numJugadores++; //Incrementamos el numero total de jugadores

    return &(s->jugadores[pos]); //Retornamos la direccion de memoria del nuevo jugador
}

//Retorna puntero al jugador con ese ID
Jugador* buscarJugadorPorID(SistemaDeportivo* s, int id){
    for(int i = 0; i < s->numJugadores; i++){
        if (s->jugadores[i].id == id){ //Si el id de esta iteracion es igual al buscado, se encontro
            return &(s->jugadores[i]);
        }
    }
    return nullptr; //Si salio del for es que nunca lo encontro
}

//Retorna array temporal de punteros a jugadores los cuales el nombre coincida
Jugador** buscarJugadoresPorNombre(SistemaDeportivo* s, const char* subcadena, int* cantidad){
    *cantidad = 0; //Reiniciamos el valor a 0

    for(int i = 0; i < s->numJugadores; i++){ //Buscamos cuantas coincidencias hay con strstr
        if(strstr(s->jugadores[i].nombre, subcadena) != nullptr){ 
            *cantidad += 1;
        }
    }

    if(*cantidad == 0){
        return nullptr; //Si no hay coincidencias retornamos nullptr
    }

    Jugador** busqueda = new Jugador*[*cantidad]; //Creamos array dinamico con la cantidad exacta

    int pos = 0;
    for(int i = 0; i < s->numJugadores; i++){ //Ahora guardamos las direcciones de esas coincidencias
        if(strstr(s->jugadores[i].nombre, subcadena) != nullptr){
            busqueda[pos] = &(s->jugadores[i]);
            pos++;
        }
    }

    return busqueda;
}

//Retorna array temporal con los jugadores de un equipo en especifico
Jugador** listarJugadoresPorEquipo(SistemaDeportivo* s, int idEquipo, int* cantidad){
    *cantidad = 0;

    for(int i = 0; i < s->numJugadores; i++){ //Contamos cuantos jugadores tienen este idEquipo
        if(s->jugadores[i].idEquipo == idEquipo){
            *cantidad += 1;
        }
    }

    if(*cantidad == 0){
        return nullptr; 
    }

    Jugador** lista = new Jugador*[*cantidad]; 

    int pos = 0;
    for(int i = 0; i < s->numJugadores; i++){ //Guardamos las direcciones de esos jugadores
        if(s->jugadores[i].idEquipo == idEquipo){
            lista[pos] = &(s->jugadores[i]);
            pos++;
        }
    }

    return lista;
}

//Retorna array temporal con punteros a todos los jugadores activos
Jugador** listarJugadores(SistemaDeportivo* s, int* cantidad){
    if(s->numJugadores == 0){ //Si hay 0 jugadores retornamos nullptr
        return nullptr;
    }

    *cantidad = s->numJugadores; 

    Jugador** lista = new Jugador*[*cantidad]; //Lista temporal

    for(int i = 0; i < s->numJugadores; i++){
        lista[i] = &(s->jugadores[i]); //Copiamos la direccion de cada jugador
    }

    return lista;
}

//Actualiza los datos editables de un jugador, menos el idEquipo
bool actualizarJugador(SistemaDeportivo* s, int id, Jugador jugadorActualizado){
    for(int i = 0; i < s->numJugadores; i++){
        if(s->jugadores[i].id == id){ //Si encontramos el id buscado, actualizamos sus datos basicos
            strcpy(s->jugadores[i].nombre, jugadorActualizado.nombre);
            strcpy(s->jugadores[i].cedula, jugadorActualizado.cedula);
            strcpy(s->jugadores[i].posicion, jugadorActualizado.posicion);
            s->jugadores[i].edad = jugadorActualizado.edad;
            s->jugadores[i].numeroDorsal = jugadorActualizado.numeroDorsal;
            
            
            return true;
        }
    }
    return false; //No se encontro el id
}

//Elimina un jugador del sistema
bool eliminarJugador(SistemaDeportivo* s, int id){
    for(int i = 0; i < s->numJugadores; i++){
        if(s->jugadores[i].id == id){ //Buscamos el id del jugador a borrar
            
            for(int j = i; j < s->numJugadores - 1; j++){ //Movemos los demas una posicion a la izquierda para cubrir el hueco
                s->jugadores[j] = s->jugadores[j + 1];
            }
            
            s->numJugadores--; //Restamos 1 al contador
            return true;
        }
    }
    return false; //No se encontro el id
}

//Retorna puntero al partido con ese ID
Partido* buscarPartidoPorID(SistemaDeportivo* s, int id){
    for(int i = 0; i < s->numPartidos; i++){
        if(s->partidos[i].id == id){//Si encontramos el id del partido, retornamos su direccion
            return &(s->partidos[i]);
        }
    }
    return nullptr;//Si no se encuentra el partido retornamos nullptr
}

//Programa un partido entre dos equipos
Partido* programarPartido(SistemaDeportivo* s, int idLocal, int idVisitante, const char* fecha, const char* descripcion){
    if(idLocal == idVisitante){//Validamos que un equipo no juegue contra el mismo
        return nullptr;
    }

    Equipo* local = buscarEquipoPorID(s, idLocal);
    Equipo* visitante = buscarEquipoPorID(s, idVisitante);
    if(local == nullptr || visitante == nullptr){//Verificamos que los dos equipos existan en el sistema
        return nullptr;
    }

    for(int i = 0; i < s->numPartidos; i++){//Verificamos si ya existe un partido programado entre los dos
        if((s->partidos[i].idEquipoLocal == idLocal && s->partidos[i].idEquipoVisitante == idVisitante) || 
           (s->partidos[i].idEquipoLocal == idVisitante && s->partidos[i].idEquipoVisitante == idLocal)){
            if(strcmp(s->partidos[i].estado, "PROGRAMADO") == 0){
                return nullptr;//Si ya tienen un partido pendiente, retornamos nullptr, es decir, abortamos la operacion
            }
        }
    }

    if(s->numPartidos == s->capacidadPartidos){ //La misma logica de redimensionamiento
        redimensionarPartidos(s);
    }

    int pos = s->numPartidos;//Siguiente posicion vacia en el array de partidos
    s->partidos[pos].id = s->siguienteIdPartido;//Asignamos el id autoincremental
    s->siguienteIdPartido++;

    s->partidos[pos].idEquipoLocal = idLocal;
    s->partidos[pos].idEquipoVisitante = idVisitante;
    s->partidos[pos].puntosLocal = 0;//Inicializa en 0 porque esta programado
    s->partidos[pos].puntosVisitante = 0;
    strcpy(s->partidos[pos].fecha, fecha);
    strcpy(s->partidos[pos].estado, "PROGRAMADO");
    strcpy(s->partidos[pos].descripcion, descripcion);

    s->numPartidos++;//Incrementamos el contador de partidos totales
    return &(s->partidos[pos]);//Retornamos el puntero del partido creado
}

//Registra el resultado de un partido PROGRAMADO
Partido* registrarResultado(SistemaDeportivo* s, int idPartido, int puntosLocal, int puntosVisitante){
    Partido* p = buscarPartidoPorID(s, idPartido);
    if(p == nullptr || strcmp(p->estado, "PROGRAMADO") != 0){//Solo registramos si existe y esta programado
        return nullptr;
    }

    Equipo* local = buscarEquipoPorID(s, p->idEquipoLocal);
    Equipo* visitante = buscarEquipoPorID(s, p->idEquipoVisitante);
    if(local == nullptr || visitante == nullptr){//Verificamos que los equipos existan
        return nullptr;
    }

    p->puntosLocal = puntosLocal;
    p->puntosVisitante = puntosVisitante;
    strcpy(p->estado, "JUGADO");//Cambiamos el estado a jugado

    //Actualizamos los puntos a favor y en contra de ambos equipos
    local->puntosAFavor += puntosLocal;
    local->puntosEnContra += puntosVisitante;
    visitante->puntosAFavor += puntosVisitante;
    visitante->puntosEnContra += puntosLocal;

    if(puntosLocal > puntosVisitante){//Si gano el equipo local
        local->puntos += 3;
        local->victorias += 1;
        visitante->derrotas += 1;
    }else if(puntosLocal == puntosVisitante){//Si hubo empate en el partido
        local->puntos += 1;
        visitante->puntos += 1;
        local->empates += 1;
        visitante->empates += 1;
    }else{//Si gano el equipo visitante
        visitante->puntos += 3;
        visitante->victorias += 1;
        local->derrotas += 1;
    }

    return p;//Retornamos el partido actualizado
}

//Retorna array de partidos en los que participo el equipo
Partido** buscarPartidosPorEquipo(SistemaDeportivo* s, int idEquipo, int* cantidad){
    *cantidad = 0;//Reiniciamos el contador a 0

    for(int i = 0; i < s->numPartidos; i++){//Contamos en cuantos partidos participo el equipo
        if(s->partidos[i].idEquipoLocal == idEquipo || s->partidos[i].idEquipoVisitante == idEquipo){
            *cantidad += 1;
        }
    }

    if(*cantidad == 0){
        return nullptr;//Si no jugo ningun partido retornamos nullptr
    }

    Partido** busqueda = new Partido*[*cantidad];//Creamos el array dinamico temporal de punteros

    int pos = 0;
    for(int i = 0; i < s->numPartidos; i++){//Guardamos las direcciones de esos partidos
        if(s->partidos[i].idEquipoLocal == idEquipo || s->partidos[i].idEquipoVisitante == idEquipo){
            busqueda[pos] = &(s->partidos[i]);
            pos++;
        }
    }

    return busqueda;
}

//Retorna array de partidos con ese estado
Partido** listarPartidosPorEstado(SistemaDeportivo* s, const char* estado, int* cantidad){
    *cantidad = 0;//Inicializamos la cantidad en 0

    for(int i = 0; i < s->numPartidos; i++){//Contamos los partidos que coincidan con el estado
        if(strcmp(s->partidos[i].estado, estado) == 0){
            *cantidad += 1;
        }
    }

    if(*cantidad == 0){
        return nullptr;
    }

    Partido** lista = new Partido*[*cantidad];//Bandeja temporal con el tamano exacto

    int pos = 0;
    for(int i = 0; i < s->numPartidos; i++){//Guardamos las direcciones de los partidos filtrados
        if(strcmp(s->partidos[i].estado, estado) == 0){
            lista[pos] = &(s->partidos[i]);
            pos++;
        }
    }

    return lista;
}

//Retorna array con todos los partidos
Partido** listarPartidos(SistemaDeportivo* s, int* cantidad){
    if(s->numPartidos == 0){//Si no hay partidos registrados retornamos nullptr
        return nullptr;
    }

    *cantidad = s->numPartidos;
    Partido** lista = new Partido*[*cantidad];//Array dinamico de punteros para la lista completa

    for(int i = 0; i < s->numPartidos; i++){
        lista[i] = &(s->partidos[i]);//Copiamos la direccion de memoria de cada partido
    }

    return lista;
}

//Cancela un partido y revierte las estadisticas si ya fue jugado
bool cancelarPartido(SistemaDeportivo* s, int idPartido){
    Partido* p = buscarPartidoPorID(s, idPartido);
    if(p == nullptr || strcmp(p->estado, "CANCELADO") == 0){//Si no existe o ya esta cancelado, salimos
        return false;
    }

    if(strcmp(p->estado, "JUGADO") == 0){//Si el partido ya se jugo, hay que revertir las estadisticas
        Equipo* local = buscarEquipoPorID(s, p->idEquipoLocal);
        Equipo* visitante = buscarEquipoPorID(s, p->idEquipoVisitante);

        if(local != nullptr && visitante != nullptr){
            //Restamos los puntos anotados a favor y en contra de ambos
            local->puntosAFavor -= p->puntosLocal;
            local->puntosEnContra -= p->puntosVisitante;
            visitante->puntosAFavor -= p->puntosVisitante;
            visitante->puntosEnContra -= p->puntosLocal;

            if(p->puntosLocal > p->puntosVisitante){//Revertimos la victoria local
                local->puntos -= 3;
                local->victorias -= 1;
                visitante->derrotas -= 1;
            }else if(p->puntosLocal == p->puntosVisitante){//Revertimos el empate
                local->puntos -= 1;
                visitante->puntos -= 1;
                local->empates -= 1;
                visitante->empates -= 1;
            }else{//Revertimos la victoria visitante
                visitante->puntos -= 3;
                visitante->victorias -= 1;
                local->derrotas -= 1;
            }
        }
    }

    strcpy(p->estado, "CANCELADO");//Cambiamos finalmente el estado a cancelado
    return true;
}


//Capa de presentacion de equipos

//Muestra los datos de un equipo
void mostrarEquipo(Equipo* equipo) {
    cout << "\n--- DATOS DEL EQUIPO ---" << endl;
    cout << "ID: " << equipo->id << endl;
    cout << "Nombre: " << equipo->nombre << endl;
    cout << "Ciudad: " << equipo->city << endl;
    cout << "Entrenador: " << equipo->entrenador << endl;
    cout << "Fecha de Registro: " << equipo->fechaRegistro << endl;
    cout << "------------------------\n" << endl;
}

//Muestra una lista de equipos en formato tabla 
void mostrarListaEquipos(Equipo** equipos, int cantidad) {
    cout << "\n╔════╦══════════════════════════════╦══════════════════════════════╗" << endl;
    cout << "║ ID ║ NOMBRE DEL EQUIPO            ║ CIUDAD                       ║" << endl;
    cout << "╠════╬══════════════════════════════╬══════════════════════════════╣" << endl;
    for (int i = 0; i < cantidad; i++) {
        cout << "║ " << setw(2) << equipos[i]->id << " ║ " 
             << left << setw(28) << equipos[i]->nombre << " ║ " 
             << left << setw(28) << equipos[i]->city << " ║" << right << endl;
    }
    cout << "╚════╩══════════════════════════════╩══════════════════════════════╝" << endl;
    cout << "Total de equipos: " << cantidad << "\n" << endl;
}

//Llama a la logica de ordenamiento y dibuja la tabla del torneo
void mostrarTablaPosiciones(SistemaDeportivo* s) {
    int cantidad = 0;
    Equipo** tabla = generarTablaPosiciones(s, &cantidad); //Llamamos a la logica

    if (tabla == nullptr || cantidad == 0) {
        cout << "\nNo hay equipos registrados para mostrar la tabla.\n" << endl;
        return;
    }

    cout << "\n╔══════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                      TABLA DE POSICIONES                             ║" << endl;
    cout << "║                      " << left << setw(40) << s->torneo.nombre << "          ║" << endl;
    cout << "╠════╦══════════════════╦═════╦═══╦═══╦═══╦════╦════╦════╗" << endl;
    cout << "║ #  ║ Equipo           ║ PTS ║ J ║ G ║ E ║ D  ║ GF ║ GC ║" << endl;
    cout << "╠════╬══════════════════╬═════╬═══╬═══╬═══╬════╬════╬════╣" << endl;

    for (int i = 0; i < cantidad; i++) {
        int jugados = tabla[i]->victorias + tabla[i]->empates + tabla[i]->derrotas;
        cout << "║ " << setw(2) << (i + 1) << " ║ "
             << left << setw(16) << tabla[i]->nombre << right << " ║ "
             << setw(3) << tabla[i]->puntos << " ║ "
             << setw(1) << jugados << " ║ "
             << setw(1) << tabla[i]->victorias << " ║ "
             << setw(1) << tabla[i]->empates << " ║ "
             << setw(2) << tabla[i]->derrotas << " ║ "
             << setw(2) << tabla[i]->puntosAFavor << " ║ "
             << setw(2) << tabla[i]->puntosEnContra << " ║" << endl;
    }
    cout << "╚════╩══════════════════╩═════╩═══╩═══╩═══╩════╩════╩════╝\n" << endl;

    delete[] tabla; //Liberamos la bandeja temporal para evitar fugas de memoria
}

//Menu para registrar un nuevo equipo interactuando con el usuario
void menuRegistrarEquipo(SistemaDeportivo* s) {
    char nombre[100], ciudad[100], entrenador[100];
    
    cout << "\n--- REGISTRAR NUEVO EQUIPO ---" << endl;
    cout << "Ingrese el nombre del equipo: ";
    leerCadena(nombre, 100);
    cout << "Ingrese la ciudad: ";
    leerCadena(ciudad, 100);
    cout << "Ingrese el nombre del entrenador: ";
    leerCadena(entrenador, 100);

    char confirmacion;
    cout << "\n¿Desea guardar este equipo? (S/N): ";
    cin >> confirmacion;
    if (toupper(confirmacion) != 'S') {
        cout << "Registro cancelado por el usuario.\n" << endl;
        return;
    }

    Equipo* nuevo = agregarEquipo(s, nombre, ciudad, entrenador);
    
    if (nuevo == nullptr) {
        cout << "ERROR: Ya existe un equipo con el nombre '" << nombre << "'.\n" << endl;
    } else {
        cout << "\n¡Equipo registrado exitosamente!" << endl;
        mostrarEquipo(nuevo);
    }
}

//Menu para buscar un equipo por subcadena
void menuBuscarEquipo(SistemaDeportivo* s) {
    char busqueda[100];
    cout << "\n--- BUSCAR EQUIPO ---" << endl;
    cout << "Ingrese el nombre o parte del nombre a buscar: ";
    leerCadena(busqueda, 100);

    int cantidad = 0;
    Equipo** resultados = buscarEquiposPorNombre(s, busqueda, &cantidad);

    if (resultados == nullptr || cantidad == 0) {
        cout << "No se encontraron equipos que coincidan con '" << busqueda << "'.\n" << endl;
    } else {
        cout << "\nResultados de la busqueda:" << endl;
        mostrarListaEquipos(resultados, cantidad);
        delete[] resultados; // Liberamos la bandeja temporal
    }
}

//Menu para actualizar datos basicos
void menuActualizarEquipo(SistemaDeportivo* s) {
    int id;
    cout << "\n--- ACTUALIZAR EQUIPO ---" << endl;
    cout << "Ingrese el ID del equipo a actualizar: ";
    leerEntero(id);

    Equipo* actual = buscarEquipoPorID(s, id);
    if (actual == nullptr) {
        cout << "ERROR: No existe ningun equipo con ID " << id << ".\n" << endl;
        return;
    }

    mostrarEquipo(actual);

    Equipo datosNuevos;
    cout << "\nIngrese el nuevo nombre: ";
    leerCadena(datosNuevos.nombre, 100);
    cout << "Ingrese la nueva ciudad: ";
    leerCadena(datosNuevos.city, 100);
    cout << "Ingrese el nuevo entrenador: ";
    leerCadena(datosNuevos.entrenador, 100);

    char confirmacion;
    cout << "\n¿Desea guardar los cambios? (S/N): ";
    cin >> confirmacion;
    
    if (toupper(confirmacion) == 'S') {
        if (actualizarEquipo(s, id, datosNuevos)) {
            cout << "Equipo actualizado exitosamente.\n" << endl;
        }
    } else {
        cout << "Actualizacion cancelada.\n" << endl;
    }
}

//Menu para eliminar un equipo
void menuEliminarEquipo(SistemaDeportivo* s) {
    int id;
    cout << "\n--- ELIMINAR EQUIPO ---" << endl;
    cout << "Ingrese el ID del equipo a eliminar: ";
    leerEntero(id);

    Equipo* actual = buscarEquipoPorID(s, id);
    if (actual == nullptr) {
        cout << "ERROR: No existe ningun equipo con ID " << id << ".\n" << endl;
        return;
    }

    mostrarEquipo(actual);

    char confirmacion;
    cout << "\nADVERTENCIA: ¿Esta seguro que desea eliminar este equipo? (S/N): ";
    cin >> confirmacion;

    if (toupper(confirmacion) == 'S') {
        if (eliminarEquipo(s, id)) {
            cout << "Equipo eliminado exitosamente del sistema.\n" << endl;
        } else {
            cout << "ERROR: No se puede eliminar el equipo porque tiene partidos asociados.\n" << endl;
        }
    } else {
        cout << "Eliminacion cancelada.\n" << endl;
    }
}

//Menu general para listar todos los equipos
void menuListarEquipos(SistemaDeportivo* s) {
    int cantidad = 0;
    Equipo** todos = listarEquipos(s, &cantidad);
    
    if (todos == nullptr) {
        cout << "\nNo hay equipos registrados en el sistema.\n" << endl;
    } else {
        cout << "\n--- LISTADO GENERAL DE EQUIPOS ---" << endl;
        mostrarListaEquipos(todos, cantidad);
        delete[] todos; //Liberamos la bandeja
    }
}


int main() {
    SetConsoleOutputCP(CP_UTF8);


    SistemaDeportivo sistema;
    Torneo torneo = {"Liga Apertura 2026", "Futbol", "GRUPOS", "2026-06-08", "2026-12-15"};
    
    cout << "Inicializando sistema..." << endl;
    inicializarSistema(&sistema, torneo);

    
    cout << "\nRegistrando equipos de prueba..." << endl;
    agregarEquipo(&sistema, "Deportivo Maracaibo", "Maracaibo", "Juan Perez");
    agregarEquipo(&sistema, "Caracas FC", "Caracas", "Luis Gomez");
    agregarEquipo(&sistema, "Lara SC", "Barquisimeto", "Pedro Ramirez");

    
    cout << "\nProbando mostrarListaEquipos..." << endl;
    int cantidadEquipos = 0;
    Equipo** listaPrueba = listarEquipos(&sistema, &cantidadEquipos);
    if(listaPrueba != nullptr){
        mostrarListaEquipos(listaPrueba, cantidadEquipos);
        delete[] listaPrueba; // Limpiamos la bandeja
    }

    
    cout << "\nSimulando partidos y probando generarTablaPosiciones..." << endl;
    //Deportivo Maracaibo (6 pts, +3 dif)
    sistema.equipos[0].puntos = 6; 
    sistema.equipos[0].victorias = 2;
    sistema.equipos[0].puntosAFavor = 5; 
    sistema.equipos[0].puntosEnContra = 2;

    //Caracas FC (3 pts, 0 dif)
    sistema.equipos[1].puntos = 3; 
    sistema.equipos[1].victorias = 1;
    sistema.equipos[1].derrotas = 1;
    sistema.equipos[1].puntosAFavor = 3; 
    sistema.equipos[1].puntosEnContra = 3;

    //Lara SC (1 pt, -3 dif)
    sistema.equipos[2].puntos = 1; 
    sistema.equipos[2].empates = 1;
    sistema.equipos[2].derrotas = 1;
    sistema.equipos[2].puntosAFavor = 2; 
    sistema.equipos[2].puntosEnContra = 5;

    //Dibujamos la tabla ordenada
    mostrarTablaPosiciones(&sistema);

  
    cout << "\nLiberando memoria del sistema..." << endl;
    liberarSistema(&sistema);
    cout << "Memoria liberada." << endl;

    return 0;
}