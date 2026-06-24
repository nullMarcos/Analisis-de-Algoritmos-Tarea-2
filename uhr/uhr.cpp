/** uhr: generic time performance tester
 * Author: LELE
 *
 * Things to set up:
 * 0. Includes: include all files to be tested,
 * 1. Time unit: in elapsed_time,
 * 2. What to write on time_data,
 * 3. Data type and distribution of RNG,
 * 4. Additive or multiplicative stepping,
 * 5. The experiments: in outer for loop. */

// Compilar con: g++ -O3 -std=c++17 uhr/uhr.cpp -o uhr/uhr

#include <cstdint>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>

#include "utils.cpp"
#include "../src/algoritmos/APSP_BellmanFord.h"
#include "../src/algoritmos/Floyd_Warshall.h"
#include "../src/generador/GraphGenerator.h"

// Include to be tested files here

int main(int argc, char *argv[])
{
    // Validate and sanitize input
    std::int64_t runs, lower, upper, step;
    TipoGrafo tipo_grafo;
    Algoritmo algoritmo;
    DistribucionPesos distribucion;
    validate_input(argc, argv, runs, lower, upper, step, tipo_grafo, algoritmo, distribucion);

    // Set up clock variables
    std::int64_t n, i, executed_runs;
    std::int64_t total_runs_additive = runs * (((upper - lower) / step) + 1);
    std::int64_t total_runs_multiplicative = runs * (floor(log(upper / double(lower)) / log(step)) + 1);
    std::vector<double> times(runs);
    std::vector<double> q;
    double mean_time, time_stdev, dev;
    auto begin_time = std::chrono::high_resolution_clock::now();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_time = end_time - begin_time;

    // Set up random number generation
    std::random_device rd;
    std::mt19937_64 rng(rd());
    std::uniform_int_distribution<std::int64_t> u_distr; // change depending on app

    // File to write time data
    std::ofstream time_data;
    time_data.open(argv[1]);
    time_data << "algoritmo,tipo_grafo,distribucion,n,m,t_mean,t_stdev,t_Q0,t_Q1,t_Q2,t_Q3,t_Q4" << std::endl;

    // Begin testing
    std::cerr << "\033[0;36mRunning tests...\033[0m" << std::endl << std::endl;
    executed_runs = 0;
    for (n = lower; n <= upper; n += step) {
        mean_time = 0;
        time_stdev = 0;

        // Test configuration goes here
        // Generamos un grafo con n vertices
        Graph G = fabricar_grafo<double>(tipo_grafo, n, distribucion);

        // Parámetros para Floyd-Warshall
        const double inf = 1e15; // infinito

        // Run to compute elapsed time
        for (i = 0; i < runs; i++) {
            // Remember to change total depending on step type
            display_progress(++executed_runs, total_runs_additive);

            begin_time = std::chrono::high_resolution_clock::now();
            // Function to test goes here
            switch(algoritmo) {
                case BELLMANFORD:
                    apspBellmanFord(G);
                    break;
                case FLOYD_WARSHALL: {
                    floyd_warshall(G, inf);
                    break;
                }
            }
            end_time = std::chrono::high_resolution_clock::now();

            elapsed_time = end_time - begin_time;
            times[i] = elapsed_time.count();

            mean_time += times[i];
        }

        // Compute statistics
        mean_time /= runs;

        for (i = 0; i < runs; i++) {
            dev = times[i] - mean_time;
            time_stdev += dev * dev;
        }

        time_stdev /= runs - 1; // Subtract 1 to get unbiased estimator
        time_stdev = std::sqrt(time_stdev);

        quartiles(times, q);

        // Obtener la cantidad real de aristas
        std::size_t m = G.getEdgeList().size();

        // Convertir en string para el CSV
        std::string alg_name = (algoritmo == BELLMANFORD) ? "Bellman-Ford" : "Floyd-Warshall";
        std::string graph_name;
        switch (tipo_grafo){
            case BIPARTITO_COMPLETO:
                graph_name = "Bipartito Completo";
                break;
            case ARBOL_BINARIO:
                graph_name = "Árbol Binario";
                break;
            case COMPONENTES_FC_CICLO_NEGATIVO:
                graph_name = "Componentes FC Ciclo Negativo";
                break;
        }

        std::string dist_name = (distribucion == DECIMAL) ? "Decimal" : "Entera";

        // Escribir fila en el CSV
        time_data << alg_name << "," << graph_name << "," << dist_name << "," << n << "," << m << "," << mean_time << "," << time_stdev << ",";
        time_data << q[0] << "," << q[1] << "," << q[2] << "," << q[3] << "," << q[4] << std::endl;
    }

    // This is to keep loading bar after testing
    std::cerr << std::endl << std::endl;
    std::cerr << "\033[1;32mDone!\033[0m" << std::endl;

    time_data.close();

    return 0;
}
