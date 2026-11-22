#ifndef MWE_H
#define MWE_H

#define LIMITE_EVENTOS 20
#define LIMITE_CUSTOS 20

typedef struct
{
    char descricao[80];
    float valor;
} Custo;

typedef struct
{
    int codigo;
    char nome[100];
    char tipo[50];
    int dia, mes, ano;
    int hora, minuto;
    char local[100];
    int quantidadePessoas;
    char cadastradoEm[20];

    char nomeContratante[100];
    char tipoDocumento[5];
    char documento[15];
    char whatsapp[12];
    char email[100];

    Custo custos[LIMITE_CUSTOS];
    int quantidadeCustos;
} Evento;

typedef struct
{
    Evento eventos[LIMITE_EVENTOS];
    int quantidadeEventos;
    int proximoCodigo;
} Agenda;

/* entrada.c */
void lerTexto(const char mensagem[], char texto[], int tamanho);
int lerInteiro(const char mensagem[], int minimo, int maximo);
float lerValor(const char mensagem[]);
char lerSimNao(const char mensagem[]);
void lerData(int *dia, int *mes, int *ano);
void lerHorario(int *hora, int *minuto);
void lerDocumento(char tipo[], char numero[]);
void lerWhatsapp(char whatsapp[]);
void lerEmail(char email[]);

/* arquivos.c */
void iniciarAgenda(Agenda *agenda);
int salvarDados(const Agenda *agenda);
void salvarComAviso(const Agenda *agenda);

/* eventos.c */
int buscarEvento(const Agenda *agenda, int codigo);
int escolherEvento(const Agenda *agenda);
void mostrarEvento(const Evento *evento);
void cadastrarEvento(Agenda *agenda);
void listarEventos(const Agenda *agenda);
void consultarEvento(const Agenda *agenda);
void editarEvento(Agenda *agenda);
void removerEvento(Agenda *agenda);

/* custos.c */
float calcularTotal(const Evento *evento);
int adicionarCusto(Evento *evento);
void gerenciarCustos(Agenda *agenda);

/* relatorios.c */
void resumoFinanceiro(const Agenda *agenda);
void relatorioMensal(const Agenda *agenda);
void gerarRelatorioTxt(const Agenda *agenda);

#endif
