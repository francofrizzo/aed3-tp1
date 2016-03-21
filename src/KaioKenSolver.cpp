#include <iostream>
#include <algorithm>  
#include <vector>

using namespace std;
//Numero de Peleadores


void printVec(vector < int > vec){
     for (int i = 0; i < vec.size(); ++i){
            cout << vec[i] << " "  ;
        }
        cout << endl ;
}

void printVecs(vector < vector < int > > vecs){
    cout << vecs.size() << endl;
    for (int i = 0; i < vecs.size(); ++i){
        printVec(vecs[i]);
       
    }
}



vector < vector < int > >  merge ( vector < vector < int > > peleasIzq , vector < vector < int > > peleasDer){
    vector < vector < int > > peleas;
    int minSize =  min(peleasIzq.size(),peleasDer.size());
    for (int i = 0; i < minSize; ++i){
        vector < int > peleaIzq  = peleasIzq[i];
        vector < int > peleaDer  = peleasDer[i];
        peleaIzq.insert(peleaIzq.end(), peleaDer.begin(), peleaDer.end());
        peleas.push_back(peleaIzq);
    }
    return peleas;
}

vector < vector < int > >  generarPeleas( int N ){
    
    vector < vector < int > > peleas;
    
    // Casos Base
    if(N == 1){
        vector < int > pelea ;
        pelea.push_back(1);

        peleas.push_back(pelea);
        return  peleas;
    }
    if (N == 2){

        vector < int > pelea ;
        pelea.push_back(1);;
        pelea.push_back(2);
        
        peleas.push_back(pelea);
        return  peleas;
    }
    if (N%2 == 0){
        peleas = merge(generarPeleas( N/2 ),generarPeleas( N/2));
    }else{
        vector < vector < int > > peleasIzq = generarPeleas( (N/2) );
        vector < vector < int > > peleasDer = generarPeleas( (N/2) + 1);  
        peleas = merge( peleasIzq,peleasDer);
        if(peleasIzq.size() != peleasDer.size()){
             vector < int > lastPeleaIzq  = peleasIzq[peleasIzq.size() -1];
            vector < int > lastPeleaDer  = peleasDer[peleasDer.size() -1];
            lastPeleaDer.insert(lastPeleaDer.end(), lastPeleaIzq.begin(), lastPeleaIzq.end());
            peleas.push_back(lastPeleaDer);
        }
       
        
    }
    
    vector < int > pelea ;
    int i;
    for (i = 0; i < N/2; ++i){
        pelea.push_back(1);
    }
    for (i; i < N; ++i){
        pelea.push_back(2);
    }
    peleas.push_back(pelea);
    return peleas;
}

int main(){
    int N ;
    cin >> N ;
    printVecs(generarPeleas(N));
    
    
}


