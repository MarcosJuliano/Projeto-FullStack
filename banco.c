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