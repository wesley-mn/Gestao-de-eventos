#include <stdio.h>
#include <string.h>
#include <time.h>
#include "mwe.h"

int buscarEvento(const Agenda *agenda, int codigo) {
    int i;

    for (i = 0; i < agenda->quantidadeEventos; i++) {
        if (agenda->eventos[i].codigo == codigo) {
            return i;
        }
    }

    return -1;
}

int escolherEvento(const Agenda *agenda) {
    int codigo;
    int posicao;

    if (agenda->quantidadeEventos == 0) {
        printf("\nNenhum evento cadastrado.\n");
        return -1;
    }

    while (1) {
        codigo = lerInteiro("Codigo do evento (0 para cancelar): ", 0, 999999);

        if (codigo == 0) {
            return -1;
        }

        posicao = buscarEvento(agenda, codigo);

        if (posicao != -1) {
            return posicao;
        }

        printf("Evento nao encontrado.\n");
    }
}

void mostrarEvento(const Evento *evento) {
    int i;

    printf("\n----------------------------------------\n");
    printf("Codigo: %d\n", evento->codigo);
    printf("Evento: %s\n", evento->nome);
    printf("Tipo: %s\n", evento->tipo);
    printf("Data: %02d/%02d/%04d\n", evento->dia, evento->mes, evento->ano);
    printf("Horario: %02d:%02d\n", evento->hora, evento->minuto);
    printf("Local: %s\n", evento->local);
    printf("Quantidade de pessoas: %d\n", evento->quantidadePessoas);
    printf("Cadastrado em: %s\n", evento->cadastradoEm);
    printf("Contratante: %s\n", evento->nomeContratante);
    printf("%s: %s\n", evento->tipoDocumento, evento->documento);
    printf("WhatsApp: %s\n", evento->whatsapp);
    printf("E-mail: %s\n", evento->email);
    printf("Custos:\n");

    if (evento->quantidadeCustos == 0) {
        printf("Nenhum custo cadastrado.\n");
    }

    for (i = 0; i < evento->quantidadeCustos; i++) {
        printf("%d. %s - R$ %.2f\n", i + 1,
               evento->custos[i].descricao,
               evento->custos[i].valor);
    }

    printf("Total de custos: R$ %.2f\n", calcularTotal(evento));
    printf("----------------------------------------\n");
}

static int dataOcupada(const Agenda *agenda, int dia, int mes,
                       int ano, int codigoIgnorado) {
    int i;

    for (i = 0; i < agenda->quantidadeEventos; i++) {
        if (agenda->eventos[i].codigo != codigoIgnorado &&
            agenda->eventos[i].dia == dia &&
            agenda->eventos[i].mes == mes &&
            agenda->eventos[i].ano == ano) {
            return 1;
        }
    }

    return 0;
}

static void lerDataLivre(const Agenda *agenda, Evento *evento,
                         int codigoIgnorado) {
    while (1) {
        lerData(&evento->dia, &evento->mes, &evento->ano);

        if (!dataOcupada(agenda, evento->dia, evento->mes,
                         evento->ano, codigoIgnorado)) {
            return;
        }

        printf("Ja existe um evento nessa data. Escolha outra.\n");
    }
}

static void registrarDataHora(char texto[]) {
    time_t agora = time(NULL);
    struct tm *dataHora = localtime(&agora);

    if (dataHora == NULL) {
        strcpy(texto, "Data indisponivel");
    } else {
        strftime(texto, 20, "%d/%m/%Y %H:%M:%S", dataHora);
    }
}

static void preencherContratante(Evento *evento) {
    lerTexto("Nome do contratante: ", evento->nomeContratante,
             sizeof(evento->nomeContratante));
    lerDocumento(evento->tipoDocumento, evento->documento);
    lerWhatsapp(evento->whatsapp);
    lerEmail(evento->email);
}

void cadastrarEvento(Agenda *agenda) {
    Evento *evento;

    if (agenda->quantidadeEventos == LIMITE_EVENTOS) {
        printf("\nO limite de eventos foi atingido.\n");
        return;
    }

    evento = &agenda->eventos[agenda->quantidadeEventos];
    memset(evento, 0, sizeof(Evento));
    evento->codigo = agenda->proximoCodigo;

    printf("\n========== CADASTRAR EVENTO ==========\n");
    lerTexto("Nome do evento: ", evento->nome, sizeof(evento->nome));
    lerTexto("Tipo do evento: ", evento->tipo, sizeof(evento->tipo));
    lerDataLivre(agenda, evento, -1);
    lerHorario(&evento->hora, &evento->minuto);
    lerTexto("Local do evento: ", evento->local, sizeof(evento->local));
    evento->quantidadePessoas =
        lerInteiro("Quantidade de pessoas: ", 1, 1000000);

    printf("\n========== CONTRATANTE ==========\n");
    preencherContratante(evento);

    while (evento->quantidadeCustos < LIMITE_CUSTOS &&
           lerSimNao("Deseja cadastrar um custo? (s/n): ") == 's') {
        adicionarCusto(evento);
    }

    registrarDataHora(evento->cadastradoEm);
    agenda->quantidadeEventos++;
    agenda->proximoCodigo++;

    printf("\nEvento cadastrado com sucesso. Codigo: %d\n", evento->codigo);
    printf("Cadastro realizado em: %s\n", evento->cadastradoEm);
    salvarComAviso(agenda);
}

void listarEventos(const Agenda *agenda) {
    int i;

    if (agenda->quantidadeEventos == 0) {
        printf("\nNenhum evento cadastrado.\n");
        return;
    }

    printf("\n========== LISTA DE EVENTOS ==========\n");

    for (i = 0; i < agenda->quantidadeEventos; i++) {
        mostrarEvento(&agenda->eventos[i]);
    }
}

void consultarEvento(const Agenda *agenda) {
    int posicao;

    printf("\n========== CONSULTAR EVENTO ==========\n");
    posicao = escolherEvento(agenda);

    if (posicao != -1) {
        mostrarEvento(&agenda->eventos[posicao]);
    }
}

void editarEvento(Agenda *agenda) {
    int posicao;
    int opcao;
    int alterou = 0;
    Evento *evento;

    printf("\n========== EDITAR EVENTO ==========\n");
    posicao = escolherEvento(agenda);

    if (posicao == -1) {
        return;
    }

    evento = &agenda->eventos[posicao];

    do {
        mostrarEvento(evento);
        printf("\n1. Nome\n");
        printf("2. Tipo\n");
        printf("3. Data\n");
        printf("4. Horario\n");
        printf("5. Local\n");
        printf("6. Quantidade de pessoas\n");
        printf("7. Dados do contratante\n");
        printf("0. Concluir\n");
        opcao = lerInteiro("Escolha o que deseja editar: ", 0, 7);

        switch (opcao) {
            case 1:
                lerTexto("Novo nome: ", evento->nome, sizeof(evento->nome));
                break;
            case 2:
                lerTexto("Novo tipo: ", evento->tipo, sizeof(evento->tipo));
                break;
            case 3:
                lerDataLivre(agenda, evento, evento->codigo);
                break;
            case 4:
                lerHorario(&evento->hora, &evento->minuto);
                break;
            case 5:
                lerTexto("Novo local: ", evento->local, sizeof(evento->local));
                break;
            case 6:
                evento->quantidadePessoas =
                    lerInteiro("Nova quantidade: ", 1, 1000000);
                break;
            case 7:
                preencherContratante(evento);
                break;
        }

        if (opcao != 0) {
            alterou = 1;
            printf("Alteracao realizada.\n");
        }
    } while (opcao != 0);

    if (alterou) {
        salvarComAviso(agenda);
    }
}

void removerEvento(Agenda *agenda) {
    int posicao;
    int i;

    printf("\n========== REMOVER EVENTO ==========\n");
    posicao = escolherEvento(agenda);

    if (posicao == -1) {
        return;
    }

    mostrarEvento(&agenda->eventos[posicao]);

    if (lerSimNao("Confirmar remocao? (s/n): ") == 'n') {
        printf("Remocao cancelada.\n");
        return;
    }

    for (i = posicao; i < agenda->quantidadeEventos - 1; i++) {
        agenda->eventos[i] = agenda->eventos[i + 1];
    }

    agenda->quantidadeEventos--;
    memset(&agenda->eventos[agenda->quantidadeEventos], 0, sizeof(Evento));
    printf("Evento removido com sucesso.\n");
    salvarComAviso(agenda);
}
