#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

void saludar(){
    cout << "hola amigo" << endl;
    int df = 0;

    for (size_t i = 0; i < 1000000; i++) {
        df += i*4;
    }
}

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


    cout << "otra cosa" << endl;

    int n  = 65;

    saludar();


    std::vector<bool> v (n);

    v[4] = true;

    for (size_t i = 0; i < n; i++) {
        cout << v[i] << endl;
    }



    return 0;
}
