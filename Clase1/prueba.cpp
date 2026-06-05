#include <iostream>
using namespace std;


void comprobarNombreChar(char (&nombre)[40]){//Esta funcion es para ingresar nombres, con un maximo de 40 espacios, se pasa simplemente el array, como parametro, al darle a enter para finalizar el nombre, la funcion termina.
    int ultimoChar;
for( ultimoChar = 0; ultimoChar < 39; ultimoChar++){
char letra = cin.get();

if(letra == '\n'){
    break;
}
nombre[ultimoChar] = letra;
}

nombre[ultimoChar] = '\0';

}

void mostrarNombreElementos(char (&nombre)[40]){ //Funcion para mostrar elemento por elemento cada char.
    for(int i = 0; i < 39; i++){
        if(nombre[i] == '\0') {
            cout << "Elemento " << i << ": [Caracter Nulo '\\0' - Aqui termina]" << endl;
            break; // Ya no hay necesidad de seguir revisando el resto de los espacios
        }
cout<<"Elemento "<<i<<": "<<nombre[i]<<endl;


}
}

void mostrarNombre(char (&nombre)[40]){//Funcion para imprimir el nombre completo
    for(int i = 0; i < 39 && nombre[i] != '\0'; i++){
cout<<nombre[i];


}
}

void ComprobarInt(int &num){
     bool salir = false; //Aqui empieza un do while de validacion para enteros
    do
    {
    cin>>num;
if(cin.fail()){
cout<<"Valor no permitido. Intentelo de nuevo.";
cin.clear();
cin.ignore(1000, '\n');
}else{
    salir = true;
}

    } while(!salir);
salir = false;

}


int main(){

char rNombre[40];
cout<<"Nombre: ";
comprobarNombreChar(rNombre);
mostrarNombreElementos(rNombre);
mostrarNombre(rNombre);

    return 0;
}