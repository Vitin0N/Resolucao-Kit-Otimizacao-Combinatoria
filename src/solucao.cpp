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

// Verifica as cidades restantes com base nas escolhidas
std::vector<int> cidadesRestantes(std::vector<int> cidadesEscolhidas, int qntCidades){
    std::vector<int> cidadesRestantes;

    for(int i = 1; i <= qntCidades; i++){
        // Verifica se a cidade já foi escolhida
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
    // Passa por todas as soluções possiveis
    for(int i = 0; i < (int)solucao.caminho.size() - 1; i++){
        int cidadeRemetente = solucao.caminho[i];
        int cidadeDestino = solucao.caminho[i+1];
        for(auto k : CL){
            // Adiciona o custo ao array de informação
            custoInsercao[l].custo = matrizDistancias[cidadeDestino][k] + matrizDistancias[cidadeRemetente][k] - matrizDistancias[cidadeDestino][cidadeRemetente];
            custoInsercao[l].noInserido = k;
            custoInsercao[l].arestaRemovida = i;
            l++;
        }
    }

    return custoInsercao;
}

Solucao Construcao(){
    Solucao s; // Inicia uma solução

    // Randomiza 3 cidades iniciais
    s.caminho = randomiza3Cidades(6);

    // Adiciona a lista de candidatos as cidades restantes
    std::vector<int> CL = cidadesRestantes(s.caminho, 6);

    while(!CL.empty()){
        std::vector<insertionInfo> custoInsercao = calcularInsercao(s, CL);

        // Ordena o custo de inserção com base no custo
        std::sort(custoInsercao.begin(), custoInsercao.end(), [](const insertionInfo& a, const insertionInfo& b) {
            return a.custo < b.custo;
        });

        // Alpha para selecionar aleatoriamente um dos primeiros pares
        double alpha = static_cast<double>(rand()) / RAND_MAX;
        int cidadeSorteada = rand() % std::max(1, ((int) ceil(alpha * custoInsercao.size())));

        // Insere a cidade escolhida
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

// ==== Funções de vinzinhaça ====

bool bestImprovementSwap(Solucao& s){
    double melhorDelta = 0;
    int melhor_i = -1, melhor_j = -1;

    for(int i = 1; i < (int)s.caminho.size() - 1; i++){
        int vi = s.caminho[i];
        int vi_prox = s.caminho[i+1];
        int vi_ant = s.caminho[i-1];

        for(int j = i + 1; j < (int)s.caminho.size() - 1; j++){
            int vj = s.caminho[j];
            int vj_prox = s.caminho[j+1];
            int vj_ant = s.caminho[j-1];

            double novoDelta;

            if (j == i + 1) {
                // Fórmula para vértices adjacentes
                novoDelta = (matrizDistancias[vi_ant][vj] + matrizDistancias[vi][vj_prox]) -
                            (matrizDistancias[vi_ant][vi] + matrizDistancias[vj][vj_prox]);
            } else {
                // Fórmula para vértices separados
                novoDelta = 
                            // Estradas adicionadas
                            +matrizDistancias[vi_ant][vj]
                            +matrizDistancias[vi][vj_prox]
                            +matrizDistancias[vj][vi_prox]
                            +matrizDistancias[vj_ant][vi]
                            // Estradas removidas
                            -(matrizDistancias[vi_ant][vi]
                            +matrizDistancias[vi][vi_prox]
                            +matrizDistancias[vj_ant][vj]
                            +matrizDistancias[vj][vj_prox]);
            }
            
            if(novoDelta < melhorDelta){
                melhorDelta = novoDelta;
                melhor_i = i;
                melhor_j = j;
            }
        }
    }

    // Se o vizinho for melhor que o atual trocamos com o que já temos
    if(melhorDelta < 0){
        std::swap(s.caminho[melhor_i], s.caminho[melhor_j]);
        s.custo = s.custo + melhorDelta; // Soma com delta negativo, assim tendo o custo do melhor vizinho.
        
        // Retorna true se foi encontrado um vizinho melhor
        return true;
    }

    return false; // Se não for encontrado nenhum vizinho melhor
}

bool bestImprovement2Opt(Solucao& s){
    double melhorDelta = 0;
    int melhor_i = -1, melhor_j = -1;

    for(int i = 0; i < (int)s.caminho.size() - 2; i++){
        int vi = s.caminho[i];
        int vi_prox = s.caminho[i+1];

        for(int j = i+2; j < (int)s.caminho.size() - 1; j++){
            int vj = s.caminho[j];
            int vj_prox = s.caminho[j+1];

            // Calcula se aquela aresta vale a pena ser trocada
            // Estradas adicionadas - Estradas removidas
            double novoDelta = matrizDistancias[vi][vj] + matrizDistancias[vi_prox][vj_prox]
                                -(matrizDistancias[vi][vi_prox] + matrizDistancias[vj][vj_prox]);

            // Se novo delta diminiu o custo, então colocamos os melhores globais como 
            // os dos casos melhorados
            if(novoDelta < melhorDelta){
                melhorDelta = novoDelta;
                melhor_i = i;
                melhor_j = j;
            }
        }
    }

    if(melhorDelta < 0){
        // Ao invés de trocar todo o caminho, apenas invertemos o conjunto que 
        // foi trocado, assim ele continua seguindo seu proposito
        std::reverse(s.caminho.begin() + melhor_i + 1, s.caminho.begin() + melhor_j + 1);
        s.custo += melhorDelta;

        // Se encontrada uma solução melhor retorna true
        return true;
    }

    // Se não encontrada solução melhor retorna falso
    return false;
}

bool bestImprovementOrOpt(Solucao& s, int k){
    if (k < 1){
        throw std::invalid_argument("k deve ser maior ou igual a 1!");
    }

    if(k >= (int)s.caminho.size() - 2){
        throw std::invalid_argument("k deve ser menor que o tamanho do caminho!");
    }

    double melhorDelta = 0;
    int melhor_i = -1, melhor_j = -1;

    for(int i = 1; i + k < (int)s.caminho.size() - 1; i++){
        // Pega os valores do começo do bloco
        int vi = s.caminho[i];
        int vi_ant = s.caminho[i-1];

        // Pega o valor do final do bloco
        int vk = s.caminho[i+k-1];
        int vk_prox = s.caminho[i+k];

        // Verifica as posições a frente do bloco
        for(int j = i + k; j < (int)s.caminho.size() - 1; j++){
            int vj = s.caminho[j];
            int vj_prox = s.caminho[j+1];

            // Estradas novas - Estradas removidas
            double novoDelta = (matrizDistancias[vj][vi] + matrizDistancias[vk][vj_prox] + matrizDistancias[vi_ant][vk_prox])
                             - (matrizDistancias[vi_ant][vi] + matrizDistancias[vk][vk_prox] + matrizDistancias[vj][vj_prox]);
                        
            if(novoDelta < melhorDelta){
                melhorDelta = novoDelta;
                melhor_i = i;
                melhor_j = j;
            }
        }

        // Verifica atrás do bloco
        for(int j = 0; j < i-1; j++){
            int vj = s.caminho[j];
            int vj_prox = s.caminho[j+1];

            // Estradas novas - Estradas removidas
            double novoDelta = (matrizDistancias[vj][vi] + matrizDistancias[vk][vj_prox] + matrizDistancias[vi_ant][vk_prox])
                             - (matrizDistancias[vi_ant][vi] + matrizDistancias[vk][vk_prox] + matrizDistancias[vj][vj_prox]);
                        
            if(novoDelta < melhorDelta){
                melhorDelta = novoDelta;
                melhor_i = i;
                melhor_j = j;
            }
        }
    }
    
    if(melhorDelta < 0){
        // Aloca um vetor para o bloco a ser inserido
        std::vector<int> blocoInserido(s.caminho.begin() + melhor_i, s.caminho.begin() + melhor_i + k);

        // Apaga o bloco da memoria
        s.caminho.erase(s.caminho.begin() + melhor_i, s.caminho.begin() + melhor_i + k);

        // Recalcua o melhor_j com a retirada das arestas
        if(melhor_j > melhor_i){
            melhor_j -= k; 
        }

        // Insere as novas arestas 
        s.caminho.insert(
            s.caminho.begin() + melhor_j + 1, 
            blocoInserido.begin(), 
            blocoInserido.end()
        );

        // Recalcula o custo
        s.custo += melhorDelta;

        // Retorna true caso encontrou um caminho melhor
        return true;
    }

    // Retorna false caso não conseguiu um caminho melhor
    return false;
}