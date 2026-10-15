#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define TAMANHO 5000

typedef struct {
    long comparacoes;
    long movimentacoes;
} Metricas;

int runCountingSort(int A[], int n, Metricas *m) {
    *m = (Metricas){0, 0};
    if (n < 0 || (n > 0 && A == NULL)) {
        return 0;
    }
    if (n <= 1) {
        return 1;
    }

    int minimo = A[0];
    int maximo = A[0];
    for (int i = 1; i < n; i++) {
        m->comparacoes++;
        if (A[i] < minimo) {
            minimo = A[i];
        }

        m->comparacoes++;
        if (A[i] > maximo) {
            maximo = A[i];
        }
    }

    unsigned long long range =
        (unsigned long long)((long long)maximo - (long long)minimo) + 1;
    if (range > (unsigned long long)(SIZE_MAX / sizeof(size_t)) ||
        (size_t)n > SIZE_MAX / sizeof(int)) {
        return 0;
    }

    size_t *contagens = calloc((size_t)range, sizeof(*contagens));
    int *saida = malloc((size_t)n * sizeof(*saida));
    if (contagens == NULL || saida == NULL) {
        free(contagens);
        free(saida);
        return 0;
    }

    for (int i = 0; i < n; i++) {
        size_t indice = (size_t)((long long)A[i] - (long long)minimo);
        contagens[indice]++;
    }

    for (size_t i = 1; i < (size_t)range; i++) {
        contagens[i] += contagens[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        size_t indice = (size_t)((long long)A[i] - (long long)minimo);
        saida[contagens[indice] - 1] = A[i];
        contagens[indice]--;
        m->movimentacoes++;
    }

    for (int i = 0; i < n; i++) {
        A[i] = saida[i];
        m->movimentacoes++;
    }

    free(contagens);
    free(saida);
    return 1;
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
    Metricas resultado;
    if (!runCountingSort(A, n, &resultado)) {
        fprintf(stderr,
                "Erro: nao foi possivel executar Counting Sort no cenario \"%s\".\n",
                nome);
        return 0;
    }

    for (int i = 1; i < n; i++) {
        if (A[i - 1] > A[i]) {
            fprintf(stderr,
                    "Erro: o vetor do cenario \"%s\" nao foi ordenado.\n",
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

    srand((unsigned int)time(NULL));
    printf("--- AVALIACAO COUNTING SORT (N = %d) ---\n\n", TAMANHO);

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
