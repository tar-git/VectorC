#include "vector_service.h"

typedef struct {
    byte* data;
    size_t size;
    delete_function delFun;
    size_t capacity;
    size_t elementSize;
} ProxyVector;

static int is_vector_initialized(Vector *vector){
    
    ProxyVector * vec = vector;
    
    if(!(vec && vec->elementSize))  {
        return STATUS_ERROR_BAD_ARG;
    }
    
    return  STATUS_OK;
}

void * get_vector_data(Vector *vector)
{
    int err = is_vector_initialized(vector);
    if (err) {
        return err;
    }
    return (void *)vector->data;
}

void * get_vector_at(Vector *vector, size_t pos)
{
    int err = is_vector_initialized(vector);
    if (err || pos >= vector->size)  {
        return NULL;
    }
    
    ProxyVector * vec = vector;
    return (void *)(vec->data + pos * vec->elementSize);
}

void *get_vector_back(Vector *vector)
{
    int err = is_vector_initialized(vector);
    if (err || !vector->size)  {
        return NULL;
    }
    
    ProxyVector * vec = vector;
    return (void *)(vec->data+(vec->size-1) * vec->elementSize);
}
