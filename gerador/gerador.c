#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gerador.h"

long long extrair_chave(const char* cpf) {
    char chave_str[10];
    strncpy(chave_str, cpf, 9);
    chave_str[9] = '\0';
    return atoll(chave_str);
}
// ==========================================


// Função para gerar um CPF aleatório (11 dígitos)
void gerar_cpf_aleatorio(char* cpf) {
    for (int i = 0; i < 11; i++) {
        cpf[i] = (rand() % 10) + '0';
    }
    cpf[11] = '\0';
}

void gerar_registros(const char* nome_arquivo, int quantidade) {
    FILE* arquivo = fopen(nome_arquivo, "wb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para escrita");
        exit(1);
    }

    srand(time(NULL));

    for (int i = 0; i < quantidade; i++) {
        Registro reg;
        sprintf(reg.nome, "Pessoa Aleatoria %d", i + 1);
        gerar_cpf_aleatorio(reg.cpf);
        reg.nota = ((float)rand() / RAND_MAX) * 100.0f;
        reg.chave = extrair_chave(reg.cpf);
        reg.ativo = 1;
        fwrite(&reg, sizeof(Registro), 1, arquivo);
    }

    printf("%d registros gerados com sucesso em '%s'.\n", quantidade, nome_arquivo);
    fclose(arquivo);
}

int main() {
    gerar_registros("../registros.dat", 10000);
    return 0;
}