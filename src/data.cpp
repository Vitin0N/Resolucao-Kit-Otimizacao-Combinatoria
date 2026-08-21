#include "data.h"

extern int dimensao;

void lerInstancias(const std::string& caminhoArquivo){
    std::ifstream arquivo(caminhoArquivo);
    if(!arquivo){
        std::cerr << "Erro ao tentar abrir o arquivo: " << caminhoArquivo << std::endl;
        exit(1);
    }

    std::string palavra;

    // Busca primeiro a dimensão do problema
    while(arquivo >> palavra){
        if(palavra == "DIMENSION:" || palavra == "DIMENSION"){
            arquivo >> dimensao; // Passa o valor da dimensão do problema
            break;
        }
    }

    // Redimensiona a matriz global(ignorando o indice 0)
    matrizDistancias.assign(dimensao + 1, std::vector<double>(dimensao + 1, 0.0));
    matrizDimensao = dimensao;

    // Busca a seção das coordenadas
    while(arquivo >> palavra){
        if(palavra == "NODE_COORD_SECTION:" || palavra ==  "NODE_COORD_SECTION"){
            break;
        }
    }

    // Le as coordenadas
    std::vector<double> coordX(dimensao + 1);
    std::vector<double> coordY(dimensao + 1);
    int id;

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

    arquivo.close();

    std::cout << "Instância carregada com sucesso! Dimensão: " << dimensao << std::endl;


}