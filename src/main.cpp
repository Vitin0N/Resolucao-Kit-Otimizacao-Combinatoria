/*
main.cpp

Aqui vai ficar o resultado da solução que eu vou contruir para o problema do TSP 
Servindo como ponto de entrada de dados e chamada dos metodos que vão resolver o problema do TSP.
*/ 

#include "solucao.h"

int main(){

    carregarMatrizDistanciasMOCK();

    Solucao s = Construcao();

    calcularValorObj(s);

    imprimirSolucao(s);

    std::cout << "Custo total (original) foi de: " << s.custo << std::endl;
    
    while(bestImprovementOrOpt(s, 2)){
        imprimirSolucao(s);
        std::cout << "Custo total foi de: " << s.custo << std::endl;
    }

    return 0;
}