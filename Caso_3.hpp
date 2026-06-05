#ifndef CASO_3_HPP
#define CASO_3_HPP

#include <vector>
#include <unordered_map>
#include <queue>
#include <cstdint>
#include <string>

#include "Caso_2.hpp" 

//Definir la estructura (Arbol de huffman) 
struct NodoHuffman {
    int gap;
    int frecuencia;
    NodoHuffman* izq;
    NodoHuffman* der;
    
    NodoHuffman(int g, int f);
};

struct CompararNodos {
    bool operator()(NodoHuffman* a, NodoHuffman* b);
};

struct ArregloComprimido {
    std::vector<uint8_t> GC_Comprimido;
    std::vector<int> Sample; 
    int salto; 
    
    // diccionario inverso O(1) para decodificacion ultra rapida (binario -> gap)
    std::unordered_map<std::string, int> Diccionario_Inv;
    
    // punteros de bits para saber donde empieza cada bloque del sample
    std::vector<int> offsets_bloques;
    int bits_validos;
};


std::unordered_map<int, int> ContarFrec(const std::vector<int>& GC);
NodoHuffman* ConstruirArbolHuff(const std::unordered_map<int, int>& frecuencia);
void BorrarArbol(NodoHuffman* nodo);
void GenerarCodigos(NodoHuffman* nodo, std::string codigo_actual, std::unordered_map<int, std::string>& diccionario);
std::unordered_map<int, std::string> CrearDiccionarioCodigos(NodoHuffman* raiz);
ArregloComprimido Comprimir(const ArregloC2& caso2, const std::unordered_map<int, std::string>& diccionario);

int Busqueda_Binaria_Sample_Caso3(const ArregloComprimido& ArregloC, int x);
int Decodificacion(const ArregloComprimido& ArregloC, int x, int L);

// funcion para el main
int Busqueda_Binaria_C3(const ArregloComprimido& ArregloC, int x);

#endif