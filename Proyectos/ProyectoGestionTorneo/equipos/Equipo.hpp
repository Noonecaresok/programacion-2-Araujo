#ifndef EQUIPO_HPP
#define EQUIPO_HPP

#include <ctime>
#include <cstddef> // Necesario para size_t

class Equipo {
private:
    // ATRIBUTOS (Deben ser EXACTAMENTE iguales al struct original del P2)
    // No cambiamos tipos ni orden para garantizar que sizeof(Equipo) sea el mismo
    // y los archivos .bin antiguos funcionen perfectamente.
    int    id;
    char   nombre[100];
    char   city[100];
    char   entrenador[100];

    int    puntos;
    int    victorias;
    int    empates;
    int    derrotas;
    int    puntosAFavor;
    int    puntosEnContra;

    int    partidosIDs[50]; 
    int    cantidadPartidos;

    bool   eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    // 1. Constructores
    Equipo(); // Constructor por defecto
    Equipo(const char* _nombre, const char* _city, const char* _entrenador); // Parametrizado

    // 2. Getters (Todos llevan 'const' porque no modifican la clase, solo leen)
    int getId() const;
    const char* getNombre() const;
    const char* getCity() const;
    const char* getEntrenador() const;
    
    int getPuntos() const;
    int getVictorias() const;
    int getEmpates() const;
    int getDerrotas() const;
    int getPuntosAFavor() const;
    int getPuntosEnContra() const;
    
    int getCantidadPartidos() const;
    bool isEliminado() const;
    time_t getFechaCreacion() const;
    time_t getFechaUltimaModificacion() const;

    // 3. Setters (Retornan bool para indicar si la validación fue exitosa o falló)
    void setId(int _id); // void porque el sistema asigna el ID, no puede fallar
    bool setNombre(const char* _nombre);
    bool setCity(const char* _city);
    bool setEntrenador(const char* _entrenador);
    void setEliminado(bool estado);
    void actualizarFechaModificacion();

    // 4. Métodos de Negocio (Abstracción y Responsabilidad Única)
    // En lugar de modificar los puntos a mano, la clase lo hace sola en base al resultado
    bool registrarPartido(int idPartido);
    void registrarResultado(int golesFavor, int golesContra);

    // 5. Métodos de presentación
    void mostrarBasico() const;
    void mostrarDetalle() const;

    // 6. Método estático requerido por el profesor
    static size_t obtenerTamano();
};

#endif