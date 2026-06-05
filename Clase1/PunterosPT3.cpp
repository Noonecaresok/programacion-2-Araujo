#include <iostream>
using namespace std;

// En esta parte, vamos a practicar el paso por referencia en funciones (Punteros y funciones)

void AnalizarTemperatura(float (&arr)[7])
{

float previusMax = 0;
float actual = 0;
float prom = 0;
int dia = 0;
for(int i = 0; i < 7 ; i++){
    actual = arr[i]; //Actual se vuelve el numero de esta iteracion
    if(actual > previusMax){ //Si actual es mayor a el numero mayor anterior, se actualiza, asi encontraremos el mayor de todos.
    previusMax = arr[i];
    dia = i + 1;
    }

prom+=arr[i]; //para sacar el promedio luego
}
prom/=7;
cout<<"El dia mas caluroso fue el dia "<<dia<<endl;
cout<<"El promedio de la temperatura de la semana fue de: "<<prom<<endl;

}

int main()
{
    float temperaturas[7];

    for (int i = 0; i < 7; i++)
    {
        float r;
        bool exit = false;
        while (!exit)
        {
            cout << "Ingresa una temperatura para este dia de la semana (" << i + 1 << "): " << endl;
            cin >> r;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Ingrese un valor valido." << endl;
            }
            else
            {
                exit = true;
                temperaturas[i] = r;
            }
        }
    }

AnalizarTemperatura(temperaturas);

    return 0;
}