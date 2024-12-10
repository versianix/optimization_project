#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include "filters.h"
#include "ids.h"

#define NUM_RECORDS 100

void load_records(const char *filename, Record *records) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", filename);
        exit(1);
    }
    for (int i = 0; i < NUM_RECORDS; ++i) {
        fscanf(file, "%f", &records[i].value);
    }
    fclose(file);
}

void load_ids(const char *filename, char (*ids)[6]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", filename);
        exit(1);
    }
    for (int i = 0; i < NUM_RECORDS; ++i) {
        fscanf(file, "%s", ids[i]);
    }
    fclose(file);
}

int compare_records(const void *a, const void *b) {
    float f1 = *((float *)a);
    float f2 = *((float *)b);
    if (f1 < f2)
        return -1;
    if (f1 > f2)
        return 1;
    return 0;
}

int main() {

    clock_t start_time = clock();

    // Aloca memória dinamicamente para os registros e IDs
    Record *recordsA = (Record *)malloc(NUM_RECORDS * sizeof(Record));
    Record *recordsB = (Record *)malloc(NUM_RECORDS * sizeof(Record));
    int *validA = (int *)malloc(NUM_RECORDS * sizeof(int));
    int *validB = (int *)malloc(NUM_RECORDS * sizeof(int));
    char(*ids)[6] = (char(*)[6])malloc(NUM_RECORDS * sizeof(*ids));

    if (!recordsA || !recordsB || !ids || !validA || !validB) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        exit(1);
    }

    load_records("../db/A.txt", recordsA);
    load_records("../db/B.txt", recordsB);
    load_ids("../db/ids.txt", ids); // Assuming ids are the same for both A and B
  
    // Filtra os registros A e B
    int numA = filter_A_records(recordsA, validA, NUM_RECORDS);
    int numB = filter_B_records(recordsB, validB, NUM_RECORDS);

    printf("Registros válidos em A: %d\n", numA);
    printf("Registros válidos em B: %d\n", numB);

    // Cria a hash table e insere todos os ids
    HashTable *hash_table = create_hash_table(HASH_TABLE_SIZE);
    for (int i = 0; i < NUM_RECORDS; ++i) {
        insert_hash_table(hash_table, ids[i], i);
    }

    FILE *output = fopen("output.csv", "w");
    if (!output) {
        fprintf(stderr, "Erro ao abrir o arquivo de saída.\n");
        free(recordsA);
        free(recordsB);
        free(ids);
        free_hash_table(hash_table);
        return 1;
    }
    fprintf(output, "ID_a_m,ID_b_M,ID',a_m,b_M,f\n");

    // Paraleliza os dois primeiros loops com OpenMP
    #pragma omp parallel for schedule(dynamic)
    for (int Ca1 = 0; Ca1 < numA; ++Ca1) {
        for (int Ca2 = Ca1 + 1; Ca2 < numA; ++Ca2) { // Inicia de Ca1 + 1 para evitar duplicatas
            int idxA1 = validA[Ca1]; // Índice válido de recordsA
            int idxA2 = validA[Ca2]; // Índice válido de recordsA
            char *id_minCa =
                (recordsA[idxA1].value < recordsA[idxA2].value) ? ids[idxA1] : ids[idxA2];
            float minCa = (recordsA[idxA1].value < recordsA[idxA2].value)
                              ? recordsA[idxA1].value
                              : recordsA[idxA2].value;

            if (minCa <= THRESHOLD_CA_MIN) {
                continue;
            }

            for (int Cb1 = 0; Cb1 < numB; ++Cb1) {
                for (int Cb2 = Cb1 + 1; Cb2 < numB; ++Cb2) { // Inicia de Cb1 + 1 para evitar duplicatas
                    int idxB1 = validB[Cb1]; // Índice válido de recordsB
                    int idxB2 = validB[Cb2]; // Índice válido de recordsB
                    char *id_maxCb = (recordsB[idxB1].value > recordsB[idxB2].value) 
                                        ? ids[idxB1] 
                                        : ids[idxB2];
                    float maxCb = (recordsB[idxB1].value > recordsB[idxB2].value)
                                      ? recordsB[idxB1].value
                                      : recordsB[idxB2].value;

                    if (maxCb >= THRESHOLD_CB_MAX) {
                        continue;
                    }

                    // Cria combined_id local
                    char combined_id[6];
                    combined_id[0] = id_minCa[0];
                    combined_id[1] = id_maxCb[1];
                    combined_id[2] = id_minCa[2];
                    combined_id[3] = id_maxCb[3];
                    combined_id[4] = id_minCa[4];
                    combined_id[5] = '\0';

                    float product = minCa * maxCb;

                    // Busca na hash table
                    int index = search_hash_table(hash_table, combined_id);
                    if (index == -1) {
                        continue; // combined_id não encontrado
                    }

                    product *= recordsA[index].value * recordsB[index].value;

                    // Escreve diretamente no arquivo utilizando uma região crítica
                    #pragma omp critical
                    {
                        fprintf(output, "%s,%s,%s,%f,%f,%f\n",
                                ids[idxA1], ids[idxB1],
                                combined_id, minCa, maxCb, product);
                    }
                }
            }
        }
    }

    fclose(output);

    // Ordena os registros com base no valor da coluna f
    system("sort -t, -k6 -n output.csv -o sorted_output.csv");

    // Libera a memória alocada dinamicamente
    free(recordsA);
    free(recordsB);
    free(ids);
    free(validA);
    free(validB);
    free_hash_table(hash_table);

    printf("Processamento completo. Resultados salvos em sorted_output.csv.\n");

    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Tempo de execução: %.2f segundos.\n", elapsed_time);


    return 0;
}
