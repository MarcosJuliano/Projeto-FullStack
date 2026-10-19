// TAD Aluno simples (conceito de regras e struct)
#include <stdio.h>
#include "banco.h"

// Apartir do momento que eu uso o BD a struct se torna menos usual

// typedef struct {
//     int matricula;
//     char nome[100];
//     float nota;
// } Aluno;

// int validar_matricula(Aluno a[], int quant, int matricula){
//     for (int i = 0; i < quant; i++)
//     {
//         if(a[i].matricula == matricula){
//             return 1; // retorna verdade e manda repetir
//         }
//     }
//     return 0; //retorna falso e n faz nada
// }

// Operação com regra de negócio
void atribuir_nota() {

    int matricula;
    double nota;
    char nome[100];
    char add = 'S';

    while (add == 'S')
    {

        printf("\n################################\n");
        printf("Qual a matricula do aluno? ");
        scanf("%d", &matricula);
        
        //a[*quant].matricula = matricula;

        // if(validar_matricula(a, *quant, matricula)){
        //     printf("ERRO: matricula ja esta cadastrada!\n");
        //     continue;
        // }

        printf("Qual o nome do aluno? ");
        scanf("%99s", nome);
        printf("Qual a nota do aluno? ");
        scanf("%lf", &nota);
        printf("################################\n");
        
        // printf("Qual o nome do aluno? ");
        // scanf("%s", a[*quant].nome);
        // printf("Qual a nota do aluno? ");
        // scanf("%f", &a[*quant].nota);
        // printf("################################\n");

        //Gravar os alunos no BD
        if(!banco_inserir_alunos(matricula, nome, nota)){
            printf("Falha ao gravar o aluno no banco.\n");
            
            printf("\nDeseja Continuar? ['S' para sim/'N' para nao]: ");
            scanf(" %c", &add);
            if(add == 'N'){
                break;
            } else {
                continue;
            }
        }
        
        printf("\nDeseja adicionar mais um aluno? ['S' para sim/'N' para nao]: ");
        scanf(" %c", &add);

        if(add == 'S'){
            //(*quant)++;
            printf("Aluno cadastrado com sucesso!\n");
        }

    }

    // if (nova_nota >= 0.0 && nova_nota <= 10.0) { // Regra do TAD
    //     a->nota = nova_nota;
    //     return 1; // Sucesso
    // }
    // return 0; // Nota inválida recusada
    printf("\n==================================================");
    printf("\n======== PROCESSO FINALIZADO COM SUCESSO! ========");
    printf("\n==================================================\n");
    //return 1;
}

// função para remover um aluno da lista
void remover_nota(void){
   int matricula;
   int resultado;
   char remover_outro = 'S';

    while (remover_outro == 'S')
    {
        printf("\nDeseja remover a nota de qual aluno [informe a matricula] ? ");
        scanf("%d", &matricula);

        resultado = banco_remover_aluno(matricula);

        if (resultado == 1){
            printf("\n=====================================================\n");
            printf("Nota removida com sucesso!\n");
            printf("Nota da matricula: %d removida.", matricula);
            printf("\n=====================================================\n");
        } else if (resultado == 0) {
            printf("\n=====================================================\n");
            printf("Nenhum aluno encontrado com esta matricula!");
            printf("\n=====================================================\n");
        } else {
            printf("não foi possivel remover o auluno devido a um erro.\n");
        }
        
        printf("\nDeseja remover outro aluno? ['S' para sim/'N' para nao]: ");
        scanf(" %c", &remover_outro);
    }
}

void modificar_nota(){
    int matricula;
    double nota;
    char mod_outro = 'S';

    while (mod_outro == 'S')
    {
        printf("\nDeseja Modificar as notas de qual aluno [informe a matricula] ? ");
        scanf("%d", &matricula);

        printf("\nQual nota nova voce quer adicionar? ");
        scanf("%lf", &nota);

        int encontrou = banco_modificar_aluno(matricula, &nota);

        if (encontrou == 1)
        {
            printf("\n==================================================\n");
            printf("O aluno com matricula: %d teve sua nota atualizada para: %.2f", matricula,nota);
            printf("\n==================================================\n");
        } else if(encontrou == 0){
            printf("\n=====================================================\n");
            printf("Matricula invalida.");
            printf("\n=====================================================\n");
        } else {
            printf("\n=====================================================\n");
            printf("Não foi possivel consultar o banco de dados.");
            printf("\n=====================================================\n");
        }
            
        printf("\nDeseja alterar a nota de outro aluno? ['S' para sim/'N' para nao]: ");
        scanf(" %c", &mod_outro);
    }

    printf("\n==================================================");
    printf("\n======== PROCESSO FINALIZADO COM SUCESSO! ========");
    printf("\n==================================================\n");
}
    
// imprimir para ver se deu certo a struct
// OBS.: antes tinha que passar parametros por conta da struct que armazenava os dados, agora n precisa pq fica tudo no BD
void imprimir(){

    int matricula;
    char nome[100];
    double nota;
    char ver_outro = 'S';

    while (ver_outro == 'S')
    {

        printf("\nDeseja ver as notas de qual aluno [informe a matricula] ? ");
        scanf("%d", &matricula);

        int encontrou = banco_buscar_aluno(matricula, nome, &nota);

        if (encontrou == 1)
        {
            printf("\n==================================================\n");
            printf("O aluno %s tem a nota: %.2f", nome, nota);
            printf("\n==================================================\n");
        } else if(encontrou == 0){
            printf("\n=====================================================\n");
            printf("Matricula invalida, tente outra ou encerre o processo.");
            printf("\n=====================================================\n");
        } else {
            printf("\n=====================================================\n");
            printf("Não foi possivel consultar o banco de dados.");
            printf("\n=====================================================\n");
        }
            
        printf("\nDeseja ver outro aluno? ['S' para sim/'N' para nao]: ");
        scanf(" %c", &ver_outro);
    }

    printf("\n==================================================");
    printf("\n======== PROCESSO FINALIZADO COM SUCESSO! ========");
    printf("\n==================================================\n");
}

int main(void){

    //Aluno alunos[100];
    //int quant = 0;
    int opc = 0;
    
    banco_iniciar();

    printf("==== Oque vc deseja fazer? ====\n 1 - adicionar a nota de um aluno\n 2 - ver a nota do aluno\n 3 - ver todas notas\n 4 - modificar a nota de um aluno\n 5 - apagar a nota de um aluno\n 6 - terminar o processo\n opção: ");
    scanf("%d", &opc);

    switch (opc)
    {
    case 1: // Adicionar nota
        atribuir_nota(); 
        break;
    case 2: // Verificar notas
        imprimir();
        break;
    case 3:
        banco_listar_alunos();
        break;
    case 4: // Modificar nota
        modificar_nota();
        break;
    case 5: // Excluir notas
        remover_nota();
        break;
    case 6: // Sair do programa
        printf("\n================================================");
        printf("\n============= Processo finalizado. =============");
        printf("\n================================================\n");
        break;
    default: // valor invalido
        printf("\n=================================================");
        printf("\n============ ERRO: opção invalida!!! ============");
        printf("\n=================================================\n");
        break;
    }
    
    return 0;
}
