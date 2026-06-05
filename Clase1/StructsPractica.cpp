#include <iostream>
#include <string>
using namespace std;

// Objetivo: Manipular tipos de datos compuestos y pasarlos a funciones.

struct Articulo
{
    string nombre;
    float precio;
    int cantidad;
};

void venderArticulo(Articulo &item, int cantidad)
{
if(cantidad <= item.cantidad){
//Venta exitosa
item.cantidad -= cantidad;
cout<<"Venta exitosa por "<<(cantidad * item.precio)<<" (Item: "<<item.nombre<<")"<<endl;
}else{
    cout<<"Has intentado comprar el item "<<item.nombre<<", pero no hay suficiente stock."<<endl;
}

}

int main()
{

    Articulo Espada = {"Espada", 10, 2};
    Articulo Pocion = {"Pocion", 3, 5};

venderArticulo(Espada, 3);
venderArticulo(Espada, 2);
venderArticulo(Pocion, 5);

cout<<"Item "<<Espada.nombre<<"\n"<<"Cantidad actual: "<<Espada.cantidad<<"\n"<<"Precio: "<<Espada.precio<<endl;
cout<<"Item "<<Pocion.nombre<<"\n"<<"Cantidad actual: "<<Pocion.cantidad<<"\n"<<"Precio: "<<Pocion.precio<<endl;
    return 0;
}