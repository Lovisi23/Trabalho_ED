#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char nome[50];
    char cpf[12];
    float nota;
    long long chave;
    int ativo;
} Registro;

int main() {
    FILE *arquivo;
    Registro reg;
    int contador = 0;

    // Abrir o arquivo em modo "rb" (leitura binária)
    arquivo = fopen("registros.dat", "rb");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo 'registros.dat'");
        exit(1);
    }

    // O loop while vai ler até o fim do arquivo
    while (fread(&reg, sizeof(Registro), 1, arquivo) == 1) {
        contador++;
        printf("Registro #%d\n", contador);
        printf("  Nome: %s\n", reg.nome);
        printf("  CPF: %s\n", reg.cpf);
        printf("  Nota: %.1f\n", reg.nota);
        printf("  Chave (do CPF): %lld\n", reg.chave);
        printf("  Ativo: %d\n", reg.ativo);
        printf("----------------------------------------\n");
    }

    fclose(arquivo);

    // Esta linha final não será gravada no arquivo de texto,
    // pois o redirecionamento captura apenas a saída do printf dentro do loop.
    // Para capturar tudo, teríamos que colocar o resumo final dentro do printf também
    // ou usar técnicas mais avançadas. Por enquanto, isso não é problema.
    // printf("\nLeitura concluída. Total de %d registros lidos.\n", contador);


    return 0;
}