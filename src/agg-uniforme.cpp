/*  Autor: Juan Miguel Gomez
    Compilar: g++ -O2 -o agg-uniforme agg-uniforme.cpp
    Ejecutar: ./agg-uniforme datos/file.txt seed
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

    //Vector con la diversidad (fitness) de cada individuo de la poblacion actual
    vector<double> population_fitness;

    //Siguiente generacion - En esta matriz utiliza para almacenar la proxima generacion en las diferentes etapas.
    vector<vector<bool>> next_generation;

    // Genera una poblacion inicial
    void initializePopulation();

    // Realiza la seleccion por el metodo de seleccion por torneo
    void selection();

    // Cruze
    void crossover();

    // Cruze
    void mutation();

    vector<bool> uniform_crossover(vector<bool> first_parent, vector<bool> second_parent);

    //Funcion que calcula la contribucion de un elemento en la solucion
    double getContribution(int idx, vector<bool> solution);

    // void repair();
    void replace();

    public:

    const int POP_SIZE = 10;

    const double CROSS_PROB = 0.8;

    const double MUT_PROB = 0.2;

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


};

int main(int argc, char const *argv[])
{

    int seed;
    string path;

    if(argc < 3){
        cout << "Error: Numero de argumentos invalido, se usaran por defecto" << endl;
        // return 1;
        path = "matrix.txt";
        seed = time(NULL);
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
    gd.geneticAlgorithm();
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
        // cout << population_fitness[i] << endl;
    }

}

void maximumDiversityProblem::selection()
{
    next_generation.resize(POP_SIZE);

    for (int i = 0; i < POP_SIZE; i++) {
        int first  =  rand() % POP_SIZE;
        int second =  rand() % POP_SIZE;

        if(population_fitness[first] > population_fitness[second]){
            next_generation[i] = population[first];
            // cout << " -> " << population_fitness[first] << endl;
        }else{
            next_generation[i] = population[second];
            // cout << " -> " << population_fitness[second] << endl;
        }
    }
}

void maximumDiversityProblem::crossover()
{
    const int CROSS_NUM = POP_SIZE * CROSS_PROB * 0.5;

    for (int i = 0; i < CROSS_NUM; i++) {
        int idx = i*2;
        vector<bool> first = uniform_crossover(population[idx],population[idx+1]);
        vector<bool> second = uniform_crossover(population[idx],population[idx+1]);

        next_generation[idx] = first;
        next_generation[idx+1] = second;
    }
}

/**    Esta funcion implementa el cruze explicado en el seminario
Vamos a estudiar como se comporta el cruze con genes no comunes
SI NO SE COMPARTE EL GEN, Se escoge aleatoriamente un padre o otro
SI SE COMPARTE EL GEN, Se escoge el gen de cualquiera de los dos, ya que es el mismo

Si tenemos en cuenta que usamos una representacion binaria
SI NO SE COMPARTE EL GEN, El gen en un padre sera 1 y en el otro 0 -> Esto es lo miso que escoger aleatoriamente 1 o 0,
ya que escogeriamos aleatoriamente un padre o otro
SI SE COMPARTE EL GEN, El gen sera el de los dos

El algoritmo va a rellenar aleatoriamete el hijo para rellenar las posiciones donde no se comparte el gen
A posteriori algoritmo va a cambiar los genes que coincidan en ambos padres al que corresponda
Por ultimo llama al reparador

El resultado es el mismo pero mas eficiente por que se generan menos numeros aleatorios
Se aprovecha/optimiza por que genero aleatoriomente los genes con muy pocos rand(), utilizando los bits del numero generado
**/
vector<bool> maximumDiversityProblem::uniform_crossover(vector<bool> first_parent, vector<bool> second_parent)
{
    vector<bool> child(n);

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
            child[nogg] = bit;
            nogg++;
            noub++;
        }
    }

    //Insertamos genes comunes
    for (int i = 0; i < n; i++) {
        if(first_parent[i] == second_parent[i]) child[i] = first_parent[i];
    }

    //Reparamos

    //Contamos los elementos seleccionados
    int v = 0;
    for (int i = 0; i < n; i++) {
        if(child[i]) v++;
    }

    // Si se necesitan mas seleccionados
    while(v<m)
    {
        int mci = 0; // Max contribution index
        double max_contribution = -1;

        for (int i = 0; i < n; i++) {

            if(!child[i]){
                double contribution = getContribution(i,child);
                if(contribution > max_contribution){
                    max_contribution = contribution;
                    mci = i;
                }
            }

        }

        child[mci] = true;
        v++;
    }

    //Si se necesitan menos seleccionados
    while(v>m)
    {
        int mci = 0; // Max contribution index
        double max_contribution = -1;

        for (int i = 0; i < n; i++) {

            if(child[i]){
                double contribution = getContribution(i,child);
                if(contribution > max_contribution){
                    max_contribution = contribution;
                    mci = i;
                }
            }

        }

        child[mci] = false;
        v--;
    }

    return child;
}

void maximumDiversityProblem::mutation()
{
    const int MUT_NUM = MUT_PROB * n;
    // cout << "mut: " << MUT_PROB * n << endl;

    for (int i = 0; i < MUT_NUM; i++) {
        int idx = rand() % POP_SIZE;
        int gen1 = rand() % n;
        int gen2;

        do gen2 = rand() % n;
        while(next_generation[idx][gen1] == next_generation[idx][gen2]);

        // cout << idx << "," << gen1 << endl << endl;

        next_generation[idx][gen1] = !next_generation[idx][gen1];
        next_generation[idx][gen2] = !next_generation[idx][gen2];
    }
}

void maximumDiversityProblem::replace()
{
    int max_fitness = -1;
    int mfi = -1;

    for (int i = 0; i < POP_SIZE; i++) {
        if(max_fitness < population_fitness[i])
        {
            mfi = i;
            max_fitness = population_fitness[i];
        }
    }

    //Sustituimos un individuo aleatorio de la poblacion por el de fitness actual
    next_generation[rand() % POP_SIZE] = population[mfi];

    population = next_generation;

    //Calculamos el fitness de cada elemento de la poblacion
    for (int i = 0; i < POP_SIZE; i++) {
        population_fitness[i] = evaluation(population[i]);
    }

}

vector<bool> maximumDiversityProblem::geneticAlgorithm()
{
    initializePopulation();

    for(int i=0; i<2000; i++){
        cout << i << endl;
        selection();
        crossover();
        mutation();
        replace();
    }

    int max_fitness = -1;
    int mfi = -1;

    for (int i = 0; i < POP_SIZE; i++) {
        if(max_fitness < population_fitness[i])
        {
            mfi = i;
            max_fitness = population_fitness[i];
        }
    }

    cout << " fitness: "  << population_fitness[mfi] << endl;

    final_solution = next_generation[mfi];

    return next_generation[mfi];
}

double maximumDiversityProblem::evaluation()
{
    double value = 0;

    vector<int> indices;

    for (int i = 0; i < n; i++) {
        if(final_solution[i]) indices.push_back(i);
    }

    //Solucion correcta
    if(indices.size() <= m){
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
    if(indices.size() <= m){
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
