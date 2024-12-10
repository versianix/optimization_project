#include "filters.h"
#include <stdio.h>

// Função para filtrar registros A onde value > 0.25
int filter_A_records(const Record *recordsA, int *validA, int n) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (recordsA[i].value > THRESHOLD_CA_MIN) {
            validA[count++] = i; // Armazena o índice original
        }
    }
    return count; // Retorna o número de registros válidos
}

// Função para filtrar registros B onde value < 0.75
int filter_B_records(const Record *recordsB, int *validB, int n) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (recordsB[i].value < THRESHOLD_CB_MAX) {
            validB[count++] = i; // Armazena o índice original
        }
    }
    return count; // Retorna o número de registros válidos
}
