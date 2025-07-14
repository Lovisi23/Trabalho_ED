#include <stdio.h>
#include <stdlib.h>

#define TAM 100000

typedef struct {
    char nome[50];
    char cpf[12];
    float nota;
    long long chave;
    int ativo;
} Registro;

int main() {
    FILE *f = fopen("hash.dat", "rb");
    if (!f) {
        perror("Erro ao abrir hash.dat");
        return 1;
    }

    Registro r;
    int contador = 0;

    while (fread(&r, sizeof(Registro), 1, f) == 1) {
        contador++;
        printf("Posicao #%d\n", contador);
        printf("  Nome: %s\n", r.nome);
        printf("  CPF: %s\n", r.cpf);
        printf("Nota: %.1f\n", r.nota); 
        printf("  Chave: %lld\n", r.chave);
        printf("  Ativo: %d\n", r.ativo);
        printf("------------------------------------\n");
    }

    fclose(f);
    printf("Total de registros lidos: %d\n", contador);

    return 0;
}
