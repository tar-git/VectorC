#include "vector.h"
#include "string.h"
#define CONST_PTR_SIZE (sizeof(const_ptr))

typedef struct {
    delete_function delFun;
    size_t capacity;
    size_t elementSize;
    const_ptr array;
} VectorData;

int is_vector_initialized(Vector *vector){
    if(!(vector && vector->data))  {
        return STATUS_ERROR_BAD_ARG;
    }
    
    VectorData * data = (VectorData *)vector->data;
    if(!data->elementSize)          {
        return STATUS_ERROR_BAD_ARG;
    }
    
    return  STATUS_OK;
}

int vector_new(Vector **vector, const size_t elementSize)
{
    if(!elementSize){
        return STATUS_ERROR_BAD_ARG;
    }
    Vector * vec      = (Vector *)malloc(sizeof(Vector));
    if(!vec)        {
        return STATUS_ERROR_NO_MEMORY;
    }
    byte * data = (byte *) malloc(sizeof(VectorData));
    if(!data)       {
        return STATUS_ERROR_NO_MEMORY;
    }
    
    VectorData * vec_data   = (VectorData *)data;
    vec_data->array         = NULL;
    vec_data->elementSize   = elementSize;
    vec_data->delFun        = NULL;
    vec_data->capacity      = 0;
    vec->data               = data;
    vec->size               = 0;
    (*vector)               = vec;
    
    return STATUS_OK;
}

int vector_sized_new(Vector** vector, const size_t elementSize, const size_t count)
{
    int err = vector_new(vector, elementSize);
    if(err) {
        return err;
    }

    Vector * p_vector   = *vector;
    VectorData * data   = (VectorData *)p_vector->data;
    data->elementSize   = elementSize;
    data->capacity      = count;
    p_vector->size      = count;
    data->array         = malloc(elementSize * count);
    const_ptr array     = data->array;
    if(!array) {
        return STATUS_ERROR_NO_MEMORY;
    }
    
    return STATUS_OK;    
}

int vector_set_delete_function(Vector *vector, delete_function deleter)
{
    int err = is_vector_initialized(vector);
    if(err){
        return err;
    }
    
    ((VectorData*)vector->data)->delFun = deleter;
    return STATUS_OK;
}

int vector_free(Vector *vector)
{
    if(!vector) {
        return STATUS_OK;
    }
    
    VectorData * data   = (VectorData *)vector->data;
    size_t elementSize  = data->elementSize;
    if(data)    {
        const_ptr array = data->array;
        
        if(array && data->delFun){
            delete_function delFun = data->delFun;
            for(unsigned i = 0; i < vector->size; ++i){
                delFun(array + i*elementSize);
            }
        }
        
        free(array);
        free(data);
    }
    
    free(vector);
    return STATUS_OK;
}

int vector_fill(Vector *vector, const size_t count, const_ptr value)
{
    int err = is_vector_initialized(vector);
    if(err){
        return err;
    }
    VectorData * data       = (VectorData *)vector->data;
    size_t capacity         = data->capacity;
    size_t elementSize      = data->elementSize;
    
    if(count > capacity){
        vector_reserve(vector, count);
    }
    
    const_ptr array       = data->array;
    for(unsigned i = 0; i < count; ++i){
        memcpy(array + i*elementSize, value, elementSize);
    }
    if(count > vector->size) {
        vector->size         = count;
    }
    
    return STATUS_OK;
}

int vector_reserve(Vector *vector, const size_t newCapacity)
{
    int err = is_vector_initialized(vector);
    if(err){
        return err;
    }
    VectorData * data = (VectorData *)vector->data;
    
    
    if(newCapacity <= data->capacity){
        return STATUS_OK;
    }
    
    void * new_data = (void *) realloc(data->array, newCapacity * data->elementSize);
    if(!new_data){
        return STATUS_ERROR_NO_MEMORY;
    }
    
    data->array     = new_data;
    data->capacity  = newCapacity;
    
    return STATUS_OK;
}

int vector_push_back(Vector* vector, const_ptr element){
    int err = is_vector_initialized(vector);
    if(err) {
        return err;
    }

    VectorData * data   = (VectorData *)vector->data;
    size_t size         = vector->size;
    size_t capacity     = data->capacity;
    size_t elementSize  = data->elementSize;
    
    if(size >= capacity) {
        int err = vector_reserve(vector, capacity==0? 1: capacity+capacity);
        if(err) {
            return err;
        }
    }
    
    memcpy(data->array + elementSize*(vector->size), element, elementSize);
    ++(vector->size);
    return STATUS_OK;
}

int vector_pop_back(Vector *vector, const_ptr *element)
{
    int err = is_vector_initialized(vector);
    if(err)  {
        return err;
    }
    
    size_t size       = vector->size;
    if(!size){
        return STATUS_ERROR_BAD_ARG;
    }
    
    VectorData * data   = (VectorData *)vector->data;
    const_ptr array     = data->array;
    size_t elementSize  = data->elementSize;
    memcpy(element, (array + (size-1)*elementSize), elementSize);
   
//    if(data->delFun){
//       data->delFun(array+(size-1)*elementSize);
//    }
    --(vector->size);
    
    return STATUS_OK;
}

int vector_insert(Vector *vector, const size_t index, const_ptr element)
{
    int err = is_vector_initialized(vector);
    if(err) {
        return err;
    }

    VectorData * data   = (VectorData *)vector->data;
    size_t size         = vector->size;
    
    if(index >= size){
        return STATUS_ERROR_BAD_ARG;
    }
    
    size_t elementSize  = data->elementSize;
    memcpy((data->array) + index*elementSize, element, elementSize);
    
    return STATUS_OK;
}

int vector_shrink_to_fit(Vector *vector)
{
    int err = is_vector_initialized(vector);
    if(err){
        return err;
    }
    
    VectorData * data   = (VectorData *)vector->data;
    size_t size         = vector->size;
    if(data->capacity == size) {
        return STATUS_OK;
    }
    void * new_data     = (void *)realloc(data->array, size * data->elementSize);
    data->array         = new_data;
    data->capacity      = size;
    
    return STATUS_OK;
}

int vector_capacity(Vector *vector, size_t *capacity)
{
    int err = is_vector_initialized(vector);
    if(err) {
        return err;
    }
    VectorData * data   = (VectorData *)vector->data;
    (*capacity)         = data->capacity;
    return STATUS_OK;
}
