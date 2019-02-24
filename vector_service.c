#include "vector_service.h"

typedef struct {
    delete_function delFun;
    size_t capacity;
    size_t elementSize;
    const_ptr array;
} VectorData;

int vector_initialized(Vector *vector){
    if(!(vector && vector->data))  {
        return STATUS_ERROR_BAD_ARG;
    }
    
    VectorData * data = (VectorData *)vector->data;
    if(!data->elementSize)          {
        return STATUS_ERROR_BAD_ARG;
    }
    
    return  STATUS_OK;
}

void * get_vector_data(Vector *vector)
{
    int err = vector_initialized(vector);
    if (err) {
        return NULL;
    }
    
    VectorData * data   = (VectorData *)vector->data;
    return (void *)data->array;
}

void * get_vector_at(Vector *vector, size_t pos)
{
    int err = vector_initialized(vector);
    if (err || pos >= vector->size)  {
        return NULL;
    }
    
    VectorData * data = (VectorData *)vector->data;
    return (void *)(data->array + pos * data->elementSize);
}

void *get_vector_front(Vector *vector)
{
    int err = vector_initialized(vector);
    if (err || !vector->size)  {
        return NULL;
    }
    
    VectorData * data = (VectorData *)vector->data;
    return (void *)(data->array);
}

void *get_vector_back(Vector *vector)
{
    int err = vector_initialized(vector);
    if (err || !vector->size)  {
        return NULL;
    }
    
    VectorData * data = (VectorData *)vector->data;
    return (void *)(data->array+(vector->size-1) * data->elementSize);
}
