#include <stdio.h>
#include "mwe.h"

#define ARQUIVO_RELATORIO "relatorio_mwe.txt"

void resumoFinanceiro(const Agenda *agenda) {
    int i;
    int maisCaro = 0;
    int maisBarato = 0;
    float total = 0;
    float valorAtual;

    if (agenda->quantidadeEventos == 0) {
        printf("\nNenhum evento cadastrado.\n");
        return;
    }

    for (i = 0; i < agenda->quantidadeEventos; i++) {
        valorAtual = calcularTotal(&agenda->eventos[i]);
        total += valorAtual;

        if (valorAtual > calcularTotal(&agenda->eventos[maisCaro])) {
            maisCaro = i;
        }

        if (valorAtual < calcularTotal(&agenda->eventos[maisBarato])) {
            maisBarato = i;
        }
    }

    printf("\n========== RESUMO FINANCEIRO ==========\n");
    printf("Eventos cadastrados: %d\n", agenda->quantidadeEventos);
    printf("Total de custos: R$ %.2f\n", total);
    printf("Media por evento: R$ %.2f\n",
           total / agenda->quantidadeEventos);
    printf("Evento mais caro: %s - R$ %.2f\n",
           agenda->eventos[maisCaro].nome,
           calcularTotal(&agenda->eventos[maisCaro]));
    printf("Evento mais barato: %s - R$ %.2f\n",
           agenda->eventos[maisBarato].nome,
           calcularTotal(&agenda->eventos[maisBarato]));
}

void relatorioMensal(const Agenda *agenda) {
    int mes;
    int ano;
    int i;
    int quantidade = 0;
    float total = 0;

    if (agenda->quantidadeEventos == 0) {
        printf("\nNenhum evento cadastrado.\n");
        return;
    }

    printf("\n========== RELATORIO MENSAL ==========\n");
    mes = lerInteiro("Mes: ", 1, 12);
    ano = lerInteiro("Ano: ", 2026, 2100);

    for (i = 0; i < agenda->quantidadeEventos; i++) {
        if (agenda->eventos[i].mes == mes &&
            agenda->eventos[i].ano == ano) {
            printf("%d - %s - %02d/%02d/%04d - R$ %.2f\n",
                   agenda->eventos[i].codigo,
                   agenda->eventos[i].nome,
                   agenda->eventos[i].dia,
                   agenda->eventos[i].mes,
                   agenda->eventos[i].ano,
                   calcularTotal(&agenda->eventos[i]));

            total += calcularTotal(&agenda->eventos[i]);
            quantidade++;
        }
    }

    if (quantidade == 0) {
        printf("Nenhum evento encontrado nesse mes.\n");
        return;
    }

    printf("Eventos no mes: %d\n", quantidade);
    printf("Total de custos do mes: R$ %.2f\n", total);
    printf("Media por evento: R$ %.2f\n", total / quantidade);
}

static void escreverEvento(FILE *arquivo, const Evento *evento) {
    int i;

    fprintf(arquivo, "\n----------------------------------------\n");
    fprintf(arquivo, "Codigo: %d\n", evento->codigo);
    fprintf(arquivo, "Evento: %s\n", evento->nome);
    fprintf(arquivo, "Tipo: %s\n", evento->tipo);
    fprintf(arquivo, "Data: %02d/%02d/%04d as %02d:%02d\n",
            evento->dia, evento->mes, evento->ano,
            evento->hora, evento->minuto);
    fprintf(arquivo, "Local: %s\n", evento->local);
    fprintf(arquivo, "Quantidade de pessoas: %d\n",
            evento->quantidadePessoas);
    fprintf(arquivo, "Cadastrado em: %s\n", evento->cadastradoEm);
    fprintf(arquivo, "Contratante: %s\n", evento->nomeContratante);
    fprintf(arquivo, "%s: %s\n", evento->tipoDocumento, evento->documento);
    fprintf(arquivo, "WhatsApp: %s\n", evento->whatsapp);
    fprintf(arquivo, "E-mail: %s\n", evento->email);
    fprintf(arquivo, "Custos:\n");

    if (evento->quantidadeCustos == 0) {
        fprintf(arquivo, "Nenhum custo cadastrado.\n");
    }

    for (i = 0; i < evento->quantidadeCustos; i++) {
        fprintf(arquivo, "%d. %s - R$ %.2f\n", i + 1,
                evento->custos[i].descricao,
                evento->custos[i].valor);
    }

    fprintf(arquivo, "Total de custos: R$ %.2f\n",
            calcularTotal(evento));
}

void gerarRelatorioTxt(const Agenda *agenda) {
    FILE *arquivo;
    float total = 0;
    int i;

    if (agenda->quantidadeEventos == 0) {
        printf("\nNao existem eventos para gerar o relatorio.\n");
        return;
    }

    arquivo = fopen(ARQUIVO_RELATORIO, "w");

    if (arquivo == NULL) {
        printf("\nNao foi possivel criar o relatorio.\n");
        return;
    }

    fprintf(arquivo, "RELATORIO GERAL - MWE HARMONIZE\n");
    fprintf(arquivo, "========================================\n");

    for (i = 0; i < agenda->quantidadeEventos; i++) {
        escreverEvento(arquivo, &agenda->eventos[i]);
        total += calcularTotal(&agenda->eventos[i]);
    }

    fprintf(arquivo, "\nTOTAL DE EVENTOS: %d\n",
            agenda->quantidadeEventos);
    fprintf(arquivo, "TOTAL GERAL DE CUSTOS: R$ %.2f\n", total);
    fclose(arquivo);

    printf("\nRelatorio criado em %s.\n", ARQUIVO_RELATORIO);
}
