#ifndef FILTERS_H
#define FILTERS_H

#include "record.h"

#define THRESHOLD_CA_MIN 0.25f
#define THRESHOLD_CB_MAX 0.75f

// Funções de pré-filtragem
int filter_A_records(const Record *recordsA, int *validA, int n);
int filter_B_records(const Record *recordsB, int *validB, int n);

#endif // FILTERS_H
