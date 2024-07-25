#ifndef FUNCIONARIOS_H_INCLUDED
#define FUNCIONARIOS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>


typedef struct Funcionario {
    int cod;
    char nome[50];
    char cpf[15];
    char data_nascimento[11];
    double salario;
} TFunc;


// Cria funcionario.
TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento, double salario);


// Salva funcionario no arquivo out, na posicao atual do cursor
void salva(TFunc *func, FILE *out);


// Le um funcionario do arquivo na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TFunc *le(FILE *in);


// Imprime funcionario
void imprime(TFunc *func);


// Retorna tamanho do funcionario em bytes
int tamanho_registro();


// retorna a quantidade de registros no arquivo
int qtdRegistros(FILE *arq);


// Cria a base de dados ordenada pelo c�digo do funcion�rio
void criarBaseOrdenada(FILE *out, int tam);


//embaralha base de dados
void embaralha(int *vet,int MAX,int trocas);

// Cria a base de dados desordenada pelo c�digo do funcion�rio
void criarBaseDesordenada(FILE *out, int tam, int qtdTrocas);

TFunc* buscaSequencial(int chave, FILE* in);


TFunc* buscaBinaria(int cod, FILE* in, int inicio, int fim);
// Imprime a base de dados
void imprimirBase(FILE *out);

void insertionSort(FILE *arq, int tam);

#endif // FUNCIONARIOS_H_INCLUDED
