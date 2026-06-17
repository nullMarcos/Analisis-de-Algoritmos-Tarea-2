#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include "Graph.h"
#include "Floyd_Warshall.h"

using namespace std;
using namespace std::chrono;

int main(){

    //Setup inicial
    vector<string> file_names = {"../../data/power-685-bus.mtx", "../../data/bio-SC-TS.edges"};
    vector<string> csv_names  = {"../../data/resultados_power_685_bus.csv", "../../data/resultados_bio_SC_TS.csv"};

    double inf = 1e15;

    for (size_t i = 0; i < file_names.size(); i++){

        //Carga del dataset
        Graph<double> graph = Graph<double>::create_graph_from_file(file_names[i]);
        vector<vector<double>> matrix = graph.toAdjacencyMatrix(inf);

        //Ejecucion y medicion del tiempo del algoritmo
        cout << "Ejecutando Floyd-Warshall para el dataset: " << file_names[i] << endl;

        auto start = high_resolution_clock::now();

        bool succes = floyd_warshall(matrix, inf);

        auto end = high_resolution_clock::now();

        //Calculo del tiempo de ejecucion
        auto duration = duration_cast<milliseconds>(end - start);
        cout << "Tiempo de ejecucion: " << duration.count() << " ms" << endl;

        if (!succes){
            cout << "El grafo tiene un ciclo negativo" << endl;
        }
        else{
            cout << "El grafo no tiene ciclos negativos" << endl;
        }

        //Guardado de resultados en CSV
        ofstream csv_file(csv_names[i]);
        if (!csv_file.is_open()){
            cerr << "Error al abrir el archivo CSV: " << csv_names[i] << endl;
            exit(1);
        }

        //Cabecera del CSV
        csv_file << "Tiempo de ejecucion (ms): " << duration.count() << "\n";
        csv_file << "Ciclo negativo: " << (succes ? "No" : "Si") << "\n";
        csv_file << "INF se define como: " << inf << "\n";
        csv_file << "nodo_1, nodo_2, distancia_minima" << "\n";

        size_t n = matrix.size();

        for (size_t u = 0; u < n; u++){
            for (size_t v = u; v < n; v++){

                //Sumamos 1 a cada nodo para 1-indexar de nuevo
                csv_file << (u+1) << ", " << (v+1) << ", ";

                //Si la distancia es infinita escribimos INF, sino escribimos la distancia
                if (matrix[u][v] > inf/2){
                    csv_file << "INF" << "\n";
                }
                else{
                    csv_file << matrix[u][v] << "\n";
                }
            }
        }
        csv_file.close();
    }
    return 0;
}