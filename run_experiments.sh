#!/bin/bash

tipo_grafos=("BIPARTITO_COMPLETO" "ARBOL_BINARIO" "COMPONENTES_FC_CICLO_NEGATIVO")
algoritmos=("BELLMANFORD" "FLOYD_WARSHALL")

for tipo_grafo in "${tipo_grafos[@]}"; do
    for algoritmo in "${algoritmos[@]}"; do
        ./uhr/uhr resultados/csv/resultados_"$tipo_grafo"_"$algoritmo".csv 5 10 20 2 "$tipo_grafo" "$algoritmo"
    done
done