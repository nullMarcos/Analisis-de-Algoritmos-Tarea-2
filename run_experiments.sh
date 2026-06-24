#!/bin/bash

tipo_grafos=("BIPARTITO_COMPLETO" "ARBOL_BINARIO" "COMPONENTES_FC_CICLO_NEGATIVO")
distribucion_pesos=("DECIMAL" "ENTERA")
algoritmos=("BELLMANFORD" "FLOYD_WARSHALL")

for tipo_grafo in "${tipo_grafos[@]}"; do
    for algoritmo in "${algoritmos[@]}"; do
        for distribucion in "${distribucion_pesos[@]}"; do
            ./uhr/uhr resultados/csv/resultados_"$tipo_grafo"_"$distribucion"_"$algoritmo".csv 4 300 300 100 "$tipo_grafo" "$algoritmo" "$distribucion"
        done
    done
done