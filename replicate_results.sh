#!/bin/bash

#Estando en la raiz entramos a la carpeta en cuestion
pushd src/algoritmos/

#Compilacion del archivo
g++ datasets_tests.cpp -o datasets_tests

PROGRAMA="./datasets_tests"

echo "EJECUCION PARA LOS DATASETS"

$PROGRAMA "power-685-bus.mtx" 0
$PROGRAMA "bio-SC-TS.edges" 0

echo "EJECUCION FINALIZADA"

echo "EJECUCION PARA LOS EJEMPLOS DE JUEGUETE"

$PROGRAMA "test_1.mtx" 1
$PROGRAMA "test_2.mtx" 1
$PROGRAMA "test_3.mtx" 1

echo "EJECUCION FINALIZADA"

popd