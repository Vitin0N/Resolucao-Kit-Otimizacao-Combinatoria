#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <vector> 
#include <iostream>
#include <random>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>

extern std::vector<std::vector<double>> matrizDistancias; // Matriz de distâncias entre os pontos do TSP
extern int matrizDimensao; // Dimensão da matriz distancia

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

// Função para colocar valores (mockados) a matriz de distâncias, para fins de teste.
void carregarMatrizDistanciasMOCK();

// ==== Funções de Calculos

// Função para calcular o valor objetivo (custo total) de uma solução.
void calcularValorObj(Solucao& solucao);


// Função para calcular a inserção de um nó em uma solução, atualizando o caminho e o custo da solução.
std::vector<insertionInfo> calcularInsercao(Solucao& solucao, std::vector<int> &CL);

// Função para construir uma solução inicial para o TSP, utilizando um algoritmo de construção.
Solucao Construcao();

// ===== Funções de vizinhança ======

// Faz o SWAP de duas arestas da sequencia
bool bestImprovementSwap(Solucao& s);

// Faz o 'desatamento' de nós
bool bestImprovement2Opt(Solucao& s);

// Faz a reinserção de um bloco com tamanho fixo (k)
bool bestImprovementOrOpt(Solucao& s, int k);

// Encontra a melhor posicção local para aquela solução
void buscaLocal(Solucao& s);

// ===== Funções de Pertubação =====

// Função para pertubar a solução afim de encontrar uma nova vizinhança
Solucao pertubacaoDoubleBridge(const Solucao &s);

// ==== Função para gerar a solução final ====

Solucao ILS(const Solucao& s, int maxIter, int maxIterILS);

#endif // SOLUCAO_H