# 📚 Projeto de Estruturas de Dados em C

Este projeto implementa três estruturas de dados diferentes em C utilizando arquivos binários:

- Árvore B+ (`bplus/`)
- Tabela Hash com Endereçamento Aberto (`hash/`)
- Heap Máxima (`heap/`)

Todas elas operam sobre os mesmos registros gerados aleatoriamente.

---

## 🏗️ Estrutura do Projeto

```
projeto/
│
├── gerador/
│   └── gerador.c
│
├── bplus/
│   └── bplus_main.c
│
├── hash/
│   └── hash_main.c
│
├── heap/
│   └── heap_main.c
│
├── leitor/
│   └── leitor.c
│
└── registros.dat (deve ser copiado para cada pasta!)
```

---

## 🧾 1. Geração dos Registros

Antes de executar qualquer módulo, é necessário gerar os dados de entrada.

### ▶️ Execute o Gerador

```bash
cd gerador
gcc gerador.c -o gerador
./gerador
```

Esse comando criará o arquivo:

```
registros.dat
```

Este arquivo contém 10.000 registros com os seguintes campos:

- Nome (50 caracteres)
- CPF (11 dígitos)
- Nota (0–100)
- Campo de ativo (para controle)

---

## 📌 2. Copiar `registros.dat` para Cada Módulo

Para que cada módulo funcione corretamente, copie o arquivo `registros.dat` para as pastas `bplus/`, `hash/` e `heap/`:

```bash
cp registros.dat ../bplus/
cp registros.dat ../hash/
cp registros.dat ../heap/
```

---

## ⚙️ 3. Execução dos Módulos

### 🌳 `bplus/` – Árvore B+

```bash
cd bplus
gcc bplus_main.c bplus.c -o bplus_main
./bplus_main
```

- Constrói uma Árvore B+ com os registros ativos.
- Imprime a árvore por nível no terminal.
- Também pode salvar os índices em um arquivo `indices_bplus.txt`.

---

### 🧮 `hash/` – Tabela Hash

```bash
cd hash
gcc hash_main.c hash.c -o hash_main
./hash_main
```

- Insere os registros em uma Tabela Hash com endereçamento aberto.
- Pode visualizar a ocupação e colisões.
- Os dados podem ser salvos em arquivos auxiliares.

---

### 🔺 `heap/` – Heap Máxima

```bash
cd heap
gcc heap_main.c heap.c -o heap_main
./heap_main
```

- Cria uma Heap Máxima ordenada por nota.
- Os registros com maior nota têm maior prioridade.
- Imprime a estrutura da heap (níveis) no terminal.

---

##  4. Visualização com `leitor.c`

O arquivo `leitor.c` pode ser usado para visualizar os registros ou estruturas adicionais sem reexecutar os algoritmos.

### Exemplo de uso:

```bash
cd leitor
gcc leitor.c -o leitor
./leitor
```

Dependendo da versão, ele pode imprimir o conteúdo de:

- `registros.dat`
- Estruturas criadas em outros módulos (usando arquivos auxiliares)

---

## ✅ Conclusão

1. Gere os dados com `gerador.c`.
2. Copie `registros.dat` para cada pasta (`bplus/`, `hash/`, `heap/`).
3. Compile e execute os arquivos principais (`*_main.c`) de cada módulo.
4. Use `leitor.c` para visualização extra.

---

> Projeto desenvolvido como exercício de manipulação de arquivos e implementação de estruturas de dados em C.
