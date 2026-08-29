import pandas as pd
import matplotlib.pyplot as plt

# Definindo os nomes das colunas do arquivo
colunas = ['nome', 'tempo', 'custo']

# Agora carregamos seus dados e os dados do benchmark original
df_meu = pd.read_csv('data/resultados_obtidos.csv', names=colunas)
df_orig = pd.read_csv('data/resultados_benchmark_original.csv', names=colunas)

# Vamos juntar os dados para poder comparar lado a lado
df = pd.merge(df_orig, df_meu, on='nome', suffixes=('_orig', '_meu'))

# Aqui a gente prepara os dados (converte pra número e remove linhas problemáticas)
colunas_numericas = ['tempo_orig', 'tempo_meu', 'custo_orig', 'custo_meu']
for col in colunas_numericas:
    if df[col].dtype == object:
        df[col] = df[col].astype(str).str.replace(',', '.')
    df[col] = pd.to_numeric(df[col], errors='coerce')

df = df.dropna()  # Tira as linhas que não conseguimos processar

# Agora calculamos as métricas importantes pra ver como você se saiu
df['speedup'] = df['tempo_orig'] / df['tempo_meu']
# E o quanto sua solução diferiu da original em termos de custo:
df['gap_custo_%'] = ((df['custo_meu'] - df['custo_orig']) / df['custo_orig']) * 100

# Mostramos os resultados na tela
print("\nResultados:\n")
print(df[['nome', 'speedup', 'gap_custo_%']].round(3).to_string(index=False))
print("\nSpeedup > 1 = foi mais rápido! Gap < 0 = encontrou solução melhor!\n")

# Criando os gráficos pra visualizar melhor seus resultados
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))

# Gráfico 1: Comparando tempos - seu algoritmo vs o original
ax1.scatter(df['tempo_orig'], df['tempo_meu'], color='#2ca02c', alpha=0.7)

# Desenha uma linha diagonal (se estiver abaixo dela, você foi mais rápido!)
if not df.empty:
    limite_max = max(df['tempo_orig'].max(), df['tempo_meu'].max())
    ax1.plot([0, limite_max], [0, limite_max], 'r--', label='Linha de Empate (Mesma Velocidade)')

ax1.set_xlabel('Tempo do Algoritmo Original (segundos)')
ax1.set_ylabel('Tempo (segundos)')
ax1.set_title('Velocidade da implementação')
ax1.legend()
ax1.grid(True, linestyle=':', alpha=0.6)

# Gráfico 2: Qualidade das soluções que você encontrou
cores = ['#d62728' if gap > 0 else '#1f77b4' for gap in df['gap_custo_%']]
ax2.bar(df['nome'], df['gap_custo_%'], color=cores)
ax2.axhline(0, color='black', linewidth=1)
ax2.set_xlabel('Instâncias do Problema')
ax2.set_ylabel('Diferença de Custo (%)')
ax2.set_title('Qualidade das soluções')
ax2.tick_params(axis='x', rotation=45)

plt.tight_layout()
plt.savefig('assets/analise_benchmarks.png', dpi=300)
print("Gráficos salvos em 'assets/analise_benchmarks.png'")
plt.show()