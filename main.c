#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Constantes do Projeto */

#define NOME_ARQUIVO_BANCO "webflix.bin"

#define MAX_FILMES 65536

#define QTD_CAMPOS 5

#define MAX_SIZE 256

/* Definições do Gerenciador de Lista de Filmes */

int quantidade_filmes = 0;

char lista_filmes[MAX_FILMES][QTD_CAMPOS][MAX_SIZE];

bool AdicionarFilmeLista(const char nome[], const char genero[], const char data[], const char classificacao_etaria[], const char nota[]);

bool RemoverFilmeLista(int id);

/* Definições das Funções de Menu */

void MenuPrincipal(void);

void MenuCatalogo(void);

void MenuGerenciamento(void);

/* Definições das Funções de Catalogo */

int ListarFilmes(const char filtro[]);

/* Definições das Funções de Gerenciamento */

bool CadastrarFilme(void);

bool RemoverFilme(void);

/* Definições das Funções do Banco de Dados */

bool SalvarDados(void);

bool CarregarDados(void);

/* Definições das Funções Utilitárias */

void MostrarTitulo(const char subtitulo[]);

void ReportarMensagem(const char mensagem[]);

void LimparTela(void);

void LimparBuffer(void);

void EsperarEnter(void);

bool VerificaStringNumerica(const char numero[]);

/* Função Principal */

int main(void) {
    CarregarDados();

    MenuPrincipal();

    SalvarDados();

    return EXIT_SUCCESS;
}

/* Gerenciador de Lista de Filmes */

bool AdicionarFilmeLista(const char nome[], const char genero[], const char data[], const char classificacao_etaria[], const char nota[]) {
    if(quantidade_filmes == MAX_FILMES) {
        return false;
    }

    strcpy(lista_filmes[quantidade_filmes][0], nome);
    strcpy(lista_filmes[quantidade_filmes][1], genero);
    strcpy(lista_filmes[quantidade_filmes][2], data);
    snprintf(lista_filmes[quantidade_filmes][3], MAX_SIZE, "%s anos", classificacao_etaria);
    snprintf(lista_filmes[quantidade_filmes][4], MAX_SIZE, "%s/10", nota);

    quantidade_filmes++;

    return true;
}

bool RemoverFilmeLista(int id) {
    for(int i = id; i < quantidade_filmes - 1; i++) {
        for(int j = 0; j < QTD_CAMPOS; j++) {
            strcpy(lista_filmes[i][j], lista_filmes[i + 1][j]);
        }
    }

    quantidade_filmes--;

    return true;
}

/* Implementação das Funções de Menu */

void MenuPrincipal(void) {
    int opcao = -1;
    do {
        MostrarTitulo(
            "1) Catálogo de filmes\n"
            "2) Gerenciamento de filmes\n"
            "0) Sair\n"
            "> "
        );

        scanf("%d", &opcao);
        LimparBuffer();

        switch (opcao) {
            case 0:
                break;
            case 1:
                MenuCatalogo();
                break;
            case 2:
                MenuGerenciamento();
                break;
            default:
                ReportarMensagem("Entrada inválida! Tente novamente.");
        }
    } while (opcao != 0);
}

void MenuCatalogo(void) {
    static char temp_filtro[256];
    strcpy(temp_filtro, "");

    /* Se não tiver filmes... */
    if(quantidade_filmes == 0) {
        MostrarTitulo("Não há filmes no catálogo no momento.\n");
        EsperarEnter();
        return;
    }

    /* Se tiver filmes... */
    while(true) {
        MostrarTitulo("");

        int quantidade = ListarFilmes(temp_filtro);

        if(quantidade == 0) {
            MostrarTitulo("Não foram encontrados filmes!\n");
            EsperarEnter();
            strcpy(temp_filtro, "");
            continue;
        }

        /* Limpar o filtro */
        strcpy(temp_filtro, "");

        printf("\n\033[1;40mPesquise por um filme (ou vazio para voltar):\033[0m ");

        scanf("%[^\n]", temp_filtro);
        LimparBuffer();
    
        /* Usuário pressionou ENTER com filtro vazio */
        if(strlen(temp_filtro) == 0) {
            return;
        }
    }
}

void MenuGerenciamento(void) {
    int opcao = -1;
    do {
        MostrarTitulo(
            "1) Cadastrar um novo filme\n"
            "2) Deletar um filme existente\n"
            "0) Voltar\n"
            "> "
        );

        scanf("%d", &opcao);
        LimparBuffer();

        switch (opcao) { 
            case 0:
                break;
            case 1:
                CadastrarFilme();
                break;
            case 2:
                RemoverFilme();
                break;
            default:
                ReportarMensagem("Entrada inválida! Tente novamente.");
        }
    } while (opcao != 0);
}

/* Implementação das Funções de Catalogo */

int ListarFilmes(const char filtro[]) {
    /* Cabeçalho da Tabela */
    printf(
        "\033[1;40m%-8s%-50s%-21s%-8s%-20s%-8s\033[0m\n",
        "ID",
        "Nome",
        "Gênero",
        "Data",
        "Classificação",
        "Nota"
    );

    int quantidade_filmes_achados = 0;

    /* Imprime cada filme */
    for(int i = 0; i < quantidade_filmes; i++) {
        /* Verifica se o filtro se aplica ao filme */
        if(
            strlen(filtro) == 0 ||                      /* Filtro vazio */
            strcmp(lista_filmes[i][0], filtro) == 0 ||  /* Igual ao nome */
            strcmp(lista_filmes[i][1], filtro) == 0 ||  /* Igual ao genero */
            strcmp(lista_filmes[i][3], filtro) == 0     /* Igual à classificação */
        ) { 
            printf(
                "%-8d%-50s%-20s%-8s%-20s%-8s\n", 
                i,
                lista_filmes[i][0], 
                lista_filmes[i][1], 
                lista_filmes[i][2], 
                lista_filmes[i][3], 
                lista_filmes[i][4]
            );

            quantidade_filmes_achados++;
        }
    }

    return quantidade_filmes_achados;
}

/* Implementação das Funções de Gerenciamento */

bool CadastrarFilme(void) {
    static char temp_nome[256];
    static char temp_genero[256];
    static char temp_data[256];
    static char temp_classificacao_etaria[256];
    static char temp_nota[256];

    /* Banco cheio */
    if(quantidade_filmes == MAX_FILMES) {
        ReportarMensagem("O banco de dados está cheio! Apague algum filme para poder cadastrar mais.");
        return false;
    }

    /* Recebe nome */
    while(true) {
        MostrarTitulo("Primeiramente, insira o nome do filme (ou vazio para voltar): ");
        
        scanf("%[^\n]", temp_nome);
        LimparBuffer();
    
        if(strlen(temp_nome) == 0) {
            return false;
        }
        break;
    }

    /* Recebe genêro */
    while(true) {
        MostrarTitulo("Depois, insira o gênero do filme (ou vazio para voltar): ");
        
        scanf("%[^\n]", temp_genero);
        LimparBuffer();

        if(strlen(temp_genero) == 0) {
            return false;
        }
        break;
    }

    /* Recebe data */
    while(true) {
        MostrarTitulo("Agora, insira a data do filme (ou vazio para voltar): ");
        
        scanf("%[^\n]", temp_data);
        LimparBuffer();

        if(strlen(temp_data) == 0) {
            return false;
        }

        if(atoi(temp_data) < 1890 || atoi(temp_data) > 2999) {
            ReportarMensagem("A data do filme é inválida! A faixa válida é de 1890 a 2999.");
            continue;
        }
        break;
    }

    /* Recebe classificação */
    while(true) {
        MostrarTitulo("Agora, insira a classificação etária do filme (ou vazio para voltar): ");
        
        scanf("%[^\n]", temp_classificacao_etaria);
        LimparBuffer();

        if(strlen(temp_classificacao_etaria) == 0) {
            return false;
        }

        if(atoi(temp_classificacao_etaria) < 0 || atoi(temp_classificacao_etaria) > 24) {
            ReportarMensagem("A classificação etária do filme é inválida! A faixa válida é de 0 a 24 anos.");
            continue;
        }
        break;
    }

    /* Recebe nota */
    while(true) {
        MostrarTitulo("Por fim, insira a nota do filme (ou vazio para voltar): ");
        
        scanf("%[^\n]", temp_nota);
        LimparBuffer();

        if(strlen(temp_nota) == 0) {
            return false;
        }

        if(atoi(temp_nota) < 0 || atoi(temp_nota) > 10) {
            ReportarMensagem("A nota do filme é inválida! A faixa válida é de 0 a 10.");
            continue;
        }
        break;
    }
    
    /* Verifica Erro ao adicionar na lista */
    if(!AdicionarFilmeLista(temp_nome, temp_genero, temp_data, temp_classificacao_etaria, temp_nota)) {
        ReportarMensagem("Houve um erro ao cadastrar o filme no banco de dados interno. Contate o administrador.");
        return false;
    }
    
    /* Erro ao adicionar no banco */
    if(!SalvarDados()) {
        ReportarMensagem("Houve um erro ao atualizar os dados no banco de dados externo. Contate o administrador.");
    }
    
    MostrarTitulo("Filme cadastrado com sucesso!\n");
    EsperarEnter();

    strcpy(temp_nome, "");
    strcpy(temp_genero, "");
    strcpy(temp_data, "");
    strcpy(temp_classificacao_etaria, "");
    strcpy(temp_nota, "");

    return true;
}

bool RemoverFilme(void) {
    static char temp_id[MAX_SIZE];

    MostrarTitulo(NULL);

    /* Banco vazio */
    if(quantidade_filmes == 0) {
        ReportarMensagem("O banco de dados está vazio!");
        return false;
    }

    /* Recebe ID */
    while(true) {
        MostrarTitulo("Insira o ID do filme a remover (ou vazio para voltar): ");
        
        /* Limpando a string temporaria */
        strcpy(temp_id, "");

        scanf("%[^\n]", temp_id);
        LimparBuffer();

        if(strlen(temp_id) == 0) {
            return false;
        }

        if(!VerificaStringNumerica(temp_id) || atoi(temp_id) < 0 || atoi(temp_id) >= quantidade_filmes) {
            ReportarMensagem("O ID inserido é inválido!");
            continue;
        }
        break;
    }

    if(!RemoverFilmeLista(atoi(temp_id))) {
        ReportarMensagem("Filme inexistente!");
        return false;
    }

    if(!SalvarDados()) {
        ReportarMensagem("Houve um erro ao atualizar os dados no banco de dados externo. Contate o administrador.");
        return false;
    }

    MostrarTitulo("Filme removido com sucesso!\n");
    EsperarEnter();

    return true;
}

/* Implementação das Funções do Banco de Dados */

bool SalvarDados(void) {
    /* Abre o arquivo do banco */
    FILE* arquivo_banco = fopen(NOME_ARQUIVO_BANCO, "w+");

    /* Verifica se existe */
    if(!arquivo_banco) {
        return false;
    }

    /* Imprime os filmes no arquivo */
    for(int i = 0; i < quantidade_filmes; i++) {
        fprintf(
            arquivo_banco,
            "%s;%s;%s;%s;%s\n", 
            lista_filmes[i][0], 
            lista_filmes[i][1], 
            lista_filmes[i][2], 
            lista_filmes[i][3], 
            lista_filmes[i][4]
        );
    }

    fclose(arquivo_banco);

    return true;
}

bool CarregarDados(void) {
    static char temp_linha[2*MAX_SIZE];

    /* Abre o arquivo do banco */
    FILE* arquivo_banco = fopen(NOME_ARQUIVO_BANCO, "r");

    /* Verifica se existe */
    if(!arquivo_banco) {
        return false;
    }

    /* Pega linha por linha do arquivo ... */
    while(fgets(temp_linha, 2*MAX_SIZE, arquivo_banco)) {
        /* Obtém os campos da linha */
        sscanf(
            temp_linha,
            "%[^;];%[^;];%[^;];%[^;];%[^\n]",
            lista_filmes[quantidade_filmes][0],     /* Nome */
            lista_filmes[quantidade_filmes][1],     /* Genero */
            lista_filmes[quantidade_filmes][2],     /* Data */
            lista_filmes[quantidade_filmes][3],     /* Classificação */
            lista_filmes[quantidade_filmes][4]      /* Nota */
        );
        quantidade_filmes++;
    }

    fclose(arquivo_banco);

    return true;
}

/* Implementação das Funções Utilitárias */

void MostrarTitulo(const char subtitulo[]) {
    LimparTela();
    printf(
        "\033[1;40m\t\t\t\t\t---------------\n"
        "\t\t\t\t\t|   \033[1;31mWebflix\033[0m\033[1;40m   |\n"
        "\t\t\t\t\t---------------\033[0m\n"       
    );
    if(subtitulo) {
        printf("\n\033[1;40m%s\033[0m", subtitulo);
    }
}

void ReportarMensagem(const char mensagem[]) {
    printf("\n%s\n", mensagem);
    EsperarEnter();
}

void LimparTela(void) {
    printf("\033[H\033[J");
}

void LimparBuffer(void) {
    while(getchar() != '\n');
}

void EsperarEnter(void) {
    printf("\nPressione ENTER para continuar...");
    while(getchar() != '\n');
}

bool VerificaStringNumerica(const char numero[]) {
    for(int i = 0; numero[i] != '\0'; i++) {
        if(numero[i] < '0' || numero[i] > '9') {
            return false;
        }
    }
    return true;
}
