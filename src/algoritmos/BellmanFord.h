#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <queue>
#include "Graph.h"

template<typename T>
struct BellmanFordResult {
    std::vector<T> dist;     // Distancia mínima final desde el nodo origen a todos los demás nodos
    std::vector<int> parent; // Guarda el nodo anterior para poder reconstruir el camino más corto (atrás hacia adelante)
    bool hasNegativeCycle;   // Indica si el grafo contiene un ciclo de peso negativo
};

template<typename T>
BellmanFordResult<T> bellmanFord(
    const std::vector<Edge<T>>& edges, 
    const std::vector<std::vector<std::size_t>>& adj_indices,
    std::size_t num_nodes, 
    std::size_t source,
    std::vector<T>& dist, 
    std::vector<int>& parent,
    bool& negativeCycle
) {
    // Inicialización
    // - Todos los nodos tienen distancia infinita ya que el algoritmo aún no sabe cómo llegar a ellos.
    // - Nadie tiene un padre (-1) porque no hay un camino conocido para llegar a ellos todavía.
    // - La distancia del nodo origen es 0 porque es el punto de partida.
    const T INF = std::numeric_limits<T>::max();
    std::fill(dist.begin(), dist.end(), INF);
    std::fill(parent.begin(), parent.end(), -1);
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

    // Verificamos si hay un ciclo de peso negativo y marcamos los nodos afectados
    negativeCycle = false;
    std::vector<bool> affected(num_nodes, false);
    for (const auto& e : edges) {
        if (dist[e.u] == INF) continue;
        // Si alguna distancia vuelve a bajar, significa que hay un ciclo de peso negativo
        if (dist[e.u] + e.w < dist[e.v]) {
            negativeCycle = true;
            affected[e.v] = true; // 'v' aún se relaja => está contaminado por un ciclo negativo
        }
    }

    if(negativeCycle) {
        // Propagamos la contaminación: cualquier nodo alcanzable desde un nodo
        // afectado también tiene distancia -infinito.
        // La propagación es hacia adelante por las aristas (u -> v),
        // no hacia atrás, y no depende de si el nodo es alcanzable desde 'source'.
        std::queue<std::size_t> q;
        for (std::size_t i = 0; i < num_nodes; ++i) {
            if (affected[i]) q.push(i);
        }

        while (!q.empty()) {
            std::size_t a = q.front();
            q.pop();
            for (std::size_t v : adj_indices[a]) {
                if (!affected[v]) {
                    affected[v] = true;
                    q.push(v);
                }
            }
        }

        // Aplicamos -INF únicamente a los nodos realmente contaminados.
        // Los nodos con dist == INF (inalcanzables desde source) o finitos
        // pero no afectados (alcanzables desde source sin pasar por el ciclo)
        // se quedan exactamente como estaban.
        const T NEG_INF = std::numeric_limits<T>::lowest();
        for (std::size_t v = 0; v < num_nodes; ++v) {
            if (affected[v]) {
                dist[v] = NEG_INF;
                parent[v] = -1;
            }
        }
    }

    return { dist, parent, negativeCycle };
}
#endif // BELLMAN_FORD_H
