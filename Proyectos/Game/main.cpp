#include <iostream>
using namespace std;
#include <cstring> // 👈 Necesario para strcpy o strncpy

int const k = 100;

struct Arma{
char nombre[100];
char descripcion[100];
int damage = 0;

};


struct Ente{
char nombre[100]; 
int vida = 100;
int fuerza = 0;
int defensa = 0;
Arma* armaEquipada;

int calcularDamage(Ente* enemigo){


int damage = this->fuerza;

if(this->armaEquipada){
    damage+=this->armaEquipada->damage;
}

damage = damage * (k/enemigo->defensa + k);

    return damage;
}

void recibirDamage(int damage){


if((this->vida-damage) < 0){
    this->vida = 0;
} else {
    this->vida-=damage;
}


}

};




struct Partida{
Ente* Jugador;



};

int main(){
//Creacion de un arma, y asignacion de atributos
Arma* arma = new Arma;
arma->damage = 10;
strcpy(arma->nombre, "Espada");
strcpy(arma->descripcion, "Una espada vieja pero filosa");
cout<<"\n\nNombre: " << arma->nombre<<endl;
cout<<"Descripcion: " << arma->descripcion<<endl;
cout<<"Damage: " << arma->damage<<endl;

//Creacion de personaje
Ente* jugador = new Ente;
strcpy(jugador->nombre, "Starry");
jugador->vida = 100;
jugador->fuerza = 5;
jugador->defensa = 3;
jugador->armaEquipada = arma;

cout<<"\n\nNombre: " << jugador->nombre<<endl;
cout<<"Vida: " << jugador->vida<<endl;
cout<<"Fuerza: " << jugador->fuerza<<endl;
cout<<"Defensa: " << jugador->defensa<<endl;
cout<<"Arma Equipada: " << jugador->armaEquipada->nombre<<endl;






delete arma;
arma = nullptr;
delete jugador;
jugador = nullptr;

    return 0;
}