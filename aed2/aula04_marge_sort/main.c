#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>

#define INF INT_MAX

typedef struct {
    long comparacoes;
    long trocas;
    double tempo;
} Metricas;

// Função auxiliar para embaralhar os vetores
void trocar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// EXERCÍCIO 1: Implementação do Merge Sort
void merge(int A[], int p, int q, int r, Metricas *m) {
    int n1 = q - p + 1;
    int n2 = r - q;

    // Aloca sub-vetores auxiliares
    int *L = (int *)malloc((n1 + 1) * sizeof(int));
    int *R = (int *)malloc((n2 + 1) * sizeof(int));

    // Copia os elementos e contabiliza movimentações
    for (int i = 0; i < n1; i++) { L[i] = A[p + i]; m->trocas++; }
    for (int j = 0; j < n2; j++) { R[j] = A[q + 1 + j]; m->trocas++; }

    // Sentinelas no final
    L[n1] = INF;
    R[n2] = INF;

    int i = 0, j = 0;
    
    // Intercala de volta no vetor original
    for (int k = p; k <= r; k++) {
        m->comparacoes++;
        if (L[i] <= R[j]) {
            A[k] = L[i];
            i++;
        } else {
            A[k] = R[j];
            j++;
        }
        m->trocas++;
    }
    free(L); free(R);
}

void mergeSortRecursivo(int A[], int p, int r, Metricas *m) {
    if (p < r) {
        int q = (p + r) / 2;
        mergeSortRecursivo(A, p, q, m);
        mergeSortRecursivo(A, q + 1, r, m);
        merge(A, p, q, r, m);
    }
}

Metricas runMergeSort(int A[], int n) {
    Metricas m = {0, 0, 0.0};
    clock_t inicio = clock();
    mergeSortRecursivo(A, 0, n - 1, &m);
    m.tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;
    return m;
}

//INSERTION SORT
Metricas runInsertionSort(int A[], int n) {
    Metricas m = {0, 0, 0.0};
    clock_t inicio = clock();

    for (int j = 1; j < n; j++) {
        int chave = A[j];
        int i = j - 1;
        m.trocas++; 

        while (i >= 0) {
            m.comparacoes++;
            if (A[i] > chave) {
                A[i + 1] = A[i];
                m.trocas++; 
                i--;
            } else {
                break;
            }
        }
        A[i + 1] = chave;
        m.trocas++; 
    }

    m.tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;
    return m;
}

//EXERCÍCIO 2: GERADORES DE VETORES
void gerarCrescente(int A[], int n) {
    for (int i = 0; i < n; i++) A[i] = i;
}

void gerarDecrescente(int A[], int n) {
    for (int i = 0; i < n; i++) A[i] = n - i;
}

void gerarAleatorioComRepeticao(int A[], int n) {
    for (int i = 0; i < n; i++) A[i] = rand() % (n / 2 != 0 ? n / 2 : 1);
}

void gerarAleatorioSemRepeticao(int A[], int n) {
    gerarCrescente(A, n);
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        trocar(&A[i], &A[j]);
    }
}

// FUNÇÃO PRINCIPAL
void testarEImprimir(const char* cenario, int *vetorOriginal, int n) {
    int *copia = (int *)malloc(n * sizeof(int));
    Metricas res;

    printf("--- %s ---\n", cenario);

    // Teste: Insertion Sort
    memcpy(copia, vetorOriginal, n * sizeof(int));
    res = runInsertionSort(copia, n);
    printf("Insertion : Comp: %-10ld | Mov: %-10ld | Tempo: %.6f s\n", res.comparacoes, res.trocas, res.tempo);

    // Teste: Merge Sort
    memcpy(copia, vetorOriginal, n * sizeof(int));
    res = runMergeSort(copia, n);
    printf("Merge     : Comp: %-10ld | Mov: %-10ld | Tempo: %.6f s\n\n", res.comparacoes, res.trocas, res.tempo);

    free(copia);
}

int main() {
    int tamanhos[] = {10, 100, 5000}; 
    int numTamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);
    
    srand(time(NULL));

    for (int i = 0; i < numTamanhos; i++) {
        int n = tamanhos[i];
        int *vetorOriginal = (int *)malloc(n * sizeof(int));
        
        printf("TESTES PARA TAMANHO N = %d\n", n);

        gerarCrescente(vetorOriginal, n);
        testarEImprimir("Vetor Crescente", vetorOriginal, n);

        gerarDecrescente(vetorOriginal, n);
        testarEImprimir("Vetor Decrescente", vetorOriginal, n);

        gerarAleatorioComRepeticao(vetorOriginal, n);
        testarEImprimir("Aleatorio COM Repeticao", vetorOriginal, n);

        gerarAleatorioSemRepeticao(vetorOriginal, n);
        testarEImprimir("Aleatorio SEM Repeticao", vetorOriginal, n);

        free(vetorOriginal);
    }

    return 0;
}