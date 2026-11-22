# MWE Harmonize

Sistema em linguagem C desenvolvido para uma agenda de eventos (festas de 15 anos e casamentos) da empresa MWE Harmonize.

O programa roda no terminal, guarda os dados entre execuções em um arquivo local e permite gerar relatórios financeiros na tela ou em um arquivo `.txt`.

## Autores

- Alexandre Zampronne Zaccaron Rocha
- Bruno Eduardo Dias Carvalho
- Francisco de Souza Lima
- Matheus Felipe Ferreira
- Wesley Henrique de Matos Nascimento

---

## Visão geral

Cada **evento** cadastrado guarda:

- Nome, tipo, data e horário do evento;
- Local e quantidade de pessoas esperadas;
- Data/hora em que o cadastro foi feito no sistema;
- Dados do contratante (nome, CPF/CNPJ, WhatsApp, e-mail);
- Uma lista de **custos** associados ao evento (descrição + valor), usada para calcular o total gasto.

Todos os eventos ficam guardados dentro de uma **Agenda**, que também controla quantos eventos já existem e qual será o próximo código disponível.

---

## Estrutura de pastas

```
MWE HArmonize/
├── include/
│   └── mwe.h          # Structs, constantes e protótipos de todas as funções
└── src/
    ├── main.c         # Ponto de entrada: menu principal e laço do programa
    ├── entrada.c      # Leitura e validação de dados digitados pelo usuário
    ├── eventos.c      # Cadastro, listagem, consulta, edição e remoção de eventos
    ├── custos.c       # Cadastro, edição e remoção de custos de um evento
    ├── relatorios.c   # Resumo financeiro, relatório mensal e relatório em .txt
    └── arquivos.c     # Salvar/carregar a Agenda em disco (persistência)
```

### O que cada arquivo faz

| Arquivo | Responsabilidade |
|---|---|
| `mwe.h` | Define as structs `Custo`, `Evento` e `Agenda`, as constantes `LIMITE_EVENTOS`/`LIMITE_CUSTOS`, e declara (protótipos) todas as funções usadas pelos outros arquivos. |
| `main.c` | Mostra o menu, lê a opção escolhida e chama a função correspondente. É o "maestro" do sistema. |
| `entrada.c` | Funções genéricas de leitura de teclado com validação: texto, número inteiro, valor em R$, sim/não, data, horário, documento (CPF/CNPJ), WhatsApp e e-mail. |
| `eventos.c` | Regras de negócio dos eventos: cadastrar, listar, buscar por código, exibir, editar campo a campo e remover (deslocando os eventos seguintes na lista). |
| `custos.c` | Regras de negócio dos custos dentro de um evento: adicionar, editar, remover e calcular o total gasto. |
| `relatorios.c` | Geração de relatórios: resumo financeiro geral (na tela), relatório filtrado por mês/ano (na tela) e relatório completo exportado em arquivo `.txt`. |
| `arquivos.c` | Persistência dos dados: carrega a Agenda do disco ao iniciar o programa e salva a Agenda no disco sempre que algo muda. |

---

## Estruturas de dados (`mwe.h`)

```c
typedef struct {
    char descricao[80];
    float valor;
} Custo;

typedef struct {
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

typedef struct {
    Evento eventos[LIMITE_EVENTOS];
    int quantidadeEventos;
    int proximoCodigo;
} Agenda;
```

- `LIMITE_EVENTOS` (20) e `LIMITE_CUSTOS` (20) definem os tamanhos máximos, já que o sistema usa vetores de tamanho fixo (sem alocação dinâmica).
- A `Agenda` é criada uma única vez em `main()` e passada por **ponteiro** (`&agenda`) para praticamente todas as funções, permitindo que cadastros, edições e remoções alterem os dados originais.

---

## Fluxo do programa (`main.c`)

1. `iniciarAgenda(&agenda)` carrega os dados salvos anteriormente (se existirem).
2. O menu é exibido em loop até o usuário escolher `0` (sair):

```
1. Cadastrar evento
2. Listar eventos
3. Consultar evento pelo codigo
4. Editar evento
5. Remover evento
6. Gerenciar custos
7. Resumo financeiro geral
8. Relatorio financeiro mensal
9. Gerar relatorio TXT
0. Sair
```

3. Cada opção chama uma função específica (`cadastrarEvento`, `listarEventos`, etc.), que faz sua tarefa e — quando os dados mudam — chama `salvarComAviso()` para persistir a Agenda em disco.

---

## Persistência de dados

O sistema usa **um arquivo binário** para guardar tudo entre execuções:

- **`dados_mwe.dat`** (gerado por `arquivos.c`): guarda a struct `Agenda` inteira, em formato binário, usando `fread`/`fwrite`. É criado/atualizado automaticamente; o usuário não precisa mexer nele.

E gera **um relatório em texto puro** quando solicitado no menu:

- **`relatorio_mwe.txt`** (gerado por `relatorios.c`, opção 9 do menu): lista todos os eventos cadastrados, seus custos e o total geral, usando `fprintf`. Pode ser aberto em qualquer editor de texto.

> Não há geração de arquivos `.csv` no sistema atual.

---

## Validações de entrada (`entrada.c`)

Todo dado digitado pelo usuário passa por validação antes de ser aceito, evitando dados inconsistentes na Agenda:

- **Texto**: não pode ficar vazio.
- **Número inteiro/valor**: precisa estar dentro de um intervalo (ex.: mês entre 1 e 12) e não pode ter lixo depois do número.
- **Data**: verifica se o dia existe no mês/ano informado (considerando anos bissextos) e se a data está no intervalo permitido (2026–2100).
- **Horário**: formato `HH:MM`, com hora entre 0–23 e minuto entre 0–59.
- **CPF/CNPJ**: exige apenas números, com 11 dígitos (CPF) ou 14 dígitos (CNPJ).
- **WhatsApp**: exige 11 dígitos numéricos (DDD + número).
- **E-mail**: exige a presença de `@` e de um `.` no texto.
- **Data ocupada**: ao cadastrar ou editar a data de um evento, o sistema verifica se já existe outro evento na mesma data e impede o conflito.

---

## Relatórios (`relatorios.c`)

| Função | O que faz |
|---|---|
| `resumoFinanceiro` | Mostra na tela: total de eventos, soma de todos os custos, média por evento, e qual evento é o mais caro/mais barato. |
| `relatorioMensal` | Pede um mês e ano e mostra na tela apenas os eventos daquele período, com total e média. |
| `gerarRelatorioTxt` | Gera o arquivo `relatorio_mwe.txt` com os dados completos de todos os eventos cadastrados e o total geral de custos. |

---

## Como compilar e executar

Usando `gcc` (Linux/macOS/WSL):

```bash
gcc -Wall -Iinclude src/*.c -o mwe_harmonize
./mwe_harmonize
```

No Windows (MinGW), o comando é o mesmo, mudando apenas o nome do executável gerado:

```bash
gcc -Wall -Iinclude src/*.c -o mwe_harmonize.exe
mwe_harmonize.exe
```

> O `-Iinclude` indica ao compilador onde encontrar o arquivo `mwe.h`.

---

## Pontos de atenção / possíveis melhorias futuras

- Os vetores de eventos e custos têm **tamanho fixo** (`LIMITE_EVENTOS` e `LIMITE_CUSTOS`); para crescer além disso seria necessário migrar para alocação dinâmica (`malloc`/listas ligadas).
- Os dados são salvos em **formato binário** (`.dat`), o que é rápido mas não é legível fora do próprio programa nem facilmente portável entre sistemas diferentes (por causa de `sizeof(Agenda)` e alinhamento de memória).
- Não existe exportação em `.csv` — pode ser adicionada de forma parecida com `gerarRelatorioTxt()`, escrevendo uma linha por evento com campos separados por vírgula.
- Não há testes automatizados; a validação depende inteiramente das funções de `entrada.c`.
