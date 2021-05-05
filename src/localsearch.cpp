/*  Autor: Juan Miguel Gomez
    Compilar: g++ -O2 -o localsearch localsearch.cpp
    Ejecutar: ./localsearch datos/file.txt
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

    //Conjunto solucion o seleccionados
    set<int> solution;

    //Numero de elementos que tenemos que escoger del conjunto para generar la solucion
    int m;

    //Valor de la diversidad de la solucion actual: Se usa para calcular solucion factorizada
    double solutionValue;

    //Devulve un vector con las soluciones ordenadas por su aportacion
    vector<int> sortSolution();

    public:

    //Constructor por defecto
    maximumDiversityProblem();

    //Lee los datos del problema
    void readData(string path);

    //Encuentra la solucion por Busqueda Local
    set<int> findLocalSearchSolution();

    //Devuelve la contribucion (o suma acumulada de distancias) del elemento i a los elementos del conjunto sol
    double getContribution(int i, set<int> set);

    // Calcula la diversidad entre los elementos seleccionados con el metodo del MaxSum
    double evaluation();
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
    gd.findLocalSearchSolution();
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);

    cout << gd.evaluation() << "\t" << duration.count() << endl;


    return 0;
}



maximumDiversityProblem::maximumDiversityProblem():n(0), m(0), solutionValue(-1.0)
{
}

void maximumDiversityProblem::readData(string path)
{
    ifstream file;
    file.open(path);

    //Leemos el numero de filas y columnas
    file >> n >> m;

    //Inicializamos la matriz a 0
    distances.resize(n);

    //Llenamos la matriz de distancias de 0
    int zeros[n] = {0};
    for(int i=0; i < n; i++){
        distances[i] = vector<double>(zeros,zeros+n);
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

set<int> maximumDiversityProblem::findLocalSearchSolution()
{
  if(!distances.empty()){

      // Generamos solucion aleatoria
      while(solution.size() < m){
          solution.insert(rand()%n);
      }

      // La valoracion de la solucion de la que partimos
      solutionValue = evaluation();

      bool isEnd = false;
      int iterations = 0;
      int maxIter = 100000;

      // Bucle que finaliza en caso de que llegamos al maximo de iteraciones o se recorre todos los vecinos sin encontrar solucion mejor
      while(!isEnd){
          // sorted es un vector con los elementos de selecionados ordenados por su contribucion
          vector<int> sorted = sortSolution();
          bool hasImproved = false;
          int i = 0;

          // Elemento candidato a extraerse de selecionados; Elemento candidato a introducirse en selecionados
          int item2pull, item2push;
          // Contribuciones del elemento a extraerse e introducirse respectivamente
          double cont2pull, cont2push, delta;

          // Mientras no mejoremos la solucion y no hayamos recorrido todos los elementos de seleccionados
          while(!hasImproved && !isEnd){
              // Obtenemos el siguiente elemento candidato a extrerse, que sera el que menos contribuya de los restantes
              item2pull = sorted[i];
              //Calculamos la contribucion
              cont2pull = getContribution(item2pull,solution);
              int j = 0;

              // Selecionados sin el elemento candidato a extraerse
              set<int> new_selected(solution);
              new_selected.erase(item2pull);

              // Mientras no mejoremos la solucion y no hayamos recorrido todos los elementos que se pueden introducir
              while(!hasImproved && !isEnd && j < n){
                  if(solution.find(j) == solution.end()){ // Comprueba que el elemento no esta en selecionados => EVITA SOLUCION INCORRECTA
                      item2push = j;
                      cont2push = getContribution(item2push, new_selected);

                      // Diferencia entre las contribuciones
                      delta = cont2push - cont2pull;

                      iterations++;

                      // Si la diferencia es positiva hemos encontrado uno que mejora y salimos para hacer el cambio => BUSQUEDA LOCAL DEL PRIMER MEJOR
                      hasImproved = delta > 0;
                      isEnd = iterations > maxIter;
                  }

                  j++;
              }

              i++;
              isEnd = i == sorted.size() || iterations > maxIter;
          }

          // Si hay mejora la solucion hace el intercambio en seleccionados y actualiza el valor de la solucion actual sin recalcular todo
          if(hasImproved){
              solution.erase(item2pull);
              solution.insert(item2push);
              solutionValue += delta;
          }
      }

  }else{
    cout << "Error: Se deben leer antes las distancias" << endl;
  }

  // cout << "valor de solucion: " << solutionValue << endl;
  return solution;
}

double maximumDiversityProblem::evaluation()
{
    double value = -1;
    //Si hemos generado la solucion
    if(solution.size() == m){
        set<int>::iterator it = solution.begin();
        auto end = prev(solution.end(),1);
        value = 0;

        for(it; it != end; it++){
            auto sub_it = next(it,1);
            for(sub_it; sub_it != solution.end(); sub_it++){
                value += distances[*it][*sub_it];
            }
        }
    }

    return value;
}

vector<int> maximumDiversityProblem::sortSolution()
{
    vector<int> sort_solution(solution.begin(),solution.end());
    vector<double> set_distances;


    auto it = solution.begin();

    // Calcula las distancias (contribucion en la diversidad) de cada elemento de seleccionados al resto en un vector
    for(it; it != solution.end(); it++){
        set_distances.push_back(getContribution(*it,solution));
    }

    int lower_idx;
    double lower_dis;

    // Ordenamos selecionados por orden de menor contribucion usando el vector anterior
    for(int i=0; i<m; i++){
        int idx = i;
        for(int j=i; j<m; j++){
            if(set_distances[j] < set_distances[idx]){
                idx = j;
            }
        }

        // Intercambio
        lower_dis = set_distances[idx];
        set_distances[idx] = set_distances[i];
        set_distances[i] = lower_dis;

        lower_idx = sort_solution[idx];
        sort_solution[idx] = sort_solution[i];
        sort_solution[i] = lower_idx;

    }

    return sort_solution;
}

double maximumDiversityProblem::getContribution(int i, set<int> set)
{
    double accum = 0;

    auto it = set.begin();
    for(it; it != set.end(); it++){
        accum += distances[i][*it];
    }

    return accum;
}
