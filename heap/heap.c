#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nome[50];
    char cpf[12];
    float nota;
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
    int maior = i;
    Registro atual, filho_esq, filho_dir;

    le_registro(f, i, &atual);

    int e = esq(i);
    if (e <= n) {
        le_registro(f, e, &filho_esq);
        if (filho_esq.nota > atual.nota) {
            maior = e;
        }
    }

    int d = dir(i);
    if (d <= n) {
        le_registro(f, d, &filho_dir);
        Registro *comparado = (maior == i) ? &atual : &filho_esq;
        if (filho_dir.nota > comparado->nota) {
            maior = d;
        }
    }

    if (maior != i) {
        Registro r_maior;
        le_registro(f, maior, &r_maior);
        salva_registro(f, i, &r_maior);
        salva_registro(f, maior, &atual);
        descer(f, maior, n);
    }
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
    FILE *f_in = fopen("registros.dat", "rb");
    FILE *f_out = fopen("heap.dat", "wb");

    if (!f_in || !f_out) {
        perror("Erro ao abrir arquivos");
        return 1;
    }

    Registro r;
    int count = 0;
    while (fread(&r, tam(), 1, f_in) == 1) {
        fwrite(&r, tam(), 1, f_out);
        count++;
    }

    fclose(f_in);
    fclose(f_out);

    constroi_heap("heap.dat", count);

    return 0;
}
