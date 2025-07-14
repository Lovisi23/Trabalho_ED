#ifndef GERADOR_H
#define GERADOR_H

// DEFINA A STRUCT E A FUNÇÃO AUXILIAR AQUI
// ==========================================
typedef struct {
    char nome[50];
    char cpf[12]; // 11 dígitos + '\0'
    float nota;
    long long chave; // Chave numérica (9 primeiros dígitos do CPF)
    int ativo; // 1 para ativo, 0 para removido
} Registro;

void gerar_registros(const char* nome_arquivo, int quantidade);

#endif // GERADOR_H