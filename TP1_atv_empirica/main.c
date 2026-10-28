#ifndef _WIN32
#define _POSIX_C_SOURCE 199309L  // clock_gettime
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

typedef struct {
    unsigned long long comparacoes;
    unsigned long long movimentacoes;
    double tempo_ms;
} Metricas;

typedef int (*AlgoritmoOrdenacao)(int *, int, Metricas *);

// Sementes fixas para que os experimentos sejam reprodutiveis.
#define SEMENTE_DADOS UINT64_C(20261021)
#define SEMENTE_ALGORITMO UINT64_C(12345)


// 1. ESTRUTURA DE METRICAS E FUNCOES AUXILIARES
static void reset_metricas(Metricas *m) {
    m->comparacoes = 0;
    m->movimentacoes = 0;
    m->tempo_ms = 0.0;
}

static double obter_tempo_ms(void) {
#ifdef _WIN32
    LARGE_INTEGER frequencia;
    LARGE_INTEGER contador;
    if (!QueryPerformanceFrequency(&frequencia) ||
        !QueryPerformanceCounter(&contador)) {
        return -1.0;
    }
    return (double)contador.QuadPart * 1000.0 /
           (double)frequencia.QuadPart;
#else
    // Relogio monotonico: nao sofre ajustes de data/hora do sistema.
    struct timespec agora;
    if (clock_gettime(CLOCK_MONOTONIC, &agora) != 0) {
        return -1.0;
    }
    return (double)agora.tv_sec * 1000.0 +
           (double)agora.tv_nsec / 1000000.0;
#endif
}

// Gerador pseudoaleatorio proprio (xorshift64*). Nao se usa rand() porque no
// Windows RAND_MAX = 32767, o que limitaria os valores gerados, o embaralhamento
// e a escolha do pivo do Quick Sort a 32.768 posicoes/valores distintos.
static uint64_t estadoPrng = UINT64_C(88172645463325252);

static void semente_prng(uint64_t semente) {
    estadoPrng = semente != 0 ? semente : UINT64_C(88172645463325252);
}

static uint64_t proximo_prng(void) {
    uint64_t x = estadoPrng;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    estadoPrng = x;
    return x * UINT64_C(2685821657736338717);
}

// Inteiro em [0, limite), limite > 0.
static int aleatorio_ate(int limite) {
    return (int)((proximo_prng() >> 11) % (uint64_t)limite);
}

static void trocar(int *a, int *b, Metricas *m) {
    if (a == b) {
        return;
    }

    int temp = *a;
    *a = *b;
    *b = temp;
    if (m != NULL) {
        m->movimentacoes += 3;
    }
}

static int comparar_ints(const void *a, const void *b) {
    int valorA = *(const int *)a;
    int valorB = *(const int *)b;
    return (valorA > valorB) - (valorA < valorB);
}

static int alocar_vetor(int **vetor, size_t quantidade) {
    if (quantidade > SIZE_MAX / sizeof(**vetor)) {
        return 0;
    }

    *vetor = malloc(quantidade * sizeof(**vetor));
    return *vetor != NULL;
}

// 2. GERADORES DE CENARIOS
static int gerar_ordenado(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
    return 1;
}

static int gerar_inversamente_ordenado(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = n - 1 - i;
    }
    return 1;
}

static int gerar_aleatorio_com_repeticao(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = aleatorio_ate(n / 2 + 1);
    }
    return 1;
}

static int gerar_aleatorio_sem_repeticao(int *arr, int n) {
    gerar_ordenado(arr, n);
    for (int i = n - 1; i > 0; i--) {
        int j = aleatorio_ate(i + 1);
        trocar(&arr[i], &arr[j], NULL);
    }
    return 1;
}

// 3. ALGORITMOS DE ORDENACAO

// a. Selection Sort
static int selection_sort(int *arr, int n, Metricas *m) {
    for (int i = 0; i < n - 1; i++) {
        int indiceMinimo = i;
        for (int j = i + 1; j < n; j++) {
            m->comparacoes++;
            if (arr[j] < arr[indiceMinimo]) {
                indiceMinimo = j;
            }
        }
        trocar(&arr[i], &arr[indiceMinimo], m);
    }
    return 1;
}

// b. Merge Sort (um unico vetor auxiliar alocado uma vez, fora das recursoes,
// para que o tempo medido nao inclua milhares de malloc/free).
static void merge(int *arr, int *aux, int inicio, int meio, int fim,
                  Metricas *m) {
    for (int i = inicio; i <= fim; i++) {
        aux[i] = arr[i];
        m->movimentacoes++;
    }

    int i = inicio;
    int j = meio + 1;
    int k = inicio;
    while (i <= meio && j <= fim) {
        m->comparacoes++;
        if (aux[i] <= aux[j]) {
            arr[k++] = aux[i++];
        } else {
            arr[k++] = aux[j++];
        }
        m->movimentacoes++;
    }
    while (i <= meio) {
        arr[k++] = aux[i++];
        m->movimentacoes++;
    }
    while (j <= fim) {
        arr[k++] = aux[j++];
        m->movimentacoes++;
    }
}

static void merge_sort_recursivo(int *arr, int *aux, int inicio, int fim,
                                 Metricas *m) {
    if (inicio >= fim) {
        return;
    }

    int meio = inicio + (fim - inicio) / 2;
    merge_sort_recursivo(arr, aux, inicio, meio, m);
    merge_sort_recursivo(arr, aux, meio + 1, fim, m);
    merge(arr, aux, inicio, meio, fim, m);
}

static int merge_sort(int *arr, int n, Metricas *m) {
    if (n <= 1) {
        return 1;
    }

    int *aux = NULL;
    if (!alocar_vetor(&aux, (size_t)n)) {
        return 0;
    }
    merge_sort_recursivo(arr, aux, 0, n - 1, m);
    free(aux);
    return 1;
}

// c. Quick Sort aleatorizado de três partições. Processa imediatamente a
// partição maior e empilha a menor, limitando a pilha auxiliar a O(log n).
static int quick_sort(int *arr, int n, Metricas *m) {
    if (n <= 1) {
        return 1;
    }

    typedef struct {
        int inicio;
        int fim;
    } Intervalo;

    size_t capacidadePilha = (size_t)n;
    if (capacidadePilha > SIZE_MAX / sizeof(Intervalo)) {
        return 0;
    }
    Intervalo *pilha = malloc(capacidadePilha * sizeof(*pilha));
    if (pilha == NULL) {
        return 0;
    }

    size_t topo = 0;
    pilha[topo++] = (Intervalo){0, n - 1};
    while (topo > 0) {
        Intervalo intervalo = pilha[--topo];
        int inicio = intervalo.inicio;
        int fim = intervalo.fim;

        while (inicio < fim) {
            int indicePivo = inicio + aleatorio_ate(fim - inicio + 1);
            int pivo = arr[indicePivo];
            m->movimentacoes++;
            int menor = inicio;
            int atual = inicio;
            int maior = fim;

            while (atual <= maior) {
                m->comparacoes++;
                if (arr[atual] < pivo) {
                    trocar(&arr[atual], &arr[menor], m);
                    atual++;
                    menor++;
                } else {
                    m->comparacoes++;
                    if (arr[atual] > pivo) {
                        trocar(&arr[atual], &arr[maior], m);
                        maior--;
                    } else {
                        atual++;
                    }
                }
            }

            int tamanhoEsquerda = menor - inicio;
            int tamanhoDireita = fim - maior;
            if (tamanhoEsquerda < tamanhoDireita) {
                if (inicio < menor - 1) {
                    pilha[topo++] = (Intervalo){inicio, menor - 1};
                }
                inicio = maior + 1;
            } else {
                if (maior + 1 < fim) {
                    pilha[topo++] = (Intervalo){maior + 1, fim};
                }
                fim = menor - 1;
            }
        }
    }

    free(pilha);
    return 1;
}

// d. Heap Sort
static void heapify(int *arr, int n, int raiz, Metricas *m) {
    for (;;) {
        int maior = raiz;
        int esquerda = 2 * raiz + 1;
        int direita = esquerda + 1;

        if (esquerda < n) {
            m->comparacoes++;
            if (arr[esquerda] > arr[maior]) {
                maior = esquerda;
            }
        }
        if (direita < n) {
            m->comparacoes++;
            if (arr[direita] > arr[maior]) {
                maior = direita;
            }
        }
        if (maior == raiz) {
            return;
        }

        trocar(&arr[raiz], &arr[maior], m);
        raiz = maior;
    }
}

static int heap_sort(int *arr, int n, Metricas *m) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i, m);
    }
    for (int i = n - 1; i > 0; i--) {
        trocar(&arr[0], &arr[i], m);
        heapify(arr, i, 0, m);
    }
    return 1;
}

// e. Counting Sort
static int counting_sort(int *arr, int n, Metricas *m) {
    if (n <= 1) {
        return 1;
    }

    int minimo = arr[0];
    int maximo = arr[0];
    for (int i = 1; i < n; i++) {
        m->comparacoes++;
        if (arr[i] < minimo) {
            minimo = arr[i];
        }
        m->comparacoes++;
        if (arr[i] > maximo) {
            maximo = arr[i];
        }
    }

    uint64_t intervalo =
        (uint64_t)((int64_t)maximo - (int64_t)minimo) + UINT64_C(1);
    if (intervalo > SIZE_MAX / sizeof(size_t) ||
        (size_t)n > SIZE_MAX / sizeof(*arr)) {
        return 0;
    }

    size_t *contagens = calloc((size_t)intervalo, sizeof(*contagens));
    int *saida = NULL;
    if (contagens == NULL || !alocar_vetor(&saida, (size_t)n)) {
        free(contagens);
        free(saida);
        return 0;
    }

    for (int i = 0; i < n; i++) {
        size_t indice = (size_t)((int64_t)arr[i] - (int64_t)minimo);
        contagens[indice]++;
    }
    for (size_t i = 1; i < (size_t)intervalo; i++) {
        contagens[i] += contagens[i - 1];
    }
    for (int i = n - 1; i >= 0; i--) {
        size_t indice = (size_t)((int64_t)arr[i] - (int64_t)minimo);
        saida[contagens[indice] - 1] = arr[i];
        contagens[indice]--;
        m->movimentacoes++;
    }
    for (int i = 0; i < n; i++) {
        arr[i] = saida[i];
        m->movimentacoes++;
    }

    free(contagens);
    free(saida);
    return 1;
}

// f. Bucket Sort
typedef struct {
    int *valores;
    size_t tamanho;
    size_t capacidade;
} Balde;

static int adicionar_ao_balde(Balde *balde, int valor) {
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

static void ordenar_balde(Balde *balde, Metricas *m) {
    for (size_t i = 1; i < balde->tamanho; i++) {
        int chave = balde->valores[i];
        size_t j = i;
        m->movimentacoes++;

        while (j > 0) {
            m->comparacoes++;
            if (balde->valores[j - 1] > chave) {
                balde->valores[j] = balde->valores[j - 1];
                m->movimentacoes++;
                j--;
            } else {
                break;
            }
        }
        balde->valores[j] = chave;
        m->movimentacoes++;
    }
}

static int bucket_sort(int *arr, int n, Metricas *m) {
    if (n <= 1) {
        return 1;
    }
    if ((size_t)n > SIZE_MAX / sizeof(Balde)) {
        return 0;
    }

    int minimo = arr[0];
    int maximo = arr[0];
    for (int i = 1; i < n; i++) {
        m->comparacoes++;
        if (arr[i] < minimo) {
            minimo = arr[i];
        }
        m->comparacoes++;
        if (arr[i] > maximo) {
            maximo = arr[i];
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
            (uint64_t)((int64_t)arr[i] - (int64_t)minimo);
        size_t indice =
            (size_t)((deslocamento * (uint64_t)n) / intervalo);
        if (indice >= (size_t)n) {
            indice = (size_t)n - 1;
        }
        if (!adicionar_ao_balde(&baldes[indice], arr[i])) {
            for (int j = 0; j < n; j++) {
                free(baldes[j].valores);
            }
            free(baldes);
            return 0;
        }
        m->movimentacoes++;
    }

    int indiceSaida = 0;
    for (int i = 0; i < n; i++) {
        ordenar_balde(&baldes[i], m);
        for (size_t j = 0; j < baldes[i].tamanho; j++) {
            arr[indiceSaida++] = baldes[i].valores[j];
            m->movimentacoes++;
        }
        free(baldes[i].valores);
    }
    free(baldes);
    return 1;
}

// 4. MOTOR DE TESTES
// Numero de repeticoes por tamanho: n pequeno termina em microssegundos e uma
// unica medicao seria ruidosa; n grande e lento, entao repete-se menos.
// Comparacoes e movimentacoes sao identicas entre repeticoes (mesma entrada e
// mesma semente do PRNG); apenas o tempo e promediado.
static int repeticoes_para(int n) {
    if (n <= 1000) {
        return 20;
    }
    if (n <= 10000) {
        return 5;
    }
    return 3;
}

static int testar_algoritmo(AlgoritmoOrdenacao algoritmo, const int *arr,
                            int n, const char *nome, const char *cenario,
                            FILE *arquivoCsv) {
    int *copia = NULL;
    int *esperado = NULL;
    int resultado = 0;
    if (!alocar_vetor(&copia, (size_t)n) ||
        !alocar_vetor(&esperado, (size_t)n)) {
        fprintf(stderr, "Erro: memoria insuficiente para validar %s, %d.\n",
                nome, n);
        goto fim;
    }

    for (int i = 0; i < n; i++) {
        esperado[i] = arr[i];
    }
    qsort(esperado, (size_t)n, sizeof(*esperado), comparar_ints);

    const int repeticoes = repeticoes_para(n);
    Metricas m;
    reset_metricas(&m);
    double tempoTotal = 0.0;

    for (int r = 0; r < repeticoes; r++) {
        for (int i = 0; i < n; i++) {
            copia[i] = arr[i];
        }

        Metricas execucao;
        reset_metricas(&execucao);
        semente_prng(SEMENTE_ALGORITMO);

        double inicio = obter_tempo_ms();
        if (inicio < 0.0) {
            fprintf(stderr,
                    "Erro: nao foi possivel iniciar a medicao de %s.\n", nome);
            goto fim;
        }
        int sucesso = algoritmo(copia, n, &execucao);
        double tempoFim = obter_tempo_ms();
        if (tempoFim < inicio) {
            fprintf(stderr, "Erro: nao foi possivel medir o tempo de %s.\n",
                    nome);
            goto fim;
        }
        if (!sucesso) {
            fprintf(stderr, "Erro: falha ao executar %s no cenario %s, n=%d.\n",
                    nome, cenario, n);
            goto fim;
        }

        for (int i = 0; i < n; i++) {
            if (copia[i] != esperado[i]) {
                fprintf(stderr,
                        "Erro: %s produziu resultado incorreto (%s, n=%d).\n",
                        nome, cenario, n);
                goto fim;
            }
        }

        tempoTotal += tempoFim - inicio;
        m.comparacoes = execucao.comparacoes;
        m.movimentacoes = execucao.movimentacoes;
    }
    m.tempo_ms = tempoTotal / (double)repeticoes;

    if (fprintf(arquivoCsv, "%s;%s;%d;%llu;%llu;%.6f;%d\n", nome, cenario, n,
                m.comparacoes, m.movimentacoes, m.tempo_ms, repeticoes) < 0) {
        fprintf(stderr, "Erro: nao foi possivel gravar os resultados no CSV.\n");
        goto fim;
    }
    printf("%s;%s;%d;%llu;%llu;%.6f;%d\n", nome, cenario, n, m.comparacoes,
           m.movimentacoes, m.tempo_ms, repeticoes);
    resultado = 1;

fim:
    free(copia);
    free(esperado);
    return resultado;
}

static int executar_cenario(int *arr, int n, const char *cenario,
                            int (*gerador)(int *, int),
                            const AlgoritmoOrdenacao algoritmos[],
                            const char *nomes[], int quantidadeAlgoritmos,
                            FILE *arquivoCsv) {
    semente_prng(SEMENTE_DADOS);
    if (!gerador(arr, n)) {
        fprintf(stderr, "Erro: nao foi possivel gerar o cenario %s, n=%d.\n",
                cenario, n);
        return 0;
    }
    for (int i = 0; i < quantidadeAlgoritmos; i++) {
        if (!testar_algoritmo(algoritmos[i], arr, n, nomes[i], cenario,
                              arquivoCsv)) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    // Sementes fixas: os mesmos dados e pivos em toda execucao (reprodutivel).

    FILE *arquivoCsv = fopen("resultados.csv", "w");
    if (arquivoCsv == NULL) {
        perror("Erro ao criar resultados.csv");
        return 1;
    }

    const int tamanhos[] = {1000, 10000, 100000};
    const int quantidadeTamanhos =
        (int)(sizeof(tamanhos) / sizeof(tamanhos[0]));
    const AlgoritmoOrdenacao algoritmos[] = {
        selection_sort, merge_sort, quick_sort,
        heap_sort, counting_sort, bucket_sort
    };
    const char *nomesAlgoritmos[] = {
        "Selection Sort", "Merge Sort", "Quick Sort",
        "Heap Sort", "Counting Sort", "Bucket Sort"
    };
    const int quantidadeAlgoritmos =
        (int)(sizeof(algoritmos) / sizeof(algoritmos[0]));

    const char *cabecalho =
        "Algoritmo;Cenario;Tamanho;Comparacoes;Movimentacoes;Tempo_ms;Repeticoes\n";
    if (fputs(cabecalho, arquivoCsv) == EOF) {
        fprintf(stderr, "Erro: nao foi possivel gravar o cabecalho do CSV.\n");
        fclose(arquivoCsv);
        return 1;
    }
    printf("%s", cabecalho);

    for (int t = 0; t < quantidadeTamanhos; t++) {
        int n = tamanhos[t];
        int *arrBase = NULL;
        if (!alocar_vetor(&arrBase, (size_t)n)) {
            fprintf(stderr, "Erro: memoria insuficiente para n=%d.\n", n);
            fclose(arquivoCsv);
            return 1;
        }

        int (*geradores[])(int *, int) = {
            gerar_ordenado,
            gerar_inversamente_ordenado,
            gerar_aleatorio_sem_repeticao,
            gerar_aleatorio_com_repeticao
        };
        const char *nomesCenarios[] = {
            "Ordenado", "Inverso", "Aleatorio_Sem_Rep", "Aleatorio_Com_Rep"
        };

        // Bateria 1: Ordenado
        // Bateria 2: Inverso
        // Bateria 3: Aleatorio sem repeticao
        // Bateria 4: Aleatorio com repeticao
        // Executa cada algoritmo em cada cenario para o tamanho atual.
        for (size_t c = 0;
             c < sizeof(geradores) / sizeof(geradores[0]); c++) {
            if (!executar_cenario(arrBase, n, nomesCenarios[c], geradores[c],
                                  algoritmos, nomesAlgoritmos,
                                  quantidadeAlgoritmos, arquivoCsv)) {
                free(arrBase);
                fclose(arquivoCsv);
                return 1;
            }
        }
        free(arrBase);
    }

    if (fclose(arquivoCsv) != 0) {
        perror("Erro ao finalizar resultados.csv");
        return 1;
    }
    printf("\nResultados gravados em resultados.csv\n");
    return 0;
}