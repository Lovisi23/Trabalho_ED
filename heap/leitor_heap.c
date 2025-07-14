#include <stdio.h>
#include <stdlib.h>

// DEFINIÇÃO DA STRUCT CORRIGIDA E COMPATÍVEL
typedef struct {
    char nome[50];
    char cpf[12];
    float nota;
    long long chave; // Campo adicionado
    int ativo;       // Campo adicionado
} Registro;

void imprime_heap(char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "rb");
    if (!f) {
        perror("Erro ao abrir heap.dat");
        return;
    }

    // Calcula o número de registros dinamicamente
    fseek(f, 0, SEEK_END);
    long tamanho_arquivo = ftell(f);
    fseek(f, 0, SEEK_SET);
    int n = tamanho_arquivo / sizeof(Registro);

    printf("\n=== Conteudo da Heap (ordenada por nota) ===\n");
    printf("Total de registros: %d\n", n);
    printf("----------------------------------------------------------------------------------\n");

    Registro r;
    for (int i = 1; i <= n; i++) {
        // Lê o próximo registro em sequência
        if (fread(&r, sizeof(Registro), 1, f) == 1) {
            // Imprime os dados, incluindo os novos campos para verificação
            printf("[%05d] Nota: %5.1f | Nome: %-20s | CPF: %s | Chave: %lld\n", i, r.nota, r.nome, r.cpf, r.chave);
        }
    }
    printf("----------------------------------------------------------------------------------\n");


    fclose(f);
}

int main() {
    imprime_heap("heap.dat");
    return 0;
}