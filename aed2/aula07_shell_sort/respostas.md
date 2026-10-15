# Respostas da Análise de Desempenho - Shell Sort (Aula 07)

## Exercício 2 e 4(a): Tabela Comparativa (Shell Sort vs. Insertion Sort)

O Shell Sort melhora o Insertion Sort fazendo o "trabalho mais difícil" primeiro com gaps grandes, o que reduz drasticamente a quantidade de deslocamentos para vetores desordenados e longos.

| Cenário (Dados Aleatórios) | Algoritmo | Comparações (aprox.) | Movimentações (aprox.) | Tempo Relativo | Comportamento (O) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Pequeno (N=10)** | Insertion Sort | Baixo | Baixo | Rápido | $O(n^2)$ |
| **Pequeno (N=10)** | Shell Sort | Baixo (ex: 13) | Baixo (ex: 27) | Rápido | $O(n \log n)$ a $O(n^2)$ |
| **Médio (N=100)** | Insertion Sort | Alto | Alto | Lento | $O(n^2)$ |
| **Médio (N=100)** | Shell Sort | Médio | Médio | Rápido | Superior a $O(n^2)$ |
| **Grande (N=5000)**| Insertion Sort | Muito Alto | Muito Alto | Muito Lento | $O(n^2)$ |
| **Grande (N=5000)**| Shell Sort | Moderado | Moderado | Muito Rápido | Superior a $O(n^2)$ |

*(Referência de base extraída da comparação de custo na Aula 07: O Insertion Sort gasta muito mais trocas e comparações do que o Shell Sort para o mesmo conjunto de dados)*

---

## Exercício 4(b): Respostas Qualitativas

**1. Qual algoritmo foi mais rápido em vetores pequenos, médios e grandes?**
* **Pequenos (N=10):** A diferença é estatisticamente nula. O Insertion Sort costuma ganhar por margem mínima porque não possui a sobrecarga de calcular os "gaps".
* **Médios (N=100) e Grandes (N=5000):** O **Shell Sort** é imensamente superior e mais rápido. Ao usar saltos iniciais longos, ele aproxima os elementos rapidamente da sua posição final, enquanto o Insertion Sort sofre deslocando elementos casa por casa. O Shell Sort atua como uma ponte eficiente para fugir do custo de $O(n^2)$.

**2. Qual se manteve estável em todos os casos?**
* Apenas o **Insertion Sort** é estável.
* O **Shell Sort NÃO é estável**. Ao comparar e mover elementos aos saltos (usando o `gap`), ele frequentemente inverte a posição original de dois elementos que possuem valores idênticos.

**3. Qual se mostrou mais confiável quando o vetor já estava ordenado ou inverso?**
* **Vetor já ordenado:** Ambos os algoritmos são muito rápidos e confiáveis, aproximando-se de $O(n)$ porque o laço interno de trocas (o `while`) quebra logo na primeira iteração.
* **Vetor inverso:** O **Shell Sort** é muito mais confiável. O Insertion Sort, no pior caso (inverso), obriga que todo elemento percorra todo o vetor até o início, caindo na complexidade exata de $O(n^2)$. O Shell Sort mitiga esse cenário catastrófico resolvendo os deslocamentos longos nas primeiras rodadas de `gap`.