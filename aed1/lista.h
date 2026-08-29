#ifndef LISTA_H
#define LISTA_H
#include "estruturas.h"

NoLista* inserirLSE(NoLista* inicio, Funcionario f);
void listarLSE(NoLista* inicio);
void buscarLSE(NoLista* inicio, int mat);
NoLista* removerLSE(NoLista* inicio, int mat);
int contarLSE(NoLista* inicio);
void exibirMaiorSalarioLSE(NoLista* inicio);

#endif