#ifndef BPTREE_H
#define BPTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// A ordem da árvore define o número máximo de chaves/filhos por nó.

#define M 255

// Estrutura que será armazenada nos nós folha.
// Contém a chave e um ponteiro para a posição do registro no arquivo original.
typedef struct {
    long long chave;
    long byte_offset; // Posição do registro em 'registros.dat'
} Par;

// Estrutura do nó da Árvore B+
typedef struct No {
    bool eh_folha;
    int num_chaves;
    long long chaves[M - 1]; // Chaves para guiar a busca
    struct No* filhos[M];    // Ponteiros para os filhos (em nós internos)
    Par pares[M];            // Pares chave/ponteiro (em nós folha)
    struct No* proximo;      // Ponteiro para o próximo nó folha (para acesso sequencial)
} No;

// Protótipos das Funções
No* criar_no_folha();
No* criar_arvore();
void insere(No** raiz, Par p);

#endif 