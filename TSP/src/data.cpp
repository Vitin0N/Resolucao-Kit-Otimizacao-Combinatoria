#include "data.h"

extern int dimensao;

void lerInstancias(const std::string& caminhoArquivo){
    std::ifstream arquivo(caminhoArquivo);
    if(!arquivo){
        std::cerr << "Erro ao tentar abrir o arquivo: " << caminhoArquivo << std::endl;
        exit(1);
    }

    std::string palavra;
    std::string linha;

    // Busca primeiro a dimensão do problema
    while(std::getline(arquivo, linha)){
        if(linha.find("DIMENSION") != std::string::npos){
            std::stringstream ss(linha);

            ss >> palavra;

            // Remove o ':' caso esteja na linha
            if(!palavra.empty() && palavra.back() == ':'){
                palavra.pop_back();
            }

            while(ss >> palavra){
                try {
                    dimensao = std::stoi(palavra);
                    break;
                }
                catch (...){
                    // Número não encontrado, continua procurando
                }
            }

            break;
        }
    }

    // Verifica o formato de leitura do arquivo
    std::string ewfTipo;

    // Busca o tipo de leitura
    while(std::getline(arquivo, linha)){
        if(linha.find("EDGE_WEIGHT_TYPE") != std::string::npos){
            std::stringstream ss(linha);

            ss >> palavra;
            
            // Limpa os dois pontos caso exista
            if(!palavra.empty() && palavra.back() == ':'){
                palavra.pop_back();
            }

            // Percorre o resto da linha procurando o tipo da linha
            while(ss >> palavra){
                if(palavra == ":") continue;

                ewfTipo = palavra;
            }

            break;
        }
    }

    // Redimensiona a matriz global(ignorando o indice 0)
    matrizDistancias.assign(dimensao + 1, std::vector<double>(dimensao + 1, 0.0));
    matrizDimensao = dimensao;
    
    // Verifica o tipo do edge weigth e aloca os dados na matriz conforme
    
    if(ewfTipo == "EUC_2D"){
        // Le as coordenadas
        std::vector<double> coordX(dimensao + 1);
        std::vector<double> coordY(dimensao + 1);
        int id;

        // Busca a seção das coordenadas
        while(arquivo >> palavra){
            if(palavra == "NODE_COORD_SECTION:" || palavra ==  "NODE_COORD_SECTION"){
                break;
            }
        }

        // Preenche os vetores das coordenadas
        for(int i = 1; i <=  dimensao; i++){
            arquivo >> id >> coordX[i] >> coordY[i];
        }
    
        // Preenchendo a matriz com as distâncias euclidianas
        for(int i = 1; i <= dimensao; i++){
            for(int j = 1; j <= dimensao; j++){
                if(i != j){
                    double deltaX = coordX[i] - coordX[j];
                    double deltaY = coordY[i] - coordY[j];
                    
                    matrizDistancias[i][j] = std::round(std::sqrt(deltaX * deltaX + deltaY * deltaY));
                }
            }
        }
    } else if(ewfTipo == "EXPLICIT"){
        // TODO: Fazer os outros metodos de leitura
    }


    arquivo.close();

    std::cout << "Instância carregada com sucesso! Dimensão: " << dimensao << std::endl;
}