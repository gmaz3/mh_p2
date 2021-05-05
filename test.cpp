#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

int main(int argc, char const *argv[]) {
    cout << "Hola Mundo" << endl;


    if(argc < 2){
        cout << "Error" << endl;
        return 1;
    }

    int semilla = stoi(argv[1]);

    cout << "La semilla: " << semilla << endl;

    srand(semilla);

    cout << "Aleatorio: " << rand() << endl;
    cout << "Aleatorio: " << rand() << endl;
    cout << "Aleatorio: " << rand() << endl;
    cout << "Aleatorio: " << rand() << endl;


    return 0;
}
