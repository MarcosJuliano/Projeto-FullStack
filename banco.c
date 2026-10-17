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
    sqlite3 *db = NULL;

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

    sqlite3_stmt *stmt = NULL;

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
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    printf("Aluno gravado no banco com sucesso!\n");
    return 1;

}

