/*  Autor: Juan Miguel Gomez
    Compilar: g++ -O2 -o age-posicion age-posicion.cpp
    Ejecutar: ./age-posicion datos/file.txt seed
*/

#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <chrono>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

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

    //Vector con la diversidad (fitness) de cada individuo de la poblacion actual
    vector<double> population_fitness;

    //Siguiente generacion - En esta matriz utiliza para almacenar la proxima generacion en las diferentes etapas.
    vector<vector<bool>> next_generation;

    // Genera una poblacion inicial
    void initializePopulation();

    // Realiza la seleccion por el metodo de seleccion por torneo
    void selection();

    // Realiza el cruze llamando a la funcion uniform_crossover 2 veces
    void crossover();

    // Realiza la mutacion
    void mutation();

    // Cruze uniforme (intento de optimizacion)
    vector<bool> position_crossover(vector<bool> first_parent, vector<bool> second_parent);

    //Funcion que calcula la contribucion de un elemento en la solucion
    double getContribution(int idx, vector<bool> solution);

    // Realiza el cambio de generacion manteniendo la mejor solucion
    void replace();

    public:

    const int POP_SIZE = 50;

    const int NG_SIZE = 2;

    const double CROSS_PROB = 0.7;

    const double MUT_PROB = 0.1;

    //Constructor por defecto
    maximumDiversityProblem();

    //Lee los datos del problema
    void readData(string path);

    //Encuentra la solucion por Busqueda Local
    vector<bool> geneticAlgorithm();

    // Calcula la diversidad entre los elementos seleccionados con el metodo del MaxSum
    double evaluation(vector<bool> solution);

    // Calcula la diversidad entre los elementos seleccionados con el metodo del MaxSum
    double evaluation();

    bool ordenFitness (vector<bool> a, vector<bool> b);
};

int main(int argc, char const *argv[])
{
    if(argc < 3){
        cout << "Error: Numero de argumentos invalido" << endl;
        return 1;
    }

    int seed = stoi(argv[2]);
    srand(seed);

    // Declaramos el tipo y hacemos que lea los datos
    maximumDiversityProblem gd;
    gd.readData(argv[1]);

    // Cronometramos el tiempo en ms
    auto start = high_resolution_clock::now();
    gd.geneticAlgorithm();
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);

    cout << gd.evaluation() << "\t" << duration.count() << endl;


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

void maximumDiversityProblem::initializePopulation()
{
    int random;
    population.resize(POP_SIZE);
    population_fitness.resize(POP_SIZE);

    //Inicializamos la poblacion a false
    for (int i = 0; i < POP_SIZE; i++) {
        for (int j = 0; j < n; j++) {
            population[i].push_back(false);
        }
    }

    //Generamos la poblacion aleatoriamente
    for (int i = 0; i < POP_SIZE; i++) {
        int nosi = 0; // Number of selected items
        while(nosi < m){
            int idx = rand()%n;
            if(!population[i][idx]){
                population[i][idx] = true;
                nosi++;
            }
        }
    }

    //Calculamos el fitness de cada elemento de la poblacion
    for (int i = 0; i < POP_SIZE; i++) {
        population_fitness[i] = evaluation(population[i]);
    }

    //Ordenamos la poblacion por fitness
    int i, j;
    for (i = 0; i < POP_SIZE-1; i++){
        double min_fitness = population_fitness[i];
        int mfi = i;

        for (j = i+1; j < POP_SIZE; j++) {
            if (population_fitness[j] < min_fitness){
                min_fitness = population_fitness[j];
                mfi = j;
            }
        }

        vector<bool> aux = population[mfi];
        population_fitness[mfi] = population_fitness[i];
        population[mfi] = population[i];
        population_fitness[i] = min_fitness;
        population[i] = aux;
    }

}

void maximumDiversityProblem::selection()
{
    next_generation.resize(NG_SIZE);

    for (int i = 0; i < NG_SIZE; i++) {
        int first  =  rand() % POP_SIZE;
        int second =  rand() % POP_SIZE;

        if(population_fitness[first] > population_fitness[second]){
            next_generation[i] = population[first];
        }else{
            next_generation[i] = population[second];
        }
    }
}

void maximumDiversityProblem::crossover()
{
    vector<bool> first = position_crossover(next_generation[0],next_generation[1]);
    vector<bool> second = position_crossover(next_generation[0],next_generation[1]);

    next_generation[0] = first;
    next_generation[1] = second;
}

vector<bool> maximumDiversityProblem::position_crossover(vector<bool> first_parent, vector<bool> second_parent)
{
    vector<bool> child(n);
    vector<bool> rest;
    vector<int> indices;

    //Insertamos genes comunes
    for (int i = 0; i < n; i++) {
        if(first_parent[i] == second_parent[i]){
            child[i] = first_parent[i];
        }else{
            rest.push_back(first_parent[i]);
            indices.push_back(i);
        }
    }

    random_shuffle (rest.begin(), rest.end());

    for (int i=0; i < rest.size(); i++) {
        child[indices[i]] = rest[i];
    }

    return child;
}

void maximumDiversityProblem::mutation()
{
    for (int i = 0; i < NG_SIZE; i++) {
        //Generamos un real aleatorio entre 0-1
        double random = rand() % (int) 1e8;
        random /= 1e8;

        if(random < MUT_PROB){
            int gen1 = rand() % n;
            int gen2;

            do gen2 = rand() % n;
            while(next_generation[i][gen1] == next_generation[i][gen2]);

            next_generation[i][gen1] = !next_generation[i][gen1];
            next_generation[i][gen2] = !next_generation[i][gen2];
        }
    }
}

void maximumDiversityProblem::replace()
{
    double fchild_fitness = evaluation(next_generation[0]);
    double schild_fitness = evaluation(next_generation[1]);

    //PRIMER HIJO
    //Busca el primer elemento de mayor fitness
    auto idx = lower_bound(population_fitness.begin(),population_fitness.end(),fchild_fitness);

    //Desplaza los de menor fitness hacia abajo y remplaza en la posicion que corresponde
    if(idx != population_fitness.begin()){
        int end = (idx - population_fitness.begin() - 1);

        for(int i=1; i <= end; i++){
            population_fitness[i-1] = population_fitness[i];
            population[i-1] = population[i];
        }
        population[end] = next_generation[0];
        population_fitness[end] = fchild_fitness;
    }

    //SEGUNDO HIJO
    //Busca el primer elemento de mayor fitness
    idx = lower_bound(population_fitness.begin(),population_fitness.end(),schild_fitness);

    //Desplaza los de menor fitness hacia abajo y remplaza en la posicion que corresponde
    if(idx != population_fitness.begin()){
        int end = (idx - population_fitness.begin() - 1);

        for(int i=1; i <= end; i++){
            population_fitness[i-1] = population_fitness[i];
            population[i-1] = population[i];
        }
        population[end] = next_generation[1];
        population_fitness[end] = schild_fitness;
    }

}

vector<bool> maximumDiversityProblem::geneticAlgorithm()
{
    initializePopulation();

    // Cada iteracion son 2 evaluaciones de la funcion objetivo -> 100.000 / 2 = 50.000
    for(int i=0; i<50000; i++){
        selection();
        crossover();
        mutation();
        replace();
    }

    final_solution = population[POP_SIZE-1];

    return population[POP_SIZE-1];
}

double maximumDiversityProblem::evaluation()
{
    double value = 0;

    vector<int> indices;

    for (int i = 0; i < n; i++) {
        if(final_solution[i]) indices.push_back(i);
    }

    //Solucion correcta
    if(indices.size() == m){
        for (int i = 0; i < indices.size() - 1; i++) {
            for (int j = i+1; j < indices.size(); j++) {
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

    for (int i = 0; i < n; i++) {
        if(solution[i]) indices.push_back(i);
    }

    //Solucion correcta
    if(indices.size() == m){
        for (int i = 0; i < indices.size() - 1; i++) {
            for (int j = i+1; j < indices.size(); j++) {
                value += distances[indices[i]][indices[j]];
            }
        }
    }else{
        cout << "Error: La solucion no es correcta" << endl;
    }

    return value;
}

double maximumDiversityProblem::getContribution(int idx, vector<bool> solution)
{
    double accum = 0;

    for(int i = 0; i<n; i++){
        if(solution[i]) accum += distances[idx][i];
    }

    return accum;
}
