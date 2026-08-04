#ifndef ARCHIVO_HEADER_HPP
#define ARCHIVO_HEADER_HPP

struct ArchivoHeader {
    int cantidadRegistros;
    int proximoID;
    int registrosActivos;
    int version;
};

#endif