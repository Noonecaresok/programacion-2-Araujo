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
    
    // Verificamos si tiene partidos
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

// Logica de Jugadores (Archivos) - Backend preparado para el siguiente commit

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

// Presentacion Equipos

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


// Main 

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
            case 2: cout << "\n[Modulo Jugadores en construccion...]\n"; break;
            case 3: cout << "\n[Modulo Partidos en construccion...]\n"; break;
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