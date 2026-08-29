import subprocess
import csv
import time

# Todos os resultados do benchmark do livro
RESULTADO_ORIGINAIS = [
    {'nome': 'instancias/a280', 'tempo': 96.623, 'custo': 2579},
    {'nome': 'instancias/ali535', 'tempo': 1525.0, 'custo': 202384},
    {'nome': 'instancias/att48', 'tempo': 0.3, 'custo': 10628},
    {'nome': 'instancias/att532', 'tempo': 1778.96, 'custo': 27731},
    {'nome': 'instancias/bayg29', 'tempo': 0.043, 'custo': 1610},
    {'nome': 'instancias/bays29', 'tempo': 0.05, 'custo': 2020},
    {'nome': 'instancias/berlin52', 'tempo': 0.374, 'custo': 7542},
    {'nome': 'instancias/bier127', 'tempo': 10.209, 'custo': 118282},
    {'nome': 'instancias/brazil58', 'tempo': 0.479, 'custo': 25395},
    {'nome': 'instancias/brg180', 'tempo': 12.824, 'custo': 1950},
    {'nome': 'instancias/burma14', 'tempo': 0.004, 'custo': 3323},
    {'nome': 'instancias/ch130', 'tempo': 10.91, 'custo': 6110},
    {'nome': 'instancias/ch150', 'tempo': 10.43, 'custo': 6528},
    {'nome': 'instancias/d198', 'tempo': 33.639, 'custo': 15780},
    {'nome': 'instancias/d493', 'tempo': 1132.48, 'custo': 35042},
    {'nome': 'instancias/dantzig42', 'tempo': 0.161, 'custo': 699},
    {'nome': 'instancias/eil101', 'tempo': 4.436, 'custo': 629},
    {'nome': 'instancias/eil51', 'tempo': 0.369, 'custo': 426},
    {'nome': 'instancias/eil76', 'tempo': 1.549, 'custo': 538},
    {'nome': 'instancias/fl417', 'tempo': 365.503, 'custo': 11861},
    {'nome': 'instancias/fri26', 'tempo': 0.033, 'custo': 937},
    {'nome': 'instancias/gil262', 'tempo': 82.271, 'custo': 2378.7},
    {'nome': 'instancias/gr120', 'tempo': 9.065, 'custo': 6942},
    {'nome': 'instancias/gr137', 'tempo': 11.348, 'custo': 69853},
    {'nome': 'instancias/gr17', 'tempo': 0.008, 'custo': 2085},
    {'nome': 'instancias/gr202', 'tempo': 37.105, 'custo': 40160.1},
    {'nome': 'instancias/gr21', 'tempo': 0.014, 'custo': 2707},
    {'nome': 'instancias/gr229', 'tempo': 61.498, 'custo': 134613},
    {'nome': 'instancias/gr24', 'tempo': 0.028, 'custo': 1272},
    {'nome': 'instancias/gr431', 'tempo': 721.745, 'custo': 171530},
    {'nome': 'instancias/gr48', 'tempo': 0.314, 'custo': 5046},
    {'nome': 'instancias/gr96', 'tempo': 3.475, 'custo': 55209},
    {'nome': 'instancias/hk48', 'tempo': 0.336, 'custo': 11461},
    {'nome': 'instancias/kroA100', 'tempo': 3.468, 'custo': 21282},
    {'nome': 'instancias/kroA150', 'tempo': 11.751, 'custo': 26524},
    {'nome': 'instancias/kroA200', 'tempo': 32.951, 'custo': 29368},
    {'nome': 'instancias/kroB100', 'tempo': 3.748, 'custo': 22141},
    {'nome': 'instancias/kroB150', 'tempo': 10.634, 'custo': 26130},
    {'nome': 'instancias/kroB200', 'tempo': 35.53, 'custo': 29437.2},
    {'nome': 'instancias/kroC100', 'tempo': 3.568, 'custo': 20749},
    {'nome': 'instancias/kroD100', 'tempo': 4.114, 'custo': 21294},
    {'nome': 'instancias/kroE100', 'tempo': 3.745, 'custo': 22068},
    {'nome': 'instancias/lin105', 'tempo': 4.355, 'custo': 14379},
    {'nome': 'instancias/lin318', 'tempo': 188.78, 'custo': 42045.7},
    {'nome': 'instancias/linhp318', 'tempo': 187.536, 'custo': 42053.1},
    {'nome': 'instancias/pcb442', 'tempo': 597.431, 'custo': 50876},
    {'nome': 'instancias/pr107', 'tempo': 4.582, 'custo': 44303},
    {'nome': 'instancias/pr124', 'tempo': 7.021, 'custo': 59030},
    {'nome': 'instancias/pr136', 'tempo': 13.632, 'custo': 96772},
    {'nome': 'instancias/pr144', 'tempo': 10.479, 'custo': 58537},
    {'nome': 'instancias/pr152', 'tempo': 8.708, 'custo': 73682},
    {'nome': 'instancias/pr226', 'tempo': 45.27, 'custo': 80369},
    {'nome': 'instancias/pr264', 'tempo': 64.758, 'custo': 49135},
    {'nome': 'instancias/pr299', 'tempo': 130.098, 'custo': 48194.8},
    {'nome': 'instancias/pr76', 'tempo': 1.366, 'custo': 108159},
    {'nome': 'instancias/rat195', 'tempo': 28.046, 'custo': 2326.1},
    {'nome': 'instancias/rat99', 'tempo': 4.115, 'custo': 1211},
    {'nome': 'instancias/rd100', 'tempo': 3.983, 'custo': 7910},
    {'nome': 'instancias/rd400', 'tempo': 498.288, 'custo': 15296.1},
    {'nome': 'instancias/si175', 'tempo': 17.333, 'custo': 21407},
    {'nome': 'instancias/si535', 'tempo': 758.534, 'custo': 48466.8},
    {'nome': 'instancias/st70', 'tempo': 1.03, 'custo': 675},
    {'nome': 'instancias/swiss42', 'tempo': 0.155, 'custo': 1273},
    {'nome': 'instancias/ts225', 'tempo': 28.869, 'custo': 126643},
    {'nome': 'instancias/tsp225', 'tempo': 45.368, 'custo': 3916},
    {'nome': 'instancias/u159', 'tempo': 10.828, 'custo': 42080},
    {'nome': 'instancias/ulysses16', 'tempo': 0.008, 'custo': 6859},
    {'nome': 'instancias/ulysses22', 'tempo': 0.019, 'custo': 7013},
]

tempos = []
custos = []

# Calcula o tempo total da execução
inicio = time.perf_counter()

for instancia in RESULTADO_ORIGINAIS:
    if instancia['nome'] != 'instancias/att48':
        continue

    file_name = f"{instancia["nome"]}.tsp"

    # Roda apenas as instâncias que tem menos de 15 segundos no resultado do benchmark
    if(instancia['tempo'] > 10):
        continue

    print(f"Executando o arquivo <{file_name}>")
    for i in range(10):
        resultado = subprocess.run(
            ["./benchmark_status", file_name],
            capture_output=True,
            text=True
        )

        # output recebe divide o resultado do arquivo
        output = resultado.stdout.split("\n")
        output.pop()

        custo, tempo = output[-1].split(" ")

        tempos.append(float(tempo))
        custos.append(float(custo))

    nome = file_name.split("/")[-1].removesuffix(".tsp")
    tempo_media = round(sum(tempos) / len(tempos), 3)
    custo_media = round(sum(custos) / len(custos), 3)

    dado = [nome, tempo_media, custo_media]

    with open("data/resultados_obtidos.csv", "a") as fp:
        writer = csv.writer(fp)
        writer.writerow(dado)

    break
    tempos = []
    custos = []

fim = time.perf_counter()

tempo_exec = fim - inicio


print("Todas as intâncias foram rodadas!")
if tempo_exec > 3600:
    tempo_exec_hr = int(tempo_exec // 3600)
    tempo_exec_min = int((tempo_exec % 3600) // 60)
    tempo_exec_seg = ((tempo_exec % 3600) // 60) % 60 
    print(f"O tempo de execução foi de {tempo_exec_hr:02d}:{tempo_exec_min:02d}:{tempo_exec_seg:.3f}")
elif tempo_exec > 60:
    tempo_exec_min = int(tempo_exec // 60)
    tempo_exec_seg = tempo_exec % 60 
    print(f"O tempo de execução foi de {tempo_exec_min:02d}:{tempo_exec_seg:.3f}")
else:
    print(f"O tempo de execução foi de {tempo_exec:.3f}")

