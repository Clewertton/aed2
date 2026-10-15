# Respostas da Análise de Desempenho - Bucket Sort (Aula 09)

## Exercícios 2 e 4(a): Tabela Comparativa de Comportamento (Bucket Sort em C)

O Bucket Sort distribui os elementos em sublistas (baldes) e depois ordena cada um individualmente. O desempenho prático depende profundamente de quão bem os dados estão distribuídos pelo intervalo estabelecido.

| Cenário (Tamanho) | Configuração dos Dados | Comparações | Movimentações | Tempo de Execução |
| :--- | :--- | :--- | :--- | :--- |
| **Pequeno (N=10)** | Ordenado / Inverso / Aleatório | Baixo | Baixo | Quase instantâneo |
| **Médio (N=100)** | Ordenado (Uniforme) | Muito Baixo | Baixo | Muito rápido |
| **Médio (N=100)** | Inversamente Ordenado | Alto (se colidir em poucos baldes) | Alto | Lento |
| **Grande (N=5000)**| Aleatório (sem repetição) | Moderado | Moderado | Muito Rápido ($O(n+k)$) |
| **Grande (N=5000)**| Aleatório (com muita repetição)| Alto (muitas colisões nos baldes) | Alto | Mais lento (tende a $O(n^2)$) |

---

## Exercício 4(b): Respostas Qualitativas (Bucket Sort vs. Insertion Sort)

**1. Qual algoritmo foi mais rápido em vetores pequenos, médios e grandes?**
* **Médios e Grandes:** O **Bucket Sort** apresenta excelente desempenho (atingindo tempo linear $O(n+k)$) para vetores grandes quando a distribuição dos dados é uniforme, superando o Insertion Sort com facilidade.
* **Pequenos:** Na linguagem **C**, a sobrecarga de gerenciar a memória dos baldes manualmente (alocação dinâmica com `malloc`/`realloc` ou matrizes estáticas) para um vetor de apenas 10 posições torna um algoritmo simples (como o Insertion Sort direto no array principal) mais rápido na prática.

**2. Qual se manteve estável em todos os casos?**
A estabilidade do Bucket Sort depende exclusivamente do algoritmo escolhido para ordenar internamente os baldes. Como utilizamos o **Insertion Sort** clássico em nossa implementação em C, **sim, a ordenação é estável** (preserva a ordem original de elementos repetidos). Contudo, o seu *tempo de execução* não é estável: no pior caso, se a distribuição for péssima e todos os dados caírem no mesmo balde, o desempenho despenca para $O(n^2)$.

**3. Qual se mostrou mais confiável quando o vetor já estava ordenado ou inverso?**
* **Vetor Ordenado:** O Insertion Sort é extremamente confiável quando o vetor já está ordenado, exigindo apenas $O(n)$ comparações.
* **Vetor Inverso:** O Insertion Sort puro sofre muito com seu pior caso $O(n^2)$. O **Bucket Sort** consegue ser mais confiável e eficiente no cenário inverso porque, ao invés de deslocar os números por todo o vetor, a sua etapa inicial fragmenta e aproxima os elementos corretos em seus respectivos baldes, reduzindo drasticamente o esforço do Insertion Sort na etapa final de cada sublista.