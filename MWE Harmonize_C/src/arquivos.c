#include <stdio.h>
#include <string.h>
#include "mwe.h"

#define ARQUIVO_DADOS "dados_mwe.dat"

void iniciarAgenda(Agenda *agenda) {
    FILE *arquivo;

    memset(agenda, 0, sizeof(Agenda));
    agenda->proximoCodigo = 1;

    arquivo = fopen(ARQUIVO_DADOS, "rb");

    if (arquivo == NULL) {
        return;
    }

    if (fread(agenda, sizeof(Agenda), 1, arquivo) != 1 ||
        agenda->quantidadeEventos < 0 ||
        agenda->quantidadeEventos > LIMITE_EVENTOS ||
        agenda->proximoCodigo < 1) {
        memset(agenda, 0, sizeof(Agenda));
        agenda->proximoCodigo = 1;
        printf("O arquivo de dados estava invalido e foi ignorado.\n");
    }

    fclose(arquivo);
}

int salvarDados(const Agenda *agenda) {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "wb");
    int resultado;

    if (arquivo == NULL) {
        return 0;
    }

    resultado = fwrite(agenda, sizeof(Agenda), 1, arquivo) == 1;
    fclose(arquivo);
    return resultado;
}

void salvarComAviso(const Agenda *agenda) {
    if (salvarDados(agenda)) {
        printf("Dados salvos com sucesso.\n");
    } else {
        printf("Nao foi possivel salvar os dados.\n");
    }
}
