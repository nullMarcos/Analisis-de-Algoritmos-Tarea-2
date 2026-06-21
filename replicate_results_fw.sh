#!/bin/bash

#Estando en la raiz entramos a la carpeta en cuestion
pushd src/algoritmos/

#Compilacion del archivo
g++ -O3 datasets_tests.cpp -o datasets_tests

PROGRAMA="./datasets_tests"

echo "EJECUCION PARA LOS DATASETS"

$PROGRAMA "power-685-bus.mtx" 0 fw
$PROGRAMA "bio-SC-TS.edges" 0 fw

echo "EJECUCION FINALIZADA"

echo "EJECUCION PARA LOS EJEMPLOS DE JUEGUETE"

$PROGRAMA "test_1.mtx" 1 fw
$PROGRAMA "test_2.mtx" 1 fw
$PROGRAMA "test_3.mtx" 1 fw
$PROGRAMA "test_4.mtx" 1 fw

echo "EJECUCION FINALIZADA"

popd