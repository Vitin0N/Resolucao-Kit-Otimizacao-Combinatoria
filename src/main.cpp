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
    
    buscaLocal(s);

    imprimirSolucao(s);
    std::cout << "Custo total foi de: " << s.custo << std::endl;

    Solucao s2 = pertubacaoDoubleBridge(s);

    imprimirSolucao(s2);
    
    return 0;
}