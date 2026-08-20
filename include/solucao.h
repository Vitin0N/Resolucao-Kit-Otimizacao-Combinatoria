#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <vector> 
#include <iostream>
#include <random>
#include <cstdlib>
#include <algorithm>

extern std::vector<std::vector<double>> matrizDistancias; // Matriz de distâncias entre os pontos do TSP

// Estrutra de uma solução, contendo o caminho percorrido e o custo associado a esse caminho.  
struct Solucao {
    std::vector<int> caminho;
    double custo;  
};

// Estrutura para armazenar informações sobre a inserção de um nó em uma solução
// incluindo o nó inserido, a aresta removida e o custo associado à inserção.
struct insertionInfo {
    int noInserido;
    int arestaRemovida;
    double custo;
};

// ==== Funções auxiliares ====]
std::vector<int> randomiza3Cidades(int qntCidade);

std::vector<int> cidadesRestantes(std::vector<int> cidadesEscolhidas, int qntCidades);


// Função para imprimir a solução no console.
void imprimirSolucao(Solucao& solucao);

// Função para calcular o valor objetivo (custo total) de uma solução.
void calcularValorObj(Solucao& solucao);

// Função para colocar valores (mockados) a matriz de distâncias, para fins de teste.
void carregarMatrizDistanciasMOCK();

// Função para calcular a inserção de um nó em uma solução, atualizando o caminho e o custo da solução.
std::vector<insertionInfo> calcularInsercao(Solucao& solucao, std::vector<int> &CL);

// Função para construir uma solução inicial para o TSP, utilizando um algoritmo de construção.
Solucao Construcao();

// ===== Funções de vizinhança ======

// Visa encontrar o melhor vizinho da solução (com menor custo)
bool bestImprovmentSwap(Solucao& s);


#endif // SOLUCAO_H