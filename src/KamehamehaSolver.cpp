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

	ostream& operator<<(ostream& os, const XY& xy){
	    os << "(" << xy.x << "," << xy.y << ")";
	    return os;
	}
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
		cout << "(" << vec[i].x << "," << vec[i].y << ")";
		if(i < vec.size() - 1){
			cout << ", ";
		}
	}
	cout << "]" << endl;
}

unsigned long minKamehamehas;

vector<vector<XY> > ordenDeDestruccion;

// Saca los enemigos que destruye de enemigosRestantes y los agrega a enemigosDestruidos
void destruirEnemigos (vector<XY> &enemigosRestantes, vector<vector<XY> > &enemigosDestruidos, FuncionLineal kamehameha){
	vector<XY> destruidos;

	unsigned int i = 0;
    while (i < enemigosRestantes.size()) {
		if (kamehameha.y(enemigosRestantes[i].x) == enemigosRestantes[i].y){
			destruidos.push_back(enemigosRestantes[i]);
			enemigosRestantes.erase(enemigosRestantes.begin() + i);
		}
		else
			i++;
    }

    enemigosDestruidos.push_back(destruidos);
}



void recursividad(vector<XY> enemigosRestantes, vector<vector<XY> > enemigosDestruidos){
	if(enemigosRestantes.size() == 0){
		for(unsigned int i = 0; i < ordenDeDestruccion.size(); i++)
			imprimir(ordenDeDestruccion[i]);
		if(enemigosDestruidos.size() < minKamehamehas){
			minKamehamehas = enemigosDestruidos.size();
			ordenDeDestruccion = enemigosDestruidos;
		}
	}
	else if(enemigosRestantes.size() == 1){
		cout << "queda uno: " << enemigosRestantes[0];
		vector<XY> vacio;
		vector<vector<XY> > copiaEnemigosDestruidos = enemigosDestruidos;
		copiaEnemigosDestruidos.push_back(enemigosRestantes);
		recursividad(vacio, enemigosDestruidos);
	}
	else{
		//Todas las posibles combinaciones de Kamehamehas sin contar las que ya hice al reves
		// Ejempĺo: si hice un kamehameha de 0,0 a 1,1 no voy a hacer de 1,1 a 0,0 porque es lo mismo
		for(unsigned int i = 0; i < enemigosRestantes.size() - 1; i++){
			for(unsigned int j = i + 1; j < enemigosRestantes.size(); j++){
				cout << "recursividad: " << enemigosRestantes[i] << " " << enemigosRestantes[j] << endl;
				vector<XY> copiaEnemigosRestantes = enemigosRestantes;
				vector<vector<XY> > copiaEnemigosDestruidos = enemigosDestruidos;
				destruirEnemigos(copiaEnemigosRestantes, copiaEnemigosDestruidos, FuncionLineal(enemigosRestantes[i], enemigosRestantes[j]));
				recursividad(copiaEnemigosRestantes, copiaEnemigosDestruidos);
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

		//imprimir(coordenadasEnemigos);

		vector<vector<XY> > enemigosDestruidos;
		recursividad(coordenadasEnemigos, enemigosDestruidos);

		for(unsigned int i = 0; i < ordenDeDestruccion.size(); i++)
			imprimir(ordenDeDestruccion[i]);
	}
}
