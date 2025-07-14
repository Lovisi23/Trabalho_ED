#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char nome[50];
    char cpf[12];
    float nota;
} Registro;

int tam() {
    return sizeof(Registro);
}

Registro *le_registro(FILE *f) {
    Registro *r = malloc(sizeof(Registro));
    if (fread(r, tam(), 1, f) != 1) {
        free(r);
        return NULL;
    }
    return r;
}

void imprime_heap(char *nome_arquivo, int n) {
    FILE *f = fopen(nome_arquivo, "rb");
    if (!f) {
        perror("Erro ao abrir heap.dat");
        return;
    }

    printf("\n=== Conteúdo da Heap (ordenada por nota) ===\n");
    for (int i = 1; i <= n; i++) {
        fseek(f, (i - 1) * tam(), SEEK_SET);
        Registro *r = le_registro(f);
        if (r != NULL) {
            printf("[%d] Nome: %-30s | CPF: %s | Nota: %.1f\n", i, r->nome, r->cpf, r->nota);
            free(r);
        }
    }

    fclose(f);
}

int main() {
    FILE *f = fopen("heap.dat", "rb");
    if (!f) {
        perror("Erro ao abrir heap.dat");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    fclose(f);

    int total_registros = tamanho / sizeof(Registro);
    imprime_heap("heap.dat", total_registros);

    return 0;
}
