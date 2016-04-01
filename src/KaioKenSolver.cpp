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

// Recibe dos listas de vectores y concatena cada vector de la primera
// lista con el respectivo de la segunda lista. Si una de las listas es
// más larga que la otra, omite los elementos sobrantes.
vector < vector < int > >  merge (vector < vector < int > > peleasIzq, vector < vector < int > > peleasDer) {
    vector < vector < int > > peleas;
    unsigned int tamMinimo =  min(peleasIzq.size(), peleasDer.size());
    for (unsigned int i = 0; i < tamMinimo; ++i) {
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
vector < vector < int > > generarPeleas(unsigned int N) {

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
    unsigned int i;
    for (i = 0; i < N / 2; ++i) {
        pelea.push_back(1);
    }
    for (; i < N; ++i) {
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
            vector < int > ultPeleaIzq;
            vector < int > ultPeleaDer;

            if (peleasIzq.size() > 0) {
                ultPeleaIzq = peleasIzq[peleasIzq.size() - 1];
            } else {
                ultPeleaIzq.push_back(1);
            }
            ultPeleaDer = peleasDer[peleasDer.size() - 1];
            
            ultPeleaIzq.insert(ultPeleaIzq.end(), ultPeleaDer.begin(), ultPeleaDer.end());
            peleas.push_back(ultPeleaIzq);
        }
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
