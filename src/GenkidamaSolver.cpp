#define CORRER_TESTS true

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

vector<int> resolverGenkidama(const int n, const int t, const vector< vector<int>> &coordenadasEnemigos, const int i, const int j){
	int m = (j - i)/2;
	if(coordenadasEnemigos[j][0] + t >= coordenadasEnemigos[i][0]){
		return vector<int>(1, j);
	}
	else if(coordenadasEnemigos[i][1] + t >= coordenadasEnemigos[j][1]){
		return vector<int>(1, i);
	}
	else if(coordenadasEnemigos[m][0] + t >= coordenadasEnemigos[i][0] && coordenadasEnemigos[m][1] + t >= coordenadasEnemigos[j][1]){
		return vector<int>(1, m);
	}
	else{
		vector<int> primerMitadGenkidama = resolverGenkidama(n, t, coordenadasEnemigos, i, m);
		vector<int> segundaMitadGenkidama = resolverGenkidama(n, t, coordenadasEnemigos, m + 1, j);
		primerMitadGenkidama.insert(primerMitadGenkidama.end(), segundaMitadGenkidama.begin(), segundaMitadGenkidama.end());
		return primerMitadGenkidama;
	}
}

void test_un_enemigo(){
	int n = 1;
	int t = 1;
	vector< vector<int>> coordenadasEnemigos = vector< vector<int>>(n, vector<int>(2));
	coordenadasEnemigos[0][0] = 1;
	coordenadasEnemigos[0][1] = 1;

	vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos, 0, n - 1);

	ASSERT_EQ(solucion[0], 0);
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

	vector<int> esperado {2};
	vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos, 0, n - 1);

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

	vector<int> esperado {0};
	vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos, 0, n - 1);

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

	vector<int> esperado {1};
	vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos, 0, n - 1);

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

	vector<int> esperado {0, 1, 2};
	vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos, 0, n - 1);

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

	vector<int> esperado {0, 2};
	vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos, 0, n - 1);

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

	vector<int> esperado {1, 2};
	vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos, 0, n - 1);

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
	}
	else{
		int n;
		int t;

		cin >> n >> t;

		vector< vector<int>> coordenadasEnemigos = vector< vector<int>>(n, vector<int>(2));
		for(int i = 0; i < n; i++){
			cin >> coordenadasEnemigos[i][0] >> coordenadasEnemigos[i][1];
		}

		vector<int> solucion = resolverGenkidama(n, t, coordenadasEnemigos, 0, n - 1);
	}
}
