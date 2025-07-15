#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bplus.h"
#include "../gerador/gerador.h"

// PROTÓTIPOS INTERNOS
static void insere_no_interno(No* no, long long chave, No* filho_direito);
static void divide_no_interno(No* no, long long* chave_promovida, No** novo_no);
static void insere_na_folha(No* folha, Par p);
static void divide_no_folha(No* folha, long long* chave_promovida, No** novo_no);
No* criar_no_interno(void);
No* criar_no_folha(void);

// CRIAÇÃO DE NÓS
No* criar_no_interno(void) {
    No* no = (No*)calloc(1, sizeof(No));
    if (!no) {
        perror("Falha ao alocar no interno");
        exit(EXIT_FAILURE);
    }
    no->eh_folha = false;
    return no;
}

No* criar_no_folha(void) {
    No* no = (No*)calloc(1, sizeof(No));
    if (!no) {
        perror("Falha ao alocar no folha");
        exit(EXIT_FAILURE);
    }
    no->eh_folha = true;
    return no;
}

No* criar_arvore(void) {
    return criar_no_folha();
}

// INSERÇÃO

static void insere_na_folha(No* folha, Par p) {
    int i = folha->num_chaves - 1;
    while (i >= 0 && folha->pares[i].chave > p.chave) {
        folha->pares[i + 1] = folha->pares[i];
        i--;
    }
    folha->pares[i + 1] = p;
    folha->num_chaves++;
}

static void divide_no_folha(No* folha, long long* chave_promovida, No** novo_no) {
    *novo_no = criar_no_folha();
    int ponto_meio = (M + 1) / 2;

    for (int i = ponto_meio; i < M; i++) {
        (*novo_no)->pares[i - ponto_meio] = folha->pares[i];
        (*novo_no)->num_chaves++;
    }

    folha->num_chaves = ponto_meio;
    *chave_promovida = (*novo_no)->pares[0].chave;

    (*novo_no)->proximo = folha->proximo;
    folha->proximo = *novo_no;
}

static void insere_no_interno(No* no, long long chave, No* filho_direito) {
    int i = no->num_chaves - 1;
    while (i >= 0 && no->chaves[i] > chave) {
        no->chaves[i + 1] = no->chaves[i];
        no->filhos[i + 2] = no->filhos[i + 1];
        i--;
    }
    no->chaves[i + 1] = chave;
    no->filhos[i + 2] = filho_direito;
    no->num_chaves++;
}

static void divide_no_interno(No* no, long long* chave_promovida, No** novo_no) {
    *novo_no = criar_no_interno();
    int ponto_meio = (M - 1) / 2;
    *chave_promovida = no->chaves[ponto_meio];

    for (int i = ponto_meio + 1; i < M - 1; i++) {
        (*novo_no)->chaves[i - ponto_meio - 1] = no->chaves[i];
        (*novo_no)->num_chaves++;
    }
    for (int i = ponto_meio + 1; i < M; i++) {
        (*novo_no)->filhos[i - ponto_meio - 1] = no->filhos[i];
    }

    no->num_chaves = ponto_meio;
}

static No* insere_recursivo(No* no, Par p, long long* chave_promovida) {
    if (no->eh_folha) {
        insere_na_folha(no, p);
        if (no->num_chaves == M) {
            No* novo_no = NULL;
            divide_no_folha(no, chave_promovida, &novo_no);
            return novo_no;
        }
        return NULL;
    }

    int i = 0;
    while (i < no->num_chaves && p.chave >= no->chaves[i]) {
        i++;
    }

    No* novo_filho = insere_recursivo(no->filhos[i], p, chave_promovida);
    if (novo_filho) {
        insere_no_interno(no, *chave_promovida, novo_filho);
        if (no->num_chaves == M) {
            No* novo_no = NULL;
            long long nova_chave;
            divide_no_interno(no, &nova_chave, &novo_no);
            *chave_promovida = nova_chave;
            return novo_no;
        }
    }
    return NULL;
}

void insere(No** raiz, Par p) {
    if (*raiz == NULL)
        *raiz = criar_arvore();

    long long chave_promovida;
    No* novo_filho = insere_recursivo(*raiz, p, &chave_promovida);

    if (novo_filho) {
        No* nova_raiz = criar_no_interno();
        nova_raiz->chaves[0] = chave_promovida;
        nova_raiz->filhos[0] = *raiz;
        nova_raiz->filhos[1] = novo_filho;
        nova_raiz->num_chaves = 1;
        *raiz = nova_raiz;
    }
}

// IMPRESSÃO POR NÍVEL

typedef struct FilaNo {
    No* no;
    int nivel;
    struct FilaNo* prox;
} FilaNo;

void enfileirar(FilaNo** frente, FilaNo** tras, No* no, int nivel) {
    FilaNo* novo = (FilaNo*)malloc(sizeof(FilaNo));
    novo->no = no;
    novo->nivel = nivel;
    novo->prox = NULL;
    if (*tras) (*tras)->prox = novo;
    else *frente = novo;
    *tras = novo;
}

FilaNo* desenfileirar(FilaNo** frente) {
    if (!*frente) return NULL;
    FilaNo* temp = *frente;
    *frente = (*frente)->prox;
    return temp;
}

void imprimir_arvore_por_nivel(No* raiz) {
    if (!raiz) {
        printf("Arvore vazia.\n");
        return;
    }

    FilaNo* frente = NULL;
    FilaNo* tras = NULL;
    enfileirar(&frente, &tras, raiz, 0);

    int nivel_atual = -1;

    printf("\n===== Estrutura da Arvore B+ (por nivel) =====\n");
    while (frente) {
        FilaNo* atual = desenfileirar(&frente);

        if (atual->nivel != nivel_atual) {
            nivel_atual = atual->nivel;
            printf("\nNivel %d (%s):\n", nivel_atual, atual->no->eh_folha ? "Folhas" : "Interno");
        }

        if (atual->no->eh_folha) {
            printf("[");
            for (int i = 0; i < atual->no->num_chaves; i++) {
                printf("%lld", atual->no->pares[i].chave);
                if (i < atual->no->num_chaves - 1) printf(" ");
            }
            printf("] ");
        } else {
            printf("{");
            for (int i = 0; i < atual->no->num_chaves; i++) {
                printf("%lld", atual->no->chaves[i]);
                if (i < atual->no->num_chaves - 1) printf(" ");
            }
            printf("} ");
            for (int i = 0; i <= atual->no->num_chaves; i++) {
                enfileirar(&frente, &tras, atual->no->filhos[i], atual->nivel + 1);
            }
        }

        free(atual);
    }
    printf("\n");
}

// MAIN

int main() {
    FILE *f_registros = fopen("../registros.dat", "rb");
    if (!f_registros) {
        perror("Erro ao abrir 'registros.dat'");
        return 1;
    }

    No* raiz = NULL;
    printf("Iniciando construcao da Arvore B+ de ordem %d...\n", M);

    Registro reg;
    long offset = 0;
    int count = 0;

    while (fread(&reg, sizeof(Registro), 1, f_registros) == 1) {
        if (reg.ativo) {
            Par p = {reg.chave, offset};
            insere(&raiz, p);
            count++;
            if (count % 1000 == 0)
                printf("Processados %d registros...\n", count);
        }
        offset = ftell(f_registros);
    }

    fclose(f_registros);
    printf("\nConstrucao finalizada. %d registros inseridos na Arvore B+.\n", count);

    imprimir_arvore_por_nivel(raiz);
    printf("%d", count);

    return 0;
}
