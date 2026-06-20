#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include "Graph.h"

template<typename T>
struct BellmanFordResult {
    std::vector<T> dist;     // Distancia mínima final desde el nodo origen a todos los demás nodos
    std::vector<int> parent; // Guarda el nodo anterior para poder reconstruir el camino más corto (atrás hacia adelante)
    bool hasNegativeCycle;   // Indica si el grafo contiene un ciclo de peso negativo
};

template<typename T>
// En caso de pasarle un grafo completo -> BellmanFordResult<T> bellmanFord(const Graph<T>& g, std::size_t source)
BellmanFordResult<T> bellmanFord(const std::vector<Edge<T>>& edges, std::size_t num_nodes, std::size_t source) {
    // Inicialización
    // - Todos los nodos tienen distancia infinita ya que el algoritmo aún no sabe cómo llegar a ellos.
    // - Nadie tiene un padre (-1) porque no hay un camino conocido para llegar a ellos todavía.
    // - La distancia del nodo origen es 0 porque es el punto de partida.
    const T INF = std::numeric_limits<T>::max();
    std::vector<T> dist(num_nodes, INF);
    std::vector<int> parent(num_nodes, -1);
    dist[source] = 0;

    // Bucle de relajación
    // En cualquier grafo sin ciclos negativos, el camino más largo posible entre dos puntos
    // es de num_nodes - 1 aristas. Después de ese número de iteraciones, la distancia mínima
    // ya no debería cambiar, ya que no hay más caminos más cortos que explorar.
    for (std::size_t i = 0; i < num_nodes - 1; ++i) {
        bool changed = false;
        for (const auto& e : edges) { // Revisamos todas las aristas
            // Si aún no sabemos cómo llegar al nodo origen de la arista (u), no hacemos nada
            if (dist[e.u] == INF) continue;
            // Si la distancia actual al nodo destino (v) es mayor que la distancia al nodo origen + el peso de la arista,
            // actualizamos la distancia y el padre del nodo destino.
            if (dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
                parent[e.v] = static_cast<int>(e.u);
                changed = true;
            }
        }
        // Si no se ha cambiado ninguna distancia, significa que ya no hay más caminos más cortos que explorar.
        if (!changed) break;
    }

    // Verificamos si hay un ciclo de peso negativo
    bool negativeCycle = false;
    for (const auto& e : edges) {
        if (dist[e.u] == INF) continue;
        // Si alguna distancia vuelve a bajar, significa que hay un ciclo de peso negativo
        if (dist[e.u] + e.w < dist[e.v]) {
            negativeCycle = true;
            break;
        }
    }

    return { dist, parent, negativeCycle };
}
#endif // BELLMAN_FORD_H
