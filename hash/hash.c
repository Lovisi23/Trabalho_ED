#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 100000

typedef struct {
    char nome[50];
    char cpf[12];
    float nota;
    long long chave;
    int ativo;
} Registro;

typedef Registro* Hash[TAM];

// Hash com base em rand()
int func_hash(long long chave, int k) {
    srand((unsigned int)chave);
    int h = rand() % TAM;
    return (h + k) % TAM;
}

void inicializa(Hash tabela) {
    for (int i = 0; i < TAM; i++) {
        tabela[i] = NULL;
    }
}

Registro* copia_registro(Registro *src) {
    Registro *r = malloc(sizeof(Registro));
    memcpy(r, src, sizeof(Registro));
    return r;
}

int busca(Hash tabela, long long chave, int *achou) {
    *achou = 0;
    int pos_livre = -1;

    for (int k = 0; k < TAM; k++) {
        int pos = func_hash(chave, k);
        if (tabela[pos] == NULL) {
            if (pos_livre == -1) pos_livre = pos;
            break;
        }
        if (tabela[pos]->chave == chave && tabela[pos]->ativo) {
            *achou = 1;
            return pos;
        }
    }

    return *achou ? -1 : pos_livre;
}

void insere(Hash tabela, Registro *r) {
    int achou;
    int pos = busca(tabela, r->chave, &achou);
    if (achou) return;
    if (pos != -1) {
        tabela[pos] = copia_registro(r);
    } else {
        printf("Tabela cheia! Overflow ao inserir CPF: %s\n", r->cpf);
    }
}

void salvar_em_arquivo(Hash tabela, const char *nome) {
    FILE *f = fopen(nome, "wb");
    if (!f) {
        perror("Erro ao abrir hash.dat");
        return;
    }

    for (int i = 0; i < TAM; i++) {
        if (tabela[i] && tabela[i]->ativo) {
            fwrite(tabela[i], sizeof(Registro), 1, f);
        }
    }

    fclose(f);
    printf("Tabela hash salva em %s.\n", nome);
}

void libera(Hash tabela) {
    for (int i = 0; i < TAM; i++) {
        if (tabela[i]) free(tabela[i]);
    }
}

int main() {
    FILE *f = fopen("registros.dat", "rb");
    if (!f) {
        perror("Erro ao abrir registros.dat");
        return 1;
    }

    Hash tabela;
    inicializa(tabela);

    Registro r;
    while (fread(&r, sizeof(Registro), 1, f) == 1) {
        insere(tabela, &r);
    }

    fclose(f);

    salvar_em_arquivo(tabela, "hash.dat");

    libera(tabela);
    return 0;
}
