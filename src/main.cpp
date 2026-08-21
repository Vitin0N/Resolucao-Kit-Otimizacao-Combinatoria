/*
main.cpp

Aqui vai ficar o resultado da solução que eu vou contruir para o problema do TSP 
Servindo como ponto de entrada de dados e chamada dos metodos que vão resolver o problema do TSP.
*/ 

#include "solucao.h"
#include "data.h"

int main(){

    std::string caminhoTsp = "instancias/cristino2.tsp";

    lerInstancias(caminhoTsp);

    Solucao s = Construcao();
    calcularValorObj(s);

    imprimirSolucao(s);
    std::cout << "Custo total (original) foi de: " << s.custo << std::endl;

    s = ILS(s, 5, 5);
    
    imprimirSolucao(s);
    std::cout << "Custo total (original) foi de: " << s.custo << std::endl;

    criarTour(s, "critino.tour");

    return 0;
}