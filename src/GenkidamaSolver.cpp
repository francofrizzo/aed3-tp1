#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>    // iota
#include <unistd.h>
#include <algorithm>
#include <chrono>
#include "mini_test.h"
#include <cmath>
#include <climits>

using namespace std;

#define T                 	  1000
#define MAX_N                 10
#define CANT_REPETICIONES     40
#define CANT_INST_DESCARTADAS 20
#define CANT_REP_COMPLETAS     1

#define PRUEBA_PEOR_CASO       0
#define PRUEBA_CASO_INTERMEDIO 1
#define PRUEBA_MEJOR_CASO      2
#define PRUEBA_VARIAR_T        3

/*
**  Variables globales
*/
unsigned int N;
vector< vector<int> > coordenadasEnemigos;

// Medición de tiempos

static chrono::time_point<chrono::high_resolution_clock> start_time;

void start_timer() {
    start_time = chrono::high_resolution_clock::now();
}

double stop_timer() {
    chrono::time_point<chrono::high_resolution_clock> end_time = chrono::high_resolution_clock::now();
    return double(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());
}




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

void imprimirVectores(vector < vector < int > > vecs) {
    cout << vecs.size() << endl;
    for (unsigned int i = 0; i < vecs.size(); ++i) {
        imprimirVector(vecs[i]);
    }
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

/*
**  Pruebas de performance
*/

// Genera una instancia del problema de tamaño n donde no existen tres enemigos
// que se encuentren alineados
vector < vector<int> > generarPeorCaso(unsigned int n,unsigned int t) {
    vector< vector<int> > enemigos = vector< vector<int> >(n, vector<int>(2));
    unsigned int normaT = (RAND_MAX / (t +1));
    vector<int> xs;
    vector<int> ys;
    int Xi;
    for (unsigned int i = 0; i < n; ++i){
	    do{
			Xi = rand() % normaT;
		}while(find(xs.begin(), xs.end(), Xi) != xs.end());
		int Yi;
		do{
			Yi = rand() % normaT ;
		}while(find(ys.begin(), ys.end(), Yi) != ys.end());
		xs.push_back(Xi);
  		ys.push_back(Yi);
	}
    
    sort (xs.begin(), xs.end());
  	sort (ys.begin(), ys.end());
  	

  	for (unsigned int i = 0; i < n; ++i){
  		enemigos[i][0] = xs[n - (i+1)] * (t + 1);
    	enemigos[i][1] = ys[i] * (t + 1);
  	}
    cout << "PEOR   " << enemigos.size() << endl;
    imprimirVectores(enemigos);

    return enemigos;
}

// Genera una instancia del problema de tamaño n donde todos los enemigos se
// encuentran sobre una única línea recta
vector < vector<int> > generarMejorCaso(unsigned int n,unsigned int t) {
    
    vector < vector<int> > enemigos = vector< vector<int> >(n, vector<int>(2));
    vector<int> xsA;
    vector<int> ysA;
    vector<int> xsB;
    vector<int> ysB;
    int genkidamaX = rand() % (RAND_MAX - (n + t) ) + t;
  	int genkidamaY = rand() % (RAND_MAX - (n + t) ) + t;
  	for (unsigned int i = 1; i < n; ++i){
  		if ( (rand()% 2) == 1){
  			// cae en A
  			int Xi = 0;
  			do{
  				Xi = (rand() % genkidamaX)  ;
  			}while(find(xsA.begin(), xsA.end(), Xi) != xsA.end());
  			int Yi = 0;
  			do{
  				Yi = (rand() % t) + genkidamaY;
  			}while(find(ysA.begin(), ysA.end(), Yi) != ysA.end());
  			xsA.push_back(Xi);
  			ysA.push_back(Yi);
  		}else{
  			// cae en B
  			int Xi = 0;
  			do{
  				Xi = (rand() % t) + genkidamaX;
  			}while(find(xsB.begin(), xsB.end(), Xi) != xsB.end());
  			int Yi = 0;
  			do{
  				Yi = (rand() % genkidamaY);
  			}while(find(ysB.begin(), ysB.end(), Yi) != ysB.end());
  			xsB.push_back(Xi);
  			ysB.push_back(Yi);
  		}
  	}
  	sort (xsA.begin(), xsA.end());
  	sort (ysA.begin(),ysA.end());
  	

  	sort (xsB.begin(), xsB.end());
  	sort (ysB.begin(), ysB.end());
  	

  	for (unsigned int i = 0; i < xsB.size(); ++i){
  		enemigos[i][0] = xsB[xsB.size() - (i+1)];
    	enemigos[i][1] = ysB[i];
  	}

  	enemigos[xsB.size()][0] = genkidamaX;
    enemigos[xsB.size()][1] = genkidamaY;


  	for (unsigned int i = xsB.size() + 1; i <  n; ++i){
  		enemigos[i][0] = xsA[n - (i +1)];
    	enemigos[i][1] = ysA[i - (xsB.size() + 1)];
  	}

    cout << "MEJOR  " << enemigos.size() << endl;
    imprimirVectores(enemigos);

    return enemigos;
}

// Genera una instancia del problema de tamaño n donde la mitad de los enemigos
// se encuentran sobre una línea recta y la otra mitad, sobre otra línea recta
// NOTA: las rectas podrían no ser distintas. Sin embargo, la probabilidad de
// que esto suceda es despreciable
vector< vector<int> > generarCasoIntermedio(unsigned int n) {
   vector< vector<int> > enemigos = vector< vector<int> >(n, vector<int>(2));
    vector<int> xs;
    vector<int> ys;
    int Xi;
    for (unsigned int i = 0; i < n; ++i){
	    do{
			Xi = rand();
		}while(find(xs.begin(), xs.end(), Xi) != xs.end());
		int Yi;
		do{
			Yi = rand() ;
		}while(find(ys.begin(), ys.end(), Yi) != ys.end());
		xs.push_back(Xi);
  		ys.push_back(Yi);
	}
    
    sort (xs.begin(), xs.end());
  	sort (ys.begin(), ys.end());
  	

  	for (unsigned int i = 0; i < n; ++i){
  		enemigos[i][0] = xs[n - (i+1)] ;
    	enemigos[i][1] = ys[i] ;
  	}
    cout << "INTERMEDIO   " << enemigos.size() << endl ;
    imprimirVectores(enemigos);

    return enemigos;
}


// Genera una instancia del problema de tamaño n dividiendo a los enemigos en
// subconjuntos de tamaño aleatorio y ubicando a cada subconjunto sobre una
// línea recta
vector < vector<int> > generarVariandoT(unsigned int t) {
   

    vector< vector<int> > enemigos = vector< vector<int> >(10, vector<int>(2));

  
    
    return enemigos;

}

void ejecutarPruebas(int prueba_id, ofstream& archivoSalida) {
    for (unsigned int i = 1; i <= MAX_N; i++) {
        double tiempos[CANT_REPETICIONES];
        double tiempo_promedio = 0;
        double desv_estandar = 0;

        N = i;

        switch (prueba_id) {
            case PRUEBA_MEJOR_CASO:
                coordenadasEnemigos = generarMejorCaso(N,T);
                break;

            case PRUEBA_CASO_INTERMEDIO:
                coordenadasEnemigos = generarCasoIntermedio(N);
                break;

            case PRUEBA_PEOR_CASO:
                coordenadasEnemigos = generarPeorCaso(N,T);
                break;

            case PRUEBA_VARIAR_T:
                coordenadasEnemigos = generarVariandoT(N);
                break;
        }
        
        for (int r = -CANT_INST_DESCARTADAS; r < CANT_REPETICIONES; r++) {
            double tiempo;
            
            start_timer();
            resolverGenkidama(N, T, coordenadasEnemigos);
            tiempo = stop_timer();

            if (r >= 0) {
                tiempos[r] = tiempo;
                tiempo_promedio += tiempos[r];
            }
        }

        tiempo_promedio = tiempo_promedio / CANT_REPETICIONES;

        for (unsigned int r = 0; r < CANT_REPETICIONES; r++) {
            desv_estandar += (tiempos[r] - tiempo_promedio) * (tiempos[r] - tiempo_promedio);
        }
        desv_estandar = sqrt(desv_estandar / CANT_REPETICIONES);

        archivoSalida << i << " " << tiempo_promedio << " " << desv_estandar << endl;
    }
}

int main(int argc, char *argv[]) {
	if(argc > 1){
		char opt;
		while ((opt = getopt(argc, argv, "tp:")) != -1) {
			switch (opt) {
				case 't':
					RUN_TEST(test_un_enemigo);
					RUN_TEST(test_tres_enemigos_superior);
					RUN_TEST(test_tres_enemigos_inferior);
					RUN_TEST(test_tres_enemigos_medio);
					RUN_TEST(test_tres_enemigos_separados);
					RUN_TEST(test_tres_enemigos_superior_medio);
					RUN_TEST(test_tres_enemigos_inferior_medio);
					RUN_TEST(test_cuatro_enemigos_dos_partes);
					RUN_TEST(test_seis_enemigos_cuatro_partes);
					break;
				case 'p':
					ofstream archivoSalida;
					srand(stoi(optarg));

	                archivoSalida.open("../exp/genkidama_peor_caso_output");
	                ejecutarPruebas(PRUEBA_PEOR_CASO, archivoSalida);
	                archivoSalida.close();

	                archivoSalida.open("../exp/genkidama_caso_intermedio_output");
	                ejecutarPruebas(PRUEBA_CASO_INTERMEDIO, archivoSalida);
	                archivoSalida.close();

                    archivoSalida.open("../exp/genkidama_mejor_caso_output");
                    ejecutarPruebas(PRUEBA_MEJOR_CASO, archivoSalida);
                    archivoSalida.close();

                    // archivoSalida.open("genkidama_variar_T_output");
                    // ejecutarPruebas(PRUEBA_VARIAR_T, archivoSalida);
                    // archivoSalida.close();

					break;
			}
		}
	}
	else{
		int n;
		int t;

		cin >> n >> t;

		coordenadasEnemigos = vector< vector<int> >(n, vector<int>(2));
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
