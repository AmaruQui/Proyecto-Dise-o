#include <iostream>
#include <vector>
#include <cstdlib> //la libreria para el rand
#include <random> //Normal
#include <algorithm>
#include "Caso_1.hpp"

std::vector<int> Arreglo_Lineal(int n,int max){
	std::vector<int> lineal(n);
	//verificacion
	if (n==0) return lineal;
	//1er elemento del arreglo
	lineal[0]=rand();
	
	//Llenar arreglo
	for (int i=1; i<n ; ++i){
		lineal[i]=lineal[i-1] + (rand() % max) + 1;
	}
	return lineal;
}

//Heapify
void heapify(std::vector<int>& arreglo, int n, int i){
	int mas_grande = i;
    int izquierdo = 2 * i + 1;
    int derecho = 2 * i + 2;

    if (izquierdo < n && arreglo[izquierdo] > arreglo[mas_grande]) {
        mas_grande = izquierdo;
    }

    if (derecho < n && arreglo[derecho] > arreglo[mas_grande]) {
        mas_grande = derecho;
    }

    if (mas_grande != i) {
        std::swap(arreglo[i], arreglo[mas_grande]);
        heapify(arreglo, n, mas_grande);
    }
}

//Es HeapSort
void Ordenar_Arreglo(std::vector<int>& arreglo){
	int n = arreglo.size();
	
	for (int i = n/2 - 1; i >= 0; i--){
		heapify(arreglo, n, i);
	}
	
	for (int i = n-1; i > 0; i--){
		std::swap(arreglo[0], arreglo[i]);
		heapify(arreglo, i, 0);
	}
}

std::vector<int> Arreglo_Normal(int n, double media , double estandar){
	std::vector<int> normal(n);
	//Verificacion
	if(n == 0) return normal;
	
	//config generar numeros en dist normal(No la entendi mucho)
	std::random_device rd; 
	std::mt19937 gen(rd());
	std::normal_distribution<> distribucion(media, estandar);
	
	//Llenar arreglo
	for (int i=0; i < n; ++i){
		normal[i] = static_cast<int>(distribucion(gen));
	}
	//En este si es necesario ordenar por como se generan los numeros
	Ordenar_Arreglo(normal);
	return normal;
}

int Busqueda_Binaria(const std::vector<int>& arreglo, int x) {
	int L = 0;
	int R = arreglo.size() - 1;

	while (L <= R) {
		int medio = L + (R - L) / 2;

		if (arreglo[medio] == x) return medio;

		if (arreglo[medio] > x){
			R = medio - 1;
		} else {
			L = medio + 1;
		}
	}
	return -1; // No encontrado
}

