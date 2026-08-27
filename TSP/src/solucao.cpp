/*
solução.cpp
  
Aqui vai ficar a implementação da solução que eu vou contruir para o problema do TSP
*/

#include "solucao.h"

std::vector<std::vector<double>> matrizDistancias; // Definindo a matrizDistancia
int matrizDimensao; // Definindo a dimensão da matriz
int dimensao;

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

    matrizDimensao = 6;
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
    s.caminho = randomiza3Cidades(matrizDimensao);

    // Adiciona a lista de candidatos as cidades restantes
    std::vector<int> CL = cidadesRestantes(s.caminho, matrizDimensao);

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

void buscaLocal(Solucao& s){
    std::vector<int> NL = {1, 2, 3, 4, 5};
    bool melhorou = false;

    while(!NL.empty()){
        int n = rand() % NL.size();
        switch (NL[n]){
            case 1:
                melhorou = bestImprovementSwap(s);
                break;
            case 2:
                melhorou = bestImprovement2Opt(s);
                break;
            case 3:
                melhorou = bestImprovementOrOpt(s, 1);
                break;
                case 4:
                melhorou = bestImprovementOrOpt(s, 2);  
                break;
            case 5:
               melhorou = bestImprovementOrOpt(s, 3);
               break;
        }

        if(melhorou){
            NL = {1,2,3,4,5};
        } else {
            NL.erase(NL.begin() + n);
        }
    }
}

// ===== Funções de Pertubação =====

Solucao pertubacaoDoubleBridge(const Solucao& solucao){

    Solucao s = solucao;

    // Gerador de aleatoriedade
    std::random_device rd;
    std::mt19937 gen(rd());

    int n = s.caminho.size()-1;

    int tamMax = std::max(2, (n + 9) / 10);
    std::uniform_int_distribution<int> distTam(2, tamMax);

    int tam_i = distTam(gen);
    int tam_j = distTam(gen);

    // Previnindo que o segmento i não vá ao final da cidade
    int max_i = n - tam_i - tam_j;
    if(max_i < 1){
        tam_i = 1;
        tam_j = 1;
        max_i = n-2;
    }

    // Decide o primeiro subconjunto de cidades
    std::uniform_int_distribution<int> dist_i(1, max_i);
    int i = dist_i(gen);
    int k_i = i + tam_i -1;

    // Decide segundo subconjunto de cidades
    int max_j = n - tam_j;
    std::uniform_int_distribution<int> dist_j(k_i + 1, max_j);
    int j = dist_j(gen);
    int k_j = j + tam_j - 1;

    // Criando o caminho do novo
    std::vector<int> novoCaminho;

    // Insere a cidade antes do i
    novoCaminho.insert(novoCaminho.end(), s.caminho.begin(), s.caminho.begin() + i);

    // Insere o segundo segmento
    novoCaminho.insert(novoCaminho.end(), s.caminho.begin() + j, s.caminho.begin() + k_j + 1);
    
    // Insere as cidades entre os segmentos
    novoCaminho.insert(novoCaminho.end(), s.caminho.begin() + k_i + 1, s.caminho.begin() + j);
    
    // Insere o primeiro segmento 
    novoCaminho.insert(novoCaminho.end(), s.caminho.begin() + i, s.caminho.begin() + k_i + 1);

    // Insere o resto do caminho
    novoCaminho.insert(novoCaminho.end(), s.caminho.begin() + k_j + 1, s.caminho.end());

    // Define o novo caminho
    s.caminho = novoCaminho;
    
    // Atualiza o custo
    calcularValorObj(s);

    return s;
}

// ==== Função para gerar a solução final ====

Solucao ILS(const Solucao& s, int maxIter, int maxIterILS){
    Solucao melhorSolucao;
    melhorSolucao.custo = INFINITY;

    for(int i = 0; i < maxIter; i++){
        Solucao sol = s;
        Solucao melhor_s = sol;

        int iterIls = 0;

        while(iterIls < maxIterILS){
            buscaLocal(sol);

            if(sol.custo < melhor_s.custo){
                melhor_s = sol;
                iterIls = 0;
            }

            sol = pertubacaoDoubleBridge(melhor_s);

            iterIls++;
        }

        if(melhor_s.custo < melhorSolucao.custo){
            melhorSolucao = melhor_s;
        }
    }

    return melhorSolucao;
}

void criarTour(const Solucao& s, std::string tour_name){
    std::ofstream arquivo(tour_name);

    size_t pos = tour_name.find(".tour");
    std::string nome = tour_name.substr(0, pos);

    if(!arquivo){
        std::cerr << "Erro ao abrir arquivo" << tour_name << std::endl;
        return;
    }

    arquivo << "NAME: " << nome << "\n";
    arquivo << "TYPE: TOUR\n";
    arquivo << "DIMENSION:" << dimensao << "\n";
    arquivo << "TOUR_SECTION\n";

    for(int cidade : s.caminho){
        arquivo << cidade << '\n';
    }

    arquivo << "-1\n";
    arquivo << "EOF\n";

    arquivo.close();
}