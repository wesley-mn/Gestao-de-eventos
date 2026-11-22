#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mwe.h"

static void lerLinha(const char mensagem[], char texto[], int tamanho) {
    int caractere;

    printf("%s", mensagem);

    if (fgets(texto, tamanho, stdin) == NULL) {
        exit(0);
    }

    if (strchr(texto, '\n') == NULL) {
        while ((caractere = getchar()) != '\n' && caractere != EOF) {
        }
    }

    texto[strcspn(texto, "\r\n")] = '\0';
}

void lerTexto(const char mensagem[], char texto[], int tamanho) {
    while (1) {
        lerLinha(mensagem, texto, tamanho);

        if (texto[0] != '\0') {
            return;
        }

        printf("Esse campo nao pode ficar vazio.\n");
    }
}

int lerInteiro(const char mensagem[], int minimo, int maximo) {
    char linha[50];
    char extra;
    int numero;

    while (1) {
        lerLinha(mensagem, linha, sizeof(linha));

        if (sscanf(linha, "%d %c", &numero, &extra) == 1 &&
            numero >= minimo && numero <= maximo) {
            return numero;
        }

        printf("Digite um numero entre %d e %d.\n", minimo, maximo);
    }
}

float lerValor(const char mensagem[]) {
    char linha[50];
    char extra;
    float valor;
    int i;

    while (1) {
        lerLinha(mensagem, linha, sizeof(linha));

        for (i = 0; linha[i] != '\0'; i++) {
            if (linha[i] == ',') {
                linha[i] = '.';
            }
        }

        if (sscanf(linha, "%f %c", &valor, &extra) == 1 && valor > 0) {
            return valor;
        }

        printf("Digite um valor maior que zero.\n");
    }
}

char lerSimNao(const char mensagem[]) {
    char resposta[10];

    while (1) {
        lerLinha(mensagem, resposta, sizeof(resposta));

        if (strlen(resposta) == 1) {
            resposta[0] = (char) tolower((unsigned char) resposta[0]);

            if (resposta[0] == 's' || resposta[0] == 'n') {
                return resposta[0];
            }
        }

        printf("Digite apenas s ou n.\n");
    }
}

static int somenteNumeros(const char texto[]) {
    int i;

    if (texto[0] == '\0') {
        return 0;
    }

    for (i = 0; texto[i] != '\0'; i++) {
        if (!isdigit((unsigned char) texto[i])) {
            return 0;
        }
    }

    return 1;
}

static int anoBissexto(int ano) {
    return ano % 400 == 0 || (ano % 4 == 0 && ano % 100 != 0);
}

static int dataValida(int dia, int mes, int ano) {
    int dias[12] = {31, 28, 31, 30, 31, 30,
                    31, 31, 30, 31, 30, 31};

    if (ano < 2026 || ano > 2100 || mes < 1 || mes > 12) {
        return 0;
    }

    if (anoBissexto(ano)) {
        dias[1] = 29;
    }

    return dia >= 1 && dia <= dias[mes - 1];
}

void lerData(int *dia, int *mes, int *ano) {
    while (1) {
        *dia = lerInteiro("Dia: ", 1, 31);
        *mes = lerInteiro("Mes: ", 1, 12);
        *ano = lerInteiro("Ano: ", 2026, 2100);

        if (dataValida(*dia, *mes, *ano)) {
            return;
        }

        printf("Essa data nao existe. Digite novamente.\n");
    }
}

void lerHorario(int *hora, int *minuto) {
    char linha[50];
    char extra;

    while (1) {
        lerLinha("Horario (HH:MM): ", linha, sizeof(linha));

        if (sscanf(linha, "%d:%d %c", hora, minuto, &extra) == 2 &&
            *hora >= 0 && *hora <= 23 &&
            *minuto >= 0 && *minuto <= 59) {
            return;
        }

        printf("Horario invalido. Exemplo: 19:30\n");
    }
}

void lerDocumento(char tipo[], char numero[]) {
    char escolha[10];
    int tamanho;

    while (1) {
        lerTexto("Tipo do documento (CPF ou CNPJ): ", escolha, sizeof(escolha));

        if (strcmp(escolha, "CPF") == 0 || strcmp(escolha, "cpf") == 0) {
            strcpy(tipo, "CPF");
            tamanho = 11;
        } else if (strcmp(escolha, "CNPJ") == 0 || strcmp(escolha, "cnpj") == 0) {
            strcpy(tipo, "CNPJ");
            tamanho = 14;
        } else {
            printf("Digite CPF ou CNPJ.\n");
            continue;
        }

        lerTexto("Numero do documento: ", numero, 15);

        if (somenteNumeros(numero) && (int) strlen(numero) == tamanho) {
            return;
        }

        printf("O %s precisa ter %d numeros.\n", tipo, tamanho);
    }
}

void lerWhatsapp(char whatsapp[]) {
    while (1) {
        lerTexto("WhatsApp com DDD (11 numeros): ", whatsapp, 12);

        if (somenteNumeros(whatsapp) && strlen(whatsapp) == 11) {
            return;
        }

        printf("O WhatsApp precisa ter 11 numeros.\n");
    }
}

void lerEmail(char email[]) {
    while (1) {
        lerTexto("E-mail: ", email, 100);

        if (strchr(email, '@') != NULL && strchr(email, '.') != NULL) {
            return;
        }

        printf("Digite um e-mail valido.\n");
    }
}
