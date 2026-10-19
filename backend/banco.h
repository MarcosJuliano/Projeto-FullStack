// Esse arquivo declara as funções para serem usadas em outros arquivos
#ifndef BANCO_H
#define BANCO_H

int banco_iniciar(void); // para iniciar o BD junto com programa

int banco_inserir_alunos(int matricula, const char *nome, float nota); // envia os dados coletados pelo C para o BD

void banco_listar_alunos(void); // mostra a lista de todos alunos

int banco_buscar_aluno(int matricula, char nome[100], double *nota);

int banco_remover_aluno(int matricula, char *nome);

int banco_modificar_aluno(int matricula, double *nota, char *nome);

#endif