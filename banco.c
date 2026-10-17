#include <stdio.h>
#include <sqlite3.h>
#include "banco.h"

int banco_iniciar()
{
    sqlite3 *db;

    int resultado = sqlite3_open("alunos.db", &db);

    if (resultado != SQLITE_OK)
    {
        printf("Erro ao abrir o banco: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    printf("Banco conectado com sucesso!\n");

    const char *sql =
        "CREATE TABLE IF NOT EXISTS alunos ("
        "matricula INTEGER PRIMARY KEY,"
        "nome TEXT NOT NULL,"
        "nota REAL NOT NULL"
        ");";

    char *mensagem_erro = NULL;

    resultado = sqlite3_exec(
        db,
        sql,
        NULL,
        NULL,
        &mensagem_erro
    );

    if (resultado != SQLITE_OK)
    {
        printf("Erro ao criar tabela: %s\n", mensagem_erro);

        sqlite3_free(mensagem_erro);
        sqlite3_close(db);

        return 0;
    }

    printf("Tabela criada/verificada com sucesso!\n");

    sqlite3_close(db);

    return 1;
}

int banco_inserir_alunos(int matricula, const char *nome, float nota){
    sqlite3 *db = NULL; // A variável db é um ponteiro para a conexão com o banco de dados.

    // Abre o BD
    int resultado = sqlite3_open("alunos.db", &db); // essa função faz abrir o arquivo e recebe um valor

    if(resultado != SQLITE_OK){
        printf("ERRO: erro ao abrir o banco: %s\n",
        db ? sqlite3_errmsg(db) : "Erro desconhecido");

        if(db != NULL){
            sqlite3_close(db);
        }

        return 0;
    }

    // Preparar os comandos SQL
    const char *sql = 
    "INSERT INTO alunos (matricula, nome, nota) "
    "VALUES (?, ?, ?);";

    sqlite3_stmt *stmt = NULL; // stmt é uma statement do SQLite, isto é, uma instrução SQL preparada para execução.

    resultado = sqlite3_prepare_v2(
        db, // conexão usada
        sql, // qual comando sera preparado
        -1, // o tamanho do texto sera definido automaticamente
        &stmt, // onde sera colocada as intruções
        NULL 
    );

    if(resultado != SQLITE_OK){
        printf("ERRO: erro ao preparar o INSERT: %s\n",
        sqlite3_errmsg(db));

        sqlite3_close(db);
        return 0;
    }

    // Adicionar os valores lidos pelo teste.c para a interrogação
    resultado = sqlite3_bind_int(stmt, 1, matricula); // Matricula

    if(resultado == SQLITE_OK){ // Nome
        resultado = sqlite3_bind_text(
            stmt, 2, nome, -1, SQLITE_TRANSIENT
        );
    }

    if(resultado == SQLITE_OK){ // Nota
        resultado = sqlite3_bind_double(stmt, 3, nota);
    }

    if(resultado != SQLITE_OK){
        printf("ERRO: erro ao adicionar os valores: %s\n",
               sqlite3_errmsg(db));
        
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // apos todo passos acima, aqui o stmt tem que voltar SQLITE_DONE
    resultado = sqlite3_step(stmt);

    if(resultado != SQLITE_DONE){
        printf("ERRO: erro ao adicionar aluno %s\n",
                sqlite3_errmsg(db));

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Liberar os recursos
    sqlite3_finalize(stmt); // Essa função finaliza a instrução preparada e libera os recursos que o SQLite reservou para ela.
    sqlite3_close(db); // Essa função fecha a conexão com o banco de dados e libera os recursos associados a ela.

    printf("Aluno gravado no banco com sucesso!\n");
    return 1;

}

void banco_listar_alunos(void){
    sqlite3 *db = NULL;

    // Abre o BD
    int resultado = sqlite3_open("alunos.db", &db);

    if(resultado != SQLITE_OK){
        printf("Erro ao abrir o banco: %s\n",
        db ? sqlite3_errmsg(db) : "Erro desconhecido");

        if (db != NULL)
        {
            sqlite3_close(db);
        }
        
        return;
    }

    // Escrever a consulta para o SQL entender
    const char *sql = "SELECT matricula, nome, nota FROM alunos;";

    // Prepara a consulta
    sqlite3_stmt *stmt = NULL;

    resultado = sqlite3_prepare_v2(
        db, sql, -1, &stmt, NULL
    );

    if(resultado != SQLITE_OK){
        printf("Erro ao preparar consulta: %s\n",
        sqlite3_errmsg(db));

        sqlite3_close(db);
        return;
    }

    // Executa a consulta e mostra os resultados
    int encontrou = 0;

    printf("\n=== Alunos Cadastrados ===\n");

    while((resultado = sqlite3_step(stmt)) == SQLITE_ROW){
        int matricula = sqlite3_column_int(stmt, 0);

        const unsigned char *nome = sqlite3_column_text(stmt, 1);

        double nota = sqlite3_column_double(stmt, 2);

        printf("Matricula: %d | Nome: %s | Nota: %.2f\n", matricula, nome, nota);

        encontrou = 1;
    }

    if (!encontrou && resultado == SQLITE_DONE)
    {
       printf("Nenhum aluno cadastrado.\n");
    }

    if (resultado != SQLITE_DONE)
    {
        printf("Erro ao consultar alunos: %s\n", sqlite3_errmsg(db));
    }

    // Liberar os recursos
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

int banco_buscar_aluno(int matricula, char nome[100], double *nota){
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;

    // Abrir o banco
    int resultado = sqlite3_open("alunos.db", &db);

    if (resultado != SQLITE_OK){
        printf("ERRO: erro ao abrir o banco: %s\n", db ? sqlite3_errmsg(db) : "Erro desconhecido");

        if(db != NULL){
            sqlite3_close(db);
        }

        return -1;
    }

    // Preparar a consulta 
    const char *sql = "SELECT nome, nota FROM alunos WHERE matricula = ?;"; // seleciona a nota e nome da lista dependendo da materia que vai receber

    resultado = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if(resultado != SQLITE_OK){
        printf("ERRO: erro na consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // Preencher o parametro com a matricula 
    resultado = sqlite3_bind_int(stmt, 1, matricula);

    if(resultado != SQLITE_OK){
        printf("ERRO: erro ao informar a matricula: %s\n",
                sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    // Executa a consulta
    resultado = sqlite3_step(stmt);

    if(resultado == SQLITE_ROW){
        const unsigned char *nome_banco = sqlite3_column_text(stmt, 0);

        *nota = sqlite3_column_double(stmt, 1);

        snprintf(nome, 100, "%s", (const char *)nome_banco);

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        return 1;
    }

    if(resultado != SQLITE_DONE){
        printf("ERRO: erro ao buscar alunos: %s\n", sqlite3_errmsg(db));

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    // Se a consulta terminar sem a matricula
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}