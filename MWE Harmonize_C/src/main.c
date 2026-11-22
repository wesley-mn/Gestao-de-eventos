#include <stdio.h>
#include "mwe.h"

void mostrarMenu(void) {
    printf("\n========== MWE HARMONIZE ==========\n");
    printf("1. Cadastrar evento\n");
    printf("2. Listar eventos\n");
    printf("3. Consultar evento pelo codigo\n");
    printf("4. Editar evento\n");
    printf("5. Remover evento\n");
    printf("6. Gerenciar custos\n");
    printf("7. Resumo financeiro geral\n");
    printf("8. Relatorio financeiro mensal\n");
    printf("9. Gerar relatorio TXT\n");
    printf("0. Sair\n");
}

int main(void) {
    Agenda agenda;
    int opcao;

    iniciarAgenda(&agenda);

    do {
        mostrarMenu();
        opcao = lerInteiro("Escolha: ", 0, 9);

        switch (opcao) {
            case 1: cadastrarEvento(&agenda); break;
            case 2: listarEventos(&agenda); break;
            case 3: consultarEvento(&agenda); break;
            case 4: editarEvento(&agenda); break;
            case 5: removerEvento(&agenda); break;
            case 6: gerenciarCustos(&agenda); break;
            case 7: resumoFinanceiro(&agenda); break;
            case 8: relatorioMensal(&agenda); break;
            case 9: gerarRelatorioTxt(&agenda); break;
        }
    } while (opcao != 0);

    printf("\nPrograma encerrado. Ate logo!\n");
    return 0;
}
