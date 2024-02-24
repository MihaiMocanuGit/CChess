
#include "../include/vector.h"

#include <string.h>

#define STARTING_CAPACITY 4
#define INCREASE_CAPACITY_BY 2
#define DECREASE_CAPACITY_BY INCREASE_CAPACITY_BY
#define CAPACITY_SIZE_RATIO_DECREASE INCREASE_CAPACITY_BY * INCREASE_CAPACITY_BY

Vector vector_construct(unsigned dataSize)
{
    Vector vector = {.DATA_SIZE = dataSize, .size = 0, .capacity = STARTING_CAPACITY};
    vector.data = malloc(vector.DATA_SIZE * vector.capacity);
    return vector;
}

void vector_destruct(Vector *vector)
{
    free(vector->data);
    vector->data = NULL;

    vector->size = 0;
    vector->capacity = 0;
}
int vector_push_back(Vector *vector, void *element)
{
    if (vector->size == vector->capacity)
    {
        vector->capacity = vector->capacity * INCREASE_CAPACITY_BY;
        void *newData = realloc(vector->data, vector->capacity);
        if (!newData)
            return -1;

        vector->data = newData;
    }
    memcpy(vector->data + vector->size * vector->DATA_SIZE, element, vector->DATA_SIZE);

    return 0;
}
int vector_remove(Vector *vector, size_t index)
{

    for (size_t i = index; i < vector->size - 1; ++i)
    {
        void * const currentElement = vector->data + i * vector->DATA_SIZE;
        void * const nextElement = currentElement + vector->DATA_SIZE; //vector->data + (i + 1) * vector->DATA_SIZE;
        memcpy(currentElement, nextElement, vector->DATA_SIZE);
    }
    vector->size--;
    const long double ratio = (long double)vector->capacity / vector->size;
    if(ratio >= CAPACITY_SIZE_RATIO_DECREASE)
    {
        vector->capacity /= DECREASE_CAPACITY_BY;
        void *newData = realloc(vector->data, vector->capacity);
        if (!newData)
            return -1;

        vector->data = newData;
    }
    return 0;
}
void* vector_at(Vector *vector, size_t index)
{
    return vector->data + index * vector->DATA_SIZE;
}