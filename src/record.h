#ifndef RECORD_H
#define RECORD_H

typedef struct {
    char id[6];   // 5 caracteres + 1 para o terminador nulo
    float value;  // Valor associado ao registro
} Record;

#endif // RECORD_H
