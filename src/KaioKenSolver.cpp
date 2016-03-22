#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

/*
**  Funciones auxiliares
*/

// Imprime un vector (de enteros) a la salida estándar.
// Los elementos se imprimen separados por espacios,
// en una única línea seguida de un caracter de fin de línea.
void printVec(vector < int > vec) {
    for (int i = 0; i < vec.size(); ++i) {
        cout << vec[i] << " ";
    }
    cout << endl;
}

// Recibe una lista de vectores. Imprime a la salida estándar
// el número de elementos de la lista, y a continuación todos
// los vectores, cada uno en una nueva línea
void printVecs(vector < vector < int > > vecs) {
    cout << vecs.size() << endl;
    for (int i = 0; i < vecs.size(); ++i) {
        printVec(vecs[i]);
    }
}

// Recibe dos listas de vectores y concatena cada vector de la primera
// lista con el respectivo de la segunda lista. Si una de las listas es
// más larga que la otra, omite los elementos sobrantes.
vector < vector < int > >  merge (vector < vector < int > > peleasIzq, vector < vector < int > > peleasDer) {
    vector < vector < int > > peleas;
    int minSize =  min(peleasIzq.size(), peleasDer.size());
    for (int i = 0; i < minSize; ++i) {
        vector < int > peleaIzq  = peleasIzq[i];
        vector < int > peleaDer  = peleasDer[i];
        peleaIzq.insert(peleaIzq.end(), peleaDer.begin(), peleaDer.end());
        peleas.push_back(peleaIzq);
    }
    return peleas;
}

/*
**  Resolución del ejercicio
*/

// Resuelve la instancia del problema correspondiente a un ejército de N guerreros.
vector < vector < int > > generarPeleas(int N) {

    vector < vector < int > > peleas;

    // Casos base
    if (N == 1) {
        return peleas;
    }

    if (N == 2) {
        vector < int > pelea;
        pelea.push_back(1);
        pelea.push_back(2);

        peleas.push_back(pelea);
        return peleas;
    }

    // Caso recursivo

    // Primer pelea
    vector < int > pelea ;
    int i;
    for (i = 0; i < N / 2; ++i) {
        pelea.push_back(1);
    }
    for (i; i < N; ++i) {
        pelea.push_back(2);
    }
    peleas.push_back(pelea);

    // Llamadas recursivas para completar las peleas faltantes
    if (N % 2 == 0) {
        // Si N es un número par, las sub-soluciones tienen igual tamaño.
        vector < vector < int > > peleasRecurs = merge(generarPeleas(N / 2), generarPeleas(N / 2));
        peleas.insert(peleas.end(), peleasRecurs.begin(), peleasRecurs.end());
    } else {
        vector < vector < int > > peleasIzq = generarPeleas(N / 2);
        vector < vector < int > > peleasDer = generarPeleas((N / 2) + 1);
        vector < vector < int > > peleasRecurs = merge(peleasIzq, peleasDer);
        peleas.insert(peleas.end(), peleasRecurs.begin(), peleasRecurs.end());

        // Si N es impar, las sub-soluciones podrían tener tamaños diferentes.
        // En tal caso, la sub-solución izquierda tendrá un elemento menos que la derecha,
        // por lo que es completada replicando su última pelea para así poder
        // concatenarla con la última pelea de la sub-solución derecha.
        if (peleasIzq.size() != peleasDer.size()) {
            vector < int > lastPeleaIzq;
            vector < int > lastPeleaDer;

            if (peleasIzq.size() > 0) {
                lastPeleaIzq = peleasIzq[peleasIzq.size() - 1];
            } else {
                lastPeleaIzq.push_back(1);
            }
            lastPeleaDer = peleasDer[peleasDer.size() - 1];
            
            lastPeleaIzq.insert(lastPeleaIzq.end(), lastPeleaDer.begin(), lastPeleaDer.end());
            peleas.push_back(lastPeleaIzq);
        }
    }

    return peleas;
}

int main() {
    int N;
    cin >> N;
    printVecs(generarPeleas(N));
}
