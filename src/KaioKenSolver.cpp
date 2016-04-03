#include <cmath>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <unistd.h>

using namespace std;

static chrono::time_point<chrono::high_resolution_clock> start_time;

/*
**  Funciones auxiliares
*/

void start_timer() {
    start_time = chrono::high_resolution_clock::now();
}

double stop_timer() {
    chrono::time_point<chrono::high_resolution_clock> end_time = chrono::high_resolution_clock::now();
    return double(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());
}

// Imprime un vector (de enteros) a la salida estándar.
// Los elementos se imprimen separados por espacios,
// en una única línea seguida de un caracter de fin de línea.
void imprimirVector(vector < int > vec) {
    for (unsigned int i = 0; i < vec.size(); ++i) {
        cout << vec[i] << " ";
    }
    cout << endl;
}

// Recibe una lista de vectores. Imprime a la salida estándar
// el número de elementos de la lista, y a continuación todos
// los vectores, cada uno en una nueva línea
void imprimirVectores(vector < vector < int > > vecs) {
    cout << vecs.size() << endl;
    for (unsigned int i = 0; i < vecs.size(); ++i) {
        imprimirVector(vecs[i]);
    }
}

/*
**  Resolución del ejercicio
*/

vector< vector<int>> generarPeleas(int n){
	int cantidadPeleas = ceil(log(n)/log(2));
    vector< vector<int>> peleas = vector< vector<int>>(cantidadPeleas, vector<int>(n, 1));
	int saltos = 1;
	for(int i = 0; i < cantidadPeleas; i++){
		for(int j = 0; j < n; j += 2*saltos){
			for(int k = 0; k < saltos; k++){
				if(j + k < n){
					peleas[i][j + k] = 2;
				}
			}
		}
		saltos *= 2;
	}
	return peleas;
}

void generarPerformanceTest(int saltos, int muestras){
	ofstream archivoSalida;
	archivoSalida.open("kaio_ken_output");
	double tiempo_promedio = 0;
	double tiempo_promedio_log = 0;
	double tiempo_promedio_c = 0;
	int potencia_dos = 1;
	int potencia = 1;

	while(potencia < saltos){
		potencia_dos *= 2;
		tiempo_promedio = 0;
		for(int j = 0; j < muestras; j++){
			start_timer();
			generarPeleas(potencia_dos);
			tiempo_promedio += stop_timer();
		}
		tiempo_promedio /= muestras;
		tiempo_promedio_log = tiempo_promedio/potencia_dos;
		tiempo_promedio_c = tiempo_promedio_log/log(potencia_dos);
		archivoSalida << potencia_dos << " " << tiempo_promedio << " " << tiempo_promedio_log << " " << tiempo_promedio_c << endl;
		potencia++;
	}

	archivoSalida.close();
}

int main(int argc, char *argv[]) {
	if(argc > 1){
		char opt;
		while ((opt = getopt(argc, argv, "tp")) != -1) {
			switch (opt) {
				case 't':
					break;
				case 'p':
					generarPerformanceTest(32, 2);
					break;
			}
		}
	}
	else{
		unsigned int n;
		cin >> n;
		imprimirVectores(generarPeleas(n));
	}
}
