const teste = document.querySelector("#testar");

async function testarConexao() {

    teste.addEventListener("click", async () => {
        const mensagem = document.querySelector("#mensagem");

        try {
            const resposta = await fetch("/api/hello");

            if (!resposta.ok) {
                throw new Error("Erro HTTP: " + resposta.status);
            }

            const dados = await resposta.json();
            mensagem.textContent = dados.mensagem;
        } catch (erro) {
            mensagem.textContent = "Erro ao conectar: " + erro.message;
        }
    });
 
}

testarConexao();