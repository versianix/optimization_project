#ifndef IDS_H
#define IDS_H

#define HASH_TABLE_SIZE 60013 // número primo próximo de 60000, ajustável

typedef struct HashEntry {
    char id[6];
    int index; // índice do registro correspondente
    struct HashEntry *next;
} HashEntry;

typedef struct {
    HashEntry **buckets; // Array de ponteiros para as listas encadeadas
    int size; // Tamanho do array
} HashTable;

// Cria uma hash table com tamanho especificado
HashTable* create_hash_table(int size);

// Libera a memória da hash table
void free_hash_table(HashTable *table);

// Insere um id na hash table
void insert_hash_table(HashTable *table, const char *id, int index);

// Busca um id na hash table, retorna o índice ou -1 se não encontrado
int search_hash_table(HashTable *table, const char *id);

#endif // IDS_H
