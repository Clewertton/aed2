#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define TAMANHO 5000

typedef struct {
    long long comparacoes;
    long long movimentacoes;
} Metricas;

typedef struct {
    int *valores;
    size_t tamanho;
    size_t capacidade;
} Balde;

static void liberarBaldes(Balde baldes[], size_t quantidade) {
    for (size_t i = 0; i < quantidade; i++) {
        free(baldes[i].valores);
    }
    free(baldes);
}

static int adicionarAoBalde(Balde *balde, int valor) {
    if (balde->tamanho == balde->capacidade) {
        size_t novaCapacidade = balde->capacidade == 0
                                    ? 4
                                    : balde->capacidade * 2;
        if (novaCapacidade < balde->capacidade ||
            novaCapacidade > SIZE_MAX / sizeof(*balde->valores)) {
            return 0;
        }

        int *novosValores =
            realloc(balde->valores, novaCapacidade * sizeof(*balde->valores));
        if (novosValores == NULL) {
            return 0;
        }

        balde->valores = novosValores;
        balde->capacidade = novaCapacidade;
    }

    balde->valores[balde->tamanho++] = valor;
    return 1;
}

static void insertionSort(Balde *balde, Metricas *metricas) {
    for (size_t i = 1; i < balde->tamanho; i++) {
        int chave = balde->valores[i];
        size_t j = i;
        metricas->movimentacoes++;

        while (j > 0) {
            metricas->comparacoes++;
            if (balde->valores[j - 1] > chave) {
                balde->valores[j] = balde->valores[j - 1];
                metricas->movimentacoes++;
                j--;
            } else {
                break;
            }
        }

        balde->valores[j] = chave;
        metricas->movimentacoes++;
    }
}

int runBucketSort(int A[], int n, Metricas *metricas) {
    if (metricas == NULL) {
        return 0;
    }
    *metricas = (Metricas){0, 0};

    if (n < 0 || (n > 0 && A == NULL)) {
        return 0;
    }
    if (n <= 1) {
        return 1;
    }
    if ((size_t)n > SIZE_MAX / sizeof(Balde)) {
        return 0;
    }

    int minimo = A[0];
    int maximo = A[0];
    for (int i = 1; i < n; i++) {
        metricas->comparacoes++;
        if (A[i] < minimo) {
            minimo = A[i];
        }

        metricas->comparacoes++;
        if (A[i] > maximo) {
            maximo = A[i];
        }
    }

    uint64_t intervalo =
        (uint64_t)((int64_t)maximo - (int64_t)minimo) + UINT64_C(1);
    Balde *baldes = calloc((size_t)n, sizeof(*baldes));
    if (baldes == NULL) {
        return 0;
    }

    for (int i = 0; i < n; i++) {
        uint64_t deslocamento =
            (uint64_t)((int64_t)A[i] - (int64_t)minimo);
        size_t indice =
            (size_t)((deslocamento * (uint64_t)n) / intervalo);
        if (indice >= (size_t)n) {
            indice = (size_t)n - 1;
        }

        if (!adicionarAoBalde(&baldes[indice], A[i])) {
            liberarBaldes(baldes, (size_t)n);
            return 0;
        }
        metricas->movimentacoes++;
    }

    int indiceSaida = 0;
    for (int i = 0; i < n; i++) {
        insertionSort(&baldes[i], metricas);
        for (size_t j = 0; j < baldes[i].tamanho; j++) {
            A[indiceSaida++] = baldes[i].valores[j];
            metricas->movimentacoes++;
        }
    }

    liberarBaldes(baldes, (size_t)n);
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

void gerarAleatorioComNegativos(int A[], int n) {
    for (int i = 0; i < n; i++) {
        A[i] = rand() % n - n / 2;
    }
}

int testarCenario(const char *nome, int A[], int n) {
    Metricas resultado;
    if (!runBucketSort(A, n, &resultado)) {
        fprintf(stderr,
                "Erro: nao foi possivel executar Bucket Sort no cenario \"%s\".\n",
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
    printf("   Comparacoes: %lld | Movimentacoes: %lld\n\n",
           resultado.comparacoes, resultado.movimentacoes);
    return 1;
}

int main(void) {
    int vetor[TAMANHO];

    srand((unsigned int)time(NULL));
    printf("--- AVALIACAO BUCKET SORT (N = %d) ---\n\n", TAMANHO);

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

    gerarAleatorioComNegativos(vetor, TAMANHO);
    if (!testarCenario("E) Aleatorio com valores negativos", vetor, TAMANHO)) {
        return 1;
    }

    return 0;
}
