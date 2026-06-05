#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <sstream>

#include "Caso_1.hpp"
#include "Caso_2.hpp"
#include "Caso_3.hpp"

using namespace std;
using namespace std::chrono;

// fase de medicion

void EjecutarTest(ofstream& archivo_csv, const string& nombre_dist, const vector<int>& A, int n, int m, int busquedas, double tiempo_build_caso1){
    cout << "\n Distribucion: " << nombre_dist << endl;

    // Caso 1
    size_t memoria_caso1 = A.capacity() * sizeof(int);

    auto start_bs_caso1 = high_resolution_clock::now();
    for(int i = 0; i < busquedas; i++){
        Busqueda_Binaria(A, A[rand() % n]);
    }
    auto stop_bs_caso1 = high_resolution_clock::now();
    double tiempo_bs_caso1 = duration_cast<microseconds>(stop_bs_caso1 - start_bs_caso1).count() / (double)busquedas;
    cout << "Caso 1 | Construccion: " << setw(8) << tiempo_build_caso1 << " us | Busqueda: " << setw(8) << tiempo_bs_caso1 << " us" <<
            "|Memoria: " << memoria_caso1 << " B" << endl;
    archivo_csv << n << "," << nombre_dist << ",Caso 1," << tiempo_build_caso1 << "," << tiempo_bs_caso1 << "," << memoria_caso1 << endl;

    // Caso 2
    auto start_build_caso2 = high_resolution_clock::now();
    ArregloC2 C2 = Crear_ArregloC2(A, m);
    auto stop_build_caso2 = high_resolution_clock::now();
    double tiempo_build_caso2 = duration_cast<microseconds>(stop_build_caso2 - start_build_caso2).count();

    size_t memoria_caso2 = (C2.GC.capacity() * sizeof(int)) + (C2.Sample.capacity() * sizeof(int)) + sizeof(int);

    auto start_bs_caso2 = high_resolution_clock::now();
    for(int i = 0; i < busquedas; i++){
        Busqueda_Binaria2(C2, A[rand() % n]);
    }
    auto stop_bs_caso2 = high_resolution_clock::now();
    double tiempo_bs_caso2 = duration_cast<microseconds>(stop_bs_caso2 - start_bs_caso2).count() / (double)busquedas;
    
    cout << "Caso 2 / Construccion: " << setw(8) << tiempo_build_caso2 << " us | Busqueda: " << setw(8) << tiempo_bs_caso2 << " us | Memoria: " << memoria_caso2 << " B" << endl;
    archivo_csv << n << "," << nombre_dist << ",Caso 2," << tiempo_build_caso2 << "," << tiempo_bs_caso2 << "," << memoria_caso2 << endl;

    // Caso 3
    auto start_build_caso3 = high_resolution_clock::now();
    unordered_map<int, int> frecuencia = ContarFrec(C2.GC);
    NodoHuffman* raiz = ConstruirArbolHuff(frecuencia);
    unordered_map<int, string> diccionario = CrearDiccionarioCodigos(raiz);
    ArregloComprimido C3 = Comprimir(C2, diccionario);
    auto stop_build_caso3 = high_resolution_clock::now();
    double tiempo_build_caso3 = duration_cast<microseconds>(stop_build_caso3 - start_build_caso3).count();
    
    size_t memoria_caso3 = (C3.GC_Comprimido.capacity()*sizeof(uint8_t)) +
                            (C3.Sample.capacity()*sizeof(int)) +
                            (C3.offsets_bloques.capacity()*sizeof(int));
    auto start_bs_caso3 = high_resolution_clock::now();
    for (int i = 0; i < busquedas; i++){
        Busqueda_Binaria_C3(C3, A[rand() % n]);
    }
    auto stop_bs_caso3 = high_resolution_clock::now();
    double tiempo_caso3 = duration_cast<microseconds>(stop_bs_caso3 - start_bs_caso3).count()/(double)busquedas;
    
    cout << "Caso 3 | Construccion: " << setw(8) << tiempo_build_caso3 << " us | Busqueda: " << setw(8) << tiempo_caso3 << " us | Memoria: " << memoria_caso3 << " B" << endl;
    archivo_csv << n << "," << nombre_dist << ",Caso 3," << tiempo_build_caso3 << "," << tiempo_caso3 << "," << memoria_caso3 << endl;
    
    BorrarArbol(raiz);
}


void EjecutarBenchmark() {
    // 10^5, 10^6, 10^7
    vector<int> tamanos = {100000, 1000000, 10000000}; 
    int busquedas = 10000; 

    ofstream archivo_csv("benchmark_resultados.csv");
    if (!archivo_csv.is_open()) {
        cerr << "Error: No se pudo crear el archivo CSV." << endl;
        return;
    }
    
    archivo_csv << "N,Distribucion,Caso,TiempoConstruccion_us,TiempoBusqueda_us,TiempoBusquedaPromedio_us,MemoriaBytes\n";

    cout << "INICIANDO BENCHMARK" << endl;

    for (int n : tamanos) {
        cout << endl;
        cout << "Evaluando n = " << n << endl;
        cout << endl;
        
        int m = n / 100; 

        // dist lineal
        auto start_lineal = high_resolution_clock::now();
        vector<int> A_lineal = Arreglo_Lineal(n, 10);
        auto stop_lineal = high_resolution_clock::now();
        double tiempo_lineal = duration_cast<microseconds>(stop_lineal - start_lineal).count();
        
        EjecutarTest(archivo_csv, "Lineal", A_lineal, n, m, busquedas, tiempo_lineal);
        A_lineal.clear(); A_lineal.shrink_to_fit(); // libera ram

        // distribución normal/desv std pequena
        auto start_normal1 = high_resolution_clock::now();
        vector<int> A_normal1 = Arreglo_Normal(n, 50000.0, 100.0);
        auto stop_normal1 = high_resolution_clock::now();
        double tiempo_normal1 = duration_cast<microseconds>(stop_normal1 - start_normal1).count();

        EjecutarTest(archivo_csv, "Normal_DesvPeq", A_normal1, n, m, busquedas, tiempo_normal1);
        A_normal1.clear(); A_normal1.shrink_to_fit(); // libera ram
        
        // dist normal/desv std grande
        auto start_normal2 = high_resolution_clock::now();
        vector<int> A_normal2 = Arreglo_Normal(n, 50000.0, 10000.0);
        auto stop_normal2 = high_resolution_clock::now();
        double tiempo_normal2 = duration_cast<microseconds>(stop_normal2 - start_normal2).count();

        EjecutarTest(archivo_csv, "Normal_DesvGrd", A_normal2, n, m, busquedas, tiempo_normal2);
        A_normal2.clear(); A_normal2.shrink_to_fit(); // libera ram
    }

    archivo_csv.close();
    cout << "\n BENCHMARK FINALIZADO. Resultados guardados en 'benchmark_resultados.csv'." << endl;
}

// Ejecucion archivo csv

void EjecutarModoArchivo(const string& ruta_csv) {
    cout << "\nINICIANDO MODO ARCHIVO" << endl;
    cout << "Cargando datos desde: " << ruta_csv << endl;

    vector<int> A;
    ifstream archivo(ruta_csv);
    string linea;

    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << ruta_csv << endl;
        return;
    }

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string valor;
        while (getline(ss, valor, ',')) {
            if (!valor.empty() && valor != "\r" && valor != "\n") {
                A.push_back(atoi(valor.c_str())); 
            }
        }
    }
    archivo.close();

    int n = A.size();
    if (n == 0) {
        cout << "Error: El archivo esta vacio o no contiene enteros validos." << endl;
        return;
    }

    cout << "Se han cargado " << n << " elementos. Ordenando Linea Base..." << endl;
    Ordenar_Arreglo(A); 

    int m = -1;
    while(m <= 0 || m > n / 2) {
        cout << "\nIngrese el tamano de la muestra (m) para el Caso 2 (debe ser > 0 y <= " << n/2 << "): ";
        if (!(cin >> m)) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Entrada invalida." << endl;
        }
    }

    cout << "\nConstruyendo estructuras (Caso 2 y Caso 3)..." << endl;
    //El caso 1 se construye leyendo el csv y ordenandolo jeje
	//Caso 2
    ArregloC2 C2 = Crear_ArregloC2(A, m);

	//Caso 3
    unordered_map<int, int> frecuencia = ContarFrec(C2.GC);
    NodoHuffman* raiz = ConstruirArbolHuff(frecuencia);
    unordered_map<int, string> diccionario = CrearDiccionarioCodigos(raiz);
    ArregloComprimido C3 = Comprimir(C2, diccionario);

    cout << "Estructuras listas. Entrando a fase interactiva.\n" << endl;

    int opcion_estructura = -1;
    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "¿En que estructura deseas buscar?" << endl;
        cout << "1. Caso 1 (Representacion Explicita)" << endl;
        cout << "2. Caso 2 (Gap-Coding)" << endl;
        cout << "3. Caso 3 (Huffman)" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";
        
        if (!(cin >> opcion_estructura)) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Entrada invalida. Ingrese un numero." << endl;
            continue;
        }

        if (opcion_estructura == 0) {
            cout << "Finalizando Modo Archivo..." << endl;
            break;
        }
        if (opcion_estructura < 1 || opcion_estructura > 3) {
            cout << "Opcion no valida." << endl;
            continue;
        }

        int x;
        cout << "Ingrese el valor numerico a buscar: ";
        if (!(cin >> x)) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Entrada invalida." << endl;
            continue;
        }

        int posicion = -1;
        auto inicio = high_resolution_clock::now();
        auto fin = high_resolution_clock::now();

        switch (opcion_estructura) {
            case 1:
                inicio = high_resolution_clock::now();
                posicion = Busqueda_Binaria(A, x);
                fin = high_resolution_clock::now();
                break;
            case 2:
                inicio = high_resolution_clock::now();
                posicion = Busqueda_Binaria2(C2, x);
                fin = high_resolution_clock::now();
                break;
            case 3:
                inicio = high_resolution_clock::now();
                posicion = Busqueda_Binaria_C3(C3, x);
                fin = high_resolution_clock::now();
                break;
        }

        double tiempo_ns = duration_cast<nanoseconds>(fin - inicio).count();

        cout << "\n => RESULTADO DE LA BUSQUEDA:" << endl;
        if (posicion != -1) {
            cout << "    [+] Elemento " << x << " ENCONTRADO en el indice: " << posicion << endl;
        } else {
            cout << "    [-] Elemento " << x << " NO ENCONTRADO (-1)." << endl;
        }
        cout << "    [~] Tiempo de busqueda: " << tiempo_ns << " nanosegundos.\n" << endl;
    }

    BorrarArbol(raiz); 
}

int main(int argc, char* argv[]) {
    srand(time(NULL)); // semilla aleatoria

    if (argc == 2 && strcmp(argv[1], "--benchmark") == 0) {
        EjecutarBenchmark();
    }
    else if (argc == 3 && strcmp(argv[1], "-i") == 0){
        EjecutarModoArchivo(argv[2]);
    } 
    else {
        cout << "Modo de uso correcto para benchmark:" << endl;
        cout << "./main --benchmark" << endl;
        cout << "\nModo de uso correcto para modo archivo:" << endl;
        cout << "./main -i ruta/del/archivo.csv " << endl;
    }

    return 0;
}