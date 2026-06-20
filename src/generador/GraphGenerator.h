#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H

#include "../algoritmos/Graph.h"
#include <random>
#include <vector>

enum TipoGrafo {
    BIPARTITO_COMPLETO,
    ARBOL_BINARIO,
    COMPONENTES_FC_CICLO_NEGATIVO
    // Puedes agregar más tipos aquí como COMPLETO, ESTRELLA, CICLO, etc.
};

template <typename T>
Graph<T> fabricar_grafo(TipoGrafo tipo, int n) {
    // Instanciamos el grafo con n nodos
    Graph<T> grafo(n);

    // Configuración para generar pesos aleatorios
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Asumiendo que quieres pesos entre 1.0 y 20.0. 
    std::uniform_real_distribution<> dis(1.0, 20.0);

    // Genera un 0 o un 1 (como cara o cruz)
    std::uniform_int_distribution<> dis_signo(0, 1);
        
    switch(tipo) {
        case BIPARTITO_COMPLETO: {
            int mitad = n/2;
            for(int i=0; i < mitad; i++){
                for(int j = mitad; j < n; j++){
                    double peso = dis(gen);
                    if(dis_signo(gen)){
                        peso = -peso;
                    }
                    grafo.addEdge(i, j, peso);
                    grafo.addEdge(j, i, peso);
                }
            }
            
            break;
        }

        case ARBOL_BINARIO: {
            for(int i = 0; i < n; ++i) {
                int hijo_izquierdo = 2 * i + 1;
                int hijo_derecho = 2 * i + 2;

                if(hijo_izquierdo < n) {
                    double peso = dis(gen);
                    // Arista dirigida: del padre al hijo (Out-tree)
                    grafo.addEdge(i, hijo_izquierdo, peso); 
                }

                if(hijo_derecho < n) {
                    double peso = dis(gen);
                    // Arista dirigida: del padre al hijo (Out-tree)
                    grafo.addEdge(i, hijo_derecho, peso);
                }
            }
            break;
        }

        case COMPONENTES_FC_CICLO_NEGATIVO: {
            // Se agrupan los vértices de a lo más 3
            std::vector<std::vector<int>> grupos;
            for (int i = 0; i < n; i += 3) {
                std::vector<int> grupo;
                for (int j = i; j < i + 3 && j < n; ++j) {
                    grupo.push_back(j);
                }
                grupos.push_back(grupo);
            }

            // Se selecciona un grupo aleatorio para garantizar al menos 1 ciclo negativo
            std::uniform_int_distribution<> dis_grupo(0, grupos.size() - 1);
            int idx_grupo_negativo = dis_grupo(gen);

            // Conectar internamente cada grupo
            for (size_t g = 0; g < grupos.size(); ++g) {
                const auto& nodos = grupos[g];
                bool es_el_negativo = (g == idx_grupo_negativo);

                // Hacemos el grupo FC 
                for (size_t i = 0; i < nodos.size(); ++i) {
                    for (size_t j = 0; j < nodos.size(); ++j) {
                        if (i != j) {
                            double peso = dis(gen); 
                            
                            // Si es el grupo elegido para el ciclo negativo, forzamos pesos negativos
                            if (es_el_negativo) {
                                peso = -peso; 
                            } 
                            grafo.addEdge(nodos[i], nodos[j], peso);
                        }
                    }
                }
            }

            // Unir los grupos unidireccionalmente (A -> B -> C) para evitar FC global
            for (size_t i = 0; i < grupos.size() - 1; ++i) {  
                // Conectamos obligatoriamente con el siguiente inmediato para mantener el grafo conexo
                std::uniform_int_distribution<> dis_nodo_origen(0, grupos[i].size() - 1);
                std::uniform_int_distribution<> dis_nodo_destino(0, grupos[i+1].size() - 1);
                
                int u = grupos[i][dis_nodo_origen(gen)];
                int v = grupos[i+1][dis_nodo_destino(gen)];
                
                // Se conectan los grupos
                grafo.addEdge(u, v, dis(gen));
                
                // Se puentea grupo i con grupo i+2 con cierta probabilidad
                for (size_t j = i + 2; j < grupos.size(); ++j) {
                    std::uniform_int_distribution<> prob_puente(1, 100);
                    if (prob_puente(gen) <= 30) { // 30% de probabilidad de puente extra
                        std::uniform_int_distribution<> dis_v(0, grupos[j].size() - 1);
                        int u_extra = grupos[i][dis_nodo_origen(gen)];
                        int v_extra = grupos[j][dis_v(gen)];
                        grafo.addEdge(u_extra, v_extra, dis(gen));
                    }
                }
            }
            break;
        }
    }

    return grafo;
}

#endif // GRAPH_GENERATOR_H
