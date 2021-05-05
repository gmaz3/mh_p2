#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main(int argc, char const *argv[]) {
    cout << "Hola Mundo" << endl;

    int semilla;

    if(argc < 2){
        cout << "Error" << endl;
        semilla = 1;
    }else{
        semilla = stoi(argv[1]);
    }


    cout << "La semilla: " << semilla << endl;

    srand(time(NULL));

    cout << "Aleatorio: " << rand() << endl;
    cout << "Aleatorio: " << rand() << endl;
    cout << "Aleatorio: " << rand() << endl;
    cout << "Aleatorio: " << rand() << endl;


    return 0;
}
