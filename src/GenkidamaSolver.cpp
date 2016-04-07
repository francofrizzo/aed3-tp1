#include <iostream>
#include <vector>
#include <unistd.h>
#include "mini_test.h"

using namespace std;

#define T                 	  10000
#define MAX_N                 10000
#define CANT_REPETICIONES     40
#define CANT_INST_DESCARTADAS 20
#define CANT_REP_COMPLETAS     1

#define PRUEBA_PEOR_CASO       0
#define PRUEBA_CASO_INTERMEDIO 1
#define PRUEBA_MEJOR_CASO      2
#define PRUEBA_RANDOM          3

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

/*
**  Pruebas de performance
*/

// Genera una instancia del problema de tamaño n donde no existen tres enemigos
// que se encuentren alineados
vector<XY> generarPeorCaso(unsigned int n) {
    vector<XY> enemigos;
    vector<int> xs;
    vector<int> ys;
    int anteriorX = rand();
	int anteriorY = rand();
	xs.push_back(anteriorX);
    ys.push_back(anteriorY);
    //genero los x e y crecientes con distancia T
    for (int i = 1; i < n; ++i){
    	anteriorX = anteriorX + T + rand();
    	anteriorY = anteriorY + T + rand();
    	xs.push_back(anteriorX);
    	ys.push_back(anteriorY);
    }
    
    // cout << "PEOR   " << enemigos.size() << endl;

    return enemigos;
}

// Genera una instancia del problema de tamaño n donde todos los enemigos se
// encuentran sobre una única línea recta
vector<XY> generarMejorCaso(unsigned int n) {
    vector<XY> enemigos;

    if (n >= 1) {
        enemigos.push_back(XY(rand(), rand()));
    }

    if (n >=2) {
        XY enemigo;
        do {
            enemigo = XY(rand(), rand());
        } while (enemigo == enemigos[0]);
        enemigos.push_back(enemigo);
    }

    if (n >= 3) {
        Recta r = Recta(enemigos[0], enemigos[1]);

        for (unsigned int i = 2; i < n; i++) {
            XY enemigo;
            bool repetido = true;
            while (repetido) {
                enemigo = r.puntoAleatorio();
                repetido = false;
                for (unsigned int j = 2; j < enemigos.size(); j++) {
                    if (enemigo == enemigos[j]) {
                        repetido = true;
                        break;
                    }
                }
            }
            enemigos.push_back(enemigo);
        }
    }

    // cout << "MEJOR  " << enemigos.size() << endl;

    return enemigos;
}

// Genera una instancia del problema de tamaño n donde la mitad de los enemigos
// se encuentran sobre una línea recta y la otra mitad, sobre otra línea recta
// NOTA: las rectas podrían no ser distintas. Sin embargo, la probabilidad de
// que esto suceda es despreciable
vector<XY> generarCasoIntermedioRandom(unsigned int n) {
    vector<XY> enemigos;

    unsigned int k;
    unsigned int restantes = n;

    while (restantes > 0) {
        if (restantes < 4) {
            k = restantes;
        } else {
            k = (rand() % (restantes - 3)) + 3;
        }
        vector<XY> subEnemigos = generarMejorCaso(k);
        enemigos.insert(enemigos.end(), subEnemigos.begin(), subEnemigos.end());
        restantes = restantes - k;
    }

    random_shuffle(enemigos.begin(), enemigos.end());

    // cout << "INTERM " << enemigos.size() << endl;

    return enemigos;
}

// Genera una instancia del problema de tamaño n de forma totalmente aleatoria
vector<XY> generarCasoRandom(unsigned int n) {
    vector<XY> enemigos;

    for (unsigned int i = 0; i < n; i ++) {
        enemigos.push_back(XY(rand(), rand()));
    }

    // cout << "RANDOM " << enemigos.size() << endl;

    return enemigos;
}

// Genera una instancia del problema de tamaño n dividiendo a los enemigos en
// subconjuntos de tamaño aleatorio y ubicando a cada subconjunto sobre una
// línea recta
vector<XY> generarCasoIntermedio(unsigned int n) {
    unsigned int k = n / 2;

    vector<XY> subEnemigos1 = generarMejorCaso(k);
    vector<XY> subEnemigos2 = generarMejorCaso(n - k);

    subEnemigos1.insert(subEnemigos1.end(), subEnemigos2.begin(), subEnemigos2.end());

    random_shuffle(subEnemigos1.begin(), subEnemigos1.end());
    // cout << "INTERM " << subEnemigos1.size() << endl;
    
    return subEnemigos1;

}

void ejecutarPruebas(int prueba_id, ofstream& archivoSalida) {
    for (unsigned int i = 1; i <= MAX_N; i++) {
        double tiempos[CANT_REPETICIONES];
        double tiempo_promedio = 0;
        double desv_estandar = 0;
        bool instancias_random = false;

        N = i;

        switch (prueba_id) {
            case PRUEBA_MEJOR_CASO:
                coordenadasEnemigos = generarMejorCaso(N);
                break;

            case PRUEBA_CASO_INTERMEDIO:
                coordenadasEnemigos = generarCasoIntermedio(N);
                break;

            case PRUEBA_PEOR_CASO:
                coordenadasEnemigos = generarPeorCaso(N);
                break;

            case PRUEBA_RANDOM:
                instancias_random = true;
                break;
        }
        
        for (int r = -CANT_INST_DESCARTADAS; r < CANT_REPETICIONES; r++) {
            double tiempo;
            if (instancias_random) {
                coordenadasEnemigos = generarCasoRandom(N);
            }
            start_timer();
            resolverKamehameha();
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

                    archivoSalida.open("genkidama_peor_caso_output");
                    ejecutarPruebas(PRUEBA_PEOR_CASO, archivoSalida);
                    archivoSalida.close();

                    archivoSalida.open("genkidama_caso_intermedio_output");
                    ejecutarPruebas(PRUEBA_CASO_INTERMEDIO, archivoSalida);
                    archivoSalida.close();

                    archivoSalida.open("genkidama_mejor_caso_output");
                    ejecutarPruebas(PRUEBA_MEJOR_CASO, archivoSalida);
                    archivoSalida.close();
					break;
			}
		}
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
