#include <stdexcept> //esto es solo por si el m es muy chico ( no me quise calentar la cabeza)
#include "Caso_2.hpp"

//Crea y genera el arreglo GC Se le ingresa un arreglo const ya que este no se debe modificar
std::vector<int> ArregloGapCoding(const std::vector<int>& arreglo_00){
	int n = arreglo_00.size();
	std::vector <int> GC(n);
	
	if (n == 0) return GC;
	
	GC[0] = arreglo_00[0];
	
	for (int i = 1; i < n; ++i){
		GC[i] = arreglo_00[i] - arreglo_00[i - 1];	
	}
	return GC;
}

ArregloC2 Crear_ArregloC2(const std::vector<int>& arreglo_00, int m){
	int n = arreglo_00.size();
	if (m <= 0 || n == 0) throw std::invalid_argument("'m' o 'n' invalidos");

	int salto = n / m;
	if (m > n/2) throw std::invalid_argument("m debe ser <<n");
	
	ArregloC2 ArregloGS;
	ArregloGS.salto = salto;
	ArregloGS.GC = ArregloGapCoding(arreglo_00);

	//Se crea el arreglo en la estructura (Sample)
	ArregloGS.Sample = std::vector<int>(m);
	 
	int j = 0;
	for (int i = 0; i < n && j < m; i += salto){
		ArregloGS.Sample[j] = arreglo_00[i];
		j++;
	}
	
	return ArregloGS;
}

int Busqueda_Binaria_Sample (const ArregloC2& ArregloGS, int x){
	int n = ArregloGS.Sample.size();
	if (n == 0 || x < ArregloGS.Sample[0]) return -1;

	int L = 0;
	int R = n - 1;
	int intervalo = 0;

	while (L <= R) {
		int medio = L + (R - L) / 2;

		if (ArregloGS.Sample[medio] <= x){
			intervalo = medio; 
			L = medio + 1;
		} else {
			R = medio - 1;
		}
	}
	return intervalo;
	
}// esta funcion encuentra un intervalo de tamaño igual al tamano de salto siendo de costo log m

int Decodificacion_Seq(const ArregloC2& ArregloGS , int x , int L){
    if (L == -1) return -1;

    int inicio = L * ArregloGS.salto;
    int sub_x = ArregloGS.Sample[L];
    
    if (sub_x == x) return inicio;
    
    int n = ArregloGS.GC.size();

    // arreglo esta ordenado, si la suma se pasa de x, cortamos el ciclo
    for (int i = inicio + 1; i < n ; i++){        
        sub_x += ArregloGS.GC[i];
        if (sub_x > x) return -1; // el numero no existe
        if (sub_x == x) return i; // encontrado
    }
    return -1; 
}

//IMPORTANTE ESTA, ES LA QUE VA EN EL MAIN OJO PIOJO 
int Busqueda_Binaria2(const ArregloC2& ArregloGS, int x){
	int L = Busqueda_Binaria_Sample(ArregloGS, x);
	return Decodificacion_Seq(ArregloGS, x, L);
}


