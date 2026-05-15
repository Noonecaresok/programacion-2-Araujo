#include <iostream>
using namespace std;

//En esta parte voy a practicar la declaracion de punteros, y la modificacion de valores

int main(){
    int numero1 = 20;
    int* puntero1 = &numero1;

    cout<<"Valor del numero 1: "<<numero1<<endl; //Sin puntero, el valor de la variable
    cout<<"Direccion de memoria del numero1: "<<&numero1<<endl;
    cout<<"Direccion de memoria del puntero1: "<<puntero1<<endl; //Cuando nos referimos a un puntero nos referimos a la direccion de memoria que guarda
    cout<<"Valor almacenado en puntero1: "<<*puntero1<<endl; //Aqui estamos accediendo al valor almacenado en esta direccion de memoria

    //Podemos cambiar el valor de una direccion de memoria de la siguiente forma

    *puntero1 = 30; //De esta forma estamos, mediante el puntero1, cambiando el valor que almacena, la direccion de memoria a la que apunta
    cout<<"Nuevo valor almacenado en puntero1: "<<*puntero1<<endl;
    //Al cambiar el valor que almacena esa direccion de memoria mediante el puntero, cambia la variable original.
    cout<<"Valor del numero 1: "<<numero1<<endl;
    
    return 0;
}