#include <iostream>
#include <vector>
#include <numeric>    // iota
#include <unistd.h>
#include "mini_test.h"

using namespace std;

/*
**  Clases y funciones auxiliares
*/

// Representa un punto en el plano bidimensional
class XY {
public:
    XY (){};
    XY (int x, int y) : x(x), y(y){};

    int x;
    int y;

};

ostream& operator<<(ostream& os, const XY& xy){
    os << "(" << xy.x << "," << xy.y << ")";
    return os;
}

// Representa una recta en el plano bidimensional
class Recta {
public:
    Recta (XY a, XY b) : a(a), b(b) {};

    bool pasaPor(XY p) {
        return (b.x - a.x) * (p.y - a.y) == (p.x - a.x) * (b.y - a.y);
    }

private:
    XY a;
    XY b;
};

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
vector<XY> coordenadasEnemigos;

/*
**  Resolución del ejercicio
*/

// Saca los enemigos que destruye de enemigosRestantes y los agrega a enemigosDestruidos
vector<unsigned int> destruirEnemigos (vector<unsigned int> &enemigosRestantes, Recta kamehameha){
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

vector<vector<unsigned int> > resolverKamehamehaRecursivo(vector<unsigned int> enemigos, unsigned int limite, bool& valido) {
    vector<vector<unsigned int> > mejorSolucion;
    valido = false;

    if (enemigos.size() == 0) {
        valido = true;
    } else if (limite > 0) {
        if (enemigos.size() == 1 || enemigos.size() == 2) {
            mejorSolucion.push_back(enemigos);
            valido = true;
        } else {
            // Todas las posibles combinaciones de Kamehamehas sin contar las que ya hice al reves
            // Ejempĺo: si hice un kamehameha de 0,0 a 1,1 no voy a hacer de 1,1 a 0,0 porque es lo mismo
            for (unsigned int i = 0; i < enemigos.size() - 1; i++){
                unsigned int minimo = limite - 1;
                bool primerIntento = true;
                for (unsigned int j = i + 1; j < enemigos.size(); j++){
                    Recta kamehameha = Recta(coordenadasEnemigos[enemigos[i]], coordenadasEnemigos[enemigos[j]]);
                    vector<unsigned int> enemigosRestantes = enemigos;

                    bool solucionValida = true;
                    vector<vector<unsigned int> > solucion;
                    solucion.push_back(destruirEnemigos(enemigosRestantes, kamehameha));

                    if (enemigosRestantes.size() > 0) {
                        vector< vector<unsigned int>> enemigosDestruidos = resolverKamehamehaRecursivo(enemigosRestantes, minimo, solucionValida);
                        if (solucionValida) {
                            solucion.insert(solucion.end(), enemigosDestruidos.begin(), enemigosDestruidos.end());
                        }
                    }

                    if (solucionValida && (solucion.size() < minimo || primerIntento)) {
                        minimo = solucion.size();
                        mejorSolucion = solucion;
                        valido = true;
                    }

                    primerIntento = false;
                }
            }
        }
    }

    return mejorSolucion;
}

vector<vector<unsigned int> > resolverKamehameha(int n) {
    vector<unsigned int> enemigos = vector<unsigned int>(n);
    iota(enemigos.begin(), enemigos.end(), 0);

    bool valido;
    vector<vector<unsigned int> > solucion = resolverKamehamehaRecursivo(enemigos, n, valido);

    return solucion;
}

/*
**  Tests unitarios
*/

void test_vacio() {
    int n = 0;
    coordenadasEnemigos = vector<XY>(n);
    // coordenadasEnemigos[0][0] = 1;
    // coordenadasEnemigos[0][1] = 1;

    vector<vector<unsigned int> > solucion = resolverKamehameha(n);
    vector<vector<unsigned int> > esperado = vector<vector<unsigned int>>();

    ASSERT(compararVectores(solucion, esperado));
}

void test_unico() {
    int n = 1;
    coordenadasEnemigos = vector<XY>(n);
    coordenadasEnemigos[0] = XY(0,0);

    vector<vector<unsigned int> > solucion = resolverKamehameha(n);

    vector<vector<unsigned int> > esperado = vector<vector<unsigned int>>(1);
    esperado[0].push_back(0);

    ASSERT(compararVectores(solucion, esperado));
}

void test_cuatro_en_linea() {
    int n = 4;
    coordenadasEnemigos = vector<XY>(n);
    coordenadasEnemigos[0] = XY(0,0);
    coordenadasEnemigos[1] = XY(1,1);
    coordenadasEnemigos[2] = XY(2,2);
    coordenadasEnemigos[3] = XY(3,3);

    vector<vector<unsigned int> > solucion = resolverKamehameha(n);

    ASSERT_EQ(solucion.size(), 1);
    ASSERT_EQ(solucion[0].size(), 4);
}

void test_cuadrado() {
    int n = 4;
    coordenadasEnemigos = vector<XY>(n);
    coordenadasEnemigos[0] = XY(50,100);
    coordenadasEnemigos[1] = XY(50,150);
    coordenadasEnemigos[2] = XY(100,150);
    coordenadasEnemigos[3] = XY(100,100);

    vector<vector<unsigned int> > solucion = resolverKamehameha(n);

    ASSERT_EQ(solucion.size(), 2);
    ASSERT_EQ(solucion[0].size(), 2);
    ASSERT_EQ(solucion[1].size(), 2);
}

void test_tres_radiales() {
    int n = 9;
    coordenadasEnemigos = vector<XY>(n);
    coordenadasEnemigos[0] = XY(22,31);
    coordenadasEnemigos[1] = XY(44,62);
    coordenadasEnemigos[2] = XY(5,21);
    coordenadasEnemigos[3] = XY(10,42);
    coordenadasEnemigos[4] = XY(15,64);
    coordenadasEnemigos[5] = XY(1001,32);
    coordenadasEnemigos[6] = XY(2002,64);
    coordenadasEnemigos[7] = XY(4004,128);
    coordenadasEnemigos[8] = XY(8008,256);

    vector<vector<unsigned int> > solucion = resolverKamehameha(n);

    ASSERT_EQ(solucion.size(), 3);
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
                    break;
            }
        }
    } else {
        // Parseo de input
        int n;
        cin >> n;

        for (int i = 0; i < n; i++){
            XY enemigo;
            cin >> enemigo.x >> enemigo.y;
            coordenadasEnemigos.push_back(enemigo);
        }

        vector< vector< unsigned int > > solucion = resolverKamehameha(n);

        imprimirSolucion(solucion);
    }
}
