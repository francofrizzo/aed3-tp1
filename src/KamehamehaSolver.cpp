#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>    // iota
#include <unistd.h>
#include <algorithm>
#include <chrono>
#include "mini_test.h"
#include <cmath>

using namespace std;

#define MAX_N 10
#define CANT_REPETICIONES 20
#define PRUEBA_PEOR_CASO 0
#define PRUEBA_CASO_INTERMEDIO 1
#define PRUEBA_MEJOR_CASO 2

/*
**  Clases y funciones auxiliares
*/

// Medición de tiempos

static chrono::time_point<chrono::high_resolution_clock> start_time;

void start_timer() {
    start_time = chrono::high_resolution_clock::now();
}

double stop_timer() {
    chrono::time_point<chrono::high_resolution_clock> end_time = chrono::high_resolution_clock::now();
    return double(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());
}

// Representa un punto en el plano bidimensional
class XY {
public:
    XY (){};
    XY (int x, int y) : x(x), y(y){};

    int x;
    int y;

    bool operator==(const XY& otro) const {
        return this->x == otro.x && this->y == otro.y;
    }

    bool operator!=(const XY& otro) const {
        return this->x != otro.x || this->y != otro.y;
    }

};

ostream& operator<<(ostream& os, const XY& xy){
    os << "(" << xy.x << "," << xy.y << ")";
    return os;
}

// Representa una recta en el plano bidimensional
class Recta {
public:
    Recta (XY a, XY b) : a(a), b(b) {};

    bool pasaPor(const XY p) const {
        return (b.x - a.x) * (p.y - a.y) == (p.x - a.x) * (b.y - a.y);
    }

    bool operator==(const Recta& otra) const {
        return this->pasaPor(otra.a) && this->pasaPor(otra.b);
    }

    ostream& imprimir(ostream& os) const {
        os << "<" << this->a << ";" << this->b << ">";
        return os;
    }

    XY puntoAleatorio() const {
        XY punto;
        do {
            int deltaX = this->a.x - this->b.x;
            int deltaY = this->a.y - this->b.y;
            int mult = rand();
            punto = XY(abs(this->a.x + mult * deltaX), abs(this->a.y + mult * deltaY));
        } while (punto == this->b || ! this->pasaPor(punto));
        return punto;
    }

private:
    XY a;
    XY b;
};

ostream& operator<<(ostream& os, const Recta& r){
    r.imprimir(os);
    return os;
}

// Imprime la solución con el formato esperado
void imprimirSolucion(vector< vector <unsigned int> > solucion) {
    cout << solucion.size() << endl;
    for (unsigned int i = 0; i < solucion.size(); i++) {
        cout << solucion[i].size();
        for (unsigned int j = 0; j < solucion[i].size(); j++) {
            cout << " " << solucion[i][j] + 1;
        }
        cout << endl;
    }
}

// Compara dos vectores
template <typename T>
bool compararVectores(const vector<T> calculado, const vector<T> esperado) {
    if (calculado.size() != esperado.size()) {
        return false;
    } else {
        for (unsigned int i = 0; i < calculado.size(); i++) {
            if (calculado[i] != esperado[i]) {
                return false;
            }
        }
    }
    return true;
}

/*
**  Variables globales
*/
unsigned int N;
vector<XY> coordenadasEnemigos;

// Imprime por la salida estándar las coordenadas de los enemigos
void imprimirEnemigos() {
    for (unsigned int i = 0; i < coordenadasEnemigos.size(); i++) {
        cout << coordenadasEnemigos[i] << "; ";
    }
    cout << endl;
}

/*
**  Resolución del ejercicio
*/

// Saca los enemigos que destruye de enemigosRestantes y los agrega a enemigosDestruidos
vector<unsigned int> destruirEnemigos(vector<unsigned int> &enemigosRestantes, Recta kamehameha){
    vector<unsigned int> destruidos;

    unsigned int i = 0;
    while (i < enemigosRestantes.size()) {
        if (kamehameha.pasaPor(coordenadasEnemigos[enemigosRestantes[i]])){
            destruidos.push_back(enemigosRestantes[i]);
            enemigosRestantes.erase(enemigosRestantes.begin() + i);
        }
        else {
            i++;
        }
    }

    return destruidos;
}

bool resolverKamehamehaRecursivo(vector<unsigned int> enemigos, vector<vector<unsigned int>>& solucion, unsigned int limite) {
    solucion = vector<vector<unsigned int>>();

    if (enemigos.size() == 0) {
        return true;
    } else if (limite > 0) {
        if (enemigos.size() == 1 || enemigos.size() == 2) {
            solucion.push_back(enemigos);
            return true;
        } else {
            vector<vector<unsigned int> > mejorSubsolucion;
            bool haySubsolucionValida = false;
            vector<Recta> kamehamehasProbados;

            // Todas las posibles combinaciones de Kamehamehas sin contar las que ya hice al reves
            // Ejempĺo: si hice un kamehameha de 0,0 a 1,1 no voy a hacer de 1,1 a 0,0 porque es lo mismo
            for (unsigned int i = 0; i < enemigos.size() - 1; i++){
                for (unsigned int j = i + 1; j < enemigos.size(); j++){
                    Recta kamehameha = Recta(coordenadasEnemigos[enemigos[i]], coordenadasEnemigos[enemigos[j]]);

                    bool repetido = false;
                    for (unsigned int k = 0; k < kamehamehasProbados.size(); k++) {
                        if (kamehameha == kamehamehasProbados[k]) {
                            repetido = true;
                            break;
                        }
                    }

                    if (!repetido) {
                        kamehamehasProbados.push_back(kamehameha);
                        vector<unsigned int> enemigosRestantes = enemigos;
                        vector<vector<unsigned int> > subsolucion;
                        subsolucion.push_back(destruirEnemigos(enemigosRestantes, kamehameha));

                        if (enemigosRestantes.size() > 0) {
                            vector<vector<unsigned int> > subsolucionRecursiva;
                            if (resolverKamehamehaRecursivo(enemigosRestantes, subsolucionRecursiva, limite - 1)) {
                                haySubsolucionValida = true;
                                subsolucion.insert(subsolucion.end(), subsolucionRecursiva.begin(), subsolucionRecursiva.end());
                                mejorSubsolucion = subsolucion;
                                limite = subsolucion.size();
                            }
                        } else {
                            haySubsolucionValida = true;
                            mejorSubsolucion = subsolucion;
                            limite = 1;
                        }
                    }
                }
            }

            if (haySubsolucionValida) {
                solucion.insert(solucion.end(), mejorSubsolucion.begin(), mejorSubsolucion.end());
                return true;
            }
        }
    }

    return false;
}

vector<vector<unsigned int> > resolverKamehameha() {
    vector<unsigned int> enemigos = vector<unsigned int>(N);
    iota(enemigos.begin(), enemigos.end(), 0);

    vector<vector<unsigned int> > solucion;
    resolverKamehamehaRecursivo(enemigos, solucion, N);
    return solucion;
}

/*
**  Tests unitarios
*/

void test_vacio() {
    N = 0;
    coordenadasEnemigos = vector<XY>(N);
    // coordenadasEnemigos[0][0] = 1;
    // coordenadasEnemigos[0][1] = 1;

    vector<vector<unsigned int> > solucion = resolverKamehameha();
    vector<vector<unsigned int> > esperado = vector<vector<unsigned int>>();

    ASSERT(compararVectores(solucion, esperado));
}

void test_unico() {
    N = 1;
    coordenadasEnemigos = vector<XY>(N);
    coordenadasEnemigos[0] = XY(0,0);

    vector<vector<unsigned int> > solucion = resolverKamehameha();

    vector<vector<unsigned int> > esperado = vector<vector<unsigned int>>(1);
    esperado[0].push_back(0);

    ASSERT(compararVectores(solucion, esperado));
}

void test_cuatro_en_linea() {
    N = 4;
    coordenadasEnemigos = vector<XY>(N);
    coordenadasEnemigos[0] = XY(0,0);
    coordenadasEnemigos[1] = XY(1,1);
    coordenadasEnemigos[2] = XY(2,2);
    coordenadasEnemigos[3] = XY(3,3);

    vector<vector<unsigned int> > solucion = resolverKamehameha();

    ASSERT_EQ(solucion.size(), 1);
    ASSERT_EQ(solucion[0].size(), 4);
}

void test_cuadrado() {
    N = 4;
    coordenadasEnemigos = vector<XY>(N);
    coordenadasEnemigos[0] = XY(50,100);
    coordenadasEnemigos[1] = XY(50,150);
    coordenadasEnemigos[2] = XY(100,150);
    coordenadasEnemigos[3] = XY(100,100);

    vector<vector<unsigned int> > solucion = resolverKamehameha();

    ASSERT_EQ(solucion.size(), 2);
    ASSERT_EQ(solucion[0].size(), 2);
    ASSERT_EQ(solucion[1].size(), 2);
}

void test_tres_radiales() {
    N = 9;
    coordenadasEnemigos = vector<XY>(N);
    coordenadasEnemigos[0] = XY(22,31);
    coordenadasEnemigos[1] = XY(44,62);
    coordenadasEnemigos[2] = XY(5,21);
    coordenadasEnemigos[3] = XY(10,42);
    coordenadasEnemigos[4] = XY(15,63);
    coordenadasEnemigos[5] = XY(1001,32);
    coordenadasEnemigos[6] = XY(2002,64);
    coordenadasEnemigos[7] = XY(4004,128);
    coordenadasEnemigos[8] = XY(8008,256);

    vector<vector<unsigned int> > solucion = resolverKamehameha();

    ASSERT_EQ(solucion.size(), 3);
}

/*
**  Pruebas de performance
*/

vector<XY> generarPeorCaso(unsigned int n) {
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

    for (unsigned int i = 2; i < n; i++) {
        XY enemigo;
        bool repetido = true;
        while (repetido) {
            enemigo = XY(rand(), rand());
            repetido = false;
            for (unsigned int j = 0; j < enemigos.size(); j++) {
                for (unsigned int k = j + 1; k < enemigos.size(); k++) {
                    Recta r = Recta(enemigos[j], enemigos[k]);
                    if (r.pasaPor(enemigo)) {
                        repetido = true;
                        break;
                        break;
                    }
                }
            }
        }
        enemigos.push_back(enemigo);
    }

    return enemigos;
}


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

    return enemigos;
}

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
    return enemigos;
}

vector<XY> generarCasoIntermedio(unsigned int n) {
    unsigned int k = n / 2;

    vector<XY> subEnemigos1 = generarMejorCaso(k);
    vector<XY> subEnemigos2 = generarMejorCaso(n - k);

    subEnemigos1.insert(subEnemigos1.end(), subEnemigos2.begin(), subEnemigos2.end());

    random_shuffle(subEnemigos1.begin(), subEnemigos1.end());
    return subEnemigos1;
}

void ejecutarPruebas(int prueba_id, ofstream& archivoSalida) {
    for (unsigned int i = 1; i <= MAX_N; i++) {
        double tiempos[CANT_REPETICIONES];
        double tiempo_promedio = 0;
        double varianza = 0;

        N = i;
        switch (prueba_id) {
            case PRUEBA_PEOR_CASO:
                coordenadasEnemigos = generarPeorCaso(N);
                break;

            case PRUEBA_CASO_INTERMEDIO:
                coordenadasEnemigos = generarCasoIntermedio(N);
                break;

            case PRUEBA_MEJOR_CASO:
                coordenadasEnemigos = generarMejorCaso(N);
                break;
        }
        
        for (unsigned int r = 0; r < CANT_REPETICIONES; r++) {
            start_timer();
            resolverKamehameha();
            tiempos[r] = stop_timer();
            tiempo_promedio += tiempos[r];
        }

        tiempo_promedio = tiempo_promedio / CANT_REPETICIONES;

        for (unsigned int r = 0; r < CANT_REPETICIONES; r++) {
            varianza += (tiempos[r] - tiempo_promedio) * (tiempos[r] - tiempo_promedio);
        }
        varianza = sqrt(varianza / CANT_REPETICIONES);

        archivoSalida << i << " " << tiempo_promedio << " " << varianza << endl;
    }
}

/*
**  Main
*/

int main (int argc, char* argv[]) {
    if (argc > 1) {
        char opt;
        while ((opt = getopt(argc, argv, "tp")) != -1) {
            switch (opt) {
                case 't':
                    RUN_TEST(test_vacio);
                    RUN_TEST(test_unico);
                    RUN_TEST(test_cuatro_en_linea);
                    RUN_TEST(test_cuadrado);
                    RUN_TEST(test_tres_radiales);
                case 'p':
                    ofstream archivoSalida;

                    archivoSalida.open("kamehameha_peor_caso_output");
                    ejecutarPruebas(PRUEBA_PEOR_CASO, archivoSalida);
                    archivoSalida.close();

                    archivoSalida.open("kamehameha_caso_intermedio_output");
                    ejecutarPruebas(PRUEBA_CASO_INTERMEDIO, archivoSalida);
                    archivoSalida.close();

                    archivoSalida.open("kamehameha_mejor_caso_output");
                    ejecutarPruebas(PRUEBA_MEJOR_CASO, archivoSalida);
                    archivoSalida.close();

                    break;
            }
        }
    } else {
        // Parseo de input
        cin >> N;

        for (unsigned int i = 0; i < N; i++){
            XY enemigo;
            cin >> enemigo.x >> enemigo.y;
            coordenadasEnemigos.push_back(enemigo);
        }

        vector< vector< unsigned int > > solucion = resolverKamehameha();

        imprimirSolucion(solucion);
    }
}
