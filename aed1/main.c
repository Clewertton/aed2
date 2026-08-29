#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "arvore.h"

void inicializarDados(NoLista** lista, NoArvore** arvore) {
    Funcionario iniciais[10] = {
        {10, "Ana", 25, 3000.0}, {5, "Bruno", 30, 4500.0}, {15, "Carlos", 28, 3200.0},
        {3, "Daniela", 40, 6000.0}, {7, "Eduardo", 22, 2500.0}, {12, "Fernanda", 35, 5000.0},
        {18, "Gustavo", 45, 8000.0}, {1, "Helena", 29, 3800.0}, {8, "Igor", 31, 4100.0},
        {14, "Julia", 27, 3400.0}
    };
    
    for (int i = 0; i < 10; i++) {
        *lista = inserirLSE(*lista, iniciais[i]);
        *arvore = inserirABB(*arvore, iniciais[i]);
    }
}

int main() {
    NoLista* lista = NULL;
    NoArvore* arvore = NULL;
    int opcao, mat;
    Funcionario novo;
    
    inicializarDados(&lista, &arvore);

    do {
        printf("\n--- MENU DE FUNCIONARIOS ---\n");
        printf("1. Inserir novo funcionario\n");
        printf("2. Listar todos (LSE)\n");
        printf("3. Buscar por matricula (LSE e ABB)\n");
        printf("4. Remover por matricula (LSE e ABB)\n");
        printf("5. Estatisticas da Lista (Quantidade e Maior Salario)\n");
        printf("6. Exibir ABB (In-Order)\n");
        printf("7. Exibir ABB (Pre-Order e Pos-Order)\n");
        printf("8. Estatisticas da Arvore (Altura e Folhas)\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Matricula: "); scanf("%d", &novo.matricula);
                printf("Nome: "); scanf(" %[^\n]", novo.nome); // Lê string com espaço
                printf("Idade: "); scanf("%d", &novo.idade);
                printf("Salario: "); scanf("%f", &novo.salario);
                
                lista = inserirLSE(lista, novo);
                arvore = inserirABB(arvore, novo);
                printf("Inserido com sucesso!\n");
                break;
            case 2:
                listarLSE(lista);
                break;
            case 3:
                printf("Digite a matricula para busca: ");
                scanf("%d", &mat);
                buscarLSE(lista, mat);
                buscarABB(arvore, mat);
                break;
            case 4:
                printf("Digite a matricula para remover: ");
                scanf("%d", &mat);
                lista = removerLSE(lista, mat);
                arvore = removerABB(arvore, mat);
                break;
            case 5:
                printf("Total de funcionarios cadastrados: %d\n", contarLSE(lista));
                exibirMaiorSalarioLSE(lista);
                break;
            case 6:
                printf("Exibicao In-Order (Crescente):\n");
                exibirInOrder(arvore);
                printf("\n");
                break;
            case 7:
                printf("Exibicao Pre-Order: ");
                exibirPreOrder(arvore);
                printf("\nExibicao Pos-Order: ");
                exibirPosOrder(arvore);
                printf("\n");
                break;
            case 8:
                printf("Altura da arvore: %d\n", alturaABB(arvore));
                printf("Quantidade de folhas: %d\n", contarFolhas(arvore));
                break;
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}