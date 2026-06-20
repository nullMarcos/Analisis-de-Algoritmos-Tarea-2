#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <string>

#include "Graph.h"
#include "Floyd_Warshall.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]){

    if(argc < 3){
        cout << "Error: Faltan parametros.\n";
        cout << "Uso: " << "./Ejecutable" << " <archivo.mtx / archivo.edge> <1 para dirigido / 0 para no dirigido>.\n";
        return 1;
    }

    //Captura de los argumentos de la terminal
    string file_name = argv[1];
    bool is_directed = string(argv[2]) == "1";

    size_t point_posicion = file_name.find_last_of('.');
    string base_name = file_name.substr(0, point_posicion);

    string ruta_dataset = "../../data/dataset/" + file_name;
    string ruta_csv     = "../../data/csv/resultados_" + base_name + ".csv";
    string ruta_txt     = "../../data/txt/ciclos_" + base_name + ".txt";

    //Declaracion de lo que se considera infinito
    double inf = 1e15;

    //CICLO PRINCIPAL PARA ITERAR SOBRE CADA ARCHIVO
    // for (size_t i = 0; i < file_names.size(); i++){

    //Cargamos el grafo a partir del archivo
    pair<Graph<double>, bool> result = Graph<double>::create_graph_from_file(ruta_dataset, is_directed);

    //Desempaquetamos el resultado (Objeto Graph, valor booleano)
    Graph<double> graph = result.first;
    bool is_one_indexed = result.second;

    //Obtenemos la matriz de adyacencia para la ejecucion del algoritmo
    vector<vector<double>> matrix = graph.toAdjacencyMatrix(inf);

    //Creamos una matriz de tamaño nxn repleta de -1's para la matriz de rutas
    size_t n = matrix.size();
    vector<vector<int>> next_node(n, vector<int>(n, -1));

    //Ejecucion y medicion del tiempo del algoritmo
    cout << "Ejecutando Floyd-Warshall para el dataset: " << ruta_dataset << endl;

    auto start = high_resolution_clock::now();

    bool succes = floyd_warshall(matrix, next_node, inf);

    auto end = high_resolution_clock::now();

    //Calculo del tiempo de ejecucion
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Tiempo de ejecucion: " << duration.count() << " ms" << endl;

    //Si el dataset estaba one-indexed entonces por el preprocesamiento se pasó a zero-indexed
    //para mantener consistencia se vuelve a one-indexed o se deja tal cual segun corresponda
    //Esto es util tanto para los prints en el csv como para los prints de los ciclos negativos
    int ajuste = is_one_indexed ? 1 : 0;

    //Impresion de un ciclo negativo por CFC y asignacion de "-INF" a cada vertice dentro de una CFC con ciclo negativo

    //Vector de visistados para llevar registro de los nodos que ya reportaron su ciclo
    vector<bool> reported(n,false);

    //Registro de los ciclos por CFC
    ofstream txt_file(ruta_txt);
    if (!txt_file.is_open()){
        cerr << "Error al abrir el archivo TXT: " << ruta_txt << endl;
        exit(1);
    }

    //Metadata para este test en el archivo txt 
    txt_file << "Tiempo de ejecucion (ms): " << duration.count() << "\n";
    txt_file << "Ciclo negativo: " << (succes ? "No" : "Si") << "\n";
    txt_file << "INF se define como: " << inf << "\n";


    //Ciclo for para la deteccion de CFC, impresión y propagacion de -INF
    for(size_t k = 0; k < n; k++){
        
        //Si el nodo k pertenece a un ciclo negativo
        if(matrix[k][k] < 0){

            //Si el nodo aun no ha sido visistado imprimimos el ciclo para esta componente conexa
            if(!reported[k]){
                vector<int> cycle;
                vector<int> cfc;
                int current = k;

                //Reconstruccion del ciclo
                do{
                    cycle.push_back(current);
                    current = next_node[current][k];
                }while(current != k && current != -1);
                cycle.push_back(current);

                //Marcamos todos los vertices de esta CFC como visitados
                for(size_t u = 0; u < n; u++){
                    //Condicion de CFC
                    if(matrix[k][u] != inf && matrix[u][k] != inf){
                        reported[u] = true;
                        cfc.push_back(u);
                    }
                }

                //Imprimimos el ciclo para esta CFC en el archivo txt
                txt_file << "[";
                for(size_t v = 0; v < cfc.size(); v++){
                    txt_file << cfc[v] + ajuste << (v < cfc.size() - 1 ? ", " : "");
                }
                txt_file << "]: ";
                for (size_t v = 0; v < cycle.size(); v++) {
                    txt_file << cycle[v] + ajuste << (v < cycle.size() - 1 ? " -> " : "");
                }
                txt_file << "\n";
            }

            //Propagacion de -INF: Si k está en un ciclo negativo entonces todos los vertices i y j que puedan conectarse a traves de k estan unidos por un camino arbitrariamente corto
            for(size_t u = 0; u < n; u++){
                for(size_t v = 0; v < n; v++){
                //Si i puede llegar hasta k y k puede llegar hasta j
                if(matrix[u][k] != inf && matrix[k][v] != inf) matrix[u][v] = -inf;
                }
            }
        }
    }
    txt_file.close();

    //Guardado de resultados en CSV
    ofstream csv_file(ruta_csv);
    if (!csv_file.is_open()){
        cerr << "Error al abrir el archivo CSV: " << ruta_csv << endl;
        exit(1);
    }

    //Cabecera del CSV
    csv_file << "nodo_1, nodo_2, distancia_minima" << "\n";

    //Escrbimos en el csv
    for (size_t u = 0; u < n; u++){

        size_t start_v = is_directed ? 0 : u;

        for (size_t v = start_v; v < n; v++){

            //Manejo de la indexacion
            csv_file << (u + ajuste) << ", " << (v + ajuste) << ", ";

            //Si la distancia es infinita (con respecto al umbral) escribimos INF, sino escribimos la distancia
            if(matrix[u][v] > inf/2){
                csv_file << "INF" << "\n";
            }else if(matrix[u][v] < -inf/2){
                csv_file << "-INF" << "\n";
            }else{
                csv_file << matrix[u][v] << "\n";
            }
        }
    }
    csv_file.close();

    return 0;
}