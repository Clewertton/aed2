#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

//inserindo no final da lista
NoLista* inserirLSE(NoLista* inicio, Funcionario f) {
    NoLista* novo = (NoLista*)malloc(sizeof(NoLista));
    novo->func = f;
    novo->prox = NULL;

    if (inicio == NULL) {
        return novo; // se a lista estava vazia
    }

    NoLista* aux = inicio;
    while (aux->prox != NULL) {
        aux = aux->prox; // fazendo o caminho até o último nó
    }
    aux->prox = novo;
    return inicio;
}

// listando todos
void listarLSE(NoLista* inicio) {
    NoLista* aux = inicio;
    while (aux != NULL) {
        printf("Mat: %d | Nome: %s | Idade: %d | Salario: %.2f\n", 
               aux->func.matricula, aux->func.nome, aux->func.idade, aux->func.salario);
        aux = aux->prox;
    }
}

// fazwndo a busca por matrícula
void buscarLSE(NoLista* inicio, int mat) {
    NoLista* aux = inicio;
    while (aux != NULL) {
        if (aux->func.matricula == mat) {
            printf("Encontrado na Lista: %s\n", aux->func.nome);
            return;
        }
        aux = aux->prox;
    }
    printf("Funcionario nao encontrado na Lista.\n");
}

//remoção por matrícula
NoLista* removerLSE(NoLista* inicio, int mat) {
    NoLista *atual = inicio, *anterior = NULL;

    while (atual != NULL && atual->func.matricula != mat) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) return inicio; // se não encontrou

    if (anterior == NULL) {
        //o elemento que vamos remover é o primeiro
        inicio = atual->prox;
    } else {
        // o elemento está no meio ou no fim
        anterior->prox = atual->prox;
    }
    free(atual);
    printf("Removido da Lista com sucesso.\n");
    return inicio;
}

//exibir a quantidade de funcionários cadastrados
int contarLSE(NoLista* inicio) {
    int cont = 0;
    NoLista* aux = inicio;
    while (aux != NULL) {
        cont++;
        aux = aux->prox;
    }
    return cont;
}

// mostrar o funcionário com o maior salário
void exibirMaiorSalarioLSE(NoLista* inicio) {
    if (inicio == NULL) {
        printf("Lista vazia.\n");
        return;
    }
    
    NoLista* aux = inicio;
    Funcionario maior = inicio->func;

    while (aux != NULL) {
        if (aux->func.salario > maior.salario) {
            maior = aux->func;
        }
        aux = aux->prox;
    }
    printf("Maior salario na Lista: %s (R$ %.2f)\n", maior.nome, maior.salario);
}