#include <iostream>
using namespace std;

int main() {
    int cantidad;
    cout << "=== SISTEMA DE REGISTRO DE PUNTAJES ===" << endl;
    cout << "Cuantos puntajes deseas registrar en esta partida?: ";
    cin >> cantidad;

    // TODO 1: Pide a la Bodega (memoria dinámica) un arreglo de enteros.
    // El tamaño del arreglo debe ser la variable 'cantidad'.
    // Guarda las coordenadas que te dé el sistema en un puntero llamado 'puntajes'.
    
    int* puntajes = new int[cantidad]; 

    // Verificamos que la computadora sí nos haya dado el espacio
    if (puntajes == nullptr) {
        cout << "Error critico: La computadora no tiene memoria disponible." << endl;
        return 1; // Salimos del programa con error
    }

    cout << "\nBodega reservada con exito! Vamos a llenarla." << endl;

    // TODO 2: Haz un ciclo for que vaya desde 0 hasta 'cantidad'.
    // Dentro del ciclo, pide al usuario que ingrese un número y 
    // guárdalo en la posición correspondiente: puntajes[i]
    
    for(int i = 0; i < cantidad; i++){

bool noSalir = true;
int puntaje = 0;

do
{
    cout<<"Ingresa el puntaje numero "<< i + 1<<":";
    cin >> puntaje;
    if(cin.fail() || puntaje < 0){
    cout<<"Valor incorrecto. Ingrese un numero para el puntaje."<<endl;
    cin.clear();
    cin.ignore(1000, '\n');

    }else{
        noSalir = false;
    }

} while(noSalir);
puntajes[i] = puntaje;

    }


    cout << "\n--- PUNTAJES GUARDADOS EN LA BODEGA ---\n";
    
    // TODO 3: Haz otro ciclo for para imprimir en pantalla todos 
    // los puntajes que el usuario acaba de guardar.
    cout<<"Puntajes:"<<endl;
    for (int i = 0; i < cantidad; i++){
cout<<puntajes[i]<<endl;

    }


    // TODO 4: Ya terminamos de usar los datos. 
    // Libera la memoria dinámica del arreglo usando la palabra mágica.
    // ¡Ojo! Como pediste un arreglo con [], debes liberar un arreglo.
    
    delete[] puntajes;
    


    // TODO 5: Buenas costumbres. Rompe el mapa del tesoro apuntándolo a la nada,
    // para asegurarnos de no intentar entrar a la bodega por accidente después.
    
    puntajes = nullptr;

    cout << "\n¡Memoria liberada correctamente! No hay Memory Leaks hoy." << endl;

    return 0;
}