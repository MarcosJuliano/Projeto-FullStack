// TAD Aluno simples (conceito de regras e struct)
#include <stdio.h>
#include "banco.h"

typedef struct {
    int matricula;
    char nome[100];
    float nota;
} Aluno;

int validar_matricula(Aluno a[], int quant, int matricula){
    for (int i = 0; i < quant; i++)
    {
        if(a[i].matricula == matricula){
            return 1; // retorna verdade e manda repetir
        }
    }
    return 0; //retorna falso e n faz nada
}

// Operação com regra de negócio
int atribui_nota(Aluno a[], int *quant) {

    char add = 'S';

    while (add == 'S')
    {

        int matricula;

        printf("\n################################\n");
        printf("Qual a matricula do aluno? ");
        scanf("%d", &matricula);

        if(validar_matricula(a, *quant, matricula)){
            printf("ERRO: matricula ja esta cadastrada!\n");
            continue;
        }

        a[*quant].matricula = matricula;

        printf("Qual o nome do aluno? ");
        scanf("%99s", a[*quant].nome);
        printf("Qual a nota do aluno? ");
        scanf("%f", &a[*quant].nota);
        printf("################################\n");
        
        // printf("Qual o nome do aluno? ");
        // scanf("%s", a[*quant].nome);
        // printf("Qual a nota do aluno? ");
        // scanf("%f", &a[*quant].nota);
        // printf("################################\n");

        //Gravar os alunos no BD
        if(!banco_inserir_alunos(a[*quant].matricula, a[*quant].nome, a[*quant].nota)){
            printf("Falha ao gravar o aluno no banco.\n");
            continue;
        }

        printf("Aluno cadastrado com sucesso!\n");

        (*quant)++;
        
        printf("\nDeseja adicionar mais um aluno? ['S' para sim/'N' para nao]: ");
        scanf(" %c", &add);

    }

    // if (nova_nota >= 0.0 && nova_nota <= 10.0) { // Regra do TAD
    //     a->nota = nova_nota;
    //     return 1; // Sucesso
    // }
    // return 0; // Nota inválida recusada
    printf("\n==================================================");
    printf("\n======== PROCESSO FINALIZADO COM SUCESSO! ========");
    printf("\n==================================================\n");
    return 1;
}

// imprimir para ver se deu certo a struct
void imprimir(Aluno a[], int quant){

    int matricula;
    char ver_outro = 'S';

    while (ver_outro == 'S')
    {

        printf("\nDeseja ver as notas de qual aluno [informe a matricula] ? ");
        scanf("%d", &matricula);

        int encontrou = 0;

        for (int i = 0; i < quant; i++)
        {  
            if (matricula == a[i].matricula)
            {
                printf("\n==================================================\n");
                printf("O aluno %s tem a nota: %.2f", a[i].nome, a[i].nota);
                printf("\n==================================================\n");

                encontrou = 1;
                break;
            }
        }

        if (!encontrou){
            {
                printf("\n=====================================================\n");
                printf("Matricula invalida, tente outra ou encerre o processo.");
                printf("\n=====================================================\n");
            }
        }
            
        printf("\nDeseja ver outro aluno? ['S' para sim/'N' para nao]: ");
        scanf(" %c", &ver_outro);
    }

    printf("\n==================================================");
    printf("\n======== PROCESSO FINALIZADO COM SUCESSO! ========");
    printf("\n==================================================\n");
}

int main(void){

    Aluno alunos[100];
    int quant = 0;
    
    banco_iniciar();

    atribui_nota(alunos, &quant); 
    imprimir(alunos, quant);
    
    return 0;
}
