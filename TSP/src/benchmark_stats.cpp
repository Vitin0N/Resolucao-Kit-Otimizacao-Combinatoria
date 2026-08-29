#include <chrono>
#include <iostream>

#include "data.h"
#include "solucao.h"

int main(int argc, char* argv[]){
    if(argc < 2){
        std::cerr << "Uso: tsp_solver_status <arquivo.tsp>" << std::endl;
        return 1;
    }

    std::string caminhoTSP = argv[1];

    lerInstancias(caminhoTSP);

    Solucao s = Construcao();
    calcularValorObj(s);

    // Pega o tempo do sistema para calcular o tempo de execução
    auto inicio_tempo = std::chrono::high_resolution_clock::now();

    // Resolve o TSP
    s = ILS(s, 50, 50);

    auto fim_tempo = std::chrono::high_resolution_clock::now();

    // Calcula o tempo de execução
    std::chrono::duration<double> tempo = fim_tempo - inicio_tempo;

    // <custo> <tempo>
    std::cout << s.custo << " " << tempo.count() << std::endl;
}