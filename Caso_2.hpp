#ifndef CASO_2_HPP
#define CASO_2_HPP

#include <vector>

struct ArregloC2{
	std::vector<int> GC;
	std::vector<int> Sample;
	int salto;
};

//Construir el arreglo GC
std::vector<int> ArregloGapCoding(const std::vector<int>& arreglo_00);

//Se construye la estructura entera
ArregloC2 Crear_ArregloC2(const std::vector<int>& arreglo_00, int m);

//Busqueda_Binaria
int Busqueda_Binaria_Sample(const ArregloC2& ArregloGS, int x);

int Busqueda_Binaria2(const ArregloC2& ArregloGS, int x);
 
//Decodificacion secuencial en GC
int Decodificacion_Seq (const ArregloC2& ArregloGS , int x , int L);

#endif