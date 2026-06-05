##Grupo 12
##Algoritmo Huffman

## Integrantes

*Martin Pozo 
*Amaru Quintrel
*Andy Briones
*Quidel Barriga

---------------------------------------------------------

## Descripción del Proyecto

Caso 1: Representación explícita (arreglo ordenado base).
Caso 2: Gap-Coding (compresión mediante diferencias con arreglo de muestreo o Sample).
Caso 3: Compresión Huffman (optimización sobre el Gap-Coding utilizando un árbol de Huffman).

---------------------------------------------------------

## Requisitos y Compilación

El proyecto está desarrollado en C++.

Para compilar el código fuente, abre una terminal en la raíz del repositorio y ejecuta:
Nota: Si está utilizando un entorno Windows con MinGW, utilice el comando mingw32-make

g++ -O3 -Wall main.cpp Caso_1.cpp Caso_2.cpp Caso_3.cpp -o main
.\main.exe --benchmark

Esto generará el ejecutable principal llamado:

Main

---------------------------------------------------------

## Modos de Ejecución y Uso

Al ejecutar el programa, se solicitará el modo de operación. Este debe ingresarse exactamente como aparece en las instrucciones.

### 1. Modo Benchmark

Comando de uso:

.\main.exe --benchmark

Al ejecutar este modo, el programa realizará automáticamente todas las pruebas correspondientes a los casos 1, 2 y 3.

Al finalizar:

Se generará un archivo .csv con los resultados obtenidos.
Se liberará la memoria utilizada durante la ejecución.

---------------------------------------------------------

### 2. Modo Archivo

Comando de uso:

bash
./main -i <ruta_del_archivo.csv>


Al seleccionar este modo, se solicitará la ruta de un archivo CSV.

#### Formato esperado del archivo

El archivo debe contener números enteros separados por comas:

csv
#Ejemplo	
	10,5,23,8,100,42


El programa leerá estos datos y los transformará en un arreglo de enteros.

Posteriormente:

1. Ordenará el arreglo.
2. Construirá las estructuras correspondientes a:

   Caso 2 (Gap-Coding).
   Caso 3 (Gap-Coding + Huffman).

Nota: Durante este proceso se solicitará el parámetro m, que define el tamaño de la muestra (Sample).

---------------------------------------------------------

### Selección de Estructura

Una vez construidas las estructuras, el programa solicitará elegir una opción:

| Opción | Estructura               |
| ------ | ------------------------ |
| 1      | Caso 1: Arreglo ordenado |
| 2      | Caso 2: Gap-Coding       |
| 3      | Caso 3: Huffman          |
| 0      | Salir                    |

Se recomienda ingresar únicamente los valores indicados.

---------------------------------------------------------

### Búsqueda de Elementos

Después de seleccionar la estructura, el programa solicitará un valor x para buscar dentro del arreglo.

Como resultado se mostrará:

 Si el elemento fue encontrado.
 La posición donde fue encontrado.
 O bien que el elemento no existe en la estructura.
 El tiempo promedio de búsqueda.

Para obtener mediciones más estables, cada búsqueda se ejecuta internamente 1.000 veces y se reporta el promedio de tiempo observado.

---------------------------------------------------------

### Liberación de Memoria

Al finalizar la ejecución:

El árbol de Huffman es liberado explícitamente.
Los arreglos se destruyen automáticamente al salir de su ámbito (scope), aprovechando la gestión automática de memoria de C++.

---------------------------------------------------------

##Especificación de Datos y Límites

### Rango de Números Aceptados

El programa trabaja internamente con el tipo de dato int estándar de C++ (32 bits con signo).

Por lo tanto, los valores contenidos en el archivo CSV y los valores ingresados para búsqueda deben encontrarse dentro del siguiente rango:

-2.147.483.648  a  2.147.483.647


---------------------------------------------------------

### Conversión de Datos

La conversión desde texto a números enteros se realiza mediante la función estándar:

cpp
atoi()


incluida en:

cpp
<cstdlib>


Ingresar valores fuera del rango permitido puede provocar:

Desbordamiento (overflow).
Truncamiento de datos.
Resultados indefinidos dependiendo de la implementación utilizada.

Por esta razón, se recomienda utilizar únicamente valores dentro del rango válido para int.
