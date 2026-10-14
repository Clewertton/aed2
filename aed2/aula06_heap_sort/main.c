#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO 5000

typedef struct {
    int comparacoes;
    int movimentacoes;
} Metricas;

//HEAPFY
void heapify(int *A, int n, int i, Metricas *stats) {
    int maior = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // Verifica o filho à esquerda
    if (left < n) {
        stats->comparacoes++;
        if (A[left] > A[maior]) {
            maior = left;
        }
    }

    // Verifica o filho à direita
    if (right < n) {
        stats->comparacoes++;
        if (A[right] > A[maior]) {
            maior = right;
        }
    }

    // Se o maior não for a raiz, troca e chama recursivamente
    if (maior != i) {
        int temp = A[i];
        A[i] = A[maior];
        A[maior] = temp;
        stats->movimentacoes++;
        
        heapify(A, n, maior, stats);
    }
}

//HEAPSORT
Metricas heap_sort(int *A, int n) {
    Metricas stats = {0, 0};

    // BUILD-HEAP: Constrói o max-heap de baixo para cima
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(A, n, i, &stats);
    }

    // Extrai os elementos um a um do heap
    for (int i = n - 1; i > 0; i--) {
        int temp = A[0];
        A[0] = A[i];
        A[i] = temp;
        stats.movimentacoes++;

        heapify(A, i, 0, &stats);
    }

    return stats;
}

// Função auxiliar para imprimir o vetor
void imprimir_vetor(int *A, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

void gerarCrescente(int A[], int n) {
    for (int i = 0; i < n; i++) {
        A[i] = i;
    }
}

void gerarDecrescente(int A[], int n) {
    for (int i = 0; i < n; i++) {
        A[i] = n - i;
    }
}

void gerarAleatorioComRepeticao(int A[], int n) {
    for (int i = 0; i < n; i++) {
        A[i] = rand() % n;
    }
}

void gerarAleatorioSemRepeticao(int A[], int n) {
    gerarCrescente(A, n);

    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = A[i];
        A[i] = A[j];
        A[j] = temp;
    }
}

void testarCenario(const char *nome, int A[], int n) {
    Metricas resultado = heap_sort(A, n);

    printf("%s:\n", nome);
    printf("   Comparacoes: %d | Trocas: %d\n\n",
           resultado.comparacoes, resultado.movimentacoes);
}

int main(void) {
    int vetor[TAMANHO];

    srand((unsigned int) time(NULL));
    printf("--- AVALIACAO HEAP SORT (N = %d) ---\n\n", TAMANHO);

    gerarCrescente(vetor, TAMANHO);
    testarCenario("A) Vetor crescente", vetor, TAMANHO);

    gerarDecrescente(vetor, TAMANHO);
    testarCenario("B) Vetor decrescente", vetor, TAMANHO);

    gerarAleatorioComRepeticao(vetor, TAMANHO);
    testarCenario("C) Aleatorio com repeticao", vetor, TAMANHO);

    gerarAleatorioSemRepeticao(vetor, TAMANHO);
    testarCenario("D) Aleatorio sem repeticao", vetor, TAMANHO);

    return 0;
}