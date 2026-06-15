#include <iostream>
using namespace std;

int const k = 100;

struct Arma{
char nombre[100];
char descripcion[100];
int damage;

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

int recibirDamage(){


}

};