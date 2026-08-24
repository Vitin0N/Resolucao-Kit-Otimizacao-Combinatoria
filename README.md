# Resolução do Kit de Otimização Combinatória

Este repositório reúne a resolução do kit de exercícios de Otimização Combinatória, com foco no problema do caixeiro viajante (TSP).

## Conteúdo

- Anotações sobre os conceitos estudados no kit.
- Implementação de métodos heurísticos para o TSP.
- Leitor de instâncias no formato `.tsp`.
- Instâncias usadas nos testes.
- Arquivo de saída com o tour encontrado.

## Organização do projeto

- `docs/kit.pdf`: material do kit de Otimização Combinatória.
- `anotacao.md`: anotações e explicações dos conceitos estudados.
- `TSP/`: implementação principal do problema do caixeiro viajante.


## Implementação do TSP

A implementação principal contém:

- Leitura de instâncias `.tsp` e criação da matriz de distâncias.
- Construção de uma solução inicial por inserção mais barata.
- Busca local com Swap, 2-opt e Or-opt.
- Perturbação Double Bridge.
- Meta-heurística ILS.
- Geração de arquivos `.tour`.

Mais detalhes estão disponíveis em [TSP/README.md](TSP/README.md).