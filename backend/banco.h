// Esse arquivo declara as funções para serem usadas em outros arquivos
#ifndef BANCO_H
#define BANCO_H
#include <stddef.h>

int banco_iniciar(void); // para iniciar o BD junto com programa

int banco_inserir_alunos(int matricula, const char *nome, float nota); // envia os dados coletados pelo C para o BD

void banco_listar_alunos(void); // mostra a lista de todos alunos
int banco_listar_alunos_json(char *buffer, size_t tamanho);
// char *buffer = área de memória onde a função escreverá o JSON.
// size_t tamanho = capacidade dessa área de memória, para evitar escrever além do espaço disponível.
// int = valor de retorno para indicar sucesso ou erro.

int banco_buscar_aluno(int matricula, char nome[100], double *nota);

int banco_remover_aluno(int matricula, char *nome);

int banco_modificar_aluno(int matricula, double *nota, char *nome);

#endif