#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO 5000

typedef struct {
    long comparacoes;
    long trocas;
} Metricas;

// EXERCÍCIO 1: Implementacao do Insertion Sort
Metricas insertionSort(int A[], int n) {
    Metricas m = {0, 0};
    int i, j, chave;

    for (j = 1; j < n; j++) {
        chave = A[j];
        i = j - 1;

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
    }

    return m;
}
// EXERCÍCIO 2: Geradores de vetores para os testes
void gerarMelhorCaso(int A[], int n) {
    for (int i = 0; i < n; i++) {
        A[i] = i;
    }
}

void gerarPiorCaso(int A[], int n) {
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
    gerarMelhorCaso(A, n);
    // Embaralha o vetor
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = A[i];
        A[i] = A[j];
        A[j] = temp;
    }
}

// Funcao Principal
int main() {
    int vetor[TAMANHO];
    Metricas resultado;
    
    srand(time(NULL));

    printf("--- AVALIACAO INSERTION SORT (N = %d) ---\n\n", TAMANHO);

    // a) Melhor caso
    gerarMelhorCaso(vetor, TAMANHO);
    resultado = insertionSort(vetor, TAMANHO);
    printf("A) Melhor Caso (Ordenado):\n");
    printf("   Comparacoes: %ld | Deslocamentos: %ld\n\n", resultado.comparacoes, resultado.trocas);

    // b) Pior caso
    gerarPiorCaso(vetor, TAMANHO);
    resultado = insertionSort(vetor, TAMANHO);
    printf("B) Pior Caso (Ordem Inversa):\n");
    printf("   Comparacoes: %ld | Deslocamentos: %ld\n\n", resultado.comparacoes, resultado.trocas);

    // c) Aleatorio com repeticao
    gerarAleatorioComRepeticao(vetor, TAMANHO);
    resultado = insertionSort(vetor, TAMANHO);
    printf("C) Aleatorio COM Repeticao:\n");
    printf("   Comparacoes: %ld | Deslocamentos: %ld\n\n", resultado.comparacoes, resultado.trocas);

    // d) Aleatorio sem repeticao
    gerarAleatorioSemRepeticao(vetor, TAMANHO);
    resultado = insertionSort(vetor, TAMANHO);
    printf("D) Aleatorio SEM Repeticao:\n");
    printf("   Comparacoes: %ld | Deslocamentos: %ld\n\n", resultado.comparacoes, resultado.trocas);

    return 0;
}