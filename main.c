#include <stdio.h>
#include <stdlib.h>
#include "funcionarios.c"

int main() {
    // Cria a base de dados e popula com alguns registros
    FILE *out = fopen("funcionarios.dat", "wb+");
    if (out == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    criarBaseOrdenada(out, 500000);

    // Realiza busca sequencial
    int cod = 254819;
    clock_t inicio, fim;
    double tempo_gasto;

    inicio = clock();
    comparacoes_sequencial = 0;
    TFunc* func_seq = buscaSequencial(cod, out);
    fim = clock();
    tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    if (func_seq != NULL) {
        printf("Funcionario encontrado pela busca sequencial:\n");
        imprime(func_seq);
        free(func_seq);
    } else {
        printf("Funcionario nao encontrado pela busca sequencial.\n");
    }
    printf("Tempo gasto na busca sequencial: %f segundos\n", tempo_gasto);
    printf("Numero de comparacoes na busca sequencial: %d\n", comparacoes_sequencial);

    // Realiza busca binaria
    inicio = clock();
    comparacoes_binaria = 0;
    fseek(out, 0, SEEK_END);
    int tamanho = ftell(out) / tamanho_registro();
    TFunc* func_bin = buscaBinaria(cod, out, 0, tamanho - 1);
    fim = clock();
    tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    if (func_bin != NULL) {
        printf("Funcionario encontrado pela busca binaria:\n");
        imprime(func_bin);
        free(func_bin);
    } else {
        printf("Funcionario nao encontrado pela busca binaria.\n");
    }
    printf("\nTempo gasto na busca binaria: %f segundos\n", tempo_gasto);
    printf("\nNumero de comparacoes na busca binaria: %d\n", comparacoes_binaria);

    fclose(out);

    return 0;
}
