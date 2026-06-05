#include <iostream>
using namespace std;

int* crearArreglo(int tamano){
    
    if(tamano <=0 ){
        cout<<"El tamano debe ser mayor a 0."<<endl;
        return nullptr;
    }
int* arreglo = new int[tamano];

return arreglo;
}

void llenarArreglo(int* arreglo, int tamano){

if (arreglo == nullptr || tamano <= 0) {
        cout << "Error: Arreglo no válido o tamaño incorrecto.\n";
        return;
    }


for(int i = 0; i < tamano; i++){
int dato = 0;
bool nosalir = true;

do
{
    cout<<"Ingrese el dato numero "<<i + 1<<": ";
    cin>>dato;

    if(cin.fail()){
        cout<<"Ingrese un valor numerico valido."<<endl;
        cin.clear();
        cin.ignore(1000, '\n');

    }else{
        nosalir = false;
    }

} while (nosalir);

arreglo[i] = dato;

}

}

void mostrarArreglo(int* arreglo, int tamano){
    if (arreglo == nullptr || tamano <= 0) {
        cout << "Error: Arreglo no válido o tamaño incorrecto.\n";
        return;
    }

cout<<"Arreglo:\n"<<endl;

for(int i = 0; i < tamano; i++){
cout<<"Elemento "<<i<<": "<<arreglo[i]<<endl;

}

}

int encontrarMayor(int* arreglo, int tamano){
    if (arreglo == nullptr || tamano <= 0) {
        cout << "Error: Arreglo no válido o tamaño incorrecto.\n";
        return -1;
    }

    int maxValue = arreglo[0];
    int pos = 0;
for (int i = 0; i < tamano; i++){
    if(arreglo[i] > maxValue){ 
    maxValue = arreglo[i];
    pos = i;
    }



}


cout<<"El valor mas grande del arreglo es el numero "<<maxValue<<", Encontrado en la posicion "<<pos<<endl;

return maxValue;

}

float calcularPromedio(int* arreglo, int tamano){
if (arreglo == nullptr || tamano <= 0) {
        cout << "Error: Arreglo no válido o tamaño incorrecto.\n";
        return -1;
    }
    float prom = 0;

    for (int i = 0; i < tamano; i++)
    {
        prom+= arreglo[i];
    }
    
    prom/=tamano;
    cout<<"El promedio fue de: "<<prom<<endl;
    return prom;
}

void liberarArreglo(int*& arreglo){

    delete[] arreglo;
    arreglo = nullptr;
    cout<<"Memoria liberada exitosamente.";
}

int main(){

int caso;
bool nosalir = true;
int tamano = 0;
int* array = nullptr;
do
{
    cout<<"1. Crear y llenar arreglo\n"<<"2. Mostrar arreglo\n"<<"3. Encontrar numero mayor\n"<<"4. Calcular promedio\n"<<"5. Salir"<<endl;

cin>> caso;

switch (caso){
case 1:
if (array != nullptr) {
                    liberarArreglo(array);
                }

    cout<<"Porfavor elija un tamano para el array."<<endl;
    cin>>tamano;
    array = crearArreglo(tamano);
    if (array != nullptr) {
                    llenarArreglo(array, tamano);
                }
    break;

case 2:
cout<<"Ha seleccionado mostrar arreglo"<<endl;
mostrarArreglo(array, tamano);
break;

case 3: 
cout<<"Ha seleccionado encontrar el numero mayor"<<endl;
encontrarMayor(array, tamano);
break;

case 4: 
cout<<"Ha seleccionado calcular promedio"<<endl;
calcularPromedio(array, tamano);
break;

case 5: 
cout<<"Has seleccionado salir."<<endl;
nosalir = false;
break;

default:
cout<<"Seleccione una opcion valida.";
    break;
}




} while (nosalir);



if (array != nullptr) {
        liberarArreglo(array);
    }



    return 0;
}