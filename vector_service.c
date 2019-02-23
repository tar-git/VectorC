#include "vector_service.h"

typedef struct {
    delete_function delFun;
    size_t capacity;
    size_t elementSize;
    const_ptr * array;
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

void **get_vector_data(Vector *vector)
{
    int err = vector_initialized(vector);
    if(err) {
        return err;
    }
    
    VectorData * data   = (VectorData *)vector->data;
    return (void **)data->array;
}
