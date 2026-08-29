#ifndef ARVORE_H
#define ARVORE_H
#include "estruturas.h"

NoArvore* inserirABB(NoArvore* raiz, Funcionario f);
void exibirInOrder(NoArvore* raiz);
void exibirPreOrder(NoArvore* raiz);
void exibirPosOrder(NoArvore* raiz);
void buscarABB(NoArvore* raiz, int mat);
int alturaABB(NoArvore* raiz);
int contarFolhas(NoArvore* raiz);
NoArvore* removerABB(NoArvore* raiz, int mat);

#endif