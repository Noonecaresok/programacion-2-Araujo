#ifndef INTERFAZ_HPP
#define INTERFAZ_HPP

class Interfaz {
private:
    //Mantenimiento y setup
    static bool inicializarSistemaArchivos();

    //Submenús principales
    static void subMenuEquipos();
    static void subMenuJugadores();
    static void subMenuPartidos();
    static void subMenuReportes();
    static void subMenuMantenimiento();

    //Funciones de Equipos
    static void menuRegistrarEquipo();
    static void menuBuscarEquipo();
    static void menuActualizarEquipo();
    static void menuEliminarEquipo();
    static void menuListarEquipos();

    //Funciones de Jugadores
    static void menuRegistrarJugador();
    static void menuBuscarJugador();
    static void menuActualizarJugador();
    static void menuEliminarJugador();
    static void menuListarJugadores();

    //Funciones de Partidos
    static void menuProgramarPartido();
    static void menuRegistrarResultado();
    static void menuBuscarPartido();
    static void menuListarPartidos();
    static void menuBuscarPartidosPorEquipo();
    static void menuListarPartidosProgramados();
    static void menuCancelarPartido();

    //Funciones de Reportes y Mantenimiento
    static void menuTablaPosiciones();
    static void menuTablaGoleadores();
    static void verificarIntegridadReferencial();
    static void crearBackup();

public:
    static void iniciar();
};

#endif