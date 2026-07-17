#include <iostream>
#include <iomanip>
#include <cstring> 
#include <windows.h>
#include <fstream>
#include <ctime>

using namespace std;

// Structs estaticos

struct ArchivoHeader {
    int cantidadRegistros;  
    int proximoID;          
    int registrosActivos;   
    int version;            
};

struct Torneo {
    char nombre[100];
    char deporte[50];
    char formato[20];
    char fechaInicio[11];
    char fechaFin[11];
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct Equipo {
    int  id;
    char nombre[100];
    char city[100];
    char entrenador[100];

    int  puntos;
    int  victorias;
    int  empates;
    int  derrotas;
    int  puntosAFavor;
    int  puntosEnContra;

    int  partidosIDs[50]; 
    int  cantidadPartidos;

    bool   eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct Jugador {
    int  id;
    int  idEquipo;
    char nombre[100];
    char cedula[20];
    char posicion[20];
    int  edad;
    int  numeroDorsal;

    int  golesAnotados;
    int  tarjetasAmarillas;
    int  tarjetasRojas;

    bool   eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct Gol {
    int  idJugador;       
    int  minuto;          
    char equipo[12];      
};

struct Partido {
    int  id;
    int  idEquipoLocal;
    int  idEquipoVisitante;
    char fecha[11];
    char estado[12];       
    char descripcion[200];

    int  golesLocal;
    int  golesVisitante;

    Gol  goles[22];        
    int  numGoles;

    bool   eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};


// Validaciones

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


// Motor de archivos binarios

bool inicializarArchivo(const char* nombreArchivo) {
    ifstream validador(nombreArchivo, ios::binary);
    if (validador.good()) {
        validador.close();
        return true; 
    }
    ofstream creador(nombreArchivo, ios::binary);
    if (creador.fail()) return false;

    ArchivoHeader header = {0, 1, 0, 1}; 
    creador.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    creador.close();
    return true;
}

bool inicializarSistemaArchivos() {
    bool eq = inicializarArchivo("equipos.bin");
    bool ju = inicializarArchivo("jugadores.bin");
    bool pa = inicializarArchivo("partidos.bin");
    
    ifstream validadorT("torneo.bin", ios::binary);
    if (!validadorT.good()) {
        ofstream creadorT("torneo.bin", ios::binary);
        Torneo t = {"Liga Apertura 2026", "Futbol", "GRUPOS", "2026-06-08", "2026-12-15", time(0), time(0)};
        creadorT.write(reinterpret_cast<const char*>(&t), sizeof(Torneo));
        creadorT.close();
    } else {
        validadorT.close();
    }
    return (eq && ju && pa);
}

ArchivoHeader leerHeader(const char* nombreArchivo) {
    ArchivoHeader header = {0, 1, 0, 1}; 
    ifstream archivo(nombreArchivo, ios::binary);
    if (archivo.is_open()) {
        archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
        archivo.close();
    }
    return header;
}

bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header) {
    ofstream archivo(nombreArchivo, ios::binary | ios::in | ios::out); 
    if (!archivo.is_open()) return false;
    archivo.seekp(0, ios::beg); 
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    return true;
}


// Logica de Equipos (Archivos)

int buscarIndiceEquipoPorID(int id) {
    ArchivoHeader h = leerHeader("equipos.bin");
    Equipo temp;
    ifstream archivo("equipos.bin", ios::binary);
    if (!archivo.is_open()) return -1;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Equipo));
        if (temp.id == id && !temp.eliminado) {
            archivo.close();
            return i; 
        }
    }
    archivo.close();
    return -1;
}

bool guardarEquipo(Equipo& equipo) {
    ArchivoHeader h = leerHeader("equipos.bin");
    equipo.id = h.proximoID;
    equipo.eliminado = false;
    equipo.fechaCreacion = time(0);
    equipo.fechaUltimaModificacion = time(0);
    equipo.cantidadPartidos = 0;
    for(int i=0; i<50; i++) equipo.partidosIDs[i] = 0;

    ofstream archivo("equipos.bin", ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;

    archivo.seekp(sizeof(ArchivoHeader) + (h.cantidadRegistros * sizeof(Equipo)), ios::beg);
    archivo.write(reinterpret_cast<const char*>(&equipo), sizeof(Equipo));
    archivo.close();

    h.cantidadRegistros++;
    h.registrosActivos++;
    h.proximoID++;
    return actualizarHeader("equipos.bin", h);
}

bool obtenerEquipoPorID(int id, Equipo& resultado) {
    int indice = buscarIndiceEquipoPorID(id);
    if (indice == -1) return false;

    ifstream archivo("equipos.bin", ios::binary);
    if (!archivo.is_open()) return false;

    archivo.seekg(sizeof(ArchivoHeader) + (indice * sizeof(Equipo)), ios::beg);
    archivo.read(reinterpret_cast<char*>(&resultado), sizeof(Equipo));
    archivo.close();
    return true;
}

bool actualizarEquipoBin(Equipo& equipo) {
    int indice = buscarIndiceEquipoPorID(equipo.id);
    if (indice == -1) return false;

    equipo.fechaUltimaModificacion = time(0);

    ofstream archivo("equipos.bin", ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;

    archivo.seekp(sizeof(ArchivoHeader) + (indice * sizeof(Equipo)), ios::beg);
    archivo.write(reinterpret_cast<const char*>(&equipo), sizeof(Equipo));
    archivo.close();
    return true;
}

bool eliminarEquipoLogico(int id) {
    Equipo equipo;
    if (!obtenerEquipoPorID(id, equipo)) return false;
    
    if (equipo.cantidadPartidos > 0) return false; 

    equipo.eliminado = true;
    equipo.fechaUltimaModificacion = time(0);

    if (actualizarEquipoBin(equipo)) {
        ArchivoHeader h = leerHeader("equipos.bin");
        h.registrosActivos--;
        actualizarHeader("equipos.bin", h);
        return true;
    }
    return false;
}

int buscarEquiposPorNombreBin(const char* subcadena, Equipo resultados[], int maxResultados) {
    ArchivoHeader h = leerHeader("equipos.bin");
    Equipo temp;
    int encontrados = 0;
    char cad[100], sub[100];
    
    for(int j = 0; subcadena[j] != '\0'; j++) sub[j] = tolower(subcadena[j]);
    sub[strlen(subcadena)] = '\0';

    ifstream archivo("equipos.bin", ios::binary);
    if (!archivo.is_open()) return 0;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros && encontrados < maxResultados; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Equipo));
        if (!temp.eliminado) {
            for(int j = 0; temp.nombre[j] != '\0'; j++) cad[j] = tolower(temp.nombre[j]);
            cad[strlen(temp.nombre)] = '\0';
            
            if (strstr(cad, sub) != nullptr) {
                resultados[encontrados] = temp;
                encontrados++;
            }
        }
    }
    archivo.close();
    return encontrados;
}

int listarTodosLosEquipos(Equipo resultados[], int maxResultados) {
    ArchivoHeader h = leerHeader("equipos.bin");
    Equipo temp;
    int encontrados = 0;

    ifstream archivo("equipos.bin", ios::binary);
    if (!archivo.is_open()) return 0;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros && encontrados < maxResultados; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Equipo));
        if (!temp.eliminado) {
            resultados[encontrados] = temp;
            encontrados++;
        }
    }
    archivo.close();
    return encontrados;
}


// Logica de Jugadores (Archivos)

int buscarIndiceJugadorPorID(int id) {
    ArchivoHeader h = leerHeader("jugadores.bin");
    Jugador temp;
    ifstream archivo("jugadores.bin", ios::binary);
    if (!archivo.is_open()) return -1;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Jugador));
        if (temp.id == id && !temp.eliminado) {
            archivo.close();
            return i; 
        }
    }
    archivo.close();
    return -1;
}

int validarUnicidadJugador(const char* cedula, int idEquipo, int dorsal) {
    ArchivoHeader h = leerHeader("jugadores.bin");
    Jugador temp;
    ifstream archivo("jugadores.bin", ios::binary);
    if (!archivo.is_open()) return 0; 

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Jugador));
        if (!temp.eliminado) {
            if (strcmp(temp.cedula, cedula) == 0) return 1; 
            if (temp.idEquipo == idEquipo && temp.numeroDorsal == dorsal) return 2; 
        }
    }
    archivo.close();
    return 0; 
}

bool guardarJugador(Jugador& jugador) {
    ArchivoHeader h = leerHeader("jugadores.bin");
    
    jugador.id = h.proximoID;
    jugador.eliminado = false;
    jugador.golesAnotados = 0;
    jugador.tarjetasAmarillas = 0;
    jugador.tarjetasRojas = 0;
    jugador.fechaCreacion = time(0);
    jugador.fechaUltimaModificacion = time(0);

    ofstream archivo("jugadores.bin", ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;

    archivo.seekp(sizeof(ArchivoHeader) + (h.cantidadRegistros * sizeof(Jugador)), ios::beg);
    archivo.write(reinterpret_cast<const char*>(&jugador), sizeof(Jugador));
    archivo.close();

    h.cantidadRegistros++;
    h.registrosActivos++;
    h.proximoID++;
    return actualizarHeader("jugadores.bin", h);
}

bool obtenerJugadorPorID(int id, Jugador& resultado) {
    int indice = buscarIndiceJugadorPorID(id);
    if (indice == -1) return false;

    ifstream archivo("jugadores.bin", ios::binary);
    if (!archivo.is_open()) return false;

    archivo.seekg(sizeof(ArchivoHeader) + (indice * sizeof(Jugador)), ios::beg);
    archivo.read(reinterpret_cast<char*>(&resultado), sizeof(Jugador));
    archivo.close();
    return true;
}

bool actualizarJugadorBin(Jugador& jugador) {
    int indice = buscarIndiceJugadorPorID(jugador.id);
    if (indice == -1) return false;

    jugador.fechaUltimaModificacion = time(0);

    ofstream archivo("jugadores.bin", ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;

    archivo.seekp(sizeof(ArchivoHeader) + (indice * sizeof(Jugador)), ios::beg);
    archivo.write(reinterpret_cast<const char*>(&jugador), sizeof(Jugador));
    archivo.close();
    return true;
}

bool eliminarJugadorLogico(int id) {
    Jugador jugador;
    if (!obtenerJugadorPorID(id, jugador)) return false;

    jugador.eliminado = true;
    jugador.fechaUltimaModificacion = time(0);

    if (actualizarJugadorBin(jugador)) {
        ArchivoHeader h = leerHeader("jugadores.bin");
        h.registrosActivos--;
        actualizarHeader("jugadores.bin", h);
        return true;
    }
    return false;
}

int buscarJugadoresPorNombreBin(const char* subcadena, Jugador resultados[], int maxResultados) {
    ArchivoHeader h = leerHeader("jugadores.bin");
    Jugador temp;
    int encontrados = 0;
    char cad[100], sub[100];
    
    for(int j = 0; subcadena[j] != '\0'; j++) sub[j] = tolower(subcadena[j]);
    sub[strlen(subcadena)] = '\0';

    ifstream archivo("jugadores.bin", ios::binary);
    if (!archivo.is_open()) return 0;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros && encontrados < maxResultados; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Jugador));
        if (!temp.eliminado) {
            for(int j = 0; temp.nombre[j] != '\0'; j++) cad[j] = tolower(temp.nombre[j]);
            cad[strlen(temp.nombre)] = '\0';
            
            if (strstr(cad, sub) != nullptr) {
                resultados[encontrados] = temp;
                encontrados++;
            }
        }
    }
    archivo.close();
    return encontrados;
}

int listarJugadoresPorEquipoBin(int idEquipo, Jugador resultados[], int maxResultados) {
    ArchivoHeader h = leerHeader("jugadores.bin");
    Jugador temp;
    int encontrados = 0;

    ifstream archivo("jugadores.bin", ios::binary);
    if (!archivo.is_open()) return 0;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros && encontrados < maxResultados; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Jugador));
        if (!temp.eliminado && temp.idEquipo == idEquipo) {
            resultados[encontrados] = temp;
            encontrados++;
        }
    }
    archivo.close();
    return encontrados;
}

int listarTodosLosJugadores(Jugador resultados[], int maxResultados) {
    ArchivoHeader h = leerHeader("jugadores.bin");
    Jugador temp;
    int encontrados = 0;

    ifstream archivo("jugadores.bin", ios::binary);
    if (!archivo.is_open()) return 0;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros && encontrados < maxResultados; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Jugador));
        if (!temp.eliminado) {
            resultados[encontrados] = temp;
            encontrados++;
        }
    }
    archivo.close();
    return encontrados;
}


// Logica de Partidos (Archivos)

int buscarIndicePartidoPorID(int id) {
    ArchivoHeader h = leerHeader("partidos.bin");
    Partido temp;
    ifstream archivo("partidos.bin", ios::binary);
    if (!archivo.is_open()) return -1;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Partido));
        if (temp.id == id && !temp.eliminado) {
            archivo.close();
            return i; 
        }
    }
    archivo.close();
    return -1;
}

bool guardarPartido(Partido& partido) {
    ArchivoHeader h = leerHeader("partidos.bin");
    
    partido.id = h.proximoID;
    partido.eliminado = false;
    partido.golesLocal = 0;
    partido.golesVisitante = 0;
    partido.numGoles = 0;
    strcpy(partido.estado, "PROGRAMADO");
    partido.fechaCreacion = time(0);
    partido.fechaUltimaModificacion = time(0);
    
    for(int i=0; i<22; i++) {
        partido.goles[i].idJugador = 0;
        partido.goles[i].minuto = 0;
        strcpy(partido.goles[i].equipo, "");
    }

    ofstream archivo("partidos.bin", ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;

    archivo.seekp(sizeof(ArchivoHeader) + (h.cantidadRegistros * sizeof(Partido)), ios::beg);
    archivo.write(reinterpret_cast<const char*>(&partido), sizeof(Partido));
    archivo.close();

    h.cantidadRegistros++;
    h.registrosActivos++;
    h.proximoID++;
    return actualizarHeader("partidos.bin", h);
}

bool obtenerPartidoPorID(int id, Partido& resultado) {
    int indice = buscarIndicePartidoPorID(id);
    if (indice == -1) return false;

    ifstream archivo("partidos.bin", ios::binary);
    if (!archivo.is_open()) return false;

    archivo.seekg(sizeof(ArchivoHeader) + (indice * sizeof(Partido)), ios::beg);
    archivo.read(reinterpret_cast<char*>(&resultado), sizeof(Partido));
    archivo.close();
    return true;
}

// Nueva funcion clave para el Commit 4 (Actualizar partido en disco)
bool actualizarPartidoBin(Partido& partido) {
    int indice = buscarIndicePartidoPorID(partido.id);
    if (indice == -1) return false;

    partido.fechaUltimaModificacion = time(0);

    ofstream archivo("partidos.bin", ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;

    archivo.seekp(sizeof(ArchivoHeader) + (indice * sizeof(Partido)), ios::beg);
    archivo.write(reinterpret_cast<const char*>(&partido), sizeof(Partido));
    archivo.close();
    return true;
}

int listarTodosLosPartidos(Partido resultados[], int maxResultados) {
    ArchivoHeader h = leerHeader("partidos.bin");
    Partido temp;
    int encontrados = 0;

    ifstream archivo("partidos.bin", ios::binary);
    if (!archivo.is_open()) return 0;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros && encontrados < maxResultados; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Partido));
        if (!temp.eliminado) {
            resultados[encontrados] = temp;
            encontrados++;
        }
    }
    archivo.close();
    return encontrados;
}

int listarPartidosPorEstadoBin(const char* estado, Partido resultados[], int maxResultados) {
    ArchivoHeader h = leerHeader("partidos.bin");
    Partido temp;
    int encontrados = 0;

    ifstream archivo("partidos.bin", ios::binary);
    if (!archivo.is_open()) return 0;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros && encontrados < maxResultados; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Partido));
        if (!temp.eliminado && strcmp(temp.estado, estado) == 0) {
            resultados[encontrados] = temp;
            encontrados++;
        }
    }
    archivo.close();
    return encontrados;
}

int listarPartidosPorEquipoBin(int idEquipo, Partido resultados[], int maxResultados) {
    ArchivoHeader h = leerHeader("partidos.bin");
    Partido temp;
    int encontrados = 0;

    ifstream archivo("partidos.bin", ios::binary);
    if (!archivo.is_open()) return 0;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < h.cantidadRegistros && encontrados < maxResultados; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Partido));
        if (!temp.eliminado && (temp.idEquipoLocal == idEquipo || temp.idEquipoVisitante == idEquipo)) {
            resultados[encontrados] = temp;
            encontrados++;
        }
    }
    archivo.close();
    return encontrados;
}


// Capa de presentacion (Equipos y Jugadores)

void mostrarEquipo(Equipo& equipo) {
    cout << "\n--- DATOS DEL EQUIPO ---" << endl;
    cout << "ID: " << equipo.id << endl;
    cout << "Nombre: " << equipo.nombre << endl;
    cout << "Ciudad: " << equipo.city << endl;
    cout << "Entrenador: " << equipo.entrenador << endl;
    cout << "------------------------\n" << endl;
}

void mostrarListaEquipos(Equipo equipos[], int cantidad) {
    cout << "\n╔════╦══════════════════════════════╦══════════════════════════════╗" << endl;
    cout << "║ ID ║ NOMBRE DEL EQUIPO            ║ CIUDAD                       ║" << endl;
    cout << "╠════╬══════════════════════════════╬══════════════════════════════╣" << endl;
    for (int i = 0; i < cantidad; i++) {
        cout << "║ " << setw(2) << equipos[i].id << " ║ " 
             << left << setw(28) << equipos[i].nombre << " ║ " 
             << left << setw(28) << equipos[i].city << " ║" << right << endl;
    }
    cout << "╚════╩══════════════════════════════╩══════════════════════════════╝" << endl;
    cout << "Total de equipos: " << cantidad << "\n" << endl;
}

void menuRegistrarEquipo() {
    Equipo nuevo;
    nuevo.puntos = 0; nuevo.victorias = 0; nuevo.empates = 0;
    nuevo.derrotas = 0; nuevo.puntosAFavor = 0; nuevo.puntosEnContra = 0;

    cout << "\n--- REGISTRAR NUEVO EQUIPO ---" << endl;
    cout << "Ingrese el nombre del equipo: ";
    leerCadena(nuevo.nombre, 100);
    
    Equipo temporales[100];
    int encontrados = buscarEquiposPorNombreBin(nuevo.nombre, temporales, 100);
    if(encontrados > 0 && strcmp(temporales[0].nombre, nuevo.nombre) == 0){
        cout << "ERROR: Ya existe un equipo con ese nombre.\n";
        return;
    }

    cout << "Ingrese la ciudad: ";
    leerCadena(nuevo.city, 100);
    cout << "Ingrese el nombre del entrenador: ";
    leerCadena(nuevo.entrenador, 100);

    char confirmacion;
    cout << "\n¿Desea guardar este equipo en el disco duro? (S/N): ";
    cin >> confirmacion;
    if (toupper(confirmacion) != 'S') {
        cout << "Registro cancelado.\n" << endl;
        return;
    }

    if (guardarEquipo(nuevo)) {
        cout << "\n¡Equipo guardado exitosamente en equipos.bin!" << endl;
        mostrarEquipo(nuevo);
    } else {
        cout << "ERROR CRITICO: No se pudo escribir en el archivo.\n" << endl;
    }
}

void menuBuscarEquipo() {
    char busqueda[100];
    cout << "\n--- BUSCAR EQUIPO ---" << endl;
    cout << "Ingrese el nombre a buscar: ";
    leerCadena(busqueda, 100);

    Equipo resultados[100]; 
    int cantidad = buscarEquiposPorNombreBin(busqueda, resultados, 100);

    if (cantidad == 0) cout << "No se encontraron equipos.\n" << endl;
    else mostrarListaEquipos(resultados, cantidad);
}

void menuActualizarEquipo() {
    int id;
    cout << "\n--- ACTUALIZAR EQUIPO ---" << endl;
    cout << "Ingrese el ID del equipo: ";
    leerEntero(id);

    Equipo actual;
    if (!obtenerEquipoPorID(id, actual)) {
        cout << "ERROR: No existe equipo activo con ID " << id << ".\n" << endl;
        return;
    }
    mostrarEquipo(actual);

    cout << "\nIngrese nuevo nombre: ";
    leerCadena(actual.nombre, 100);
    cout << "Ingrese nueva ciudad: ";
    leerCadena(actual.city, 100);
    cout << "Ingrese nuevo entrenador: ";
    leerCadena(actual.entrenador, 100);

    char confirmacion;
    cout << "\n¿Guardar cambios? (S/N): ";
    cin >> confirmacion;
    if (toupper(confirmacion) == 'S') {
        if (actualizarEquipoBin(actual)) cout << "Actualizado correctamente.\n";
        else cout << "Error al escribir en archivo.\n";
    }
}

void menuEliminarEquipo() {
    int id;
    cout << "\n--- ELIMINAR EQUIPO ---" << endl;
    cout << "Ingrese el ID del equipo: ";
    leerEntero(id);

    Equipo actual;
    if (!obtenerEquipoPorID(id, actual)) {
        cout << "ERROR: Equipo no encontrado.\n" << endl;
        return;
    }
    mostrarEquipo(actual);

    char confirmacion;
    cout << "\nADVERTENCIA: ¿Eliminar este equipo lógicamente? (S/N): ";
    cin >> confirmacion;
    if (toupper(confirmacion) == 'S') {
        if (eliminarEquipoLogico(id)) cout << "Equipo eliminado en disco.\n";
        else cout << "ERROR: No se pudo eliminar (puede tener partidos asociados).\n";
    }
}

void menuListarEquipos() {
    Equipo buffer[100]; 
    int cantidad = listarTodosLosEquipos(buffer, 100);
    
    if (cantidad == 0) cout << "\nNo hay equipos registrados.\n" << endl;
    else {
        cout << "\n--- LISTADO GENERAL DE EQUIPOS ---" << endl;
        mostrarListaEquipos(buffer, cantidad);
    }
}

void subMenuEquipos() {
    int opcion;
    do {
        cout << "\n╔═══════════════════════════════════════════╗" << endl;
        cout << "║          GESTION DE EQUIPOS               ║" << endl;
        cout << "╠═══════════════════════════════════════════╣" << endl;
        cout << "║  1. Registrar equipo                      ║" << endl;
        cout << "║  2. Buscar equipo                         ║" << endl;
        cout << "║  3. Actualizar equipo                     ║" << endl;
        cout << "║  4. Listar equipos                        ║" << endl;
        cout << "║  5. Eliminar equipo (Borrado Logico)      ║" << endl;
        cout << "║  0. Volver al menu principal              ║" << endl;
        cout << "╚═══════════════════════════════════════════╝" << endl;
        cout << "Seleccione una opcion: ";
        leerEntero(opcion);

        switch (opcion) {
            case 1: menuRegistrarEquipo(); break;
            case 2: menuBuscarEquipo(); break;
            case 3: menuActualizarEquipo(); break;
            case 4: menuListarEquipos(); break;
            case 5: menuEliminarEquipo(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}

void mostrarJugador(Jugador& jugador) {
    cout << "\n--- DATOS DEL JUGADOR ---" << endl;
    cout << "ID: " << jugador.id << endl;
    cout << "Nombre: " << jugador.nombre << endl;
    cout << "Cedula: " << jugador.cedula << endl;
    cout << "Posicion: " << jugador.posicion << endl;
    cout << "Edad: " << jugador.edad << " anos" << endl;
    cout << "Dorsal: " << jugador.numeroDorsal << endl;
    
    Equipo eq;
    if(obtenerEquipoPorID(jugador.idEquipo, eq)) {
        cout << "Equipo: " << eq.nombre << " (ID: " << eq.id << ")" << endl;
    } else {
        cout << "Equipo: [Referencia Rota - No encontrado]" << endl;
    }
    cout << "Goles Anotados: " << jugador.golesAnotados << endl;
    cout << "-------------------------\n" << endl;
}

void mostrarListaJugadores(Jugador jugadores[], int cantidad) {
    cout << "\n╔════╦══════════════════╦══════════════╦═══════════════╦═════╦═════════╗" << endl;
    cout << "║ ID ║ Nombre           ║ Equipo       ║ Posición      ║ Edad║ Dorsal  ║" << endl;
    cout << "╠════╬══════════════════╬══════════════╬═══════════════╬═════╬═════════╣" << endl;
    
    for (int i = 0; i < cantidad; i++) {
        Equipo eq;
        char nombreEquipo[15] = "Desconocido";
        if(obtenerEquipoPorID(jugadores[i].idEquipo, eq)){
            strncpy(nombreEquipo, eq.nombre, 14);
            nombreEquipo[14] = '\0';
        }

        cout << "║ " << setw(2) << jugadores[i].id << " ║ "
             << left << setw(16) << jugadores[i].nombre << right << " ║ "
             << left << setw(12) << nombreEquipo << right << " ║ "
             << left << setw(13) << jugadores[i].posicion << right << " ║ "
             << setw(3) << jugadores[i].edad << " ║ "
             << setw(7) << jugadores[i].numeroDorsal << " ║" << endl;
    }
    cout << "╚════╩══════════════════╩══════════════╩═══════════════╩═════╩═════════╝" << endl;
    cout << "Total de jugadores: " << cantidad << "\n" << endl;
}

void menuRegistrarJugador() {
    Jugador nuevo;
    char confirmacion;
    
    cout << "\n--- REGISTRAR NUEVO JUGADOR ---" << endl;
    cout << "Ingrese el ID del Equipo al que pertenece: ";
    leerEntero(nuevo.idEquipo);
    
    Equipo eq;
    if(!obtenerEquipoPorID(nuevo.idEquipo, eq)){
        cout << "ERROR: No existe equipo activo con ID " << nuevo.idEquipo << ".\n" << endl;
        return;
    }
    cout << "Equipo seleccionado: " << eq.nombre << "\n" << endl;

    cout << "Ingrese el nombre del jugador: ";
    leerCadena(nuevo.nombre, 100);
    cout << "Ingrese la cedula: ";
    leerCadena(nuevo.cedula, 20);
    cout << "Ingrese el numero dorsal (1 - 99): ";
    leerEntero(nuevo.numeroDorsal);

    int validacion = validarUnicidadJugador(nuevo.cedula, nuevo.idEquipo, nuevo.numeroDorsal);
    if (validacion == 1) {
        cout << "ERROR: La cedula " << nuevo.cedula << " ya esta en uso.\n"; return;
    } else if (validacion == 2) {
        cout << "ERROR: El dorsal " << nuevo.numeroDorsal << " ya lo usa otro jugador en " << eq.nombre << ".\n"; return;
    }

    cout << "Ingrese la posicion (PORTERO, DEFENSA, MEDIOCAMPISTA, DELANTERO): ";
    leerCadena(nuevo.posicion, 20);
    if(strcmp(nuevo.posicion, "PORTERO") != 0 && strcmp(nuevo.posicion, "DEFENSA") != 0 && 
       strcmp(nuevo.posicion, "MEDIOCAMPISTA") != 0 && strcmp(nuevo.posicion, "DELANTERO") != 0) {
        cout << "ERROR: Posicion invalida.\n" << endl; return;
    }

    cout << "Ingrese la edad (14 - 50): ";
    leerEntero(nuevo.edad);
    if(nuevo.edad < 14 || nuevo.edad > 50){
        cout << "ERROR: Edad fuera de rango.\n" << endl; return;
    }

    cout << "\n¿Desea guardar este jugador en disco? (S/N): ";
    cin >> confirmacion;
    if (toupper(confirmacion) != 'S') return;

    if (guardarJugador(nuevo)) {
        cout << "\n¡Jugador guardado exitosamente!" << endl;
        mostrarJugador(nuevo);
    }
}

void menuBuscarJugador() {
    char busqueda[100];
    cout << "\n--- BUSCAR JUGADOR ---" << endl;
    cout << "Ingrese el nombre o parte del nombre a buscar: ";
    leerCadena(busqueda, 100);

    Jugador resultados[100];
    int cantidad = buscarJugadoresPorNombreBin(busqueda, resultados, 100);

    if (cantidad == 0) cout << "No se encontraron jugadores.\n" << endl;
    else mostrarListaJugadores(resultados, cantidad);
}

void menuActualizarJugador() {
    int id;
    cout << "\n--- ACTUALIZAR JUGADOR ---" << endl;
    cout << "Ingrese el ID del jugador: ";
    leerEntero(id);

    Jugador actual;
    if (!obtenerJugadorPorID(id, actual)) {
        cout << "ERROR: Jugador no encontrado.\n" << endl; return;
    }
    mostrarJugador(actual);

    cout << "\nIngrese nuevo nombre: ";
    leerCadena(actual.nombre, 100);
    cout << "Ingrese nueva cedula: ";
    leerCadena(actual.cedula, 20);
    cout << "Ingrese nueva posicion: ";
    leerCadena(actual.posicion, 20);
    cout << "Ingrese nueva edad: ";
    leerEntero(actual.edad);
    cout << "Ingrese nuevo dorsal: ";
    leerEntero(actual.numeroDorsal);

    char confirmacion;
    cout << "\n¿Guardar cambios? (S/N): ";
    cin >> confirmacion;
    
    if (toupper(confirmacion) == 'S') {
        if (actualizarJugadorBin(actual)) cout << "Jugador actualizado.\n";
        else cout << "Error al escribir.\n";
    }
}

void menuListarJugadores() {
    Jugador buffer[100];
    int cantidad = listarTodosLosJugadores(buffer, 100);
    
    if (cantidad == 0) cout << "\nNo hay jugadores registrados.\n" << endl;
    else {
        cout << "\n--- LISTADO GENERAL DE JUGADORES ---" << endl;
        mostrarListaJugadores(buffer, cantidad);
    }
}

void menuEliminarJugador() {
    int id;
    cout << "\n--- ELIMINAR JUGADOR ---" << endl;
    cout << "Ingrese el ID del jugador: ";
    leerEntero(id);

    Jugador actual;
    if (!obtenerJugadorPorID(id, actual)) {
        cout << "ERROR: Jugador no encontrado.\n" << endl; return;
    }
    mostrarJugador(actual);

    char confirmacion;
    cout << "\n¿Esta seguro que desea eliminar este jugador lógicamente? (S/N): ";
    cin >> confirmacion;

    if (toupper(confirmacion) == 'S') {
        if (eliminarJugadorLogico(id)) cout << "Jugador eliminado.\n" << endl;
        else cout << "ERROR: No se pudo eliminar.\n" << endl;
    }
}

void subMenuJugadores() {
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
        cout << "║  6. Eliminar jugador (Borrado Lógico)     ║" << endl;
        cout << "║  0. Volver al menu principal              ║" << endl;
        cout << "╚═══════════════════════════════════════════╝" << endl;
        cout << "Seleccione una opcion: ";
        leerEntero(opcion);

        switch (opcion) {
            case 1: menuRegistrarJugador(); break;
            case 2: menuBuscarJugador(); break;
            case 3: menuActualizarJugador(); break;
            case 4: menuListarJugadores(); break;
            case 5: {
                int idEq;
                cout << "Ingrese el ID del Equipo: ";
                leerEntero(idEq);
                Jugador lista[100];
                int cant = listarJugadoresPorEquipoBin(idEq, lista, 100);
                if(cant > 0) mostrarListaJugadores(lista, cant);
                else cout << "No hay jugadores en ese equipo.\n";
                break;
            }
            case 6: menuEliminarJugador(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}


// Capa de presentacion de Partidos (Operaciones compuestas)

void mostrarPartido(Partido& partido) {
    Equipo local, visitante;
    char nomLocal[100] = "Desconocido";
    char nomVisitante[100] = "Desconocido";
    
    if (obtenerEquipoPorID(partido.idEquipoLocal, local)) strcpy(nomLocal, local.nombre);
    if (obtenerEquipoPorID(partido.idEquipoVisitante, visitante)) strcpy(nomVisitante, visitante.nombre);

    cout << "\n╔══════════════════════════════════════════════════╗" << endl;
    cout << "║              DETALLE DE PARTIDO                  ║" << endl;
    cout << "╠══════════════════════════════════════════════════╣" << endl;
    cout << "║ ID Partido  : " << left << setw(35) << partido.id << "║" << endl;
    cout << "║ Estado      : " << left << setw(35) << partido.estado << "║" << endl;
    cout << "║ Fecha       : " << left << setw(35) << partido.fecha << "║" << endl;
    cout << "║                                                  ║" << endl;
    cout << "║ " << right << setw(18) << nomLocal << "  " 
         << setw(2) << partido.golesLocal << "-" << left << setw(2) << partido.golesVisitante 
         << "  " << left << setw(18) << nomVisitante << " ║" << endl;
    cout << "║      (Local)                  (Visitante)        ║" << endl;
    
    if(partido.numGoles > 0) {
        cout << "╠══════════════════════════════════════════════════╣" << endl;
        cout << "║ GOLES REGISTRADOS:                               ║" << endl;
        for(int i=0; i < partido.numGoles; i++) {
            Jugador j;
            char nombreJugador[40] = "Desconocido";
            if(partido.goles[i].idJugador != 0 && obtenerJugadorPorID(partido.goles[i].idJugador, j)) {
                strncpy(nombreJugador, j.nombre, 39);
            }
            cout << "║ [" << left << setw(9) << partido.goles[i].equipo << "] Min." << setw(3) << partido.goles[i].minuto 
                 << " - " << left << setw(22) << nombreJugador << " ║" << endl;
        }
    }
    
    cout << "╠══════════════════════════════════════════════════╣" << endl;
    cout << "║ Notas: " << left << setw(42) << partido.descripcion << "║" << endl;
    cout << "╚══════════════════════════════════════════════════╝\n" << endl;
}

void mostrarListaPartidos(Partido partidos[], int cantidad) {
    cout << "\n--- LISTADO DE PARTIDOS ---" << endl;
    for (int i = 0; i < cantidad; i++) {
        mostrarPartido(partidos[i]);
    }
    cout << "Total de partidos mostrados: " << cantidad << "\n" << endl;
}

void menuProgramarPartido() {
    Partido nuevo;
    cout << "\n--- PROGRAMAR PARTIDO ---" << endl;
    cout << "Ingrese el ID del Equipo Local: ";
    leerEntero(nuevo.idEquipoLocal);
    cout << "Ingrese el ID del Equipo Visitante: ";
    leerEntero(nuevo.idEquipoVisitante);

    if (nuevo.idEquipoLocal == nuevo.idEquipoVisitante) {
        cout << "ERROR: Un equipo no puede jugar contra si mismo.\n" << endl;
        return;
    }

    Equipo local, visitante;
    if (!obtenerEquipoPorID(nuevo.idEquipoLocal, local) || !obtenerEquipoPorID(nuevo.idEquipoVisitante, visitante)) {
        cout << "ERROR: Uno o ambos equipos no existen o estan eliminados.\n" << endl;
        return;
    }

    Partido tempBuffer[100];
    int cantidad = listarPartidosPorEstadoBin("PROGRAMADO", tempBuffer, 100);
    for(int i = 0; i < cantidad; i++) {
        if ((tempBuffer[i].idEquipoLocal == nuevo.idEquipoLocal && tempBuffer[i].idEquipoVisitante == nuevo.idEquipoVisitante) ||
            (tempBuffer[i].idEquipoLocal == nuevo.idEquipoVisitante && tempBuffer[i].idEquipoVisitante == nuevo.idEquipoLocal)) {
            cout << "ERROR: Estos equipos ya tienen un partido PROGRAMADO pendiente.\n" << endl;
            return;
        }
    }

    cout << "Ingrese la fecha (YYYY-MM-DD): ";
    leerCadena(nuevo.fecha, 11);
    if (!validarFechaFormato(nuevo.fecha)) {
        cout << "ERROR: Formato invalido.\n" << endl; return;
    }

    cout << "Ingrese una nota o descripcion (opcional): ";
    leerCadena(nuevo.descripcion, 200);

    char confirmacion;
    cout << "\n¿Guardar programacion en disco? (S/N): ";
    cin >> confirmacion;
    
    if (toupper(confirmacion) == 'S') {
        if (guardarPartido(nuevo)) {
            cout << "\n¡Partido programado con exito!" << endl;
            mostrarPartido(nuevo);
        } else {
            cout << "Error critico al escribir partido.\n" << endl;
        }
    }
}

// Operacion Compuesta 1: Registrar Resultado
void menuRegistrarResultado() {
    int idPartido;
    cout << "\n--- REGISTRAR RESULTADO ---" << endl;
    cout << "Ingrese el ID del Partido: ";
    leerEntero(idPartido);

    Partido p;
    if (!obtenerPartidoPorID(idPartido, p)) {
        cout << "ERROR: No existe el partido.\n" << endl; return;
    }
    if (strcmp(p.estado, "PROGRAMADO") != 0) {
        cout << "ERROR: El partido ya fue jugado o cancelado.\n" << endl; return;
    }

    Equipo local, visitante;
    if (!obtenerEquipoPorID(p.idEquipoLocal, local) || !obtenerEquipoPorID(p.idEquipoVisitante, visitante)) {
        cout << "ERROR: Los equipos asociados al partido ya no existen.\n" << endl; return;
    }

    mostrarPartido(p);

    cout << "\nGoles del equipo Local (" << local.nombre << "): ";
    leerEntero(p.golesLocal);
    cout << "Goles del equipo Visitante (" << visitante.nombre << "): ";
    leerEntero(p.golesVisitante);

    if (p.golesLocal < 0 || p.golesVisitante < 0) {
        cout << "ERROR: Los goles no pueden ser negativos.\n" << endl; return;
    }
    
    int totalGoles = p.golesLocal + p.golesVisitante;
    if (totalGoles > 22) {
        cout << "ERROR: El sistema soporta un limite maximo de 22 goles por partido.\n" << endl; return;
    }

    // Array temporal para almacenar los goles y luego guardarlos de golpe
    Gol golesTemp[22];
    int contadorGoles = 0;

    if (p.golesLocal > 0) {
        cout << "\n--- DETALLE DE GOLES LOCALES (" << local.nombre << ") ---" << endl;
        for (int i = 0; i < p.golesLocal; i++) {
            int min, idJug;
            cout << "Gol Local #" << i+1 << " - Minuto (1-120): ";
            leerEntero(min);
            cout << "Gol Local #" << i+1 << " - ID del Jugador (0 = Autogol/Desconocido): ";
            leerEntero(idJug);

            if (idJug != 0) {
                Jugador j;
                if (!obtenerJugadorPorID(idJug, j) || j.idEquipo != local.id) {
                    cout << "ADVERTENCIA: Jugador invalido o no pertenece a " << local.nombre << ". Se marcara como Desconocido (0).\n";
                    idJug = 0;
                }
            }
            golesTemp[contadorGoles].idJugador = idJug;
            golesTemp[contadorGoles].minuto = min;
            strcpy(golesTemp[contadorGoles].equipo, "LOCAL");
            contadorGoles++;
        }
    }

    if (p.golesVisitante > 0) {
        cout << "\n--- DETALLE DE GOLES VISITANTES (" << visitante.nombre << ") ---" << endl;
        for (int i = 0; i < p.golesVisitante; i++) {
            int min, idJug;
            cout << "Gol Visitante #" << i+1 << " - Minuto (1-120): ";
            leerEntero(min);
            cout << "Gol Visitante #" << i+1 << " - ID del Jugador (0 = Autogol/Desconocido): ";
            leerEntero(idJug);

            if (idJug != 0) {
                Jugador j;
                if (!obtenerJugadorPorID(idJug, j) || j.idEquipo != visitante.id) {
                    cout << "ADVERTENCIA: Jugador invalido o no pertenece a " << visitante.nombre << ". Se marcara como Desconocido (0).\n";
                    idJug = 0;
                }
            }
            golesTemp[contadorGoles].idJugador = idJug;
            golesTemp[contadorGoles].minuto = min;
            strcpy(golesTemp[contadorGoles].equipo, "VISITANTE");
            contadorGoles++;
        }
    }

    char confirmacion;
    cout << "\n¿Procesar y guardar resultado definitivo en todos los archivos? (S/N): ";
    cin >> confirmacion;
    
    if (toupper(confirmacion) == 'S') {
        // 1. Guardar goles en el partido
        p.numGoles = contadorGoles;
        for(int i=0; i<contadorGoles; i++) {
            p.goles[i] = golesTemp[i];
        }
        strcpy(p.estado, "JUGADO");

        // 2. Actualizar estadisticas de equipos
        local.puntosAFavor += p.golesLocal;
        local.puntosEnContra += p.golesVisitante;
        visitante.puntosAFavor += p.golesVisitante;
        visitante.puntosEnContra += p.golesLocal;

        if (p.golesLocal > p.golesVisitante) {
            local.puntos += 3; local.victorias++; visitante.derrotas++;
        } else if (p.golesLocal == p.golesVisitante) {
            local.puntos += 1; visitante.puntos += 1; local.empates++; visitante.empates++;
        } else {
            visitante.puntos += 3; visitante.victorias++; local.derrotas++;
        }
        
        // Agregar ID del partido al historial del equipo
        if(local.cantidadPartidos < 50) local.partidosIDs[local.cantidadPartidos++] = p.id;
        if(visitante.cantidadPartidos < 50) visitante.partidosIDs[visitante.cantidadPartidos++] = p.id;

        // 3. Escribir Equipos y Partido en disco
        actualizarEquipoBin(local);
        actualizarEquipoBin(visitante);
        actualizarPartidoBin(p);

        // 4. Actualizar estadisticas de jugadores (abriendo disco jugador por jugador)
        for(int i=0; i<contadorGoles; i++) {
            if(golesTemp[i].idJugador != 0) {
                Jugador goleador;
                if(obtenerJugadorPorID(golesTemp[i].idJugador, goleador)) {
                    goleador.golesAnotados++;
                    actualizarJugadorBin(goleador);
                }
            }
        }

        cout << "\nOperacion completada: Archivos de Partidos, Equipos y Jugadores sincronizados.\n";
        mostrarPartido(p);
    }
}

// Operacion Compuesta 2: Cancelar Partido
void menuCancelarPartido() {
    int idPartido;
    cout << "\n--- CANCELAR PARTIDO ---" << endl;
    cout << "Ingrese el ID del Partido: ";
    leerEntero(idPartido);

    Partido p;
    if (!obtenerPartidoPorID(idPartido, p)) {
        cout << "ERROR: Partido no existe.\n"; return;
    }
    if (strcmp(p.estado, "CANCELADO") == 0) {
        cout << "ERROR: Ya esta cancelado.\n"; return;
    }

    mostrarPartido(p);

    char confirmacion;
    cout << "\nADVERTENCIA: ¿Seguro que desea CANCELARLO? (S/N): ";
    cin >> confirmacion;
    if (toupper(confirmacion) != 'S') return;

    // Si ya fue jugado, revertimos toda la data
    if (strcmp(p.estado, "JUGADO") == 0) {
        Equipo local, visitante;
        if (obtenerEquipoPorID(p.idEquipoLocal, local) && obtenerEquipoPorID(p.idEquipoVisitante, visitante)) {
            // Revertir goles de equipos
            local.puntosAFavor -= p.golesLocal;
            local.puntosEnContra -= p.golesVisitante;
            visitante.puntosAFavor -= p.golesVisitante;
            visitante.puntosEnContra -= p.golesLocal;

            // Revertir puntos y resultados
            if (p.golesLocal > p.golesVisitante) {
                local.puntos -= 3; local.victorias--; visitante.derrotas--;
            } else if (p.golesLocal == p.golesVisitante) {
                local.puntos -= 1; visitante.puntos -= 1; local.empates--; visitante.empates--;
            } else {
                visitante.puntos -= 3; visitante.victorias--; local.derrotas--;
            }

            actualizarEquipoBin(local);
            actualizarEquipoBin(visitante);
        }

        // Revertir goles de jugadores
        for(int i=0; i<p.numGoles; i++) {
            if(p.goles[i].idJugador != 0) {
                Jugador goleador;
                if(obtenerJugadorPorID(p.goles[i].idJugador, goleador)) {
                    goleador.golesAnotados--;
                    actualizarJugadorBin(goleador);
                }
            }
        }
        
        // Limpiar array de goles del partido
        p.golesLocal = 0;
        p.golesVisitante = 0;
        p.numGoles = 0;
        for(int i=0; i<22; i++) {
            p.goles[i].idJugador = 0;
            p.goles[i].minuto = 0;
            strcpy(p.goles[i].equipo, "");
        }
    }

    strcpy(p.estado, "CANCELADO");
    actualizarPartidoBin(p);
    
    cout << "\nPartido cancelado. Si tenia resultados registrados, se han revertido en la base de datos.\n";
}

void menuBuscarPartido() {
    int id;
    cout << "\n--- BUSCAR PARTIDO ---" << endl;
    cout << "Ingrese el ID del Partido: ";
    leerEntero(id);

    Partido p;
    if (!obtenerPartidoPorID(id, p)) {
        cout << "No se encontro partido con ID " << id << ".\n" << endl;
    } else {
        mostrarPartido(p);
    }
}

void menuListarPartidos() {
    Partido buffer[100];
    int cantidad = listarTodosLosPartidos(buffer, 100);
    
    if (cantidad == 0) {
        cout << "\nNo hay partidos registrados.\n" << endl;
    } else {
        mostrarListaPartidos(buffer, cantidad);
    }
}

void menuBuscarPartidosPorEquipo() {
    int id;
    cout << "\n--- PARTIDOS POR EQUIPO ---" << endl;
    cout << "Ingrese el ID del Equipo: ";
    leerEntero(id);
    
    Partido buffer[100];
    int cantidad = listarPartidosPorEquipoBin(id, buffer, 100);
    
    if (cantidad == 0) cout << "No hay partidos para ese equipo.\n" << endl;
    else mostrarListaPartidos(buffer, cantidad);
}

void menuListarPartidosProgramados() {
    Partido buffer[100];
    int cantidad = listarPartidosPorEstadoBin("PROGRAMADO", buffer, 100);
    
    if (cantidad == 0) cout << "No hay partidos programados pendientes.\n" << endl;
    else mostrarListaPartidos(buffer, cantidad);
}

void subMenuPartidos() {
    int opcion;
    do {
        cout << "\n╔═══════════════════════════════════════════╗" << endl;
        cout << "║        GESTION DE PARTIDOS                ║" << endl;
        cout << "╠═══════════════════════════════════════════╣" << endl;
        cout << "║  1. Programar partido                     ║" << endl;
        cout << "║  2. Registrar resultado (COMPLETO)        ║" << endl;
        cout << "║  3. Buscar partido (Por ID)               ║" << endl;
        cout << "║  4. Listar TODOS los partidos             ║" << endl;
        cout << "║  5. Buscar partidos por Equipo            ║" << endl;
        cout << "║  6. Listar partidos PROGRAMADOS           ║" << endl;
        cout << "║  7. Cancelar partido (COMPLETO)           ║" << endl;
        cout << "║  0. Volver al menu principal              ║" << endl;
        cout << "╚═══════════════════════════════════════════╝" << endl;
        cout << "Seleccione una opcion: ";
        leerEntero(opcion);

        switch (opcion) {
            case 1: menuProgramarPartido(); break;
            case 2: menuRegistrarResultado(); break;
            case 3: menuBuscarPartido(); break;
            case 4: menuListarPartidos(); break;
            case 5: menuBuscarPartidosPorEquipo(); break;
            case 6: menuListarPartidosProgramados(); break;
            case 7: menuCancelarPartido(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}


// MAIN 

int main() {
    SetConsoleOutputCP(CP_UTF8);

    if(!inicializarSistemaArchivos()) {
        cout << "ERROR CRITICO: No se pudieron inicializar los archivos binarios." << endl;
        return 1;
    }

    int opcion;
    do {
        cout << "\n╔═══════════════════════════════════════════╗" << endl;
        cout << "║    SISTEMA DE GESTION DE TORNEOS          ║" << endl;
        cout << "║    Liga Apertura 2026 - Futbol            ║" << endl;
        cout << "╠═══════════════════════════════════════════╣" << endl;
        cout << "║  1. Gestion de Equipos                    ║" << endl;
        cout << "║  2. Gestion de Jugadores                  ║" << endl;
        cout << "║  3. Gestion de Partidos                   ║" << endl;
        cout << "║  4. Tabla de Posiciones                   ║" << endl;
        cout << "║  5. Reportes                              ║" << endl;
        cout << "║  6. Mantenimiento                         ║" << endl;
        cout << "║  0. Salir                                 ║" << endl;
        cout << "╚═══════════════════════════════════════════╝" << endl;
        cout << "Seleccione una opcion: ";
        leerEntero(opcion);

        switch (opcion) {
            case 1: subMenuEquipos(); break;
            case 2: subMenuJugadores(); break;
            case 3: subMenuPartidos(); break;
            case 4: cout << "\n[Tabla en construccion...]\n"; break;
            case 5: cout << "\n[Reportes en construccion...]\n"; break;
            case 6: cout << "\n[Mantenimiento en construccion...]\n"; break;
            case 0: {
                char confSalir;
                cout << "\n¿Esta seguro que desea salir del programa? (S/N): ";
                cin >> confSalir;
                if (toupper(confSalir) == 'S') {
                    cout << "\nGuardando en disco y saliendo. ¡Hasta luego!\n" << endl;
                } else {
                    opcion = -1; 
                }
                break;
            }
            default: cout << "Opcion invalida. Intente de nuevo.\n" << endl;
        }
    } while (opcion != 0);

    return 0;
}