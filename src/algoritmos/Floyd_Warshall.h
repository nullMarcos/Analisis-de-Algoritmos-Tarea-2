#ifndef FLOYD_WARSHALL_H
#define FLOYD_WARSHALL_H

#include <vector>
#include <algorithm>

using namespace std;

template<typename T>

bool floyd_warshall(vector<vector<T>>& matrix, T inf){
    
    //Numero de nodos del grfo
    size_t n = matrix.size();

    //Iteracion sobre vertices intermedios
    for (size_t k = 0; k < n; k++){
        //Itereacion sobre vertices de origen
        for (size_t i = 0; i < n; i++){
            //Iteracion sobre vertices de destino
            for (size_t j = 0; j < n; j++){

                //Para evitar overflow, si alguna de las distancias es infinita, no se realiza la suma
                //Se compara con un umbral [inf/2] para evitar problemas de precision con floats/doubles
                if(matrix[i][k] > inf/2 || matrix[k][j] > inf/2) continue;
                matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j]);
            }
        }
    }

    //Al finalizar, si un vertice tiene un peso negativo a si mismo, entonces hay un ciclo negativo en el grafo
    for (size_t i = 0; i < n; i++){
        if(matrix[i][i] < 0){
            return false;
        }
    }
    return true;
}

#endif