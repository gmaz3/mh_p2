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

    //
    // cout << "La semilla: " << semilla << endl;
    //
    // srand(time(NULL));
    //
    // cout << "Aleatorio: " << rand() << endl;
    // cout << "Aleatorio: " << rand() << endl;
    // cout << "Aleatorio: " << rand() << endl;
    // cout << "Aleatorio: " << rand() << endl;

    int n = 5;
    int m = 2;

    bool first_parent [5];
    bool second_parent[5];

    // Rellenar aleatoriamente el hijo.
    int random;
    int nogg = 0; //Number of generated genes

    while(nogg < n){
        random = rand();
        // cout << " " << random << endl;
        int noub = 0; //Number of used bits

        while(noub < 32 && nogg < n){
            bool bit = random & 0x00000001;
            random =  random >> 1;
            first_parent[nogg] = bit;
            nogg++;
            noub++;
        }
    }
    // Rellenar aleatoriamente el hijo.
    // int random;
    // int nogg = 0; //Number of generated genes
    nogg = 0;

    while(nogg < n){
        random = rand();
        // cout << " " << random << endl;
        int noub = 0; //Number of used bits

        while(noub < 32 && nogg < n){
            bool bit = random & 0x00000001;
            random =  random >> 1;
            second_parent[nogg] = bit;
            nogg++;
            noub++;
        }
    }

    for (size_t i = 0; i < n; i++) {
        cout << first_parent[i];
    }
    cout << endl;

    for (size_t i = 0; i < n; i++) {
        cout << second_parent[i];
    }
    cout << endl;

    for (size_t i = 0; i < n; i++) {
        cout << " -";
    }
    cout << endl;


    vector<bool> child(n);

    // Rellenar aleatoriamente el hijo.
    // int random;
    nogg = 0; //Number of generated genes

    while(nogg < n){
        random = rand();
        // cout << " " << random << endl;
        int noub = 0; //Number of used bits

        while(noub < 32 && nogg < n){
            bool bit = random & 0x00000001;
            random =  random >> 1;
            child[nogg] = bit;
            nogg++;
            noub++;
        }
    }

    //Insertamos genes comunes
    for (size_t i = 0; i < n; i++) {
        if(first_parent[i] == second_parent[i]) child[i] = first_parent[i];
    }

    for (size_t i = 0; i < n; i++) {
        cout << child[i];
    }
    cout << endl;

    //Reparamos

    //Contamos los elementos seleccionados
    int v = 0;
    for (size_t i = 0; i < n; i++) {
        if(child[i]) v++;
    }

    cout << "cuenta: " << v << endl;

    cout << "raparando" << endl;

    while(v<m)
    {
        int mci = 0; // Max contribution index
        double max_contribution = -1;

        for (size_t i = 0; i < n; i++) {

            if(!child[i]){
                double contribution = getContribution(i,child);
                if(contribution > max_contribution){
                    max_contribution = contribution;
                    mci = i;
                }
            }

        }

        child[mci] = true;
        m++;

        

        for (size_t i = 0; i < n; i++) {
            cout << child[i];
        }
        cout << endl;

    }

    return 0;
}
