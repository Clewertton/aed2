#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO 5000

//rmazenar as métricas pedidas
typedef struct {
    long long comparacoes;
    long long trocas;
} Metricas;

// EXERCÍCIO 1: Função Selection Sort com contagem de métricas
Metricas selectionSort(int A[], int n) {
    Metricas metricas = {0, 0};
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        
        //procura o menor valor na parte não ordenada
        for (int j = i + 1; j < n; j++) {
            metricas.comparacoes++;
            if (A[j] < A[min]) {
                min = j;
            }
        }
        
        //troca os elementos se o menor não estiver na posição atual
        if (min != i) {
            int temp = A[i];
            A[i] = A[min];
            A[min] = temp;
            metricas.trocas++;
        }
    }
    return metricas;
}

// EXERCÍCIO 2: Funções para gerar os cenários de teste
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
    // Primeiro preenche ordenado (sem repetição)
    gerarMelhorCaso(A, n);
    // Depois embaralha (Fisher-Yates Shuffle)
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = A[i];
        A[i] = A[j];
        A[j] = temp;
    }
}

// Função principal da avaliação
int main() {
    int vetor[TAMANHO];
    Metricas resultado;
    
    srand(time(NULL)); // Inicializa a semente de números aleatórios

    printf("--- AVALIACAO SELECTION SORT (N = %d) ---\n\n", TAMANHO);

    // a) Melhor caso
    gerarMelhorCaso(vetor, TAMANHO);
    resultado = selectionSort(vetor, TAMANHO);
    printf("A) Melhor Caso (Ordenado):\n");
    printf("   Comparacoes: %lld | Trocas: %lld\n\n", resultado.comparacoes, resultado.trocas);

    // b) Pior caso
    gerarPiorCaso(vetor, TAMANHO);
    resultado = selectionSort(vetor, TAMANHO);
    printf("B) Pior Caso (Ordem Inversa):\n");
    printf("   Comparacoes: %lld | Trocas: %lld\n\n", resultado.comparacoes, resultado.trocas);

    // c) Aleatório com repetição
    gerarAleatorioComRepeticao(vetor, TAMANHO);
    resultado = selectionSort(vetor, TAMANHO);
    printf("C) Aleatorio COM Repeticao:\n");
    printf("   Comparacoes: %lld | Trocas: %lld\n\n", resultado.comparacoes, resultado.trocas);

    // d) Aleatório sem repetição
    gerarAleatorioSemRepeticao(vetor, TAMANHO);
    resultado = selectionSort(vetor, TAMANHO);
    printf("D) Aleatorio SEM Repeticao:\n");
    printf("   Comparacoes: %lld | Trocas: %lld\n\n", resultado.comparacoes, resultado.trocas);

    return 0;
}