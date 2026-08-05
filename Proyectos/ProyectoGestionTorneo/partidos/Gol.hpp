#ifndef GOL_HPP
#define GOL_HPP

class Gol {
private:
    int  idJugador;
    int  minuto;
    char equipo[12]; // "LOCAL" o "VISITANTE"

public:
    Gol(); // Constructor por defecto
    Gol(int _idJugador, int _minuto, const char* _equipo);

    // Getters
    int getIdJugador() const;
    int getMinuto() const;
    const char* getEquipo() const;

    // Setters
    void setIdJugador(int _idJugador);
    void setMinuto(int _minuto);
    void setEquipo(const char* _equipo);
};

#endif