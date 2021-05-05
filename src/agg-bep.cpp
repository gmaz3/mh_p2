/*  Autor: Juan Miguel Gomez
    Compilar: g++ -O2 -o agg-bep agg-bep.cpp
    Ejecutar: ./agg-bep datos/file.txt num_semilla
*/


#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <chrono>
#include <string>
#include <stdlib.h>
#include <time.h>

#define MAX 100000

using namespace std;
using namespace std::chrono;

class maximumDiversityProblem
{
    private:
    //Matriz de distancias (simetrica para trabajar sin complicaciones)
    vector<vector<double>> distances;

    //Tamanio del conjunto de los datos
    int n;

    //Conjunto solucion o seleccionados: True -> Escogido, False -> No Escogido
    vector<bool> final_solution;

    //Numero de elementos que tenemos que escoger del conjunto para generar la solucion
    int m;

    //Poblacion actual
    vector<vector<bool>> population;

    public:

    const int POP_SIZE = 50;

    //Constructor por defecto
    maximumDiversityProblem();

    //Lee los datos del problema
    void readData(string path);

    //Encuentra la solucion por Busqueda Local
    vector<bool> findLocalSearchSolution();

    // Calcula la diversidad entre los elementos seleccionados con el metodo del MaxSum
    double evaluation(vector<bool> solution);

    // Calcula la diversidad entre los elementos seleccionados con el metodo del MaxSum
    double evaluation();

    // Genera una poblacion inicial
    void initializePopulation();
};

int main(int argc, char const *argv[])
{

    int seed;
    string path;

    if(argc < 3){
        cout << "Error: Numero de argumentos invalido, se usaran por defecto" << endl;
        // return 1;
        path = "matrix.txt";
        seed = 1;
    }else{
        path = argv[1];
        seed = stoi(argv[2]);
    }

    srand(seed);

    // Declaramos el tipo y hacemos que lea los datos
    maximumDiversityProblem gd;
    gd.readData(path);

    // Cronometramos el tiempo en ms
    // auto start = high_resolution_clock::now();
    gd.findLocalSearchSolution();
    // auto stop = high_resolution_clock::now();

    // auto duration = duration_cast<microseconds>(stop - start);

    // cout << gd.evaluation() << "\t" << duration.count() << endl;


    return 0;
}

maximumDiversityProblem::maximumDiversityProblem():n(0), m(0)
{
}

void maximumDiversityProblem::readData(string path)
{
    ifstream file;
    file.open(path);

    //Leemos el numero de filas y columnas
    file >> n >> m;

    //Establecemos el tamano de las soluciones
    final_solution.resize(n);

    //Inicializamos la matriz a 0
    distances.resize(n);

    //Inicializamos la matriz de distancias de 0 y el vecotr solucion a falso
    int zeros[n] = {0};
    for(int i=0; i < n; i++){
        distances[i] = vector<double>(zeros,zeros+n);
        final_solution[i] = false;
    }

    int i, j;
    double value;

    //Leemos el fichero completo e introducimos los valores a la matriz
    while(!file.eof()){
        file >> i >> j >> value;
        distances[i][j] = value;
        distances[j][i] = value; // Matriz simetrica pq es mas sencillo medir distancias
    }

    file.close();
}

void initializePopulation()
{
    int aleatorio = rand();
}

vector<bool> maximumDiversityProblem::findLocalSearchSolution()
{

    // for (size_t i = 0; i < n; i++) {
    //     for (size_t j = 0; j < n; j++) {
    //         cout << distances [i][j] << "\t" ;
    //     }
    //     cout << endl;
    // }


    // final_solution[2] = true;
    // final_solution[7] = true;
    // final_solution[0] = true;
    //
    //
    // for (size_t i = 0; i < n; i++) {
    //     cout << final_solution[i] << " " ;
    // }

    // evaluation();
    cout << "fasdf" << endl;

    cout << evaluation() << endl;

    return final_solution;
}

double maximumDiversityProblem::evaluation()
{
    double value = 0;

    vector<int> indices;

    for (size_t i = 0; i < n; i++) {
        if(final_solution[i]) indices.push_back(i);
    }

    //Solucion correcta
    if(indices.size() <= m){
        for (size_t i = 0; i < indices.size() - 1; i++) {
            for (size_t j = i+1; j < indices.size(); j++) {
                value += distances[indices[i]][indices[j]];
            }
        }
    }else{
        cout << "Error: La solucion no es correcta" << endl;
    }

    return value;
}

double maximumDiversityProblem::evaluation(vector<bool> solution)
{
    double value = 0;

    vector<int> indices;

    for (size_t i = 0; i < n; i++) {
        if(solution[i]) indices.push_back(i);
    }

    //Solucion correcta
    if(indices.size() <= m){
        for (size_t i = 0; i < indices.size() - 1; i++) {
            for (size_t j = i+1; j < indices.size(); j++) {
                value += distances[indices[i]][indices[j]];
            }
        }
    }else{
        cout << "Error: La solucion no es correcta" << endl;
    }

    return value;
}
