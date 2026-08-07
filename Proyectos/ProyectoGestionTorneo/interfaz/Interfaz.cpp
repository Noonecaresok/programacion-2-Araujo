#include "Interfaz.hpp"
#include "../utilidades/Formatos.hpp"
#include "../equipos/OperacionesEquipo.hpp"
#include "../equipos/Equipo.hpp"
#include "../jugadores/OperacionesJugador.hpp"
#include "../jugadores/Jugador.hpp"
#include "../partidos/OperacionesPartido.hpp"
#include "../partidos/Partido.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../torneo/Torneo.hpp"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <fstream> 

using namespace std;

bool Interfaz::inicializarSistemaArchivos() {
    bool eq = GestorArchivos::inicializarArchivo<Equipo>(OperacionesEquipo::RUTA_EQUIPOS);
    bool ju = GestorArchivos::inicializarArchivo<Jugador>(OperacionesJugador::RUTA_JUGADORES);
    bool pa = GestorArchivos::inicializarArchivo<Partido>(OperacionesPartido::RUTA_PARTIDOS);
    
    Torneo t_temp;
    if (!Torneo::leerTorneoActual(t_temp)) {
        Torneo t("Liga Apertura 2026", "Futbol", "GRUPOS", "2026-06-08", "2026-12-15");
        
        
        ofstream outT("datos/torneo.bin", ios::binary);
        outT.write(reinterpret_cast<char*>(&t), sizeof(Torneo));
        outT.close();
        
    }
    return (eq && ju && pa);
}

void Interfaz::iniciar() {
    if(!inicializarSistemaArchivos()) {
        cout << "ERROR CRITICO: No se pudieron inicializar los archivos binarios." << endl;
        return;
    }

    Torneo torneoActual;
    Torneo::leerTorneoActual(torneoActual);

    int opcion;
    do {
        Formatos::limpiarPantalla();
        cout << "\n╔═══════════════════════════════════════════╗" << endl;
        cout << "║    SISTEMA DE GESTION DE TORNEOS          ║" << endl;
        cout << "║    " << left << setw(39) << torneoActual.getNombre() << "║" << endl;
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
        Formatos::leerEntero(opcion);

        switch (opcion) {
            case 1: subMenuEquipos(); break;
            case 2: subMenuJugadores(); break;
            case 3: subMenuPartidos(); break;
            case 4: menuTablaPosiciones(); break;
            case 5: subMenuReportes(); break;
            case 6: subMenuMantenimiento(); break;
            case 0: {
                char confSalir;
                cout << "\n¿Esta seguro que desea salir del programa? (S/N): ";
                cin >> confSalir;
                if (toupper(confSalir) == 'S') {
                    cout << "\nCerrando archivos y saliendo. ¡Hasta luego!\n" << endl;
                } else {
                    opcion = -1; 
                }
                break;
            }
            default: cout << "Opcion invalida. Intente de nuevo.\n" << endl; Formatos::pausar();
        }
    } while (opcion != 0);
}


//MÓDULO EQUIPOS


void Interfaz::subMenuEquipos() {
    int opcion;
    do {
        Formatos::limpiarPantalla();
        cout << "\n╔═══════════════════════════════════════════╗" << endl;
        cout << "║        GESTION DE EQUIPOS                 ║" << endl;
        cout << "╠═══════════════════════════════════════════╣" << endl;
        cout << "║  1. Registrar equipo                      ║" << endl;
        cout << "║  2. Buscar equipo                         ║" << endl;
        cout << "║  3. Actualizar equipo                     ║" << endl;
        cout << "║  4. Listar equipos                        ║" << endl;
        cout << "║  5. Eliminar equipo (Borrado Logico)      ║" << endl;
        cout << "║  0. Volver al menu principal              ║" << endl;
        cout << "╚═══════════════════════════════════════════╝" << endl;
        cout << "Seleccione una opcion: ";
        Formatos::leerEntero(opcion);

        switch (opcion) {
            case 1: menuRegistrarEquipo(); break;
            case 2: menuBuscarEquipo(); break;
            case 3: menuActualizarEquipo(); break;
            case 4: menuListarEquipos(); break;
            case 5: menuEliminarEquipo(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n"; Formatos::pausar();
        }
    } while (opcion != 0);
}

void Interfaz::menuRegistrarEquipo() {
    char nombre[100], city[100], entrenador[100];
    cout << "\n--- REGISTRAR NUEVO EQUIPO ---" << endl;
    cout << "Ingrese el nombre del equipo: ";
    Formatos::leerCadena(nombre, 100);
    
    Equipo temporales[1];
    if(OperacionesEquipo::buscarPorNombre(nombre, temporales, 1) > 0 && strcmp(temporales[0].getNombre(), nombre) == 0){
        cout << "ERROR: Ya existe un equipo con ese nombre.\n";
        Formatos::pausar(); return;
    }

    cout << "Ingrese la ciudad: ";
    Formatos::leerCadena(city, 100);
    cout << "Ingrese el nombre del entrenador: ";
    Formatos::leerCadena(entrenador, 100);

    char confirmacion;
    cout << "\n¿Desea guardar este equipo? (S/N): ";
    cin >> confirmacion;
    if (toupper(confirmacion) == 'S') {
        Equipo nuevo(nombre, city, entrenador);
        if (GestorArchivos::guardarRegistro<Equipo>(OperacionesEquipo::RUTA_EQUIPOS, nuevo)) {
            cout << "\n¡Equipo guardado exitosamente!" << endl;
            nuevo.mostrarBasico();
        }
    }
    Formatos::pausar();
}

void Interfaz::menuBuscarEquipo() {
    char busqueda[100];
    cout << "\n--- BUSCAR EQUIPO ---" << endl;
    cout << "Ingrese el nombre a buscar: ";
    Formatos::leerCadena(busqueda, 100);

    Equipo resultados[100]; 
    int cantidad = OperacionesEquipo::buscarPorNombre(busqueda, resultados, 100);

    if (cantidad == 0) cout << "No se encontraron equipos.\n" << endl;
    else {
        for (int i = 0; i < cantidad; i++) resultados[i].mostrarDetalle();
    }
    Formatos::pausar();
}

void Interfaz::menuActualizarEquipo() {
    int id;
    cout << "\n--- ACTUALIZAR EQUIPO ---" << endl;
    cout << "Ingrese el ID del equipo: ";
    Formatos::leerEntero(id);

    Equipo actual;
    if (!GestorArchivos::obtenerRegistroPorID<Equipo>(OperacionesEquipo::RUTA_EQUIPOS, id, actual)) {
        cout << "ERROR: Equipo no encontrado.\n" << endl; Formatos::pausar(); return;
    }

    int opcion;
    bool modificado = false;
    do {
        Formatos::limpiarPantalla();
        cout << "\n--- MODIFICANDO EQUIPO ID: " << actual.getId() << " ---" << endl;
        cout << "1. Nombre actual     : " << actual.getNombre() << endl;
        cout << "2. Ciudad actual     : " << actual.getCity() << endl;
        cout << "3. Entrenador actual : " << actual.getEntrenador() << endl;
        cout << "0. Terminar y guardar cambios" << endl;
        cout << "Seleccione el campo que desea modificar: ";
        Formatos::leerEntero(opcion);

        switch (opcion) {
            case 1: {
                char nuevoNombre[100];
                cout << "Nuevo nombre: "; Formatos::leerCadena(nuevoNombre, 100);
                actual.setNombre(nuevoNombre);
                modificado = true;
                break;
            }
            case 2: {
                char nuevaCity[100];
                cout << "Nueva ciudad: "; Formatos::leerCadena(nuevaCity, 100);
                actual.setCity(nuevaCity);
                modificado = true;
                break;
            }
            case 3: {
                char nuevoEnt[100];
                cout << "Nuevo entrenador: "; Formatos::leerCadena(nuevoEnt, 100);
                actual.setEntrenador(nuevoEnt);
                modificado = true;
                break;
            }
            case 0: break;
            default: cout << "Opcion invalida.\n"; Formatos::pausar();
        }
    } while (opcion != 0);

    if (modificado) {
        char confirmacion;
        cout << "\n¿Guardar cambios en el disco? (S/N): ";
        cin >> confirmacion;
        if (toupper(confirmacion) == 'S') {
            if (GestorArchivos::actualizarRegistro<Equipo>(OperacionesEquipo::RUTA_EQUIPOS, actual)) {
                cout << "¡Equipo actualizado correctamente!\n";
            }
        } else {
            cout << "Cambios descartados.\n";
        }
    } else {
        cout << "No se realizaron cambios.\n";
    }
    Formatos::pausar();
}

void Interfaz::menuEliminarEquipo() {
    int id;
    cout << "\n--- ELIMINAR EQUIPO ---" << endl;
    cout << "Ingrese el ID del equipo: ";
    Formatos::leerEntero(id);

    Equipo actual;
    if (!GestorArchivos::obtenerRegistroPorID<Equipo>(OperacionesEquipo::RUTA_EQUIPOS, id, actual)) {
        cout << "ERROR: Equipo no encontrado.\n" << endl; Formatos::pausar(); return;
    }
    actual.mostrarBasico();

    char confirmacion;
    cout << "\nADVERTENCIA: ¿Eliminar este equipo lógicamente? (S/N): ";
    cin >> confirmacion;
    if (toupper(confirmacion) == 'S') {
        if (OperacionesEquipo::eliminarEquipo(id)) cout << "Equipo eliminado en disco.\n";
        else cout << "ERROR: No se pudo eliminar (tiene partidos asociados).\n";
    }
    Formatos::pausar();
}

void Interfaz::menuListarEquipos() {
    Equipo buffer[100]; 
    int cantidad = OperacionesEquipo::listarTodos(buffer, 100);
    if (cantidad == 0) cout << "\nNo hay equipos registrados.\n" << endl;
    else {
        cout << "\n--- LISTADO GENERAL DE EQUIPOS ---" << endl;
        for (int i = 0; i < cantidad; i++) buffer[i].mostrarBasico();
        cout << "Total: " << cantidad << "\n";
    }
    Formatos::pausar();
}


//MÓDULO JUGADORES


void Interfaz::subMenuJugadores() {
    int opcion;
    do {
        Formatos::limpiarPantalla();
        cout << "\n╔═══════════════════════════════════════════╗" << endl;
        cout << "║        GESTION DE JUGADORES               ║" << endl;
        cout << "╠═══════════════════════════════════════════╣" << endl;
        cout << "║  1. Registrar jugador                     ║" << endl;
        cout << "║  2. Buscar jugador                        ║" << endl;
        cout << "║  3. Actualizar jugador                    ║" << endl;
        cout << "║  4. Listar jugadores (todos)              ║" << endl;
        cout << "║  5. Listar jugadores por equipo           ║" << endl;
        cout << "║  6. Eliminar jugador (Borrado Logico)     ║" << endl;
        cout << "║  0. Volver al menu principal              ║" << endl;
        cout << "╚═══════════════════════════════════════════╝" << endl;
        cout << "Seleccione una opcion: ";
        Formatos::leerEntero(opcion);

        switch (opcion) {
            case 1: menuRegistrarJugador(); break;
            case 2: menuBuscarJugador(); break;
            case 3: menuActualizarJugador(); break;
            case 4: menuListarJugadores(); break;
            case 5: {
                int idEq; cout << "Ingrese ID del Equipo: "; Formatos::leerEntero(idEq);
                Jugador lista[100]; int cant = OperacionesJugador::listarPorEquipo(idEq, lista, 100);
                if(cant > 0) for(int i=0; i<cant; i++) cout << "ID: " << lista[i].getId() << " | " << lista[i].getNombre() << endl;
                else cout << "No hay jugadores.\n";
                Formatos::pausar(); break;
            }
            case 6: menuEliminarJugador(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n"; Formatos::pausar();
        }
    } while (opcion != 0);
}

void Interfaz::menuRegistrarJugador() {
    char nombre[100], cedula[20], posicion[20];
    int idEquipo, edad, dorsal;
    
    cout << "\n--- REGISTRAR NUEVO JUGADOR ---" << endl;
    cout << "Ingrese el ID del Equipo al que pertenece: "; Formatos::leerEntero(idEquipo);
    
    Equipo eq;
    if(!GestorArchivos::obtenerRegistroPorID<Equipo>(OperacionesEquipo::RUTA_EQUIPOS, idEquipo, eq)){
        cout << "ERROR: No existe equipo con ID " << idEquipo << ".\n"; Formatos::pausar(); return;
    }
    
    cout << "Ingrese el nombre del jugador: "; Formatos::leerCadena(nombre, 100);
    cout << "Ingrese la cedula: "; Formatos::leerCadena(cedula, 20);
    cout << "Ingrese el numero dorsal (1 - 99): "; Formatos::leerEntero(dorsal);

    int validacion = OperacionesJugador::validarUnicidad(cedula, idEquipo, dorsal);
    if (validacion == 1) { cout << "ERROR: Cedula en uso.\n"; Formatos::pausar(); return; }
    else if (validacion == 2) { cout << "ERROR: Dorsal en uso en ese equipo.\n"; Formatos::pausar(); return; }

    cout << "Ingrese la posicion: "; Formatos::leerCadena(posicion, 20);
    cout << "Ingrese la edad (14 - 50): "; Formatos::leerEntero(edad);

    char confirmacion; cout << "\n¿Guardar jugador? (S/N): "; cin >> confirmacion;
    if (toupper(confirmacion) == 'S') {
        Jugador nuevo(idEquipo, nombre, cedula, posicion, edad, dorsal);
        if (GestorArchivos::guardarRegistro<Jugador>(OperacionesJugador::RUTA_JUGADORES, nuevo)) {
            cout << "¡Jugador guardado exitosamente!\n";
        }
    }
    Formatos::pausar();
}

void Interfaz::menuBuscarJugador() {
    char busqueda[100];
    cout << "\n--- BUSCAR JUGADOR ---" << endl;
    cout << "Nombre a buscar: "; Formatos::leerCadena(busqueda, 100);
    Jugador resultados[100];
    int cantidad = OperacionesJugador::buscarPorNombre(busqueda, resultados, 100);
    if (cantidad == 0) cout << "No se encontraron jugadores.\n";
    else for (int i = 0; i < cantidad; i++) cout << "ID: " << resultados[i].getId() << " | " << resultados[i].getNombre() << " | Cedula: " << resultados[i].getCedula() << endl;
    Formatos::pausar();
}

void Interfaz::menuActualizarJugador() {
    int id; 
    cout << "\n--- ACTUALIZAR JUGADOR ---\nIngrese ID: "; 
    Formatos::leerEntero(id);
    
    Jugador actual;
    if (!GestorArchivos::obtenerRegistroPorID<Jugador>(OperacionesJugador::RUTA_JUGADORES, id, actual)) {
        cout << "ERROR: Jugador no encontrado.\n"; Formatos::pausar(); return;
    }

    int opcion;
    bool modificado = false;
    do {
        Formatos::limpiarPantalla();
        cout << "\n--- MODIFICANDO JUGADOR ID: " << actual.getId() << " ---" << endl;
        cout << "1. Nombre actual   : " << actual.getNombre() << endl;
        cout << "2. Posicion actual : " << actual.getPosicion() << endl;
        cout << "3. Edad actual     : " << actual.getEdad() << endl;
        cout << "4. Dorsal actual   : " << actual.getNumeroDorsal() << endl;
        cout << "0. Terminar y guardar cambios" << endl;
        cout << "Seleccione el campo que desea modificar: ";
        Formatos::leerEntero(opcion);

        switch (opcion) {
            case 1: {
                char nuevoNombre[100];
                cout << "Nuevo nombre: "; Formatos::leerCadena(nuevoNombre, 100);
                actual.setNombre(nuevoNombre);
                modificado = true;
                break;
            }
            case 2: {
                char nuevaPos[20];
                cout << "Nueva posicion: "; Formatos::leerCadena(nuevaPos, 20);
                actual.setPosicion(nuevaPos);
                modificado = true;
                break;
            }
            case 3: {
                int nuevaEdad;
                cout << "Nueva edad: "; Formatos::leerEntero(nuevaEdad);
                actual.setEdad(nuevaEdad);
                modificado = true;
                break;
            }
            case 4: {
                int nuevoDorsal;
                cout << "Nuevo dorsal: "; Formatos::leerEntero(nuevoDorsal);
                actual.setNumeroDorsal(nuevoDorsal);
                modificado = true;
                break;
            }
            case 0: break;
            default: cout << "Opcion invalida.\n"; Formatos::pausar();
        }
    } while (opcion != 0);

    if (modificado) {
        char conf; 
        cout << "\n¿Guardar cambios en el disco? (S/N): "; 
        cin >> conf;
        if (toupper(conf) == 'S') {
            GestorArchivos::actualizarRegistro<Jugador>(OperacionesJugador::RUTA_JUGADORES, actual);
            cout << "¡Jugador actualizado correctamente!\n";
        } else {
            cout << "Cambios descartados.\n";
        }
    } else {
        cout << "No se realizaron cambios.\n";
    }
    Formatos::pausar();
}

void Interfaz::menuListarJugadores() {
    Jugador buffer[100]; int cant = OperacionesJugador::listarTodos(buffer, 100);
    if (cant == 0) cout << "No hay jugadores.\n";
    else for (int i = 0; i < cant; i++) cout << "ID: " << buffer[i].getId() << " | " << buffer[i].getNombre() << " | Eq ID: " << buffer[i].getIdEquipo() << endl;
    Formatos::pausar();
}

void Interfaz::menuEliminarJugador() {
    int id; cout << "Ingrese ID de jugador a eliminar: "; Formatos::leerEntero(id);
    if(OperacionesJugador::eliminarJugador(id)) cout << "Eliminado.\n";
    else cout << "Error al eliminar.\n";
    Formatos::pausar();
}


//MÓDULO PARTIDOS


void Interfaz::subMenuPartidos() {
    int opcion;
    do {
        Formatos::limpiarPantalla();
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
        cout << "Seleccione una opcion: "; Formatos::leerEntero(opcion);

        switch (opcion) {
            case 1: menuProgramarPartido(); break;
            case 2: menuRegistrarResultado(); break;
            case 3: menuBuscarPartido(); break;
            case 4: menuListarPartidos(); break;
            case 5: {
                int id; cout << "ID de Equipo: "; Formatos::leerEntero(id);
                Partido buffer[100]; int cant = OperacionesPartido::listarPorEquipo(id, buffer, 100);
                for(int i=0; i<cant; i++) cout << "ID " << buffer[i].getId() << " | Estado: " << buffer[i].getEstado() << endl;
                Formatos::pausar(); break;
            }
            case 6: menuListarPartidosProgramados(); break;
            case 7: menuCancelarPartido(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n"; Formatos::pausar();
        }
    } while (opcion != 0);
}

void Interfaz::menuProgramarPartido() {
    int idL, idV; char fecha[11], desc[200];
    cout << "\n--- PROGRAMAR PARTIDO ---\nID Equipo Local: "; Formatos::leerEntero(idL);
    cout << "ID Equipo Visitante: "; Formatos::leerEntero(idV);
    
    if (idL == idV) { cout << "ERROR: Equipos iguales.\n"; Formatos::pausar(); return; }
    Equipo eL, eV;
    if (!GestorArchivos::obtenerRegistroPorID<Equipo>(OperacionesEquipo::RUTA_EQUIPOS, idL, eL) || 
        !GestorArchivos::obtenerRegistroPorID<Equipo>(OperacionesEquipo::RUTA_EQUIPOS, idV, eV)) {
        cout << "ERROR: Uno de los equipos no existe.\n"; Formatos::pausar(); return;
    }

    cout << "Fecha (YYYY-MM-DD): "; Formatos::leerCadena(fecha, 11);
    cout << "Descripcion: "; Formatos::leerCadena(desc, 200);

    char conf; cout << "¿Guardar? (S/N): "; cin >> conf;
    if(toupper(conf) == 'S') {
        Partido p(idL, idV, fecha, desc);
        GestorArchivos::guardarRegistro<Partido>(OperacionesPartido::RUTA_PARTIDOS, p);
        cout << "Programado.\n";
    }
    Formatos::pausar();
}

void Interfaz::menuRegistrarResultado() {
    int id; cout << "ID del Partido: "; Formatos::leerEntero(id);
    Partido p;
    if (!GestorArchivos::obtenerRegistroPorID<Partido>(OperacionesPartido::RUTA_PARTIDOS, id, p) || strcmp(p.getEstado(), "PROGRAMADO") != 0) {
        cout << "Partido no existe o ya no esta programado.\n"; Formatos::pausar(); return;
    }
    Equipo eL, eV;
    GestorArchivos::obtenerRegistroPorID<Equipo>(OperacionesEquipo::RUTA_EQUIPOS, p.getIdEquipoLocal(), eL);
    GestorArchivos::obtenerRegistroPorID<Equipo>(OperacionesEquipo::RUTA_EQUIPOS, p.getIdEquipoVisitante(), eV);

    int gL, gV;
    cout << "Goles " << eL.getNombre() << " (Local): "; Formatos::leerEntero(gL);
    cout << "Goles " << eV.getNombre() << " (Visitante): "; Formatos::leerEntero(gV);

    for (int i = 0; i < gL; i++) {
        int min, idJug; cout << "Gol Local #" << i+1 << " - Minuto: "; Formatos::leerEntero(min);
        cout << "ID Jugador (0=Desc): "; Formatos::leerEntero(idJug);
        p.registrarGol(idJug, min, "LOCAL");
        if(idJug!=0) {
            Jugador j; if(GestorArchivos::obtenerRegistroPorID<Jugador>(OperacionesJugador::RUTA_JUGADORES, idJug, j)) { j.registrarGol(); GestorArchivos::actualizarRegistro<Jugador>(OperacionesJugador::RUTA_JUGADORES, j); }
        }
    }
    for (int i = 0; i < gV; i++) {
        int min, idJug; cout << "Gol Visitante #" << i+1 << " - Minuto: "; Formatos::leerEntero(min);
        cout << "ID Jugador (0=Desc): "; Formatos::leerEntero(idJug);
        p.registrarGol(idJug, min, "VISITANTE");
        if(idJug!=0) {
            Jugador j; if(GestorArchivos::obtenerRegistroPorID<Jugador>(OperacionesJugador::RUTA_JUGADORES, idJug, j)) { j.registrarGol(); GestorArchivos::actualizarRegistro<Jugador>(OperacionesJugador::RUTA_JUGADORES, j); }
        }
    }

    p.finalizarPartido();
    eL.registrarResultado(gL, gV); eL.registrarPartido(p.getId());
    eV.registrarResultado(gV, gL); eV.registrarPartido(p.getId());

    GestorArchivos::actualizarRegistro<Partido>(OperacionesPartido::RUTA_PARTIDOS, p);
    GestorArchivos::actualizarRegistro<Equipo>(OperacionesEquipo::RUTA_EQUIPOS, eL);
    GestorArchivos::actualizarRegistro<Equipo>(OperacionesEquipo::RUTA_EQUIPOS, eV);
    cout << "Resultado procesado en todos los archivos.\n";
    Formatos::pausar();
}

void Interfaz::menuBuscarPartido() {
    int id; cout << "ID Partido: "; Formatos::leerEntero(id);
    Partido p;
    if(GestorArchivos::obtenerRegistroPorID<Partido>(OperacionesPartido::RUTA_PARTIDOS, id, p)) 
        cout << "ID: " << p.getId() << " | Estado: " << p.getEstado() << " | Resultado: " << p.getGolesLocal() << "-" << p.getGolesVisitante() << endl;
    else cout << "No encontrado.\n";
    Formatos::pausar();
}

void Interfaz::menuListarPartidos() {
    Partido b[100]; int cant = OperacionesPartido::listarTodos(b, 100);
    for(int i=0; i<cant; i++) cout << "ID: " << b[i].getId() << " | " << b[i].getEstado() << " | Fecha: " << b[i].getFecha() << endl;
    Formatos::pausar();
}

void Interfaz::menuListarPartidosProgramados() {
    Partido b[100]; int cant = OperacionesPartido::listarPorEstado("PROGRAMADO", b, 100);
    for(int i=0; i<cant; i++) cout << "ID: " << b[i].getId() << " | " << b[i].getFecha() << endl;
    Formatos::pausar();
}

void Interfaz::menuCancelarPartido() {
    int id; cout << "ID Partido a cancelar: "; Formatos::leerEntero(id);
    Partido p;
    if(GestorArchivos::obtenerRegistroPorID<Partido>(OperacionesPartido::RUTA_PARTIDOS, id, p)) {
        p.setEstado("CANCELADO");
        GestorArchivos::actualizarRegistro<Partido>(OperacionesPartido::RUTA_PARTIDOS, p);
        cout << "Cancelado.\n";
    } else cout << "No existe.\n";
    Formatos::pausar();
}


//MÓDULO REPORTES Y MANTENIMIENTO


void Interfaz::subMenuReportes() {
    int opcion;
    do {
        Formatos::limpiarPantalla();
        cout << "\n--- REPORTES ---\n1. Tabla Posiciones\n2. Tabla Goleadores\n0. Volver\nOpcion: ";
        Formatos::leerEntero(opcion);
        if(opcion==1) menuTablaPosiciones();
        if(opcion==2) menuTablaGoleadores();
    } while(opcion!=0);
}

void Interfaz::menuTablaPosiciones() {
    Equipo tabla[100]; int cant = OperacionesEquipo::listarTodos(tabla, 100);
    for(int a=0; a<cant-1; a++) {
        for(int b=0; b<cant-a-1; b++) {
            if(tabla[b].getPuntos() < tabla[b+1].getPuntos()) {
                Equipo aux = tabla[b]; tabla[b] = tabla[b+1]; tabla[b+1] = aux;
            }
        }
    }
    cout << "\n--- TABLA POSICIONES ---\n";
    for(int i=0; i<cant; i++) cout << i+1 << ". " << tabla[i].getNombre() << " | PTS: " << tabla[i].getPuntos() << endl;
    Formatos::pausar();
}

void Interfaz::menuTablaGoleadores() {
    Jugador tabla[200]; int cant = OperacionesJugador::listarTodos(tabla, 200);
    for(int a=0; a<cant-1; a++) {
        for(int b=0; b<cant-a-1; b++) {
            if(tabla[b].getGolesAnotados() < tabla[b+1].getGolesAnotados()) {
                Jugador aux = tabla[b]; tabla[b] = tabla[b+1]; tabla[b+1] = aux;
            }
        }
    }
    cout << "\n--- GOLEADORES ---\n";
    int lim = (cant<10)?cant:10;
    for(int i=0; i<lim; i++) if(tabla[i].getGolesAnotados()>0) cout << i+1 << ". " << tabla[i].getNombre() << " | Goles: " << tabla[i].getGolesAnotados() << endl;
    Formatos::pausar();
}

void Interfaz::subMenuMantenimiento() {
    int opcion;
    do {
        Formatos::limpiarPantalla();
        cout << "\n--- MANTENIMIENTO ---\n1. Integridad Referencial\n2. Backup\n0. Volver\nOpcion: ";
        Formatos::leerEntero(opcion);
        if(opcion==1) verificarIntegridadReferencial();
        if(opcion==2) crearBackup();
    } while(opcion!=0);
}

void Interfaz::verificarIntegridadReferencial() {
    cout << "Verificando... (Modulo simulado para P3, las BD relacionales previenen esto naturalmente).\n";
    Formatos::pausar();
}

void Interfaz::crearBackup() {
    cout << "\nGenerando copia de seguridad...\n";
    system("mkdir backup_torneo 2>nul");
    system("copy datos\\*.bin backup_torneo\\ >nul");
    cout << "Backup generado en carpeta backup_torneo/\n";
    Formatos::pausar();
}