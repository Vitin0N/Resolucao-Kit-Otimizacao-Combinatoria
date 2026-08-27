# TSP - Capítulo 2

Implementação do problema do caixeiro viajante (TSP) desenvolvido a partir do capítulo 2 do kit de Otimização Combinatória.

## O que foi feito

- Leitura de instâncias no formato `.tsp`.
- Criação da matriz de distâncias euclidianas.
- Construção de uma solução inicial usando inserção mais barata com escolha aleatória entre os melhores candidatos.
- Cálculo do custo total do tour.
- Busca local com as vizinhanças:
  - Swap;
  - 2-opt;
  - Or-opt com blocos de tamanho 1, 2 e 3.
- Perturbação Double Bridge.
- Meta-heurística ILS (Iterated Local Search).
- Geração de um arquivo de saída no formato `.tour`.

## Organização

- `src/main.cpp`: ponto de entrada do programa.
- `src/data.cpp`: leitura das instâncias.
- `src/solucao.cpp`: construção, busca local, perturbação e ILS.
- `include/data.h`: declarações relacionadas à leitura dos dados.
- `include/solucao.h`: estruturas e declarações das soluções.
- `instancias/`: instâncias do TSP.
- `makefile`: comandos de compilação.

## Como compilar

Dentro da pasta `TSP`, execute:

```bash
make
```

Também é possível compilar diretamente:

```bash
g++ -Wall -Iinclude -O3 src/main.cpp src/solucao.cpp src/data.cpp -o tsp_solver
```

## Como executar

Ainda dentro da pasta `TSP`:

```bash
./tsp_solver
```

No Windows, o executável gerado pelo `make` pode ser executado com:

```powershell
.\tsp_solver.exe
```

Atualmente, o programa usa a instância `instancias/cristino2.tsp`, definida em `src/main.cpp`. Ao final, gera o arquivo `critino.tour`.

## Fluxo do programa

1. A instância é carregada.
2. Uma solução inicial é construída.
3. O custo da solução inicial é exibido.
4. O ILS aplica busca local e perturbações para tentar melhorar a solução.
5. A melhor solução encontrada e seu custo são exibidos.
6. O tour é salvo em um arquivo `.tour`.