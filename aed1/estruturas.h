#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

typedef struct {
    int matricula;
    char nome[100];
    int idade;
    float salario;
} Funcionario;

typedef struct NoLista {
    Funcionario func;
    struct NoLista *prox;
} NoLista;

typedef struct NoArvore {
    Funcionario func;
    struct NoArvore *esq;
    struct NoArvore *dir;
} NoArvore;

#endif