
# Tutorial de Compilação e Execução do Projeto

Este tutorial descreve passo a passo como compilar e executar o projeto em sistemas Linux/macOS, tanto para a versão sequencial (`seq.c`) quanto para as versões otimizadas (`main.c` e `mainParalel.c`).

## Pré-Requisitos

- **Compilador C**: GCC ou Clang.
- **Biblioteca OpenMP** (para paralelização).
  - **macOS**: Instalar via Homebrew (`brew install libomp`).
  - **Linux**: Geralmente já disponível com GCC.
- **Ferramentas de ordenação**: O comando `sort` deve estar disponível no sistema.

## Estrutura do Repositório

Certifique-se de que o projeto está organizado da seguinte forma:

```
root/
├── db/
│   ├── db.c
│   ├── A.txt
│   ├── B.txt
│   └── ids.txt
├── src/
│   ├── seq.c
│   ├── main.c
│   ├── mainParalel.c
│   ├── filters.c
│   ├── filters.h
│   ├── ids.c
│   ├── ids.h
│   └── hash_table.c
├── doc/
│   └── solution.md
├── tutorial/
│   └── tutorial.md
└── README.md
```

## Compilação

### 1. Compilação da Versão Sequencial Não Otimizada (`seq.c`)

A versão sequencial não otimizada serve como referência para comparar as otimizações realizadas.

#### Em Linux com GCC

Abra o terminal e navegue até a pasta `src/`:

```bash
cd src
gcc -o seq seq.c
```

#### Em macOS com Clang

No macOS, use o Clang para compilar:

```bash
cd src
clang -o seq seq.c
```

### 2. Compilação da Versão Sequencial Otimizada (`main.c`)

Esta versão introduz pré-filtragem e uso de hash table para otimizar a busca de IDs.

#### Em Linux com GCC

```bash
cd src
gcc -o main main.c filters.c ids.c
```

#### Em macOS com Clang

```bash
cd src
clang -o main main.c filters.c ids.c
```

### 3. Compilação da Versão Paralelizada Otimizada (`mainParalel.c`)

Esta versão tenta paralelizar o processamento usando OpenMP, mas observou-se que, neste caso específico, a versão paralelizada não apresentou speedup em relação à versão otimizada sequencial.

#### Em Linux com GCC

Geralmente, o GCC no Linux já possui suporte a OpenMP. Compile com a flag `-fopenmp`:

```bash
cd src
gcc -fopenmp -o paralel mainParalel.c filters.c ids.c 
```

#### Em macOS com Clang

No macOS, é necessário instalar a biblioteca `libomp` via Homebrew e especificar os caminhos durante a compilação:

1. **Instalar libomp**:

   ```bash
   brew install libomp
   ```

2. **Compilar com Clang**:

   ```bash
   cd src
   clang -Xclang -fopenmp          -L/opt/homebrew/opt/libomp/lib          -I/opt/homebrew/opt/libomp/include          -o paralel mainParalel.c filters.c ids.c -lomp
   ```

   **Nota**: Ajuste o caminho `/opt/homebrew/opt/libomp/` conforme sua instalação do Homebrew, se necessário.

## Execução

Antes de executar as versões otimizadas, assegure-se de que os arquivos de base de dados (`A.txt`, `B.txt`, `ids.txt`) estejam presentes na pasta `db/`. Se ainda não tiver gerado esses arquivos, compile e execute `db.c` para criá-los.

### 1. Gerar a Base de Dados

Caso ainda não tenha os arquivos `A.txt`, `B.txt` e `ids.txt`, compile e execute `db.c`:

```bash
cd db
gcc -o db db.c
./db
cd ..
```

Isso gerará os arquivos de dados necessários dentro da pasta `db/`.

### 2. Executar a Versão Sequencial Não Otimizada

```bash
cd src
./seq
```

Isso irá gerar um arquivo `output.csv` e `sorted_output.csv` na pasta `src/`. O tempo de execução será exibido no terminal.

### 3. Executar a Versão Sequencial Otimizada

```bash
cd src
./main
```

Similarmente, isso gerará `output.csv` e `sorted_output.csv` com os resultados otimizados. O tempo de execução será exibido.

### 4. Executar a Versão Paralelizada Otimizada

```bash
cd src
./paralel
```
