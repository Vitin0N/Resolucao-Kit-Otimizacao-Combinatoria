/*
solução.cpp
  
Aqui vai ficar a implementação da solução que eu vou contruir para o problema do TSP
*/

#include "solucao.h"

std::vector<std::vector<double>> matrizDistancias; // Definindo a matrizDistancia

// ==== Funções Auxiliares ====
std::vector<int> randomiza3Cidades(int qntCidade){
    std::vector<int> cidades;

    for(int i = 1; i <= qntCidade; i++){
        cidades.push_back(i);
    }

    // Gerador de aleatoriedade
    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(cidades.begin(), cidades.end(), gen);

    cidades.resize(3); // Deixa o tamnho igual a três
    cidades.push_back(cidades[0]); // Faz a volta para a primeira cidade

    return cidades;
}

std::vector<int> cidadesRestantes(std::vector<int> cidadesEscolhidas, int qntCidades){
    std::vector<int> cidadesRestantes;

    for(int i = 1; i <= qntCidades; i++){
        if(std::find(cidadesEscolhidas.begin(), cidadesEscolhidas.end(), i) == cidadesEscolhidas.end()){
            cidadesRestantes.push_back(i);
        }
    }

    return cidadesRestantes;
}

void imprimirSolucao(Solucao& solucao){
    for(int i = 0; i < (int)solucao.caminho.size() - 1; i++){
        std::cout << solucao.caminho[i] << " ->";
    }
    // Imprime o último elemento do caminho
    std::cout << solucao.caminho.back() << std::endl; 
}

void calcularValorObj(Solucao& solucao){
    solucao.custo = 0;
    for(int i = 0; i < (int)solucao.caminho.size() -1; i++){
        int cidadeAtual = solucao.caminho[i];
        int proxCidade = solucao.caminho[i+1];

        solucao.custo += matrizDistancias[cidadeAtual][proxCidade];
    }
}

void carregarMatrizDistanciasMOCK(){
    // Preenchendo a matriz de distâncias com valores mockados para fins de teste
    matrizDistancias = {
        {0,   0,   0,   0,   0,   0,   0}, // Índice 0 (ignorado)
        {0,   0, 245, 174, 118,  59, 129}, // Cidade 1
        {0, 245,   0, 250, 226, 186, 147}, // Cidade 2
        {0, 174, 250,   0, 274, 169, 114}, // Cidade 3
        {0, 118, 226, 274,   0, 105, 185}, // Cidade 4
        {0,  59, 186, 169, 105,   0,  87}, // Cidade 5
        {0, 129, 147, 114, 185,  87,   0}  // Cidade 6
    };
}

std::vector<insertionInfo> calcularInsercao(Solucao& solucao, std::vector<int> &CL){
    std::vector<insertionInfo> custoInsercao((solucao.caminho.size() - 1) * CL.size());

    int l = 0;
    for(int i = 0; i < (int)solucao.caminho.size() - 1; i++){
        int cidadeRemetente = solucao.caminho[i];
        int cidadeDestino = solucao.caminho[i+1];
        for(auto k : CL){
            custoInsercao[l].custo = matrizDistancias[cidadeDestino][k] + matrizDistancias[cidadeRemetente][k] - matrizDistancias[cidadeDestino][cidadeRemetente];
            custoInsercao[l].noInserido = k;
            custoInsercao[l].arestaRemovida = i;
            l++;
        }
    }

    return custoInsercao;
}

Solucao Construcao(){
    Solucao s;
    s.caminho = randomiza3Cidades(6);

    std::vector<int> CL = cidadesRestantes(s.caminho, 6);

    while(!CL.empty()){
        std::vector<insertionInfo> custoInsercao = calcularInsercao(s, CL);

        // Ordena o custo de inserção
        std::sort(custoInsercao.begin(), custoInsercao.end(), [](const insertionInfo& a, const insertionInfo& b) {
            return a.custo < b.custo;
        });

        double alpha = static_cast<double>(rand()) / RAND_MAX;
        int cidadeSorteada = rand() % ((int) ceil(alpha * custoInsercao.size()));

        insertionInfo escolhida = custoInsercao[cidadeSorteada];
        s.caminho.insert(s.caminho.begin() + escolhida.arestaRemovida + 1, escolhida.noInserido);

        // Apaga a cidade escolhida
        auto it = std::find(CL.begin(), CL.end(), escolhida.noInserido);

        if(it != CL.end()){
            CL.erase(it);
        }
    }

    return s;
}