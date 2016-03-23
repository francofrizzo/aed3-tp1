#define CORRER_TESTS false

#include <iostream>
#include <vector>
#include "mini_test.h"

using namespace std;

void imprimirVector(const vector<int> &vectorAImprimir){
	cout << "[";
	for(unsigned int i = 0; i < vectorAImprimir.size(); i++){
		cout << vectorAImprimir[i];
		if(i < vectorAImprimir.size() - 1){
			cout << ", ";
		}
	}
	cout << "]" << endl;
}

bool compararVectores(const vector<int> calculado, const vector<int> esperado){
	if(calculado.size() != esperado.size()){
		return false;
	}
	else{
		for(unsigned int i = 0; i < calculado.size(); i++){
			if(calculado[i] != esperado[i]){
				return false;
			}
		}
	}
	return true;
}

vector<int> resolverGenkidama(const int n, const int t, const vector< vector<int>> &coordenadasEnemigos){
	vector<int> solucion;
	int max_y = 0;
	int x_a_cubrir = 0;
	if(coordenadasEnemigos.size() > 1){
		// Primer elemento, si no está cubierto por el siguiente, lo agrego
		if(coordenadasEnemigos[1][0] + t < coordenadasEnemigos[0][0]){
			solucion.push_back(1);
			// Me guardo la altura para ver si cubre a alguno que esté delante
			max_y = coordenadasEnemigos[0][1] + t;
		}
		else{
			x_a_cubrir = coordenadasEnemigos[0][0];
		}
		// Elementos entre el primer y último
		for(unsigned int i = 1; i < coordenadasEnemigos.size() - 1; i++){
			// Si no está cubierto por un elemento anterior
			if(coordenadasEnemigos[i][1] > max_y){
				// Si el siguiente elemento me cubre
				if(coordenadasEnemigos[i + 1][0] + t >= coordenadasEnemigos[i][0]){
					x_a_cubrir = (x_a_cubrir == 0) ? coordenadasEnemigos[i][0] : x_a_cubrir;
					// Si no cubre al que yo si, entonces me agrego
					if(coordenadasEnemigos[i + 1][0] + t < x_a_cubrir){
						solucion.push_back(i + 1);
						max_y = coordenadasEnemigos[i][1] + t;
						x_a_cubrir = 0;
					}
				}
				else{
					solucion.push_back(i + 1);
					max_y = coordenadasEnemigos[i][1] + t;
					x_a_cubrir = 0;
				}
			}
		}
		// Último elemento
		if(coordenadasEnemigos[n - 1][1] > max_y){
			solucion.push_back(n);
		}
	}
	else{
		// Solución para único elemento
		solucion.push_back(1);
	}

	return solucion;
}

void test_un_enemigo(){
	int n = 1;
	int t = 1;
	vector< vector<int>> coordenadasEnemigos = vector< vector<int>>(n, vector<int>(2));
	coordenadasEnemigos[0][0] = 1;
	coordenadasEnemigos[0][1] = 1;

	vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos);

	ASSERT_EQ(solucion[0], 1);
}

void test_tres_enemigos_superior(){
	int n = 3;
	int t = 2;
	vector< vector<int>> coordenadasEnemigos = vector< vector<int>>(n, vector<int>(2));
	coordenadasEnemigos[0][0] = 2;
	coordenadasEnemigos[0][1] = 0;
	coordenadasEnemigos[1][0] = 1;
	coordenadasEnemigos[1][1] = 1;
	coordenadasEnemigos[2][0] = 0;
	coordenadasEnemigos[2][1] = 5;

	vector<int> esperado {3};
	vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos);

	ASSERT(compararVectores(solucion, esperado));
}

void test_tres_enemigos_inferior(){
	int n = 3;
	int t = 2;
	vector< vector<int>> coordenadasEnemigos = vector< vector<int>>(n, vector<int>(2));
	coordenadasEnemigos[0][0] = 5;
	coordenadasEnemigos[0][1] = 0;
	coordenadasEnemigos[1][0] = 1;
	coordenadasEnemigos[1][1] = 1;
	coordenadasEnemigos[2][0] = 0;
	coordenadasEnemigos[2][1] = 2;

	vector<int> esperado {1};
	vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos);

	ASSERT(compararVectores(solucion, esperado));
}

void test_tres_enemigos_medio(){
	int n = 3;
	int t = 1;
	vector< vector<int>> coordenadasEnemigos = vector< vector<int>>(n, vector<int>(2));
	coordenadasEnemigos[0][0] = 2;
	coordenadasEnemigos[0][1] = 0;
	coordenadasEnemigos[1][0] = 1;
	coordenadasEnemigos[1][1] = 1;
	coordenadasEnemigos[2][0] = 0;
	coordenadasEnemigos[2][1] = 2;

	vector<int> esperado {2};
	vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos);

	ASSERT(compararVectores(solucion, esperado));
}

void test_tres_enemigos_separados(){
	int n = 3;
	int t = 1;
	vector< vector<int>> coordenadasEnemigos = vector< vector<int>>(n, vector<int>(2));
	coordenadasEnemigos[0][0] = 4;
	coordenadasEnemigos[0][1] = 0;
	coordenadasEnemigos[1][0] = 2;
	coordenadasEnemigos[1][1] = 2;
	coordenadasEnemigos[2][0] = 0;
	coordenadasEnemigos[2][1] = 4;

	vector<int> esperado {1, 2, 3};
	vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos);

	ASSERT(compararVectores(solucion, esperado));
}

void test_tres_enemigos_superior_medio(){
	int n = 3;
	int t = 1;
	vector< vector<int>> coordenadasEnemigos = vector< vector<int>>(n, vector<int>(2));
	coordenadasEnemigos[0][0] = 4;
	coordenadasEnemigos[0][1] = 0;
	coordenadasEnemigos[1][0] = 1;
	coordenadasEnemigos[1][1] = 1;
	coordenadasEnemigos[2][0] = 0;
	coordenadasEnemigos[2][1] = 4;

	vector<int> esperado {1, 3};
	vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos);

	ASSERT(compararVectores(solucion, esperado));
}

void test_tres_enemigos_inferior_medio(){
	int n = 3;
	int t = 1;
	vector< vector<int>> coordenadasEnemigos = vector< vector<int>>(n, vector<int>(2));
	coordenadasEnemigos[0][0] = 4;
	coordenadasEnemigos[0][1] = 0;
	coordenadasEnemigos[1][0] = 3;
	coordenadasEnemigos[1][1] = 1;
	coordenadasEnemigos[2][0] = 0;
	coordenadasEnemigos[2][1] = 4;

	vector<int> esperado {2, 3};
	vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos);

	ASSERT(compararVectores(solucion, esperado));
}

void test_cuatro_enemigos_dos_partes(){
	int n = 4;
	int t = 1;
	vector< vector<int>> coordenadasEnemigos = vector< vector<int>>(n, vector<int>(2));
	coordenadasEnemigos[0][0] = 6;
	coordenadasEnemigos[0][1] = 0;
	coordenadasEnemigos[1][0] = 5;
	coordenadasEnemigos[1][1] = 2;
	coordenadasEnemigos[2][0] = 3;
	coordenadasEnemigos[2][1] = 4;
	coordenadasEnemigos[3][0] = 2;
	coordenadasEnemigos[3][1] = 6;

	vector<int> esperado {2, 4};
	vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos);

	ASSERT(compararVectores(solucion, esperado));
}

void test_seis_enemigos_cuatro_partes(){
	int n = 6;
	int t = 2;
	vector< vector<int>> coordenadasEnemigos = vector< vector<int>>(n, vector<int>(2));
	coordenadasEnemigos[0][0] = 9;
	coordenadasEnemigos[0][1] = 0;
	coordenadasEnemigos[1][0] = 8;
	coordenadasEnemigos[1][1] = 3;

	coordenadasEnemigos[2][0] = 6;
	coordenadasEnemigos[2][1] = 6;

	coordenadasEnemigos[3][0] = 3;
	coordenadasEnemigos[3][1] = 9;
	coordenadasEnemigos[4][0] = 2;
	coordenadasEnemigos[4][1] = 12;

	coordenadasEnemigos[5][0] = 0;
	coordenadasEnemigos[5][1] = 15;

	vector<int> esperado {2, 3, 5, 6};
	vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos);

	ASSERT(compararVectores(solucion, esperado));
}

int main(){
	if(CORRER_TESTS){
		RUN_TEST(test_un_enemigo);
		RUN_TEST(test_tres_enemigos_superior);
		RUN_TEST(test_tres_enemigos_inferior);
		RUN_TEST(test_tres_enemigos_medio);
		RUN_TEST(test_tres_enemigos_separados);
		RUN_TEST(test_tres_enemigos_superior_medio);
		RUN_TEST(test_tres_enemigos_inferior_medio);
		RUN_TEST(test_cuatro_enemigos_dos_partes);
		RUN_TEST(test_seis_enemigos_cuatro_partes);
	}
	else{
		int n;
		int t;

		cin >> n >> t;

		vector< vector<int>> coordenadasEnemigos = vector< vector<int>>(n, vector<int>(2));
		for(int i = 0; i < n; i++){
			cin >> coordenadasEnemigos[i][0] >> coordenadasEnemigos[i][1];
		}

		vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos);

		cout << solucion.size() << endl;
		for(unsigned int i = 0; i < solucion.size(); i++){
			cout << solucion[i] << " ";
		}
		cout << endl;
	}
}
