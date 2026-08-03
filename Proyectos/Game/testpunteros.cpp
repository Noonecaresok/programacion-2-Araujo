#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

struct Header {
    int cantidadRegistros;
    int proximoID;
};

struct Arma {
    int id;
    char nombre[50];
    int damage;
};

int main(){

ofstream archivo("armas.bin", ios::binary);

Header h = {3,4};

}

