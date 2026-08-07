#ifndef JUGADOR_HPP
#define JUGADOR_HPP

#include <ctime>
#include <cstddef>

class Jugador {
private:
    //Mismo orden exacto del struct original del P2
    int    id;
    int    idEquipo;
    char   nombre[100];
    char   cedula[20];
    char   posicion[20];
    int    edad;
    int    numeroDorsal;

    int    golesAnotados;
    int    tarjetasAmarillas;
    int    tarjetasRojas;

    bool   eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    Jugador();
    Jugador(int _idEquipo, const char* _nombre, const char* _cedula, const char* _posicion, int _edad, int _dorsal);

    //Getters
    int getId() const;
    int getIdEquipo() const;
    const char* getNombre() const;
    const char* getCedula() const;
    const char* getPosicion() const;
    int getEdad() const;
    int getNumeroDorsal() const;
    int getGolesAnotados() const;
    bool isEliminado() const;

    //Setters
    void setId(int _id);
    void setIdEquipo(int _idEquipo); // Permite transferencias
    bool setNombre(const char* _nombre);
    bool setPosicion(const char* _posicion);
    bool setEdad(int _edad);
    bool setNumeroDorsal(int _dorsal);
    void setEliminado(bool estado);
    void actualizarFechaModificacion();

    //Lógica de negocio interna
    void registrarGol();

    static size_t obtenerTamano();
};

#endif