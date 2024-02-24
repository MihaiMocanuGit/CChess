#ifndef CCHESS_VECTOR_H
#define CCHESS_VECTOR_H

#include <stdlib.h>

typedef struct  {
    const unsigned DATA_SIZE;
    size_t size;
    size_t capacity;
    void *data;
} Vector;

Vector vector_construct(unsigned dataSize);
void vector_destruct(Vector *vector);
int vector_push_back(Vector *vector, void *element);
int vector_remove(Vector *vector, size_t index);
void* vector_at(Vector *vector, size_t index);
#endif //CCHESS_VECTOR_H
