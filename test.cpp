#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
    cout << "Hola Mundo" << endl;

    int semilla;

    if(argc < 2){
        // cout << "Error" << endl;
        semilla = time(NULL);
    }else{
        semilla = stoi(argv[1]);
    }

    srand(semilla);

    // int random = rand() ;

    double cuenta = 0;

    for(int i = 0; i< 100000; i++){
        double random = rand() % (int) 1e8;
        random /= 1e8;

        if(random < 0.015) cuenta++;
    }


    cout << "cociente: " << cuenta / 100000.0 << endl;



    return 0;
}
