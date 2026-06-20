// Compilar con:
// g++ -O3 test_apsp.cpp -o test_apsp
// ./test_apsp
#include <iostream>
#include <vector>
#include <limits>
#include <cassert>
#include "APSP_BellmanFord.h" // Ajusta la ruta a tu archivo

const double INF = std::numeric_limits<double>::max();

// Función auxiliar para imprimir matrices en caso de error

template<typename T>
void print_graph(const std::vector<Edge<T>>& edges, std::size_t num_nodes) {
    std::cout << "\n=========================================\n";
    std::cout << "       REPRESENTACIÓN DEL GRAFO\n";
    std::cout << "=========================================\n";

    // Agrupamos las aristas por nodo origen para que se vea ordenado
    std::vector<std::vector<std::pair<std::size_t, T>>> adj(num_nodes);
    for (const auto& e : edges) {
        adj[e.u].push_back({e.v, e.w});
    }

    for (std::size_t u = 0; u < num_nodes; ++u) {
        std::cout << " Nodo [" << u << "]";
        if (adj[u].empty()) {
            std::cout << "  (sin aristas salientes)";
        } else {
            for (const auto& edge : adj[u]) {
                std::cout << " ---> " << edge.first << " (peso: " << edge.second << ")";
            }
        }
        std::cout << "\n";
    }
    std::cout << "=========================================\n";
}

template<typename T>
void print_matrix(const std::vector<std::vector<T>>& mat) {
    std::cout << "\n=========================================\n";
    std::cout << "      MATRIZ DE DISTANCIAS MÍNIMAS\n";
    std::cout << "=========================================\n";

    // Imprimir cabecera de columnas
    std::cout << "Dest:\t";
    for (std::size_t j = 0; j < mat.size(); ++j) std::cout << "[" << j << "]\t";
    std::cout << "\n\n";

    // Imprimir filas
    for (std::size_t i = 0; i < mat.size(); ++i) {
        std::cout << "[" << i << "]\t"; // Origen
        for (std::size_t j = 0; j < mat[i].size(); ++j) {
            if (mat[i][j] == std::numeric_limits<T>::max() || mat[i][j] >= 1e14) {
                // Captura tanto tu INF de Bellman-Ford como el 1e15 de Floyd-Warshall
                std::cout << "INF\t";
            } else {
                std::cout << mat[i][j] << "\t";
            }
        }
        std::cout << "\n";
    }
    std::cout << "=========================================\n\n";
}

void test_grafo_estandar() {
    std::cout << "[Test 1] Corriendo grafo estándar sin ciclos negativos... ";

    // Grafo de 3 nodos (0, 1, 2)
    // 0 -> 1 (peso 3)
    // 1 -> 2 (peso 1)
    // 0 -> 2 (peso 5) -> El camino más corto debería ser 0 -> 1 -> 2 (peso 4)
    std::size_t num_nodes = 3;
    std::vector<Edge<double>> edges = {
        {0, 1, 3.0},
        {1, 2, 1.0},
        {0, 2, 5.0}
    };

    // Solución esperada calculada a mano
    std::vector<std::vector<double>> expected_dist = {
        {0.0, 3.0, 4.0},  // Desde nodo 0
        {INF, 0.0, 1.0},  // Desde nodo 1
        {INF, INF, 0.0}   // Desde nodo 2
    };

    auto result = apspBellmanFord(edges, num_nodes);
    print_graph(edges, num_nodes);

    if (!result.hasNegativeCycle) {
        print_matrix(result.dist);
    } else {
        std::cout << "[!] El algoritmo reportó un ciclo negativo.\n";
    }

    // Verificaciones (Asserts)
    assert(result.hasNegativeCycle == false && "Error: Se detectó un ciclo negativo falso.");

    for (std::size_t i = 0; i < num_nodes; ++i) {
        for (std::size_t j = 0; j < num_nodes; ++j) {
            if (expected_dist[i][j] != result.dist[i][j]) {
                std::cerr << "\n\033[1;31mFAIL\033[0m: Distancia incorrecta desde " << i << " a " << j << "\n";
                std::cerr << "Esperado: " << expected_dist[i][j] << ", Obtenido: " << result.dist[i][j] << "\n";
                exit(1);
            }
        }
    }
    std::cout << "\033[1;32mPASSED\033[0m\n";
}

void test_ciclo_negativo() {
    std::cout << "[Test 2] Corriendo grafo con ciclo negativo... ";

    // Grafo de 2 nodos con un ciclo de costo neto negativo
    // 0 -> 1 (peso 1)
    // 1 -> 0 (peso -2) -> Ciclo total = -1
    std::size_t num_nodes = 2;
    std::vector<Edge<double>> edges = {
        {0, 1, 1.0},
        {1, 0, -2.0}
    };

    auto result = apspBellmanFord(edges, num_nodes);

    // Verificación
    assert(result.hasNegativeCycle == true && "Error: No se detectó el ciclo negativo.");
    std::cout << "\033[1;32mPASSED\033[0m\n";
}

int main() {
    std::cout << "\033[0;36m=== INICIANDO PRUEBAS DE CORRECTITUD ===\033[0m\n\n";

    test_grafo_estandar();
    test_ciclo_negativo();

    std::cout << "\n\033[1;32mTodas las pruebas pasaron con éxito.\033[0m\n";
    return 0;
}
