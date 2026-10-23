#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#include "mongoose.h"
#include "banco.h"

static char frontend_dir[PATH_MAX];

static void tratar_requisicao(struct mg_connection *c,
                              int ev,
                              void *ev_data)
{
    if (ev != MG_EV_HTTP_MSG) {
        return;
    }

    struct mg_http_message *hm = ev_data;

    if (mg_match(hm->uri, mg_str("/api/hello"), NULL)) {
        mg_http_reply(
            c,
            200,
            "Content-Type: application/json\r\n",
            "{\"mensagem\":\"Servidor C funcionando!\"}\n"
        );
        return;
    }

    if (mg_match(hm->uri, mg_str("/api/alunos"), NULL)) {
        // Reserva memória para montar o JSON, buffer tem capacidade para 65536 bytes.
        char json[65536];

        // Consulta o banco e escreve os alunos no buffer.
        int sucesso = banco_listar_alunos_json(json,sizeof(json));

        // Se a consulta ou a montagem do JSON falhar, devolve um erro HTTP 500.
        if (!sucesso) {
            mg_http_reply(
                c,
                500,
                "Content-Type: application/json\r\n",
                "{\"erro\":\"Falha ao listar alunos\"}\n"
            );
            return;
        }

        // Envia o JSON ao cliente com status HTTP 200.
        mg_http_reply(
            c,
            200,
            "Content-Type: application/json; charset=utf-8\r\n",
            "%s\n",
            json
        );

        return;
    }

    struct mg_http_serve_opts opcoes = {
        .root_dir = frontend_dir
    };

    if (mg_match(hm->uri, mg_str("/"), NULL)) {
        mg_http_serve_file(
            c,
            hm,
            "../frontend/index.html",
            &opcoes
        );
        return;
    }

    mg_http_serve_dir(c, hm, &opcoes);
}

int main(void)
{

    if (realpath("../frontend", frontend_dir) == NULL) {
        perror("Erro ao localizar a pasta frontend");
        return 1;
    }

    printf("Frontend localizado em: %s\n", frontend_dir);

    if (banco_iniciar() != 1) {
        fprintf(stderr, "Falha ao inicializar o banco.\n");
        return 1;
    }

    struct mg_mgr gerenciador;
    mg_mgr_init(&gerenciador);

    struct mg_connection *servidor =
        mg_http_listen(
            &gerenciador,
            "http://127.0.0.1:8000",
            tratar_requisicao,
            NULL
        );

    if (servidor == NULL) {
        fprintf(stderr, "Erro ao iniciar o servidor HTTP.\n");
        mg_mgr_free(&gerenciador);
        return 1;
    }

    printf("Servidor iniciado em http://127.0.0.1:8000\n");

    for (;;) {
        mg_mgr_poll(&gerenciador, 1000);
    }

    mg_mgr_free(&gerenciador);
    return 0;
}
