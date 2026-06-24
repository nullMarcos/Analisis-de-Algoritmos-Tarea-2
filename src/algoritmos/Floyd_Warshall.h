#ifndef FLOYD_WARSHALL_H
#define FLOYD_WARSHALL_H

#include <vector>
#include <algorithm>
#include "Graph.h"

using namespace std;

template<typename T>
struct FloydWarshallResult {
    vector<vector<T>> dist;
    vector<vector<int>> next_node;
    bool succes;
};

template<typename T>
FloydWarshallResult<T> floyd_warshall(const Graph<T>& G, T inf, bool track_paths){
    
    // Generar la matriz de adyacencia de manera interna
    vector<vector<T>> matrix = G.toAdjacencyMatrix(inf);
    
    //Numero de nodos del grafo
    size_t n = matrix.size();

    // Matriz de rutas para Floyd-Warshall
    vector<vector<int>> next_node;
    if(track_paths){
        next_node.assign(n, vector<int>(n, -1));
    }

    //Para poder reportar los ciclos encontrados se debe aplicar una matriz de rutas
    for(size_t i = 0; i < n; i++){
        for(size_t j = 0; j < n; j++){
            if(matrix[i][j] < inf/2){
                next_node[i][j] = j;
            }
        }
    }

    //Iteracion sobre vertices intermedios
    for(size_t k = 0; k < n; k++){
        //Itereacion sobre vertices de origen
        for(size_t i = 0; i < n; i++){
            //Iteracion sobre vertices de destino
            for(size_t j = 0; j < n; j++){

                //Para evitar overflow, si alguna de las distancias es infinita, no se realiza la suma
                //Se compara con un umbral [inf/2] para evitar problemas de precision con floats/doubles
                if(matrix[i][k] > inf/2 || matrix[k][j] > inf/2) continue;

                if(matrix[i][j] > matrix[i][k] + matrix[k][j]){
                    matrix[i][j] = matrix[i][k] + matrix[k][j];
                    if(track_paths){
                        next_node[i][j] = next_node[i][k];
                    }
                }
            }
        }
    }

    //Al finalizar, si un vertice tiene un peso negativo a si mismo, entonces hay un ciclo negativo en el grafo
    bool succes = true;
    for(size_t i = 0; i < n; i++){
        if(matrix[i][i] < 0){
            succes = false;
            break;
        }
    }
    //Si track_paths era falso, next_node simplemente se retorna como un vector vacio.
    return { matrix, next_node, succes };
}

#endif