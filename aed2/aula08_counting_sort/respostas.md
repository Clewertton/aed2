# Respostas da Análise de Desempenho - Counting Sort (Aula 08)

## Exercício 1(b): A ausência de comparações
O Counting Sort é um algoritmo de ordenação por contagem, o que significa que ele **não ordena comparando elementos** (como $A[i] > A[j]$). Por isso, em qualquer execução, o número de comparações de ordenação será **0**. Suas métricas de custo baseiam-se unicamente nas movimentações (cópias de memória para os vetores auxiliares).

## Exercício 2 e 4(a): Tabela Comparativa de Complexidade Teórica

Diferente dos algoritmos baseados em comparação cujo limite ótimo é $O(n \log n)$, o Counting Sort opera em complexidade linear $O(n + k)$, onde $k$ é o maior valor do vetor.

| Algoritmo | Complexidade de Tempo (Todos os Casos) | Complexidade de Espaço | Comparações | É Estável? |
| :--- | :--- | :--- | :--- | :--- |
| **Bubble Sort** | $O(n^2)$ | $O(1)$ | Altíssimas | Sim |
| **Merge Sort** | $O(n \log n)$ | $O(n)$ | Altas | Sim |
| **Counting Sort**| $O(n + k)$ | $O(n + k)$ | **Zero (0)** | Sim |

---

## Exercício 4(b): Respostas Qualitativas

**1. Qual algoritmo foi mais rápido em vetores pequenos, médios e grandes?**
* Se o valor máximo $k$ dos dados for razoável, o **Counting Sort destrói qualquer outro algoritmo** nos cenários médios (N=100) e grandes (N=5000), pois seu custo de $O(n + k)$ é praticamente imediato.
* A ressalva é: O Counting Sort é ruim se o maior valor $k$ for excessivamente gigante em relação ao tamanho $n$ do vetor, pois consumiria muita memória.

**2. Qual se manteve estável em todos os casos?**
* **Todos os três** (Bubble, Merge e Counting Sort).
* O **Counting Sort é rigorosamente estável** porque, na sua fase final (Construção), ele percorre o vetor original de trás para frente, garantindo que chaves iguais repetidas mantenham a ordem cronológica relativa.

**3. Qual se mostrou mais confiável quando o vetor já estava ordenado ou inverso?**
* O **Counting Sort** e o **Merge Sort**.
* O **Counting Sort** ignora completamente a disposição inicial dos elementos. Como ele apenas faz a contagem das frequências e calcula posições matemáticas, o tempo será rigidamente $O(n + k)$ e fará exatamente $2n$ movimentações, quer o vetor esteja já ordenado, invertido ou embaralhado.