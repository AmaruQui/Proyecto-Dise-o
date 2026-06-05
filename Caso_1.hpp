#ifndef CASO_1_HPP
#define CASO_1_HPP
#include <vector>

//ARREGLOS
std::vector<int> Arreglo_Lineal(int n, int max);
std::vector<int> Arreglo_Normal(int n, double media, double estandar);

// HEAP SORT
void heapify(std::vector<int>& arreglo, int n, int i);
void Ordenar_Arreglo(std::vector<int>& arreglo);

//BINARIA
int Busqueda_Binaria(const std::vector<int>& arreglo, int x);

#endif