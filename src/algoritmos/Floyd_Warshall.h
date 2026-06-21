#ifndef FLOYD_WARSHALL_H
#define FLOYD_WARSHALL_H

#include <vector>
#include <algorithm>

using namespace std;

template<typename T>

bool floyd_warshall(vector<vector<T>>& matrix, vector<vector<int>>& next_node, T inf){
    
    //Numero de nodos del grafo
    size_t n = matrix.size();

    //Para que el rastreo de ciclos sea opcional
    bool track_paths = next_node.size() == n;

    //Para poder reportar los ciclos encontrados se debe aplicar una matriz de rutas
    //Asumiendo que es de tamaño nxn y esta repleta de 1's
    if(track_paths){
        for(size_t i = 0; i < n; i++){
            for(size_t j = 0; j < n; j++){
                if(matrix[i][j] < inf/2){
                    next_node[i][j] = j;
                }
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
    for(size_t i = 0; i < n; i++){
        if(matrix[i][i] < 0){
            return false;
        }
    }
    return true;
}

#endif