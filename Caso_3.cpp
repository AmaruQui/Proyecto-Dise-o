#include "Caso_3.hpp"
#include <stdexcept>
	
//El constructor
NodoHuffman::NodoHuffman(int g, int f) : gap(g), frecuencia(f), izq(nullptr), der(nullptr) {}

//comparador para la cola de prioridad (Haremos un min heap)
bool CompararNodos::operator()(NodoHuffman* a, NodoHuffman* b){
	return a->frecuencia > b->frecuencia; //Si a tiene mayor frecuencia que b, entonces a es "mayor" que b en el min heap
};

//contador de frecuencia en GC para armar el arbol
std::unordered_map<int, int> ContarFrec(const std::vector<int>& GC){
	std::unordered_map<int, int> contador_frec;	
	for (int gap : GC){ // por cada gap en GC se suma 1 cada vez que ve un gap
		contador_frec[gap]++;
	}
	return contador_frec;
}

NodoHuffman* ConstruirArbolHuff(const std::unordered_map<int, int>& frecuencia){
	std::priority_queue<NodoHuffman*, std::vector<NodoHuffman*>, CompararNodos> pq; //pq es la cola de prioridad o priority_queue por si tenias dudas
	//IMPORTANTE Ese for crea la hoja con gap y frecuencia
	// se puede ocupar pair pero al parecer es mas economico ocupar auto (en espacio)
	for (auto const& par : frecuencia){
		pq.push(new NodoHuffman(par.first , par.second));
	}
	
	if(pq.empty()) return nullptr;
	
	//Unir los nodos
	while (pq.size() > 1){
		
		//como es un min heap extrae el con frec mas baja y lo saca(Como se arma el arbol de huff)
		NodoHuffman* izq = pq.top(); pq.pop();
		NodoHuffman* der = pq.top(); pq.pop();
		
		//crear nodo padre con la frec de ambos 
		// es -1 ya que no es una "hoja" o un gap
		NodoHuffman* padre = new NodoHuffman(-1, izq->frecuencia + der->frecuencia);
		padre->izq = izq; 
		padre->der = der; 
		
		//se vuelve a mater el nodo XY a la cola para que se siga armando hasta que haya solo un nodo (significa que el arbol esta armado)
		pq.push(padre);
	}
	
	//retorna el unico nodo que quede en la cola (El arbol ya armado)
	return pq.top();
}


//IMPORTANTE Borrar el arbol
void BorrarArbol(NodoHuffman* nodo) {
    if (nodo == nullptr) return;
    BorrarArbol(nodo->izq);
    BorrarArbol(nodo->der);
    delete nodo;
}

// aqui se generan los codigos del arbol de huffman izq 0 derecha 1
void GenerarCodigos(NodoHuffman* nodo, std::string codigo_actual, std::unordered_map<int, std::string>& diccionario){
	if (nodo == nullptr) return; 
	
	//Verificar que es hoja y no un nodo que une a otros 2 nodos (Es hoja)
	if (nodo->izq == nullptr && nodo->der == nullptr){
		diccionario[nodo->gap] = codigo_actual;
		return;
	}
	
	//Bajamos a la izq y se agrega un 0 al codigo
	GenerarCodigos(nodo->izq, codigo_actual + "0", diccionario);
	//Derecha y agrega un 1 al codigo el (es or) (Pa los codigos ocupamos bitwise)
	GenerarCodigos(nodo->der , codigo_actual + "1", diccionario );
}

//crea todos con ayuda de la otra
std::unordered_map<int, std::string> CrearDiccionarioCodigos(NodoHuffman* raiz){
	std::unordered_map<int, std::string> diccionario;
	if (raiz && raiz->izq == nullptr && raiz->der == nullptr) {
		// si el arbol tiene solo un nodo, asignamos un codigo arbitrario (por ejemplo, "0")
		diccionario[raiz->gap] = "0";
	}else {
		GenerarCodigos(raiz, "", diccionario);
	}
	return diccionario;
}

ArregloComprimido Comprimir(const ArregloC2& caso2, const std::unordered_map<int, std::string>& diccionario){
	ArregloComprimido caso3;
	caso3.Sample = caso2.Sample;
	caso3.salto = caso2.salto;
	
	//arma el diccionario inverso con los gap (Codiso->gap(El original))
	for (auto const& par : diccionario){
		caso3.Diccionario_Inv[par.second] = par.first;
	}
	
	std::string stream_binario = "";
	int n = caso2.GC.size();
	
	caso3.offsets_bloques.reserve(caso2.Sample.size());

	//convertimos todos los gaps a un flujo binario continuo sin espacios
	for (int i = 0 ; i < n; ++i){
		stream_binario += diccionario.at(caso2.GC[i]);
		if (i % caso2.salto == 0) {
			// guardamos en que bit exacto empieza cada bloque del sample
			caso3.offsets_bloques.push_back(stream_binario.length()); //
		}
	}
	
	caso3.bits_validos = stream_binario.length();

	// rellena con 0 al final para que sea multiplo de 8 bits
	while (stream_binario.length() % 8 != 0){
		stream_binario += "0";
	}

	for (size_t i = 0; i < stream_binario.length(); i += 8){
		std::string byte_str = stream_binario.substr(i, 8);
		uint8_t byte_val = static_cast<uint8_t>(std::stoi(byte_str, nullptr, 2)); // convierte el string de 8 bits a un byte
		caso3.GC_Comprimido.push_back(byte_val);
	}

	return caso3;
}

// busqueda binaria sobre el sample para encontrar el limite inferior
int Busqueda_Binaria_Sample_Caso3(const ArregloComprimido& ArregloC, int x){
	int m = ArregloC.Sample.size();
	if (m == 0 || x < ArregloC.Sample[0]) return -1; // Si el sample esta vacio o x es menor que el primer elemento, no se encuentra
	
	int L = 0;
	int R = m - 1;
	int lim_inf = 0;
	
	while (L <= R){
		int medio = L + (R - L) / 2;
		
		if (ArregloC.Sample[medio] <= x){
			lim_inf = medio; // actualiza el limite inferior
			L = medio + 1; // busca a la drecha
		}else{
			R = medio - 1; // busca a la izquierda
		}
	}
	return lim_inf; // Retorna el indice del limite inferior encontrado
}

//Decodifica secuencialmente sobre el arreglo comprimido con x como el numero a buscar y L como el inicio de el intervalo(Sample)
int Decodificacion(const ArregloComprimido& ArregloC, int x, int L){
    if (L == -1) return -1; 
    
    int inicio = L * ArregloC.salto;
    int sub_x = ArregloC.Sample[L];
    if (sub_x == x) return inicio;
    
    int bit_actual = ArregloC.offsets_bloques[L]; 

    std::string codigo_acumulado = "";
    int indice_virtual = inicio + 1;
 
    // Se detendra apenas el valor supere x o se acaben los bits.
    while (bit_actual < ArregloC.bits_validos){
        
        size_t byte_index = bit_actual / 8; 
        if (byte_index >= ArregloC.GC_Comprimido.size()) break; 
        
        int bit_index = 7 - (bit_actual % 8); 
        bool bit = (ArregloC.GC_Comprimido[byte_index] >> bit_index) & 1; 

        codigo_acumulado += (bit ? "1" : "0"); 
        bit_actual++; 
    
        auto it = ArregloC.Diccionario_Inv.find(codigo_acumulado);
        if (it != ArregloC.Diccionario_Inv.end()){
            int gap_decodificado = it->second; 
            sub_x += gap_decodificado; 

            if (sub_x > x) return -1; // el numero no existe
            if (sub_x == x) return indice_virtual; // encontrado

            codigo_acumulado = ""; 
            indice_virtual++; 
        }
    }
    return -1; 
}

int Busqueda_Binaria_C3(const ArregloComprimido& ArregloC, int x){
	int L = Busqueda_Binaria_Sample_Caso3(ArregloC, x); // encuentra el limite inferior en el sample
	return Decodificacion(ArregloC, x, L);
}