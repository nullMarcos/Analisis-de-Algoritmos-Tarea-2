# Análisis de Algoritmos: Tarea 2

## Integrantes:

- Gabriel Sebastián Castillo Castillo
- Daniel Alexander Támaro Sierra
- Marcos Emiliano Martínez Rojas

## Estructura del Proyecto

```text
Analisis-de-Algoritmos-Tarea-1/
├── data/              # Directorio con los datos de los grafos
│   ├── csv/           # Datos CSV solicitados en punto 2.3 de la tarea
│   └── dataset/       # Conjuntos de datos
├── resultados/
│   ├── csv/           # Archivos csv de salida
│   └── graficas/      # Gráficas de los resultados
├── src/
│   ├── algoritmos/    # Algoritmos para calcular camino más corto
│   └── generador/     # Código para generar grafos de prueba
└── uhr/               # Utilidades sugeridas para las pruebas
```

## Instrucciones de Compilación
Para compilar el código, ejecute el siguiente comando en la carpeta raíz:
```bash
g++ -std=c++11 -O0 uhr/uhr.cpp -o uhr/uhr
```

## Ejecución de Pruebas
Para ejecutar las pruebas, ejecute el siguiente comando en la carpeta raíz:
```bash
chmod +x run_experiments.sh
./run_experiments.sh
```

## Ejecución sobre los datasets
En el punto 2.3 de la tarea se solicitó probar el algoritmo Floyd-Warshall sobre los datasets bio-SC-TS y 
power-685-bus. Para complementar esto, adicionalmente se crearon 4 tests de juguete de fabricación propia para verificar la correctitud del algoritmo. Para la ejecución de estas pruebas se debe ejecutar el siguiente comando desde la carpeta raíz del proyecto:

```bash
chmod +x replicate_results_fw.sh
./replicate_results_fw.sh
```
Una vez ejecutado el script, los resultados solicitados en formato CSV se almacenarán en data/csv/