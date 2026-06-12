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

bool validarFechaFormato(const char* fecha) {
    if (strlen(fecha) != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (fecha[i] < '0' || fecha[i] > '9') return false;
    }
    return true;
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
    *cantidad = 0; 
    char cad[100], sub[100];
    
    //Convertimos subcadena a minusculas para comparar
    for(int j = 0; subcadena[j] != '\0'; j++) sub[j] = tolower(subcadena[j]);
    sub[strlen(subcadena)] = '\0';

    for(int i = 0; i < s->numEquipos; i++){ 
        for(int j = 0; s->equipos[i].nombre[j] != '\0'; j++) cad[j] = tolower(s->equipos[i].nombre[j]);
        cad[strlen(s->equipos[i].nombre)] = '\0';
        if(strstr(cad, sub) != nullptr) *cantidad += 1;
    }

    Equipo** busqueda = new Equipo*[*cantidad]; 
    int pos = 0;
    for(int i = 0; i < s->numEquipos; i++){ 
        for(int j = 0; s->equipos[i].nombre[j] != '\0'; j++) cad[j] = tolower(s->equipos[i].nombre[j]);
        cad[strlen(s->equipos[i].nombre)] = '\0';
        if(strstr(cad, sub) != nullptr){
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
    *cantidad = 0; 
    char cad[100], sub[100];
    for(int j = 0; subcadena[j] != '\0'; j++) sub[j] = tolower(subcadena[j]);
    sub[strlen(subcadena)] = '\0';

    for(int i = 0; i < s->numJugadores; i++){ 
        for(int j = 0; s->jugadores[i].nombre[j] != '\0'; j++) cad[j] = tolower(s->jugadores[i].nombre[j]);
        cad[strlen(s->jugadores[i].nombre)] = '\0';
        if(strstr(cad, sub) != nullptr) *cantidad += 1;
    }
    if(*cantidad == 0) return nullptr; 

    Jugador** busqueda = new Jugador*[*cantidad]; 
    int pos = 0;
    for(int i = 0; i < s->numJugadores; i++){ 
        for(int j = 0; s->jugadores[i].nombre[j] != '\0'; j++) cad[j] = tolower(s->jugadores[i].nombre[j]);
        cad[strlen(s->jugadores[i].nombre)] = '\0';
        if(strstr(cad, sub) != nullptr){
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
    Equipo** tabla = generarTablaPosiciones(s, &cantidad); 

    if (tabla == nullptr || cantidad == 0) {
        cout << "\nNo hay equipos registrados para mostrar la tabla.\n" << endl;
        return;
    }

    cout << "\n╔═════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                       TABLA DE POSICIONES                               ║" << endl;
    cout << "║                       " << left << setw(42) << s->torneo.nombre << "║" << endl;
    cout << "╠════╦═══════════════════════╦═════╦═══╦═══╦═══╦════╦════╦════╗           " << endl;
    cout << "║ #  ║ Equipo                ║ PTS ║ J ║ G ║ E ║ D  ║ GF ║ GC ║           " << endl;
    cout << "╠════╬═══════════════════════╬═════╬═══╬═══╬═══╬════╬════╬════╣           " << endl;

    for (int i = 0; i < cantidad; i++) {
        int jugados = tabla[i]->victorias + tabla[i]->empates + tabla[i]->derrotas;
        cout << "║ " << setw(2) << (i + 1) << " ║ "
             << left << setw(21) << tabla[i]->nombre << right << " ║ "
             << setw(3) << tabla[i]->puntos << " ║ "
             << setw(1) << jugados << " ║ "
             << setw(1) << tabla[i]->victorias << " ║ "
             << setw(1) << tabla[i]->empates << " ║ "
             << setw(2) << tabla[i]->derrotas << " ║ "
             << setw(2) << tabla[i]->puntosAFavor << " ║ "
             << setw(2) << tabla[i]->puntosEnContra << " ║" << endl;
    }
    cout << "╚════╩═══════════════════════╩═════╩═══╩═══╩═══╩════╩════╩════╝\n" << endl;

    delete[] tabla; 
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

//Muestra los datos de un solo jugador y busca el nombre de su equipo
void mostrarJugador(Jugador* jugador, SistemaDeportivo* s) {
    cout << "\n--- DATOS DEL JUGADOR ---" << endl;
    cout << "ID: " << jugador->id << endl;
    cout << "Nombre: " << jugador->nombre << endl;
    cout << "Cedula: " << jugador->cedula << endl;
    cout << "Posicion: " << jugador->posicion << endl;
    cout << "Edad: " << jugador->edad << " años" << endl;
    cout << "Dorsal: " << jugador->numeroDorsal << endl;
    
    Equipo* eq = buscarEquipoPorID(s, jugador->idEquipo);
    if(eq != nullptr) {//Si el equipo existe mostramos su nombre
        cout << "Equipo: " << eq->nombre << " (ID: " << eq->id << ")" << endl;
    } else {//Si no existe marcamos como no encontrado
        cout << "Equipo: [No encontrado]" << endl;
    }
    
    cout << "Fecha de Registro: " << jugador->fechaRegistro << endl;
    cout << "-------------------------\n" << endl;
}

//Muestra una lista de jugadores en formato de tabla
void mostrarListaJugadores(Jugador** jugadores, int cantidad, SistemaDeportivo* s) {
    cout << "\n╔═══════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                      LISTADO DE JUGADORES                             ║" << endl;
    cout << "╠════╦══════════════════╦══════════════╦═══════════════╦═════╦═════════╣" << endl;
    cout << "║ ID ║ Nombre           ║ Equipo       ║ Posición      ║ Edad║ Dorsal  ║" << endl;
    cout << "╠════╬══════════════════╬══════════════╬═══════════════╬═════╬═════════╣" << endl;
    
    for (int i = 0; i < cantidad; i++) {
        Equipo* eq = buscarEquipoPorID(s, jugadores[i]->idEquipo);
        char nombreEquipo[15] = "Desconocido";
        if(eq != nullptr){//Acortamos el nombre del equipo por si es muy largo para la tabla
            strncpy(nombreEquipo, eq->nombre, 14);
            nombreEquipo[14] = '\0';
        }

        cout << "║ " << setw(2) << jugadores[i]->id << " ║ "
             << left << setw(16) << jugadores[i]->nombre << right << " ║ "
             << left << setw(12) << nombreEquipo << right << " ║ "
             << left << setw(13) << jugadores[i]->posicion << right << " ║ "
             << setw(3) << jugadores[i]->edad << " ║ "
             << setw(7) << jugadores[i]->numeroDorsal << " ║" << endl;
    }
    cout << "╚════╩══════════════════╩══════════════╩═══════════════╩═════╩═════════╝" << endl;
    cout << "Total de jugadores: " << cantidad << "\n" << endl;
}

//Menu interactivo para registrar un jugador, permite cancelar en cualquier string
void menuRegistrarJugador(SistemaDeportivo* s) {
    int idEquipo, edad, dorsal;
    char nombre[100], cedula[20], posicion[20];
    
    cout << "\n--- REGISTRAR NUEVO JUGADOR ---" << endl;
    cout << "(NOTA: Puede escribir 'CANCELAR' en los campos de texto para abortar)\n" << endl;
    
    cout << "Ingrese el ID del Equipo al que pertenece: ";
    leerEntero(idEquipo);
    
    Equipo* eq = buscarEquipoPorID(s, idEquipo);
    if(eq == nullptr){
        cout << "ERROR: No existe ningun equipo con ID " << idEquipo << ".\n" << endl;
        return;
    }

    cout << "Equipo seleccionado: " << eq->nombre << "\n" << endl;

    cout << "Ingrese el nombre del jugador: ";
    leerCadena(nombre, 100);
    if(strcmp(nombre, "CANCELAR") == 0) { cout << "Registro cancelado.\n"; return; }

    cout << "Ingrese la cedula: ";
    leerCadena(cedula, 20);
    if(strcmp(cedula, "CANCELAR") == 0) { cout << "Registro cancelado.\n"; return; }

    
    for(int i = 0; i < s->numJugadores; i++){
        if(strcmp(s->jugadores[i].cedula, cedula) == 0){
            cout << "ERROR: La cedula '" << cedula << "' ya esta registrada.\n" << endl;
            return;
        }
        if(s->jugadores[i].idEquipo == idEquipo && s->jugadores[i].numeroDorsal == dorsal){
            cout << "ERROR: El dorsal " << dorsal << " ya esta en uso en el equipo '" << eq->nombre << "'.\n" << endl;
            return;
        }
    }

    cout << "Ingrese la posicion (PORTERO, DEFENSA, MEDIOCAMPISTA, DELANTERO): ";
    leerCadena(posicion, 20);
    if(strcmp(posicion, "CANCELAR") == 0) { cout << "Registro cancelado.\n"; return; }
    
    if(strcmp(posicion, "PORTERO") != 0 && strcmp(posicion, "DEFENSA") != 0 && 
       strcmp(posicion, "MEDIOCAMPISTA") != 0 && strcmp(posicion, "DELANTERO") != 0) {
        cout << "ERROR: Posicion invalida.\n" << endl;
        return;
    }

    cout << "Ingrese la edad (14 - 50): ";
    leerEntero(edad);
    if(edad < 14 || edad > 50){
        cout << "ERROR: La edad debe estar entre 14 y 50 años.\n" << endl;
        return;
    }

    cout << "Ingrese el numero dorsal (1 - 99): ";
    leerEntero(dorsal);
    if(dorsal < 1 || dorsal > 99){
        cout << "ERROR: El dorsal debe estar entre 1 y 99.\n" << endl;
        return;
    }

    // Chequeo de dorsal despues de pedirlo
    for(int i = 0; i < s->numJugadores; i++){
        if(s->jugadores[i].idEquipo == idEquipo && s->jugadores[i].numeroDorsal == dorsal){
            cout << "ERROR: El dorsal " << dorsal << " ya esta en uso en el equipo '" << eq->nombre << "'.\n" << endl;
            return;
        }
    }

    char confirmacion;
    cout << "\n¿Desea guardar este jugador? (S/N): ";
    cin >> confirmacion;
    if (toupper(confirmacion) != 'S') {
        cout << "Registro cancelado por el usuario.\n" << endl;
        return;
    }

    Jugador* nuevo = agregarJugador(s, idEquipo, nombre, cedula, posicion, edad, dorsal);
    if (nuevo != nullptr) {
        cout << "\n¡Jugador registrado exitosamente!" << endl;
        mostrarJugador(nuevo, s);
    }
}

//Menu para buscar jugador por subcadena
void menuBuscarJugador(SistemaDeportivo* s) {
    char busqueda[100];
    cout << "\n--- BUSCAR JUGADOR ---" << endl;
    cout << "Ingrese el nombre o parte del nombre a buscar: ";
    leerCadena(busqueda, 100);

    int cantidad = 0;
    Jugador** resultados = buscarJugadoresPorNombre(s, busqueda, &cantidad);

    if (resultados == nullptr || cantidad == 0) {//Si no hay resultados
        cout << "No se encontraron jugadores que coincidan con '" << busqueda << "'.\n" << endl;
    } else {
        cout << "\nResultados de la busqueda:" << endl;
        mostrarListaJugadores(resultados, cantidad, s);
        delete[] resultados; //Liberamos la bandeja temporal
    }
}

//Menu para actualizar los datos de un jugador
void menuActualizarJugador(SistemaDeportivo* s) {
    int id;
    cout << "\n--- ACTUALIZAR JUGADOR ---" << endl;
    cout << "Ingrese el ID del jugador a actualizar: ";
    leerEntero(id);

    Jugador* actual = buscarJugadorPorID(s, id);
    if (actual == nullptr) {//Validamos que exista
        cout << "ERROR: No existe ningun jugador con ID " << id << ".\n" << endl;
        return;
    }

    mostrarJugador(actual, s);

    Jugador datosNuevos;
    cout << "\nIngrese el nuevo nombre: ";
    leerCadena(datosNuevos.nombre, 100);
    cout << "Ingrese la nueva cedula: ";
    leerCadena(datosNuevos.cedula, 20);
    cout << "Ingrese la nueva posicion (PORTERO, DEFENSA, MEDIOCAMPISTA, DELANTERO): ";
    leerCadena(datosNuevos.posicion, 20);
    cout << "Ingrese la nueva edad: ";
    leerEntero(datosNuevos.edad);
    cout << "Ingrese el nuevo dorsal: ";
    leerEntero(datosNuevos.numeroDorsal);

    char confirmacion;
    cout << "\n¿Desea guardar los cambios? (S/N): ";
    cin >> confirmacion;
    
    if (toupper(confirmacion) == 'S') {
        if (actualizarJugador(s, id, datosNuevos)) {
            cout << "Jugador actualizado exitosamente.\n" << endl;
        }
    } else {
        cout << "Actualizacion cancelada.\n" << endl;
    }
}

//Menu general para listar todos los jugadores
void menuListarJugadores(SistemaDeportivo* s) {
    int cantidad = 0;
    Jugador** todos = listarJugadores(s, &cantidad);
    
    if (todos == nullptr) {
        cout << "\nNo hay jugadores registrados en el sistema.\n" << endl;
    } else {
        cout << "\n--- LISTADO GENERAL DE JUGADORES ---" << endl;
        mostrarListaJugadores(todos, cantidad, s);
        delete[] todos; //Liberamos la bandeja
    }
}

//Menu para eliminar jugador
void menuEliminarJugador(SistemaDeportivo* s) {
    int id;
    cout << "\n--- ELIMINAR JUGADOR ---" << endl;
    cout << "Ingrese el ID del jugador a eliminar: ";
    leerEntero(id);

    Jugador* actual = buscarJugadorPorID(s, id);
    if (actual == nullptr) {
        cout << "ERROR: No existe ningun jugador con ID " << id << ".\n" << endl;
        return;
    }

    mostrarJugador(actual, s);

    char confirmacion;
    cout << "\n¿Esta seguro que desea eliminar este jugador? (S/N): ";
    cin >> confirmacion;

    if (toupper(confirmacion) == 'S') {
        if (eliminarJugador(s, id)) {
            cout << "Jugador eliminado exitosamente del sistema.\n" << endl;
        } else {
            cout << "ERROR: No se pudo eliminar el jugador.\n" << endl;
        }
    } else {
        cout << "Eliminacion cancelada.\n" << endl;
    }
}

//Muestra los detalles de un partido, buscando los nombres de los equipos locales y visitantes
void mostrarPartido(Partido* partido, SistemaDeportivo* s) {
    Equipo* local = buscarEquipoPorID(s, partido->idEquipoLocal);
    Equipo* visitante = buscarEquipoPorID(s, partido->idEquipoVisitante);
    
    char nomLocal[100] = "Desconocido";
    char nomVisitante[100] = "Desconocido";
    
    if (local != nullptr) strcpy(nomLocal, local->nombre);
    if (visitante != nullptr) strcpy(nomVisitante, visitante->nombre);

    cout << "\n╔══════════════════════════════════════════════════╗" << endl;
    cout << "║              DETALLE DE PARTIDO                  ║" << endl;
    cout << "╠══════════════════════════════════════════════════╣" << endl;
    cout << "║ ID Partido  : " << left << setw(35) << partido->id << "║" << endl;
    cout << "║ Estado      : " << left << setw(35) << partido->estado << "║" << endl;
    cout << "║ Fecha       : " << left << setw(35) << partido->fecha << "║" << endl;
    cout << "║                                                  ║" << endl;
    
    // Alineacion corregida matematicamente para 50 espacios
    cout << "║ " << right << setw(18) << nomLocal << "  " 
         << setw(2) << partido->puntosLocal << "-" << left << setw(2) << partido->puntosVisitante 
         << "  " << left << setw(18) << nomVisitante << " ║" << endl;
         
    cout << "║      (Local)                  (Visitante)        ║" << endl;
    cout << "║                                                  ║" << endl;
    cout << "║ Notas: " << left << setw(42) << partido->descripcion << "║" << endl;
    cout << "╚══════════════════════════════════════════════════╝\n" << endl;
}

//Muestra una lista iterando sobre un array de punteros a partidos
void mostrarListaPartidos(Partido** partidos, int cantidad, SistemaDeportivo* s) {
    cout << "\n--- LISTADO DE PARTIDOS ---" << endl;
    for (int i = 0; i < cantidad; i++) {
        mostrarPartido(partidos[i], s);
    }
    cout << "Total de partidos mostrados: " << cantidad << "\n" << endl;
}

//Menu para programar un partido nuevo entre dos equipos
void menuProgramarPartido(SistemaDeportivo* s) {
    int idLocal, idVisitante;
    char fecha[11], descripcion[200];

    cout << "\n--- PROGRAMAR PARTIDO ---" << endl;
    cout << "Ingrese el ID del Equipo Local: ";
    leerEntero(idLocal);
    cout << "Ingrese el ID del Equipo Visitante: ";
    leerEntero(idVisitante);

    if (idLocal == idVisitante) {
        cout << "ERROR: No se puede programar un partido de un equipo contra si mismo.\n" << endl;
        return;
    }

    cout << "Ingrese la fecha (YYYY-MM-DD): ";
    leerCadena(fecha, 11);
    if (!validarFechaFormato(fecha)) {
        cout << "ERROR: Formato de fecha invalido. Debe ser YYYY-MM-DD.\n" << endl;
        return;
    }

    cout << "Ingrese una nota o descripcion (opcional): ";
    leerCadena(descripcion, 200);

    char confirmacion;
    cout << "\n¿Desea guardar y programar este partido? (S/N): ";
    cin >> confirmacion;
    if (toupper(confirmacion) != 'S') {
        cout << "Programacion cancelada por el usuario.\n" << endl;
        return;
    }

    Partido* p = programarPartido(s, idLocal, idVisitante, fecha, descripcion);

    if (p == nullptr) {
        cout << "ERROR: Equipos invalidos o ya tienen un partido PROGRAMADO.\n" << endl;
    } else {
        cout << "\n¡Partido programado con exito!" << endl;
        mostrarPartido(p, s);
    }
}

//Menu para asigarle un resultado a un partido que estaba programado
void menuRegistrarResultado(SistemaDeportivo* s) {
    int idPartido, golesLocal, golesVisitante;
    
    cout << "\n--- REGISTRAR RESULTADO ---" << endl;
    cout << "Ingrese el ID del Partido: ";
    leerEntero(idPartido);

    Partido* actual = buscarPartidoPorID(s, idPartido);
    if (actual == nullptr) {
        cout << "ERROR: No existe el partido con ID " << idPartido << ".\n" << endl;
        return;
    }

    if (strcmp(actual->estado, "PROGRAMADO") != 0) {
        cout << "ERROR: El partido ID " << idPartido << " ya tiene resultado registrado o esta cancelado.\n" << endl;
        return;
    }

    mostrarPartido(actual, s);

    cout << "Ingrese goles del Equipo Local: ";
    leerEntero(golesLocal);
    cout << "Ingrese goles del Equipo Visitante: ";
    leerEntero(golesVisitante);

    if (golesLocal < 0 || golesVisitante < 0) {
        cout << "ERROR: Los puntos/goles deben ser mayores o iguales a 0.\n" << endl;
        return;
    }

    char confirmacion;
    cout << "\n¿Desea registrar este resultado de forma definitiva? (S/N): ";
    cin >> confirmacion;
    if (toupper(confirmacion) != 'S') {
        cout << "Registro de resultado cancelado.\n" << endl;
        return;
    }

    Partido* p = registrarResultado(s, idPartido, golesLocal, golesVisitante);

    if (p != nullptr) {
        cout << "\n¡Resultado registrado y estadisticas actualizadas!" << endl;
        mostrarPartido(p, s);
    }
}

//Menu para buscar un partido especifico por su ID
void menuBuscarPartido(SistemaDeportivo* s) {
    int idPartido;
    cout << "\n--- BUSCAR PARTIDO ---" << endl;
    cout << "Ingrese el ID del Partido: ";
    leerEntero(idPartido);

    Partido* p = buscarPartidoPorID(s, idPartido);
    if (p == nullptr) {
        cout << "No se encontro partido con ID " << idPartido << ".\n" << endl;
    } else {
        mostrarPartido(p, s);
    }
}

//Menu general para mostrar todos los partidos del sistema
void menuListarPartidos(SistemaDeportivo* s) {
    int cantidad = 0;
    Partido** todos = listarPartidos(s, &cantidad);

    if (todos == nullptr) {
        cout << "\nNo hay partidos registrados en el sistema.\n" << endl;
    } else {
        mostrarListaPartidos(todos, cantidad, s);
        delete[] todos; //Liberamos la bandeja temporal
    }
}

//Menu para cancelar un partido y revertir los puntos si es necesario
void menuCancelarPartido(SistemaDeportivo* s) {
    int idPartido;
    cout << "\n--- CANCELAR PARTIDO ---" << endl;
    cout << "Ingrese el ID del Partido a cancelar: ";
    leerEntero(idPartido);

    Partido* p = buscarPartidoPorID(s, idPartido);
    if (p == nullptr) {
        cout << "ERROR: No existe el partido con ID " << idPartido << ".\n" << endl;
        return;
    }

    mostrarPartido(p, s);

    char confirmacion;
    cout << "\nADVERTENCIA: ¿Seguro que desea CANCELAR este partido? Si ya fue jugado, se revertiran los puntos. (S/N): ";
    cin >> confirmacion;

    if (toupper(confirmacion) == 'S') {
        if (cancelarPartido(s, idPartido)) {
            cout << "Partido cancelado y estadisticas revertidas con exito.\n" << endl;
        } else {
            cout << "ERROR: El partido ya estaba cancelado.\n" << endl;
        }
    } else {
        cout << "Operacion abortada.\n" << endl;
    }
}

//Submenu para gestionar la capa de equipos
void subMenuEquipos(SistemaDeportivo* s) {
    int opcion;
    do {
        cout << "\n╔═══════════════════════════════════════════╗" << endl;
        cout << "║          GESTION DE EQUIPOS               ║" << endl;
        cout << "╠═══════════════════════════════════════════╣" << endl;
        cout << "║  1. Registrar equipo                      ║" << endl;
        cout << "║  2. Buscar equipo                         ║" << endl;
        cout << "║  3. Actualizar equipo                     ║" << endl;
        cout << "║  4. Listar equipos                        ║" << endl;
        cout << "║  5. Eliminar equipo                       ║" << endl;
        cout << "║  0. Volver al menu principal              ║" << endl;
        cout << "╚═══════════════════════════════════════════╝" << endl;
        cout << "Seleccione una opcion: ";
        leerEntero(opcion);

        switch (opcion) {
            case 1: menuRegistrarEquipo(s); break;
            case 2: menuBuscarEquipo(s); break;
            case 3: menuActualizarEquipo(s); break;
            case 4: menuListarEquipos(s); break;
            case 5: menuEliminarEquipo(s); break;
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}

//Submenu para gestionar la capa de jugadores
void subMenuJugadores(SistemaDeportivo* s) {
    int opcion;
    do {
        cout << "\n╔═══════════════════════════════════════════╗" << endl;
        cout << "║        GESTION DE JUGADORES               ║" << endl;
        cout << "╠═══════════════════════════════════════════╣" << endl;
        cout << "║  1. Registrar jugador                     ║" << endl;
        cout << "║  2. Buscar jugador                        ║" << endl;
        cout << "║  3. Actualizar jugador                    ║" << endl;
        cout << "║  4. Listar jugadores (todos)              ║" << endl;
        cout << "║  5. Listar jugadores por equipo           ║" << endl;
        cout << "║  6. Eliminar jugador                      ║" << endl;
        cout << "║  0. Volver al menu principal              ║" << endl;
        cout << "╚═══════════════════════════════════════════╝" << endl;
        cout << "Seleccione una opcion: ";
        leerEntero(opcion);

        switch (opcion) {
            case 1: menuRegistrarJugador(s); break;
            case 2: menuBuscarJugador(s); break;
            case 3: menuActualizarJugador(s); break;
            case 4: menuListarJugadores(s); break;
            case 5: {
                int idEq;
                cout << "Ingrese el ID del Equipo: ";
                leerEntero(idEq);
                int cant = 0;
                Jugador** lista = listarJugadoresPorEquipo(s, idEq, &cant);
                if(lista != nullptr) {
                    mostrarListaJugadores(lista, cant, s);
                    delete[] lista;//Liberamos la bandeja temporal
                } else {
                    cout << "No hay jugadores registrados en ese equipo.\n";
                }
                break;
            }
            case 6: menuEliminarJugador(s); break;
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}

void menuBuscarPartidosPorEquipo(SistemaDeportivo* s) {
    int idEq;
    cout << "\n--- BUSCAR PARTIDOS POR EQUIPO ---" << endl;
    cout << "Ingrese el ID del Equipo: ";
    leerEntero(idEq);
    int cant = 0;
    Partido** lista = buscarPartidosPorEquipo(s, idEq, &cant);
    if(lista == nullptr) cout << "No se encontraron partidos para ese equipo.\n" << endl;
    else { mostrarListaPartidos(lista, cant, s); delete[] lista; }
}

void menuListarPartidosProgramados(SistemaDeportivo* s) {
    int cant = 0;
    Partido** lista = listarPartidosPorEstado(s, "PROGRAMADO", &cant);
    if(lista == nullptr) cout << "No hay partidos en estado PROGRAMADO.\n" << endl;
    else { mostrarListaPartidos(lista, cant, s); delete[] lista; }
}

//Submenu para gestionar la capa de partidos
void subMenuPartidos(SistemaDeportivo* s) {
    int opcion;
    do {
        cout << "\n╔═══════════════════════════════════════════╗" << endl;
        cout << "║        GESTION DE PARTIDOS                ║" << endl;
        cout << "╠═══════════════════════════════════════════╣" << endl;
        cout << "║  1. Programar partido                     ║" << endl;
        cout << "║  2. Registrar resultado                   ║" << endl;
        cout << "║  3. Buscar partido (Por ID)               ║" << endl;
        cout << "║  4. Listar TODOS los partidos             ║" << endl;
        cout << "║  5. Buscar partidos por Equipo            ║" << endl;
        cout << "║  6. Listar partidos PROGRAMADOS           ║" << endl;
        cout << "║  7. Cancelar partido                      ║" << endl;
        cout << "║  0. Volver al menu principal              ║" << endl;
        cout << "╚═══════════════════════════════════════════╝" << endl;
        cout << "Seleccione una opcion: ";
        leerEntero(opcion);

        switch (opcion) {
            case 1: menuProgramarPartido(s); break;
            case 2: menuRegistrarResultado(s); break;
            case 3: menuBuscarPartido(s); break;
            case 4: menuListarPartidos(s); break;
            case 5: menuBuscarPartidosPorEquipo(s); break;
            case 6: menuListarPartidosProgramados(s); break;
            case 7: menuCancelarPartido(s); break;
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}



int main() {
    SetConsoleOutputCP(CP_UTF8);

    SistemaDeportivo sistema;
    Torneo torneo = {"Liga Apertura 2026", "Futbol", "GRUPOS", "2026-06-08", "2026-12-15"};
    
    inicializarSistema(&sistema, torneo);

    int opcion;
    do {
        cout << "\n╔═══════════════════════════════════════════╗" << endl;
        cout << "║    SISTEMA DE GESTION DE TORNEOS          ║" << endl;
        cout << "║    Torneo: " << left << setw(30) << sistema.torneo.nombre << " ║" << endl;
        cout << "║    Deporte: " << left << setw(10) << sistema.torneo.deporte << " | Formato: " << left << setw(8) << sistema.torneo.formato << " ║" << endl;
        cout << "╠═══════════════════════════════════════════╣" << endl;
        cout << "║  1. Gestion de Equipos                    ║" << endl;
        cout << "║  2. Gestion de Jugadores                  ║" << endl;
        cout << "║  3. Gestion de Partidos                   ║" << endl;
        cout << "║  4. Tabla de Posiciones                   ║" << endl;
        cout << "║  0. Salir                                 ║" << endl;
        cout << "╚═══════════════════════════════════════════╝" << endl;
        cout << "Seleccione una opcion: ";
        leerEntero(opcion);

        switch (opcion) {
            case 1:
                subMenuEquipos(&sistema);
                break;
            case 2:
                subMenuJugadores(&sistema);
                break;
            case 3:
                subMenuPartidos(&sistema);
                break;
            case 4:
                mostrarTablaPosiciones(&sistema);
                break;
            case 0: {
                char confSalir;
                cout << "\n¿Esta seguro que desea salir del programa? (S/N): ";
                cin >> confSalir;
                if (toupper(confSalir) == 'S') {
                    cout << "\nGuardando y saliendo del sistema. ¡Hasta luego!\n" << endl;
                } else {
                    opcion = -1; //Le cambiamos el valor para que el while no se rompa y el programa siga
                    cout << "Salida cancelada.\n" << endl;
                }
                break;
            }
            default:
                cout << "Opcion invalida. Intente de nuevo.\n" << endl;
        }
    } while (opcion != 0);

    //Limpiamos toda la memoria dinamica antes de cerrar el programa
    liberarSistema(&sistema);

    return 0;
}