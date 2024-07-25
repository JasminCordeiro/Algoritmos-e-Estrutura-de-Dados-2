#include "funcionarios.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

int comparacoes_sequencial = 0;
int comparacoes_binaria = 0;


// Cria funcionario.
TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento, double salario) {
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    //inicializa espaco de memoria com ZEROS
    if (func) memset(func, 0, sizeof(TFunc));
    //copia valores para os campos de func
    func->cod = cod;
    strcpy(func->nome, nome);
    strcpy(func->cpf, cpf);
    strcpy(func->data_nascimento, data_nascimento);
    func->salario = salario;
    return func;
}


// Salva funcionario no arquivo out, na posicao atual do cursor
void salva(TFunc *func, FILE *out) {
    fwrite(&func->cod, sizeof(int), 1, out);
    //func->nome ao inves de &func->nome, pois string ja eh um ponteiro
    fwrite(func->nome, sizeof(char), sizeof(func->nome), out);
    fwrite(func->cpf, sizeof(char), sizeof(func->cpf), out);
    fwrite(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), out);
    fwrite(&func->salario, sizeof(double), 1, out);
}


// Le um funcionario do arquivo  na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TFunc *le(FILE *in) {
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    if (0 >= fread(&func->cod, sizeof(int), 1, in)) {
        free(func);
        return NULL;
    }
    fread(func->nome, sizeof(char), sizeof(func->nome), in);
    fread(func->cpf, sizeof(char), sizeof(func->cpf), in);
    fread(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), in);
    fread(&func->salario, sizeof(double), 1, in);
    return func;
}


// Imprime funcionario
void imprime(TFunc *func) {
    printf("**********************************************");
    printf("\nFuncionario de codigo ");
    printf("%d", func->cod);
    printf("\nNome: ");
    printf("%s", func->nome);
    printf("\nCPF: ");
    printf("%s", func->cpf);
    printf("\nData de Nascimento: ");
    printf("%s", func->data_nascimento);
    printf("\nSalario: ");
    printf("%4.2f", func->salario);
    printf("\n**********************************************");
}


// Retorna tamanho do funcionario em bytes
int tamanho_registro() {
    return sizeof(int)  //cod
           + sizeof(char) * 50 //nome
           + sizeof(char) * 15 //cpf
           + sizeof(char) * 11 //data_nascimento
           + sizeof(double); //salario
}


// retorna a quantidade de registros no arquivo
int qtdRegistros(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanho_registro());
    return tam;
}

// Cria a base de dados ordenada pelo codigo do funcionario
void criarBaseOrdenada(FILE *out, int tam){

    int vet[tam];
    TFunc *f;

    for(int i=0;i<tam;i++)
        vet[i] = i+1;

    printf("\nGerando a base de dados...\n");

    for (int i=0;i<tam;i++){
        f = funcionario(vet[i], "A", "000.000.000-00", "01/01/1980", 3000*i);
        salva(f, out);
    }

     free(f);
}


//embaralha base de dados
void embaralha(int *vet,int max,int trocas) {
    srand(time(NULL));
    for (int i = 0; i <= trocas; i++) {
        int j = rand() % (max-1);
        int k = rand() % (max-1);
        int tmp = vet[j];
        vet[j] = vet[k];
        vet[k] = tmp;
    }
}

// Cria a base de dados desordenada pelo c�digo do funcion�rio
void criarBaseDesordenada(FILE *out, int tam, int qtdTrocas){

    int vet[tam];
    TFunc *f;

    for(int i=0;i<tam;i++)
        vet[i] = i+1;

    embaralha(vet,tam,qtdTrocas);

    printf("\nGerando a base de dados...\n");

    for (int i=0;i<tam;i++){
        f = funcionario(vet[i], "A", "000.000.000-00", "01/01/1980", 3000*i);
        salva(f, out);
    }

    free(f);

}


TFunc *busca(FILE *in, int tam, int cod ){
    for(int i = 0; i < tam; i++){
        fseek(in,tamanho_registro()*i,SEEK_SET);
        TFunc *f = le(in);
        if(f->cod == cod){
            return f;
        }
    }
    return NULL;
}

TFunc* buscaBinaria(int cod, FILE* in, int inicio, int fim) {
     comparacoes_binaria = 0;  // Inicialize as comparações, caso necessário

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        fseek(in, meio * tamanho_registro(), SEEK_SET);

        TFunc *f = le(in);
        comparacoes_binaria++;

        if (f->cod == cod) {
            return f;
        }

        if (f->cod > cod) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
        free(f);
    }
    return NULL;
}


TFunc *buscaSequencial(int chave, FILE *in){

    TFunc *f;
    int achou;
    rewind(in);

    while ((f = le(in)) != NULL){
        comparacoes_sequencial++;
        if(f->cod == chave){
           //return f;
           achou = 1;
           break;
        }
    }
        if(achou == 1)
            return f;
        else return printf("Funcionario nao encontrado");

        free(f);
}


void imprimirBase(FILE *out){

printf("\nImprimindo a base de dados...\n");

    rewind(out);
    TFunc *f;

    while ((f = le(out)) != NULL)
        imprime(f);

    free(f);

}

void insertionSort(FILE *arq, int tam) {
    int i;
    //faz o insertion sort
    for (int j = 2; j <= tam; j++) {
        //posiciona o arquivo no registro j
        fseek(arq, (j-1) * tamanho_registro(), SEEK_SET);
        TFunc *fj = le(arq);
        i = j - 1;
        //posiciona o cursor no registro i
        fseek(arq, (i-1) * tamanho_registro(), SEEK_SET);
        TFunc *fi = le(arq);
        while ((i > 0) && (fi->cod > fj->cod)) {
            //posiciona o cursor no registro i+1
            fseek(arq, i * tamanho_registro(), SEEK_SET);
            salva(fi, arq);
            i = i - 1;
            //lê registro i
            fseek(arq, (i-1) * tamanho_registro(), SEEK_SET);
            fi = le(arq);
        }
        //posiciona cursor no registro i + 1
        fseek(arq, (i) * tamanho_registro(), SEEK_SET);
        //salva registro j na posição i
        salva(fj, arq);
    }
    //descarrega o buffer para ter certeza que dados foram gravados
    fflush(arq);
}
