#!/bin/bash

tipo_grafos=("ARBOL_BINARIO" "COMPONENTES_FC_CICLO_NEGATIVO" "BIPARTITO_COMPLETO")
distribucion_pesos=("DECIMAL" "ENTERA")
algoritmos=("FLOYD_WARSHALL" "BELLMANFORD")

for tipo_grafo in "${tipo_grafos[@]}"; do
    for algoritmo in "${algoritmos[@]}"; do
        for distribucion in "${distribucion_pesos[@]}"; do
            if [ "$tipo_grafo" == "BIPARTITO_COMPLETO" ] && [ "$algoritmo" == "BELLMANFORD" ]; then
                ./uhr/uhr resultados/csv/resultados_"$tipo_grafo"_"$distribucion"_"$algoritmo".csv 32 100 300 100 "$tipo_grafo" "$algoritmo" "$distribucion"
            else
                ./uhr/uhr resultados/csv/resultados_"$tipo_grafo"_"$distribucion"_"$algoritmo".csv 32 100 1000 100 "$tipo_grafo" "$algoritmo" "$distribucion"
            fi
        done
    done
done