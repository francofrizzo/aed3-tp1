#include <iostream>
#include <vector>
#include <unistd.h>

using namespace std;

class XY {
public:
	XY (){};
	XY (double x, double y) : x(x), y(y){};

	double x;
	double y;
};

class FuncionLineal{
public:
	FuncionLineal(XY a, XY b){
		pendiente = (a.y - b.y) / (a.x - b.x);
		ordenadaAlOrigen = a.y - pendiente * a.x;
	}

	double y(double x){
		return pendiente * x + ordenadaAlOrigen;
	}

private:
	double pendiente;
	double ordenadaAlOrigen;
};

void imprimir(vector<XY> vec){
	cout << "[";
	for(unsigned int i = 0; i < vec.size(); i++){
		cout << "(" << vec[i].x << "," << vec[i] << ")";
		if(i < vec.size() - 1){
			cout << ", ";
		}
	}
	cout << "]" << endl;
}

long minKamehamehas;

vector<vector<XY> > ordenDeDestruccion;

// Devuelve los enemigos que destruye el kamehameha en tiempo lineal
vector<XY> enemigosQueMueren (vector<XY> enemigos, FuncionLineal kamehameha){
	vector<XY> destruidos;

    for (int i = 0; i < enemigos.size(); i++) {
		if (kamehameha.y(enemigos[i].x) == enemigos[i].y)
			destruidos.push_back(enemigos[i]);
    }

    return destruidos;
}

void recursividad(vector<XY> enemigosRestantes, vector<vector<XY> > enemigosDestruidos){
	if(enemigosRestantes.size() == 0){
		if(enemigosDestruidos.size() < minKamehamehas){
			minKamehamehas = enemigosDestruidos.size();
			ordenDeDestruccion = enemigosDestruidos;
		}
	}
	else{
		if(enemigosRestantes.size() == 1){
			enemigosDestruidos.push_back(enemigosRestantes);
			enemigosRestantes.pop_back();
			recursividad(enemigosRestantes, enemigosDestruidos);
		}
		else{
			//Todas las posibles combinaciones de Kamehamehas sin contar las que ya hice al reves
			// Ejempĺo: si hice un kamehameha de 0,0 a 1,1 no voy a hacer de 1,1 a 0,0 porque es lo mismo
			for(unsigned int i = 0; i < enemigosRestantes.size() - 1; i++){
				for(unsigned int j = i + 1; j < enemigosRestantes.size(); j++){
					vector<XY> porDestruir = enemigosQueMueren(enemigosRestantes, FuncionLineal(enemigosRestantes[i], enemigosRestantes[j]));
					imprimir(porDestruir);
				}
			}
		}
	}
}

int main(int argc, char *argv[]){
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

		int n;
		cin >> n;

		vector<XY> coordenadasEnemigos;
		for(int i = 0; i < n; i++){
			XY enemigo;
			cin >> enemigo.x >> enemigo.y;
			coordenadasEnemigos.push_back(enemigo);
		}

		minKamehamehas = n;
	}
}
