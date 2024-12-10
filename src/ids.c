#include "ids.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Função hash (DJB2), comum e eficiente para strings curtas
static unsigned long hash_function(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    return hash;
}

HashTable* create_hash_table(int size) {
    HashTable *table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) {
        fprintf(stderr, "Erro ao alocar memória para a hash table.\n");
        exit(1);
    }
    table->size = size;
    table->buckets = (HashEntry**)calloc(size, sizeof(HashEntry*));
    if (!table->buckets) {
        fprintf(stderr, "Erro ao alocar memória para os buckets da hash table.\n");
        exit(1);
    }
    return table;
}

void free_hash_table(HashTable *table) {
    for (int i = 0; i < table->size; i++) {
        HashEntry *entry = table->buckets[i];
        while (entry) {
            HashEntry *temp = entry;
            entry = entry->next;
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

void insert_hash_table(HashTable *table, const char *id, int index) {
    unsigned long hash = hash_function(id) % table->size;
    HashEntry *new_entry = (HashEntry*)malloc(sizeof(HashEntry));
    if (!new_entry) {
        fprintf(stderr, "Erro ao alocar memória para HashEntry.\n");
        exit(1);
    }

    strcpy(new_entry->id, id);
    new_entry->index = index;
    new_entry->next = table->buckets[hash];
    table->buckets[hash] = new_entry;
}

int search_hash_table(HashTable *table, const char *id) {
    unsigned long hash = hash_function(id) % table->size;
    HashEntry *entry = table->buckets[hash];
    while (entry) {
        if (strcmp(entry->id, id) == 0) {
            return entry->index;
        }
        entry = entry->next;
    }
    return -1;
}
