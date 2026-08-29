#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

//inserindo na árvore
NoArvore* inserirABB(NoArvore* raiz, Funcionario f) {
    if (raiz == NULL) {
        NoArvore* novo = (NoArvore*)malloc(sizeof(NoArvore));
        novo->func = f;
        novo->esq = NULL;
        novo->dir = NULL;
        return novo;
    }
    if (f.matricula < raiz->func.matricula) {
        raiz->esq = inserirABB(raiz->esq, f);
    } else if (f.matricula > raiz->func.matricula) {
        raiz->dir = inserirABB(raiz->dir, f);
    }
    return raiz;
}

//In-Order (Crescente)
void exibirInOrder(NoArvore* raiz) {
    if (raiz != NULL) {
        exibirInOrder(raiz->esq);
        printf("Mat: %d | Nome: %s\n", raiz->func.matricula, raiz->func.nome);
        exibirInOrder(raiz->dir);
    }
}

//Pre-order e Pos-order
void exibirPreOrder(NoArvore* raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->func.matricula);
        exibirPreOrder(raiz->esq);
        exibirPreOrder(raiz->dir);
    }
}

void exibirPosOrder(NoArvore* raiz) {
    if (raiz != NULL) {
        exibirPosOrder(raiz->esq);
        exibirPosOrder(raiz->dir);
        printf("%d ", raiz->func.matricula);
    }
}

void buscarABB(NoArvore* raiz, int mat) {
    if (raiz == NULL) {
        printf("Funcionario nao encontrado na Arvore.\n");
        return;
    }
    
    if (mat == raiz->func.matricula) {
        printf("Encontrado na Arvore - Nome: %s | Salario: %.2f\n", raiz->func.nome, raiz->func.salario);
    } else if (mat < raiz->func.matricula) {
        buscarABB(raiz->esq, mat); // busca na subárvore esquerda
    } else {
        buscarABB(raiz->dir, mat); //faz a busca na subárvore direita
    }
}

// verificando a altura e Folhas
int alturaABB(NoArvore* raiz) {
    if (raiz == NULL) return -1;
    int altEsq = alturaABB(raiz->esq);
    int altDir = alturaABB(raiz->dir);
    return (altEsq > altDir ? altEsq : altDir) + 1;
}

int contarFolhas(NoArvore* raiz) {
    if (raiz == NULL) return 0;
    if (raiz->esq == NULL && raiz->dir == NULL) return 1;
    return contarFolhas(raiz->esq) + contarFolhas(raiz->dir);
}

// remoção nos casos a, b, c
NoArvore* menorNoArvore(NoArvore* no) {
    NoArvore* atual = no;
    while (atual && atual->esq != NULL) atual = atual->esq;
    return atual;
}

NoArvore* removerABB(NoArvore* raiz, int mat) {
    if (raiz == NULL) return raiz;

    if (mat < raiz->func.matricula)
        raiz->esq = removerABB(raiz->esq, mat);
    else if (mat > raiz->func.matricula)
        raiz->dir = removerABB(raiz->dir, mat);
    else {
        // a. Nó folha ou b. Nó com um filho
        if (raiz->esq == NULL) {
            NoArvore* temp = raiz->dir;
            free(raiz);
            return temp;
        } else if (raiz->dir == NULL) {
            NoArvore* temp = raiz->esq;
            free(raiz);
            return temp;
        }
        // c. Nó com dois filhos
        NoArvore* temp = menorNoArvore(raiz->dir);
        raiz->func = temp->func; // Copia os dados
        raiz->dir = removerABB(raiz->dir, temp->func.matricula); // Deleta o substituto
    }
    return raiz;
}