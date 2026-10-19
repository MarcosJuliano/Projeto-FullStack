#include <stdio.h>
#include "mongoose.h"
#include "banco.h"

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

    struct mg_http_serve_opts opcoes = {
        .root_dir = "/tmp/estudo-frontend"
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
