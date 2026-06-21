
#ifndef GRAPH_H
#define GRAPH_H
/*
 * Graph.h
 * Lista de aristas para representar un grafo.
 * Con templates para soportar diferentes tipos de pesos (int, float, double, etc.).
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <string>
#include <utility>

using namespace std;

template<typename T>
struct Edge {
    std::size_t u; // Nodo origen
    std::size_t v; // Nodo destino
    T w;           // Peso de la arista
};

template<typename T>
class Graph {
public:
    std::size_t n;              // Número de nodos
    std::vector<Edge<T>> edges; // Lista de aristas

    Graph(std::size_t n): n(n) {}

    // Método para conectar el nodo u al nodo v con peso w
    void addEdge(std::size_t u, std::size_t v, T w) {
        edges.push_back({u, v, w});
    }

    // Método para obtener el número de nodos
    std::size_t size() const {
        return n;
    }

    // VISTA PARA BELLMAN-FORD: Devuelve la lista de aristas
    const std::vector<Edge<T>>& getEdgeList() const {
        return edges;
    }

    // VISTA PARA FLOYD-WARSHALL: Genera la matriz de adyacencia
    std::vector<std::vector<T>> toAdjacencyMatrix(T infinito) const {
        // Inicializamos la matriz llena de "infinitos"
        std::vector<std::vector<T>> matrix(n, std::vector<T>(n, infinito));

        // Distancia de un nodo a sí mismo es siempre 0
        for (std::size_t i = 0; i < n; ++i) {
            matrix[i][i] = T{}; // o 0, pero asi mantenemos el codigo generico
        }

        // Volcamos las aristas en la matriz
        for (const auto& e : edges) {
            matrix[e.u][e.v] = std::min(matrix[e.u][e.v], e.w);
        }

        return matrix;
    }

    //Metodo para probar los grafos de los datasets en Floyd-Warshall
    static pair<Graph<T>, bool> create_graph_from_file(const string& filename, bool is_directed){

        ifstream file(filename);

        if (!file.is_open()){
            cerr << "Error al abrir el archivo: " << filename << endl;
            exit(1);
        }

        //Leemos la cantidad de nodos y aristas
        size_t nodos_u, nodos_v, aristas;
        file >> nodos_u >> nodos_v >> aristas;

        //Instancia del grafo en cuestion
        Graph<T> graph(nodos_u);

        //Leemos las aristas
        long long u, v;
        T w;

        //Agregamos una flag puesto que un archivo es zero-indexed y el otro es one-indexed.
        //Esta flag funcion asumiendo que los el primer par de vertices indica si es one o zero indexed (que es el caso de los archivos en cuestion).
        bool is_one_indexed = true;

        //Lectura de la primera fila
        file >> u >> v >> w;

        //Chequeo de zero-indexed
        if (u == 0 || v == 0){
            is_one_indexed = false;
        }
        
        //Agregamos la primera arista segun corresponda, si el dataset está one-indexed entonces se transforma a zero-indexed
        if (is_one_indexed){
            u = u - 1;
            v = v - 1;
        }
        graph.addEdge(u, v, w);

        if (u != v && !is_directed) graph.addEdge(v, u, w);

        while (file >> u >> v >> w){

            //Agregamos la arista a la lista de aristas
            if (is_one_indexed){
                u = u - 1;
                v = v - 1;
            }
            graph.addEdge(u, v, w);

            //Como los grafos de los datasets son no dirigidos/simetricos, agregamos la arista inversa
            if (u != v && !is_directed) graph.addEdge(v, u, w);
        }

        file.close();
        return make_pair(graph, is_one_indexed);
    }
};
#endif // GRAPH_H
