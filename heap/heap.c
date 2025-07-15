#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DEFINIÇÃO DA STRUCT CORRIGIDA E COMPATÍVEL
typedef struct {
    char nome[50];
    char cpf[12];
    float nota;
    long long chave; // Campo adicionado
    int ativo;       // Campo adicionado
} Registro;

int tam() {
    return sizeof(Registro);
}

void le_registro(FILE *f, int pos, Registro *r) {
    fseek(f, (pos - 1) * tam(), SEEK_SET);
    fread(r, tam(), 1, f);
}

void salva_registro(FILE *f, int pos, Registro *r) {
    fseek(f, (pos - 1) * tam(), SEEK_SET);
    fwrite(r, tam(), 1, f);
}

int pai(int i) { return i / 2; }
int esq(int i) { return i * 2; }
int dir(int i) { return i * 2 + 1; }

void descer(FILE *f, int i, int n) {
    int maior_idx = i;
    Registro r_i, r_maior;

    le_registro(f, i, &r_i);
    r_maior = r_i;

    int e = esq(i);
    if (e <= n) {
        Registro r_esq;
        le_registro(f, e, &r_esq);
        if (r_esq.nota > r_maior.nota) {
            maior_idx = e;
            r_maior = r_esq;
        }
    }

    int d = dir(i);
    if (d <= n) {
        Registro r_dir;
        le_registro(f, d, &r_dir);
        if (r_dir.nota > r_maior.nota) {
            maior_idx = d;
        }
    }

    if (maior_idx != i) {
        Registro record_to_swap;
        le_registro(f, maior_idx, &record_to_swap);
        salva_registro(f, i, &record_to_swap);
        salva_registro(f, maior_idx, &r_i);
        descer(f, maior_idx, n);
    }
}
Registro remover_maior(const char *nome_arquivo, int *n) {
    FILE *f = fopen(nome_arquivo, "rb+");
    if (!f) {
        perror("Erro ao abrir arquivo para remocao");
        exit(1);
    }

    if (*n == 0) {
        printf("Heap vazio!\n");
        Registro r_vazio = {.chave = -1};
        return r_vazio;
    }

    Registro maior, ultimo;
    le_registro(f, 1, &maior);      // Lê a raiz (maior elemento)
    le_registro(f, *n, &ultimo);    // Lê o último elemento

    salva_registro(f, 1, &ultimo); // Move o último para a raiz
    (*n)--;                        // Decrementa o tamanho

    descer(f, 1, *n);              // Corrige a propriedade do heap

    fclose(f);
    return maior;
}

void constroi_heap(const char *nome_arquivo, int n) {
    FILE *f = fopen(nome_arquivo, "rb+");
    if (!f) {
        perror("Erro ao abrir arquivo heap.dat");
        exit(1);
    }

    for (int i = n / 2; i >= 1; i--) {
        descer(f, i, n);
    }

    fclose(f);
}

int main() {
    FILE *f_in = fopen("../registros.dat", "rb"); // Ajuste o caminho se necessário
    FILE *f_out = fopen("heap.dat", "wb");

    if (!f_in || !f_out) {
        perror("Erro ao abrir arquivos");
        return 1;
    }

    Registro r;
    int count = 0;
    // Agora a leitura será feita com o tamanho correto da struct
    while (fread(&r, sizeof(Registro), 1, f_in) == 1) {
        fwrite(&r, sizeof(Registro), 1, f_out);
        count++;
    }

    fclose(f_in);
    fclose(f_out);

    printf("Copiados %d registros para heap.dat. Construindo o heap...\n", count);
    constroi_heap("heap.dat", count);
    printf("Heap construído com sucesso.\n");

    return 0;
}