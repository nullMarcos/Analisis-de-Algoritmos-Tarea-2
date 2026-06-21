#ifndef APSP_BELLMANFORD_H
#define APSP_BELLMANFORD_H

#include "BellmanFord.h"

// Estructura para almacenar el resultado de APSP (Matrices n x n)
template<typename T>
struct APSPResult {
    std::vector<std::vector<T>> dist;     // Matriz de distancias mínimas u -> v
    std::vector<std::vector<int>> parent; // Matriz de padres para reconstrucción de caminos
    bool hasNegativeCycle;                // Indica si se abortó por ciclo negativo
};

template<typename T>
APSPResult<T> apspBellmanFord(const std::vector<Edge<T>>& edges, std::size_t num_nodes) {
    APSPResult<T> result;
    result.hasNegativeCycle = false;

    // Inicialización de las matrices n x n
    const T INF = std::numeric_limits<T>::max();
    result.dist.resize(num_nodes, std::vector<T>(num_nodes, INF));    // Distancias inicializadas a infinito
    result.parent.resize(num_nodes, std::vector<int>(num_nodes, -1)); // Padres inicializados a -1

    // Bucle principal, utilizar Bellman-Ford desde cada nodo 'u' como origen
    for (std::size_t u = 0; u < num_nodes; ++u) {
        // Problema SSSP, pero se repite para cada nodo 'u' como origen
        BellmanFordResult<T> bf_res = bellmanFord(edges, num_nodes, u);

        if (bf_res.hasNegativeCycle) {
            result.hasNegativeCycle = true;
        }

        // Mover los datos a las filas de la matriz
        // Usamos std::move ya que 'bf_res' va a morir en la siguiente iteración, así evitamos copias costosas.
        result.dist[u] = std::move(bf_res.dist);
        result.parent[u] = std::move(bf_res.parent);
    }

    return result;
}

#endif // APSP_BELLMANFORD_H
