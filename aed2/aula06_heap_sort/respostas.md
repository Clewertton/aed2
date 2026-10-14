# Relatório de Análise: Algoritmos de Ordenação

Este documento contém as respostas detalhadas para a análise teórica e prática comparando os algoritmos Heap Sort, Quick Sort, Merge Sort e Insertion Sort, baseando-se em vetores de diferentes tamanhos e disposições.

## Tabela Comparativa de Desempenho Teórico

| Algoritmo | Complexidade (Médio) | Complexidade (Pior Caso) | Memória Extra | Estável |
| :--- | :--- | :--- | :--- | :--- |
| **Heap Sort** | $O(n \log n)$ | $O(n \log n)$ | $O(1)$ | Não |
| **Quick Sort**| $O(n \log n)$ | $O(n^2)$ | $O(\log n)$ | Não |
| **Merge Sort**| $O(n \log n)$ | $O(n \log n)$ | $O(n)$ | Sim |

---

## Respostas às Questões Propostas

### 1. Qual algoritmo foi mais rápido em vetores pequenos, médios e grandes?
*   **Pequenos ($N=10$):** O **Insertion Sort** costuma ser o mais rápido na prática. Sua baixa sobrecarga operacional (por não possuir chamadas recursivas) compensa a sua complexidade teórica em volumes muito reduzidos de dados.
*   **Médios e Grandes ($N=100$, $N=5000$):** O **Quick Sort** geralmente domina apresentando o menor tempo de execução absoluto em vetores aleatórios, graças ao seu excelente uso de localidade de cache de memória. O Heap Sort e o Merge Sort ficam logo atrás, com tempos muito próximos.

### 2. Qual se manteve estável em todos os casos?
Apenas o **Merge Sort** (além do Insertion Sort) é um algoritmo estável neste grupo. Ele preserva rigorosamente a ordem original de chaves iguais. 
*   *Nota:* O Heap Sort quebra a estabilidade durante as frequentes trocas a longas distâncias na função `heapify`. O Quick Sort também quebra a estabilidade durante as trocas do particionamento.

### 3. Qual se mostrou mais confiável quando o vetor já estava ordenado ou inversamente ordenado?
O **Heap Sort** e o **Merge Sort** são os mais confiáveis nesses cenários de pior caso. Eles garantem rigorosamente o tempo $O(n \log n)$ independentemente da disposição inicial dos dados. O Quick Sort, em sua implementação clássica (com pivô na extremidade), sofre uma degradação severa, caindo para $O(n^2)$ nestes mesmos cenários.

### 4. Qual é o consumo de memória auxiliar de cada algoritmo?
*   **Heap Sort:** Ordena *in-place* (no próprio vetor), utilizando apenas $O(1)$ de memória auxiliar para a variável temporária de troca.
*   **Quick Sort:** Também é considerado *in-place* na alocação de dados, mas consome espaço na pilha de recursão equivalente a $O(\log n)$ no caso médio e até $O(n)$ no pior caso.
*   **Merge Sort:** Não é *in-place*. Exige a alocação dinâmica de $O(n)$ de memória extra para criar os subvetores temporários durante a fase de união (merge).