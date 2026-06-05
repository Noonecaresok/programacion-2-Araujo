#include <iostream>
#include <iomanip>
using namespace std;

struct Materia {
    int codigo;
    char nombre[40];
    int creditos;
};

struct Estudiante {
    int id;
    char nombre[40];
    float promedio;
    int codigoMateriaPrincipal;
};

struct Escuela {
    Estudiante* estudiantes;
    int cantidadEstudiantes;
    Materia* materias;
    int cantidadMaterias;
};

void ComprobarInt(int &num){
    bool salir = false; //Aqui empieza un do while de validacion para enteros
    do {
        cin>>num;
        if(cin.fail()){
            cout<<"Valor no permitido. Intentelo de nuevo."<<endl;
            cin.clear();
            cin.ignore(1000, '\n');
        }else{
            salir = true;
        }
    } while(!salir);
}

void comprobarNombreChar(char (&nombre)[40]){//Esta funcion es para ingresar nombres, con un maximo de 40 espacios, se pasa simplemente el array, como parametro, al darle a enter para finalizar el nombre, la funcion termina.
    cin.ignore(1000, '\n');
    
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
    cout << endl;
}

void crearEscuela(Escuela& escuela, int cantEst, int cantMat){
    if(cantEst <= 0 || cantMat <= 0){
        cout<<"La cantidad de estudiantes y materias deben ser mayores a 0."<<endl;
        escuela.estudiantes = nullptr;
        escuela.materias = nullptr;
        return;
    }

    escuela.estudiantes = new Estudiante[cantEst];
    escuela.materias = new Materia[cantMat];
    escuela.cantidadEstudiantes = cantEst;
    escuela.cantidadMaterias = cantMat;
    cout<<"Escuela creada exitosamente."<<endl;
}

void llenarMaterias(Escuela& escuela) {
    if(escuela.materias == nullptr){
        cout<<"ERROR: materias = nullptr"<<endl;
        return;
    }

    int rCodigo;
    int rCreditos;

    cout<<"==A continuacion, ingresara los datos de las materias=="<<endl;
    for(int i = 0; i < escuela.cantidadMaterias; i++){
        cout<<"\nMateria "<<i + 1<<endl; //Materia i

        bool idexistente = false;
        do {
            idexistente = false;
            cout<<"Codigo: "; //Empezamos por pedir el codigo de la meteria
            ComprobarInt(rCodigo);
            for(int j = 0; j < i; j++){
                if(escuela.materias[j].codigo == rCodigo){
                    idexistente = true;
                    cout<<"El codigo ingresado, ya existe, intente otro."<<endl;
                    break;
                }
            }
        } while (idexistente);

        escuela.materias[i].codigo = rCodigo;

        cout<<"Nombre: "; //Ahora pedimos el nombre de la materia
        comprobarNombreChar(escuela.materias[i].nombre);

        cout<<"Creditos: "; //Ahora pedimos los creditos que vale la materia
        ComprobarInt(rCreditos);
        escuela.materias[i].creditos = rCreditos;
    }
}

void llenarEstudiantes(Escuela& escuela){
    if(escuela.estudiantes == nullptr){
        cout<<"ERROR: estudiantes = nullptr"<<endl;
        return;
    }
    int rID;
    float rPromedio;
    int rCodEstudiante;

    cout<<"==A continuacion, ingresara los datos de los estudiantes=="<<endl;
    for(int i = 0; i < escuela.cantidadEstudiantes; i++){
        cout<<"\nEstudiante "<<i + 1<<endl; //Estudiante i
        bool idexistente = false;

        do {
            idexistente = false;
            cout<<"ID: "; //Empezamos por pedir el ID del estudiante
            ComprobarInt(rID); 
            for(int j = 0; j < i; j++){
                if(escuela.estudiantes[j].id == rID){
                    idexistente = true;
                    cout<<"El ID ingresado, ya existe, intente otro."<<endl;
                    break;
                }
            }
        } while (idexistente);

        escuela.estudiantes[i].id = rID;

        cout<<"Nombre: "; //Ahora pedimos el nombre del estudiante
        comprobarNombreChar(escuela.estudiantes[i].nombre);

        cout<<"Promedio: "; //Ahora pedimos el promedio del estudiante
        bool salir = false;
        do {
            cin>>rPromedio; 
            if(cin.fail() || rPromedio < 0 || rPromedio > 20){
                cout<<"Valor no permitido. Intentelo de nuevo: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }else{
                salir = true;
            }
        } while(!salir);
        escuela.estudiantes[i].promedio = rPromedio;

        cout<<"Codigo materia principal: "; //Ahora pedimos el cod de materia del estudiante
        salir = false;
        do {
            cout<<"\n\nCodigos disponibles: "<<endl;
            for(int j = 0; j < escuela.cantidadMaterias; j++){
                cout<<"Materia "<< j + 1<<endl;
                cout<<"Nombre: "<<escuela.materias[j].nombre<<endl;
                cout<<"Creditos: "<<escuela.materias[j].creditos<<endl;
                cout<<"Codigo: "<<escuela.materias[j].codigo<<endl;
            }

            cin>>rCodEstudiante; 
            if(cin.fail()){
                cout<<"Valor no permitido. Intentelo de nuevo.";
                cin.clear();
                cin.ignore(1000, '\n');
            }else{
                for(int j = 0; j < escuela.cantidadMaterias; j++){
                    if(escuela.materias[j].codigo == rCodEstudiante){
                        cout<<"Codigo encontrado."<<endl;
                        escuela.estudiantes[i].codigoMateriaPrincipal = rCodEstudiante;
                        salir = true;
                        break;
                    }
                }
                if(salir == false){
                    cout<<"No se ha encontrado el codigo, Ingrese un codigo disponible."<<endl;
                }
            }
        } while(!salir);
    }
}

void mostrarEscuela(const Escuela& escuela){
    if(escuela.estudiantes == nullptr || escuela.materias == nullptr){
        cout<<"No se ha podido mostrar la escuela, porque estudiantes o materias es nullptr."<<endl;
        return;
    }
    
    cout<<"\nLista de materias\n"<<endl;
    cout<<left;
    cout<<setw(25)<<"Nombre"<< setw(12) << "Codigo"<< setw(10) <<"Creditos"<<endl;
    for(int i = 0; i < escuela.cantidadMaterias; i++){
        cout<<setw(25)<<escuela.materias[i].nombre<< setw(12) << escuela.materias[i].codigo<< setw(10) <<escuela.materias[i].creditos<<endl;
    }
    
    cout<<"\nLista de estudiantes\n"<<endl;
    cout<<left;
    cout<<setw(12)<<"ID"<< setw(25) << "Nombre"<< setw(12) <<"Promedio"<<setw(25)<<"Codigo materia principal"<<endl;
    for(int i = 0; i < escuela.cantidadEstudiantes; i++){
        cout<<setw(12)<<escuela.estudiantes[i].id<< setw(25) << escuela.estudiantes[i].nombre<< setw(12) <<escuela.estudiantes[i].promedio<< setw(25) <<escuela.estudiantes[i].codigoMateriaPrincipal<<endl;
    }
}

int buscarMejorPromedio(const Escuela& escuela){
    if(escuela.estudiantes == nullptr || escuela.cantidadEstudiantes <= 0){
        return -1;
    }

    float maxprom = escuela.estudiantes[0].promedio;
    int indiceMax = 0;

    for(int i = 1; i < escuela.cantidadEstudiantes; i++){
        if(escuela.estudiantes[i].promedio > maxprom){
            maxprom = escuela.estudiantes[i].promedio;
            indiceMax = i;
        }
    }

    return indiceMax;
}

float calcularPromedioGeneral(const Escuela& escuela) {
    if(escuela.estudiantes == nullptr || escuela.cantidadEstudiantes <= 0){
        return -1;
    }

    float promgeneral = 0;
    for(int i = 0; i < escuela.cantidadEstudiantes; i++){
        promgeneral+=escuela.estudiantes[i].promedio;
    }

    return promgeneral / escuela.cantidadEstudiantes; 
}

int buscarEstudiantePorId(const Escuela& escuela, int id) {
    if(escuela.estudiantes == nullptr){
        return -1;
    }
    
    for(int i = 0; i < escuela.cantidadEstudiantes; i++){
        if(escuela.estudiantes[i].id == id){
            return i;
        }
    }

    return -1;
}

void liberarEscuela(Escuela& escuela) {
    if (escuela.estudiantes != nullptr) {
        delete[] escuela.estudiantes;
        escuela.estudiantes = nullptr;
    }
    if (escuela.materias != nullptr) {
        delete[] escuela.materias;
        escuela.materias = nullptr;
    }
    escuela.cantidadEstudiantes = 0;
    escuela.cantidadMaterias = 0;
}

void mostrarMenu() {
    cout << "\n=== GESTION DE ESTUDIANTES ===" << endl;
    cout << "1. Crear y llenar escuela" << endl;
    cout << "2. Mostrar escuela" << endl;
    cout << "3. Estudiante con mejor promedio" << endl;
    cout << "4. Promedio general" << endl;
    cout << "5. Buscar por id" << endl;
    cout << "6. Salir" << endl;
    cout << "Seleccione una opcion: ";
}

int main() {
    Escuela escuela;
    escuela.estudiantes = nullptr;
    escuela.materias = nullptr;
    escuela.cantidadEstudiantes = 0;
    escuela.cantidadMaterias = 0;

    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int cantEst = 0;
                int cantMat = 0;
                cout << "Ingrese la cantidad de estudiantes: ";
                cin >> cantEst;
                cout << "Ingrese la cantidad de materias: ";
                cin >> cantMat;

                liberarEscuela(escuela);
                crearEscuela(escuela, cantEst, cantMat);

                if (escuela.estudiantes != nullptr && escuela.materias != nullptr) {
                    llenarMaterias(escuela);
                    llenarEstudiantes(escuela);
                    cout << "Escuela creada y llenada correctamente." << endl;
                } else {
                    cout << "Error al crear la escuela." << endl;
                }
                break;
            }

            case 2: {
                if (escuela.estudiantes != nullptr && escuela.materias != nullptr) {
                    mostrarEscuela(escuela);
                } else {
                    cout << "No hay escuela creada. Use la opcion 1 primero." << endl;
                }
                break;
            }

            case 3: {
                if (escuela.estudiantes != nullptr && escuela.cantidadEstudiantes > 0) {
                    int indice = buscarMejorPromedio(escuela);
                    if (indice >= 0) {
                        cout << "Mejor promedio: " << escuela.estudiantes[indice].nombre
                             << " (" << fixed << setprecision(2)
                             << escuela.estudiantes[indice].promedio << ")" << endl;
                    } else {
                        cout << "No hay estudiantes en la lista." << endl;
                    }
                } else {
                    cout << "No hay escuela creada o esta vacia." << endl;
                }
                break;
            }

            case 4: {
                if (escuela.estudiantes != nullptr && escuela.cantidadEstudiantes > 0) {
                    float promedio = calcularPromedioGeneral(escuela);
                    cout << "Promedio general: " << fixed << setprecision(2)
                         << promedio << endl;
                } else {
                    cout << "No hay escuela creada o esta vacia." << endl;
                }
                break;
            }

            case 5: {
                if (escuela.estudiantes != nullptr && escuela.cantidadEstudiantes > 0) {
                    int idBuscado;
                    cout << "Ingrese el ID del estudiante a buscar: ";
                    cin >> idBuscado;
                    
                    int indice = buscarEstudiantePorId(escuela, idBuscado);
                    if (indice >= 0) {
                        cout << "Encontrado: " << escuela.estudiantes[indice].nombre
                             << " (" << fixed << setprecision(2)
                             << escuela.estudiantes[indice].promedio << ")" << endl;
                    } else {
                        cout << "No se encontro un estudiante con ese id." << endl;
                    }
                } else {
                    cout << "No hay escuela creada o esta vacia." << endl;
                }
                break;
            }

            case 6: {
                cout << "Saliendo del programa..." << endl;
                break;
            }

            default: {
                cout << "Opcion invalida. Intente nuevamente." << endl;
                cin.clear();
                cin.ignore(1000, '\n');
                break;
            }
        }
    } while (opcion != 6);

    liberarEscuela(escuela);
    cout << "Programa finalizado. Memoria liberada." << endl;
    return 0;
}