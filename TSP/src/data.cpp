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
                    matrizDistancias[j][i] = matrizDistancias[i][j];
                } else {
                    matrizDistancias[i][j] = 0;
                }
            }
        }

    } else if(ewfTipo == "EXPLICIT"){
        std::string ewfFormat;

        // Busca EDGE_WEIGHT_FORMAT
        while(std::getline(arquivo, linha)){
            if(linha.find("EDGE_WEIGHT_FORMAT") != std::string::npos){
                std::stringstream ss(linha);
                std::string token;

                while(ss >> token){
                    if(token == "EDGE_WEIGHT_FORMAT:"){
                        ss >> ewfFormat;
                        break;
                    }

                    if(token == "EDGE_WEIGHT_FORMAT"){
                        ss >> token;

                        if(token == ":"){
                            ss >> ewfFormat;
                        } else {
                            ewfFormat = token;
                        }

                        break;
                    }
                }

                break;
            }
        }

        if(ewfFormat == "FUNCTION"){
            std::cerr << "EDGE_WEIGHT_FORMAT FUNCTION não suportado."
                      << std::endl;
            return;
        }

        // Busca EDGE_WEIGHT_SECTION
        bool encontrouSecao = false;
        while(arquivo >> palavra){
            if(palavra == "EDGE_WEIGHT_SECTION" ||
               palavra == "EDGE_WEIGHT_SECTION:"){
                encontrouSecao = true;
                break;
            }
        }

        if(!encontrouSecao){
            std::cerr << "EDGE_WEIGHT_SECTION não encontrada."
                      << std::endl;
            return;
        }

        // FULL_MATRIX
        if(ewfFormat == "FULL_MATRIX"){
            for(int i = 1; i <= dimensao; i++){
                for(int j = 1; j <= dimensao; j++){
                    arquivo >> matrizDistancias[i][j];

                    if(i == j){
                        matrizDistancias[i][j] = 0.0;
                    }
                }
            }
        }

        // UPPER_ROW
        else if(ewfFormat == "UPPER_ROW"){
            for(int i = 1; i <= dimensao; i++){
                for(int j = i + 1; j <= dimensao; j++){
                    arquivo >> matrizDistancias[i][j];
                    matrizDistancias[j][i] = matrizDistancias[i][j];
                }
            }
        }

        // LOWER_ROW
        else if(ewfFormat == "LOWER_ROW"){
            for(int i = 2; i <= dimensao; i++){
                for(int j = 1; j < i; j++){
                    arquivo >> matrizDistancias[i][j];
                    matrizDistancias[j][i] = matrizDistancias[i][j];
                }
            }
        }

        // UPPER_DIAG_ROW
        else if(ewfFormat == "UPPER_DIAG_ROW"){
            for(int i = 1; i <= dimensao; i++){
                for(int j = i; j <= dimensao; j++){
                    arquivo >> matrizDistancias[i][j];
                    matrizDistancias[j][i] = matrizDistancias[i][j];

                    if(i == j){
                        matrizDistancias[i][j] = 0.0;
                    }
                }
            }
        }

        // LOWER_DIAG_ROW
        else if(ewfFormat == "LOWER_DIAG_ROW"){
            for(int i = 1; i <= dimensao; i++){
                for(int j = 1; j <= i; j++){
                    arquivo >> matrizDistancias[i][j];
                    matrizDistancias[j][i] = matrizDistancias[i][j];

                    if(i == j){
                        matrizDistancias[i][j] = 0.0;
                    }
                }
            }
        }

        // UPPER_COL
        else if(ewfFormat == "UPPER_COL"){
            for(int j = 2; j <= dimensao; j++){
                for(int i = 1; i < j; i++){
                    arquivo >> matrizDistancias[i][j];
                    matrizDistancias[j][i] = matrizDistancias[i][j];
                }
            }
        }

        // LOWER_COL
        else if(ewfFormat == "LOWER_COL"){
            for(int j = 1; j < dimensao; j++){
                for(int i = j + 1; i <= dimensao; i++){
                    arquivo >> matrizDistancias[i][j];
                    matrizDistancias[j][i] = matrizDistancias[i][j];
                }
            }
        }

        // UPPER_DIAG_COL
        else if(ewfFormat == "UPPER_DIAG_COL"){
            for(int j = 1; j <= dimensao; j++){
                for(int i = 1; i <= j; i++){
                    arquivo >> matrizDistancias[i][j];
                    matrizDistancias[j][i] = matrizDistancias[i][j];

                    if(i == j){
                        matrizDistancias[i][j] = 0.0;
                    }
                }
            }
        }

        // LOWER_DIAG_COL
        else if(ewfFormat == "LOWER_DIAG_COL"){
            for(int j = 1; j <= dimensao; j++){
                for(int i = j; i <= dimensao; i++){
                    arquivo >> matrizDistancias[i][j];
                    matrizDistancias[j][i] = matrizDistancias[i][j];

                    if(i == j){
                        matrizDistancias[i][j] = 0.0;
                    }
                }
            }
        }

        else{
            std::cerr << "EDGE_WEIGHT_FORMAT não suportado: "
                      << ewfFormat << std::endl;
            return;
        }
    } else if(ewfTipo == "CEIL_2D"){
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
                    
                    matrizDistancias[i][j] = std::ceil(std::sqrt(deltaX * deltaX + deltaY * deltaY));
                    matrizDistancias[j][i] = matrizDistancias[i][j];
                } else {
                    matrizDistancias[i][j] = 0;
                }
            }
        }
    } else if(ewfTipo == "GEO"){
        std::vector<double> latitude(dimensao + 1);
        std::vector<double> longitude(dimensao + 1);
        int id;

        // Busca a seção das coordenadas
        while(arquivo >> palavra){
            if(palavra == "NODE_COORD_SECTION:" || palavra == "NODE_COORD_SECTION"){
                break;
            }
        }

        // Lê latitude e longitude no formato GEO do TSPLIB
        for(int i = 0; i < dimensao; i++){
            arquivo >> id >> latitude[id] >> longitude[id];
        }

        const double PI = 3.141592;
        const double RRR = 6378.388; // Raio médio da Terra

        // Converte graus/minutos para radianos
        for(int i = 1; i <= dimensao; i++){
            int grausLatitude = static_cast<int>(latitude[i]);
            int grausLongitude = static_cast<int>(longitude[i]);

            double minutosLatitude = latitude[i] - grausLatitude;

            double minutosLongitude = longitude[i] - grausLongitude;

            latitude[i] = PI * (grausLatitude + 5.0 * minutosLatitude / 3.0) / 180.0;

            longitude[i] = PI * (grausLongitude + 5.0 * minutosLongitude / 3.0) / 180.0;
        }

        // Calcula as distâncias GEO
        for(int i = 1; i <= dimensao; i++){
            for(int j = 1; j <= dimensao; j++){
                if(i == j){
                    matrizDistancias[i][j] = 0.0;
                    continue;
                }

                double q1 = std::cos(longitude[i] - longitude[j]);
                double q2 = std::cos(latitude[i] - latitude[j]);
                double q3 = std::cos(latitude[i] + latitude[j]);

                double argumento = 0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3);

                // Evita erro numérico no acos
                if(argumento > 1.0) argumento = 1.0;
                if(argumento < -1.0) argumento = -1.0;

                matrizDistancias[i][j] =
                    static_cast<int>(
                        RRR * std::acos(argumento) + 1.0
                    );
            }
        }
    } else if(ewfTipo == "ATT") {
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
                    
                    double r_ij = std::sqrt((deltaX * deltaX + deltaY * deltaY / 10.0));
                    double t_ij = std::floor(r_ij + 0.5);

                    if(t_ij < r_ij){
                        matrizDistancias[i][j] = t_ij + 1.0;
                    } else {
                        matrizDistancias[i][j] = t_ij;
                    }

                } else {
                    matrizDistancias[i][j] = 0;
                }
            }
        }
    } else {
        std::cerr << "Error: " << ewfTipo << " nao eh suportado!" << std::endl;
        arquivo.close();
        exit(EXIT_FAILURE);
    }

    arquivo.close();

    std::cout << "Instância carregada com sucesso! Dimensão: " << dimensao << std::endl;
}