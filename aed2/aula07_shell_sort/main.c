#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO 5000

typedef struct {
    long comparacoes;
    long movimentacoes;
} Metricas;

Metricas runShellSort(int A[], int n) {
    Metricas m = {0, 0};
    int gap = n / 2;

    while (gap > 0) {
        for (int j = gap; j < n; j++) {
            int chave = A[j];
            m.movimentacoes++;

            int i = j - gap;
            while (i >= 0) {
                m.comparacoes++;
                if (A[i] > chave) {
                    A[i + gap] = A[i];
                    m.movimentacoes++;
                    i -= gap;
                } else {
                    break;
                }
            }

            A[i + gap] = chave;
            m.movimentacoes++;
        }
        gap /= 2;
    }

    return m;
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

int testarCenario(const char *nome, int A[], int n) {
    Metricas resultado = runShellSort(A, n);

    for (int i = 1; i < n; i++) {
        if (A[i - 1] > A[i]) {
            fprintf(stderr, "Erro: o vetor do cenario \"%s\" nao foi ordenado.\n",
                    nome);
            return 0;
        }
    }

    printf("%s:\n", nome);
    printf("   Comparacoes: %ld | Movimentacoes: %ld\n\n",
           resultado.comparacoes, resultado.movimentacoes);
    return 1;
}

int main(void) {
    int vetor[TAMANHO];

    srand((unsigned int) time(NULL));
    printf("--- AVALIACAO SHELL SORT (N = %d) ---\n\n", TAMANHO);

    gerarCrescente(vetor, TAMANHO);
    if (!testarCenario("A) Vetor crescente", vetor, TAMANHO)) {
        return 1;
    }

    gerarDecrescente(vetor, TAMANHO);
    if (!testarCenario("B) Vetor decrescente", vetor, TAMANHO)) {
        return 1;
    }

    gerarAleatorioComRepeticao(vetor, TAMANHO);
    if (!testarCenario("C) Aleatorio com repeticao", vetor, TAMANHO)) {
        return 1;
    }

    gerarAleatorioSemRepeticao(vetor, TAMANHO);
    if (!testarCenario("D) Aleatorio sem repeticao", vetor, TAMANHO)) {
        return 1;
    }

    return 0;
}
