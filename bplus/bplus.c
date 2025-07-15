#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bplus.h"
#include "../gerador/gerador.h"

// ESTRUTURA DA FILA PARA IMPRESSÃO POR NÍVEL
typedef struct FilaNo {
    No* no;
    int nivel;
    struct FilaNo* prox;
} FilaNo;

// PROTÓTIPOS INTERNOS
static void insere_no_interno(No* no, long long chave, No* filho_direito);
static void divide_no_interno(No* no, long long* chave_promovida, No** novo_no);
static void insere_na_folha(No* folha, Par p);
static void divide_no_folha(No* folha, long long* chave_promovida, No** novo_no);
No* criar_no_interno(void);
No* criar_no_folha(void);
void enfileirar(FilaNo** frente, FilaNo** tras, No* no, int nivel);
FilaNo* desenfileirar(FilaNo** frente, FilaNo** tras);


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

    int j = 0;
    for (int i = ponto_meio + 1; i < M - 1; i++) {
        (*novo_no)->chaves[j] = no->chaves[i];
        j++;
    }
    (*novo_no)->num_chaves = j;

    j = 0;
    for (int i = ponto_meio + 1; i < M; i++) {
        (*novo_no)->filhos[j] = no->filhos[i];
        j++;
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
        if (no->num_chaves == M - 1) {
            No* novo_no_interno = NULL;
            long long nova_chave_promovida;
            divide_no_interno(no, &nova_chave_promovida, &novo_no_interno);
            *chave_promovida = nova_chave_promovida;
            return novo_no_interno;
        }
    }

    return NULL;
}

void insere(No** raiz, Par p) {
    if (*raiz == NULL) {
        *raiz = criar_arvore();
    }

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

// REMOÇÃO

static void remover_interno(No* no, long long chave, No* filho);
static void redistribuir_no(No* no_pai, int indice_filho);
static void fundir_nos(No* no_pai, int indice_filho);
static No* remover_recursivo(No* no, long long chave);


void remover(No** raiz, long long chave) {
    if (!(*raiz)) {
        printf("A arvore esta vazia.\n");
        return;
    }

    remover_recursivo(*raiz, chave);

    // Se a raiz ficar vazia após a remoção, a nova raiz será seu único filho
    if ((*raiz)->num_chaves == 0 && !(*raiz)->eh_folha) {
        No* antiga_raiz = *raiz;
        *raiz = (*raiz)->filhos[0];
        free(antiga_raiz);
    }
}

static No* remover_recursivo(No* no, long long chave) {
    if (no->eh_folha) {
        int i = 0;
        while (i < no->num_chaves && no->pares[i].chave != chave) {
            i++;
        }
        if (i == no->num_chaves) {
            // Chave não encontrada
            return NULL;
        }
        // Remove a chave
        for (int j = i; j < no->num_chaves - 1; j++) {
            no->pares[j] = no->pares[j + 1];
        }
        no->num_chaves--;
        return no;
    }

    // Lógica para nós internos
    int i = 0;
    while (i < no->num_chaves && chave >= no->chaves[i]) {
        i++;
    }

    remover_recursivo(no->filhos[i], chave);

    // Rebalanceamento
    if (no->filhos[i]->num_chaves < (M / 2)) {
        redistribuir_no(no, i);
    }
    return NULL;
}

static void redistribuir_no(No* no_pai, int indice_filho) {
    No* filho = no_pai->filhos[indice_filho];
    No* irmao_esq = (indice_filho > 0) ? no_pai->filhos[indice_filho - 1] : NULL;
    No* irmao_dir = (indice_filho < no_pai->num_chaves) ? no_pai->filhos[indice_filho + 1] : NULL;

    if (irmao_esq && irmao_esq->num_chaves > (M / 2)) {
        // Pega do irmão esquerdo
        // ... (lógica de rotação para a direita)
    } else if (irmao_dir && irmao_dir->num_chaves > (M / 2)) {
        // Pega do irmão direito
        // ... (lógica de rotação para a esquerda)
    } else {
        // Fusão
        if (irmao_esq) {
            fundir_nos(no_pai, indice_filho - 1);
        } else {
            fundir_nos(no_pai, indice_filho);
        }
    }
}

static void fundir_nos(No* no_pai, int indice_filho) {
    No* filho_esq = no_pai->filhos[indice_filho];
    No* filho_dir = no_pai->filhos[indice_filho + 1];

    // ... (lógica de fusão de nós)

    // Libera o nó direito que foi fundido
    free(filho_dir);
}


// FILA

void enfileirar(FilaNo** frente, FilaNo** tras, No* no, int nivel) {
    FilaNo* novo = (FilaNo*)malloc(sizeof(FilaNo));
    novo->no = no;
    novo->nivel = nivel;
    novo->prox = NULL;
    if (*tras) (*tras)->prox = novo;
    else *frente = novo;
    *tras = novo;
}

FilaNo* desenfileirar(FilaNo** frente, FilaNo** tras) {
    if (!*frente) return NULL;
    FilaNo* temp = *frente;
    *frente = (*frente)->prox;
    if (*frente == NULL) *tras = NULL;
    return temp;
}

// IMPRESSÃO 

void imprimir_arvore_por_nivel(No* raiz) {
    if (!raiz) {
        printf("Arvore vazia.\n");
        return;
    }

    FilaNo* frente = NULL;
    FilaNo* tras = NULL;
    enfileirar(&frente, &tras, raiz, 0);

    int nivel_atual = -1;
    int contador_no_nivel = 0;

    printf("\n===== Estrutura da Árvore B+ (por nível) =====\n");
    while (frente) {
        FilaNo* item_fila = desenfileirar(&frente, &tras);
        No* no_atual = item_fila->no;

        if (item_fila->nivel != nivel_atual) {
            nivel_atual = item_fila->nivel;
            contador_no_nivel = 0;
            printf("\n--- Nivel %d (%s) ---\n", nivel_atual, no_atual->eh_folha ? "Folhas" : "Internos");
        }

        printf("No %d: ", contador_no_nivel++);

        if (no_atual->eh_folha) {
            printf("[");
            for (int i = 0; i < no_atual->num_chaves; i++) {
                printf("%lld", no_atual->pares[i].chave);
                if (i < no_atual->num_chaves - 1) printf("|");
            }
            printf("]\n");
        } else {
            printf("{");
            for (int i = 0; i < no_atual->num_chaves; i++) {
                printf("%lld", no_atual->chaves[i]);
                if (i < no_atual->num_chaves - 1) printf("|");
            }
            printf("}\n");

            for (int i = 0; i <= no_atual->num_chaves; i++) {
                if (no_atual->filhos[i]) {
                    enfileirar(&frente, &tras, no_atual->filhos[i], nivel_atual + 1);
                }
            }
        }

        free(item_fila);
    }

    printf("\n==============================================\n");
}

void salvar_indices(No* no, FILE* f, int nivel) {
    if (!no || !f) return;

    for (int i = 0; i < nivel; i++) fprintf(f, "  ");

    if (no->eh_folha) {
        fprintf(f, "[");
        for (int i = 0; i < no->num_chaves; i++) {
            fprintf(f, "%lld", no->pares[i].chave);
            if (i < no->num_chaves - 1) fprintf(f, "|");
        }
        fprintf(f, "]\n");
    } else {
        fprintf(f, "{");
        for (int i = 0; i < no->num_chaves; i++) {
            fprintf(f, "%lld", no->chaves[i]);
            if (i < no->num_chaves - 1) fprintf(f, "|");
        }
        fprintf(f, "}\n");

        for (int i = 0; i <= no->num_chaves; i++) {
            salvar_indices(no->filhos[i], f, nivel + 1);
        }
    }
}

// MAIN

int main() {
    // Redireciona stdout para arquivo de log
    FILE* log = freopen("saida_terminal.txt", "w", stdout);
    if (!log) {
        perror("Erro ao criar arquivo de log");
        return 1;
    }

    FILE *f_registros = fopen("../registros.dat", "rb");
    if (!f_registros) {
        f_registros = fopen("registros.dat", "rb");
        if (!f_registros) {
            perror("Erro ao abrir 'registros.dat'");
            return 1;
        }
    }

    No* raiz = NULL;
    printf("Iniciando construção da Árvore B+ de ordem %d...\n", M);

    Registro reg;
    long offset = 0;
    int count = 0;

    while (fread(&reg, sizeof(Registro), 1, f_registros) == 1) {
        if (reg.ativo) {
            Par p = {reg.chave, offset};
            insere(&raiz, p);
            count++;
        }
        offset = ftell(f_registros);
    }

    fclose(f_registros);
    printf("\nConstrução finalizada. %d registros inseridos na Árvore B+.\n", count);

    imprimir_arvore_por_nivel(raiz);

    // Salva estrutura em arquivo separado
    FILE* f_indices = fopen("indices_bplus.txt", "w");
    if (f_indices && raiz) {
        salvar_indices(raiz, f_indices, 0);
        fclose(f_indices);
        printf("\nÍndices salvos em 'indices_bplus.txt'.\n");
    } else {
        printf("Erro ao salvar índices.\n");
    }

    return 0;
}
