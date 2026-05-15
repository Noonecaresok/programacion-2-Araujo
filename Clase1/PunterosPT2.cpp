#include <iostream>
using namespace std;

//Aqui practicare Punteros y arreglos

int main(){

    int numeros[3] = {2,5,6}; //Este es un array comun
    cout<<numeros<<endl; //Si imprimimos un array, es imprimir su direccion de memoria directamente

    int* punteroArray = numeros; //Ya que el array es la direccion de memoria, no hace falta poner el & a la hora de hacer un puntero 
    cout<<punteroArray<<endl; //Se imprime tal cual la direccion de memoria, mediante el puntero

    //Ahora, ya que sabemos como hacer un puntero de un array, y entendemos que el array mismo en la direccion de memoria
    //Intentemos acceder a los valores que contienen este array, mediante punteros

    cout<<"Elemento 1: "<<*punteroArray<<endl; //Con la indireccion, de un array, accedemos directamente al primer elemento (pos 0)
    cout<<"Elemento 2: "<<*(punteroArray + 1)<<endl; //Para acceder al segundo elemento, simplemente sumamos 1 a la direccion de memoria
    cout<<"Elemento 3: "<<*(numeros + 2)<<endl; //Como podemos ver, al usar un puntero, o el array en si, es lo mismo porque ambos apuntan a la direccion de memoria

    //Ahora haremos el mismo ejercicio que tiene el profesor en su repositorio, recorreremos con un for el array e imprimiremos cada elemento.
    cout<<"\n"<<"Entra en ejecucion el for"<<endl;

    for(int i = 0; i < 3; i++){
        cout<<"Elemento "<< i + 1 << ": " << *(punteroArray + i) << endl;
    }

    //Ahora vamos a modificar mediante el puntero, el valor de cada elemento
    cout<<"\n"<<"Ahora vamos a modificar mediante el puntero, el valor de cada elemento"<<endl;
    for(int i = 0; i < 3; i++){
        *(punteroArray + i) = *(punteroArray + i) * 2;
        cout<<"Nuevo elemento "<<i+1<<": "<<*(punteroArray + i)<<endl;
    }

    //Imprimamos uno fuera de un for
    cout<<"Elemento 1: " << *punteroArray<<endl;

    return 0;
}