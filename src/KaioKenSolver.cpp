#include <iostream>
#include <algorithm>
#include <vector>
#include <unistd.h>

using namespace std;

/*
**  Funciones auxiliares
*/

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

int main(int argc, char *argv[]) {
	if(argc > 1){
		char opt;
		while ((opt = getopt(argc, argv, "tp")) != -1) {
			switch (opt) {
				case 't':
					break;
				case 'p':
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
