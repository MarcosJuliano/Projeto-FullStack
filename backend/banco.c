#include <stdio.h>
#include <sqlite3.h>
#include "banco.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>

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

    printf("\n======= Alunos Cadastrados =======\n");

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

int banco_remover_aluno(int matricula, char *nome){
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;

    int resultado = sqlite3_open("alunos.db", &db);

    if (resultado != SQLITE_OK)
    {
        printf("ERRO: erro ao abrir o banco: %s\n",
        db ? sqlite3_errmsg(db) : "Erro desconhecido");

        if (db != NULL)
        {
            sqlite3_close(db);
        }

        return -1;
    }

    // Consulta o nome do aluno para exibir na mensagem de sucesso
    const char *sql_select = "SELECT nome FROM alunos WHERE matricula = ?;";
    resultado = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);

    if (resultado == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, matricula);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            if (sqlite3_column_text(stmt, 0) != NULL && nome != NULL) {
                snprintf(nome, 100, "%s", (const char *)sqlite3_column_text(stmt, 0));
            }
        }
        sqlite3_finalize(stmt);
    }

    const char *sql = "DELETE FROM alunos WHERE matricula = ?;";

    resultado = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if(resultado != SQLITE_OK){
        printf("Erro ao preparar a exclusao: %s\n",
        sqlite3_errmsg(db));

        sqlite3_close(db);
        return -1;
    }

    resultado = sqlite3_bind_int(stmt, 1, matricula);

    if (resultado != SQLITE_OK)
    {
        printf("Erro ao informar a matricula: %s\n",
        sqlite3_errmsg(db));

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }
    
    resultado = sqlite3_step(stmt);

    if(resultado != SQLITE_DONE){
        printf("Erro ao remover aluno: %s\n",
        sqlite3_errmsg(db));

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    int removido = sqlite3_changes(db);

    //sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (removido > 0)
    {
        return 1;
    }
    
    return 0;
}

int banco_modificar_aluno(int matricula, double *nota, char *nome){
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;

    int resultado = sqlite3_open("alunos.db", &db);

    if (resultado != SQLITE_OK)
    {
        printf("ERRO: erro ao abrir o banco: %s\n",
        db ? sqlite3_errmsg(db) : "Erro desconhecido");

        if (db != NULL)
        {
            sqlite3_close(db);
        }

        return -1;
    }

    const char *sql = 
    "UPDATE alunos SET nota = ? WHERE matricula = ?;";

    resultado = sqlite3_prepare_v2(
        db, // conexão usada
        sql, // qual comando sera preparado
        -1, // o tamanho do texto sera definido automaticamente
        &stmt, // onde sera colocada as intruções
        NULL 
    );

    if(resultado != SQLITE_OK){
        printf("ERRO: erro ao preparar o UPDATE: %s\n",
        sqlite3_errmsg(db));

        sqlite3_close(db);
        return 0;
    }

    // Adicionar os valores lidos pelo teste.c para a interrogação
    resultado = sqlite3_bind_int(stmt, 2, matricula); // Matricula

    if(resultado == SQLITE_OK){ // Nota
        resultado = sqlite3_bind_double(stmt, 1, *nota);
    }

    if(resultado != SQLITE_OK){
        printf("ERRO: erro ao modificar os valores: %s\n",
               sqlite3_errmsg(db));
        
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // apos todo passos acima, aqui o stmt tem que voltar SQLITE_DONE
    resultado = sqlite3_step(stmt);

    if(resultado != SQLITE_DONE){
        printf("ERRO: erro ao modificar a nota%s\n",
                sqlite3_errmsg(db));

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Consulta o nome do aluno para exibir na mensagem de sucesso
    const char *sql_select = "SELECT nome FROM alunos WHERE matricula = ?;";
    resultado = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);

    if (resultado == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, matricula);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            if (sqlite3_column_text(stmt, 0) != NULL && nome != NULL) {
                snprintf(nome, 100, "%s", (const char *)sqlite3_column_text(stmt, 0));
            }
        }
        sqlite3_finalize(stmt);
    }

    // Liberar os recursos
    // sqlite3_finalize(stmt); // Essa função finaliza a instrução preparada e libera os recursos que o SQLite reservou para ela.
    sqlite3_close(db); // Essa função fecha a conexão com o banco de dados e libera os recursos associados a ela.

    printf("Nota atualizada no banco com sucesso!\n");
    return 1;
}

// Adicionar o texto ao buffer sem ultrapassar o espaço que tem. Retorna 1 em sucesso e 0 se não tiver espaço suficiente.

static int adicionar_texto(
    char *buffer,
    size_t tamanho,
    size_t *posicao,
    const char *formato,
    ...
){
    if (*posicao >= tamanho)
    {
        return 0;
    }

    va_list argumentos;
    va_start(argumentos, formato);

    int escritos = vsnprintf(
        buffer + *posicao,
        tamanho - *posicao,
        formato,
        argumentos
    );

    va_end(argumentos);

    if (escritos < 0 || (size_t) escritos >= tamanho - *posicao)
    {
        return 0;
    }

    *posicao += (size_t) escritos;
    return 1;   
}

static int adicionar_string_json(
    char *buffer,
    size_t tamanho,
    size_t *posicao,
    const unsigned char *texto
){
    if (!adicionar_texto(buffer, tamanho, posicao, "\""))
    {
        return 0;
    }

    if (texto == NULL)
    {
        texto = (const unsigned char *) "";
    }
    
    for (const unsigned char *p = texto; *p != '\0'; p++)
    {
        switch (*p)
        {
        case '"':
            if (!adicionar_texto(buffer, tamanho, posicao, "\\\""))
            {
                return 0;
            }
            
            break;
        case '\\':
            if (!adicionar_texto(buffer,tamanho, posicao, "\\\\"))
            {
                return 0;
            }
            break;
        case '\f':
            if (!adicionar_texto(buffer, tamanho, posicao, "\\f"))
            {
                return 0;
            }
            break;
        case '\n':
            if (!adicionar_texto(buffer, tamanho, posicao, "\\n")) {
                return 0;
            }
            break;
        case '\r':
            if (!adicionar_texto(buffer, tamanho, posicao, "\\r")) {
                return 0;
            }
            break;
        case '\t':
            if (!adicionar_texto(buffer, tamanho, posicao, "\\t")) {
                return 0;
            }
            break;
        default:
            if (*p < 0x20)
            {
                if (!adicionar_texto(buffer,tamanho,posicao, "\\u%04x", *p))
                {
                    return 0;
                }
                
            }else{
                if (!adicionar_texto(buffer,tamanho,posicao, "%c", *p))
                {
                    return 0;
                }
            }
            
            break;
        }
    }
    
    return adicionar_texto(buffer, tamanho, posicao, "\"");
}

// Consultar todos alunos e escrever o resultado em JSON

int banco_listar_alunos_json(char *buffer, size_t tamanho){
    if (buffer == NULL || tamanho == 0)
    {
        return 0;
    }

    buffer[0] = '\0';

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;

    int resultado = sqlite3_open("alunos.db", &db);

    if (resultado != SQLITE_OK){
        fprintf(stderr, "ERRO: erro ao abrir o banco: %s\n",
                db != NULL ? sqlite3_errmsg(db) : "Erro desconhecido");
        if (db != NULL)
        {
            sqlite3_close(db);
        }
        return 0;
    }

    const char *sql = "SELECT matricula, nome, nota FROM alunos;";

    resultado = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (resultado != SQLITE_OK)
    {
        fprintf(stderr, "ERRO: erro ao preparar consulta: %s\n",
                sqlite3_errmsg(db));

        sqlite3_close(db);
        return 0;
    }
    
    size_t posicao = 0;
    int primeiro = 1;
    int sucesso = adicionar_texto(buffer, tamanho, &posicao, "[");

    while (sucesso && (resultado = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int matricula = sqlite3_column_int(stmt, 0);
        const unsigned char *nome = sqlite3_column_text(stmt, 1);
        double nota = sqlite3_column_double(stmt, 2);

        if (!primeiro)
        {
            sucesso = adicionar_texto(buffer, tamanho, &posicao, ",");
        }

        if (sucesso)
        {
            sucesso = adicionar_texto(buffer, tamanho, &posicao, "{\"matricula\":%d,\"nome\":", matricula);
        }
        
        if (sucesso)
        {
            sucesso = adicionar_string_json(buffer, tamanho, &posicao, nome);
        }

        if (sucesso)
        {
            sucesso = adicionar_texto(buffer, tamanho, &posicao, ",\"nota\":%.15g}", nota);
        }
        
        primeiro = 0;
        
    }

    if (resultado != SQLITE_DONE) {
        sucesso = 0;
    }

    if (sucesso) {
        sucesso = adicionar_texto(buffer, tamanho, &posicao, "]");
    }

    if (!sucesso) {
        buffer[0] = '\0';
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return sucesso ? 1 : 0;  
    
}