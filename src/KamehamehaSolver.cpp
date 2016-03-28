#include <iostream>
#include <vector>
#include <unistd.h>


using namespace std;

int minKamehamehas;
vector<vector<pair<int, int> > > ordenDeDestruccion;

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

		vector< pair<int, int> > coordenadasEnemigos;
		for(int i = 0; i < n; i++){
			pair<int, int> coordenada;
			cin >> coordenada.first >> coordenada.second;
			coordenadasEnemigos.push_back(coordenada);
		}

		minimo = n;
	}
}

void recursividad(vector<pair<int, int> > enemigosRestantes, vector<vector<pair<int, int> > > enemigosDestruidos){
	if(enemigosRestantes.size() == 0){
		if(enemigosDestruidos.size() < minKamehamehas){
			minKamehamehas = enemigosDestruidos.size();
			ordenDeDestruccion = enemigosDestruidos;
		}
	}
	else{
		if(enemigosRestantes.size() == 1)
			recursividad(enemigosRestantes.pop_back(), enemigosDestruidos.push_back(enemigosRestantes));
		else{
			//Todas las posibles combinaciones de Kamehamehas sin contar las que ya hice al reves
			// Ejempĺo: si hice un kamehameha de 0,0 a 1,1 no voy a hacer de 1,1 a 0,0 porque es lo mismo
			for(int i = 0; i < enemigosRestantes.size() - 1; i++){
				for(int j = i + 1; j < enemigosRestantes.size(); j++){

				}
			}
		}
	}
}

