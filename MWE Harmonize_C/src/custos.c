#include <stdio.h>
#include "mwe.h"

float calcularTotal(const Evento *evento) {
    float total = 0;
    int i;

    for (i = 0; i < evento->quantidadeCustos; i++) {
        total += evento->custos[i].valor;
    }

    return total;
}

int adicionarCusto(Evento *evento) {
    Custo *custo;

    if (evento->quantidadeCustos == LIMITE_CUSTOS) {
        printf("Limite de custos atingido.\n");
        return 0;
    }

    custo = &evento->custos[evento->quantidadeCustos];
    lerTexto("Descricao do custo: ", custo->descricao,
             sizeof(custo->descricao));
    custo->valor = lerValor("Valor do custo: R$ ");
    evento->quantidadeCustos++;
    return 1;
}

void gerenciarCustos(Agenda *agenda) {
    int posicao;
    int opcao;
    int numero;
    int i;
    int alterou = 0;
    Evento *evento;

    printf("\n========== GERENCIAR CUSTOS ==========\n");
    posicao = escolherEvento(agenda);

    if (posicao == -1) {
        return;
    }

    evento = &agenda->eventos[posicao];

    do {
        mostrarEvento(evento);
        printf("\n1. Adicionar custo\n");
        printf("2. Editar custo\n");
        printf("3. Remover custo\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha: ", 0, 3);

        if (opcao == 1) {
            if (adicionarCusto(evento)) {
                alterou = 1;
            }
        } else if (opcao == 2) {
            if (evento->quantidadeCustos == 0) {
                printf("Nao existem custos para editar.\n");
                continue;
            }

            numero = lerInteiro("Numero do custo: ",
                                1, evento->quantidadeCustos) - 1;
            lerTexto("Nova descricao: ", evento->custos[numero].descricao,
                     sizeof(evento->custos[numero].descricao));
            evento->custos[numero].valor = lerValor("Novo valor: R$ ");
            alterou = 1;
            printf("Custo editado.\n");
        } else if (opcao == 3) {
            if (evento->quantidadeCustos == 0) {
                printf("Nao existem custos para remover.\n");
                continue;
            }

            numero = lerInteiro("Numero do custo: ",
                                1, evento->quantidadeCustos) - 1;

            if (lerSimNao("Confirmar remocao? (s/n): ") == 's') {
                for (i = numero; i < evento->quantidadeCustos - 1; i++) {
                    evento->custos[i] = evento->custos[i + 1];
                }

                evento->quantidadeCustos--;
                alterou = 1;
                printf("Custo removido.\n");
            }
        }
    } while (opcao != 0);

    if (alterou) {
        salvarComAviso(agenda);
    }
}
