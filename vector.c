#include "vector.h"
#include "string.h"
#define CONST_PTR_SIZE (sizeof(const_ptr))

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

int vector_new(Vector **vector, const size_t elementSize)
{
    if(!elementSize){
        return STATUS_ERROR_BAD_ARG;
    }
    ProxyVector * vec = (ProxyVector *)malloc(sizeof(ProxyVector));
    if(!vec)        {
        return STATUS_ERROR_NO_MEMORY;
    }
    
    vec->elementSize = elementSize;
    vec->capacity    = 0;
    vec->size        = 0;
    vec->delFun      = NULL;
    vec->data        = NULL;
    (*vector)        = (Vector*)vec;
    
    return STATUS_OK;
}

int vector_sized_new(Vector** vector, const size_t elementSize, const size_t count)
{
    if(!elementSize){
        return STATUS_ERROR_BAD_ARG;
    }
    ProxyVector * vec = (ProxyVector *)malloc(sizeof(ProxyVector));
    if(!vec)        {
        return STATUS_ERROR_NO_MEMORY;
    }
    
    vec->elementSize = elementSize;
    vec->capacity    = count;
    vec->size        = count;
    vec->delFun      = NULL;
    vec->data        = count ? malloc(elementSize * count) : 0;
    
    if(vec->data) {
        (*vector)    = (Vector*)vec;
        return STATUS_OK;
    }
    else{
        return STATUS_ERROR_NO_MEMORY;
    }  
}

int vector_set_delete_function(Vector *vector, delete_function deleter)
{
    int err = is_vector_initialized(vector);
    if (err) {
        return err;
    }
    
    ((ProxyVector *)vector)->delFun = deleter;
    return STATUS_OK;
}

int vector_free(Vector *vector)
{
    if(!vector) {
        return STATUS_OK;
    }
    
    ProxyVector * vec       = vector;
    size_t elementSize      = vec->elementSize;
    byte * data             = vec->data;
    delete_function delFun  = vec->delFun;
    
    if(data && delFun) {
        for(unsigned i = 0; i < vec->size; ++i) {
            delFun(data + i*elementSize);
        }
    }
    
    free(data);
    free(vec);
    return STATUS_OK;
}

int vector_fill(Vector *vector, const size_t count, const_ptr value)
{
    int err = is_vector_initialized(vector);
    if (err) {
        return err;
    }
    ProxyVector * vec   = vector;
    size_t capacity     = vec->capacity;
    size_t elementSize  = vec->elementSize;
    
    if(count > capacity){
        vector_reserve(vector, count);
    }
    
    byte * data         = vec->data;
    for(unsigned i = 0; i < count; ++i){
        memmove(data + i*elementSize, value, elementSize);
    }
    if(count > vec->size) {
        vec->size         = count;
    }
    
    return STATUS_OK;
}

int vector_reserve(Vector *vector, const size_t newCapacity)
{
    int err = is_vector_initialized(vector);
    if (err) {
        return err;
    }
    ProxyVector * vec = vector;
    if(newCapacity <= vec->capacity) {
        return STATUS_OK;
    }
    
    byte * new_data   = (byte *) realloc(vec->data, newCapacity * vec->elementSize);
    if(!new_data) {
        return STATUS_ERROR_NO_MEMORY;
    }
    
    vec->data         = new_data;
    vec->capacity     = newCapacity;
    
    return STATUS_OK;
}

int vector_push_back(Vector* vector, const_ptr element){
    int err = is_vector_initialized(vector);
    if (err) {
        return err;
    }

    ProxyVector * vec  = vector;
    size_t elementSize = vec->elementSize;
    size_t capacity    = vec->capacity;
    size_t size        = vec->size;
    
    if(size >= capacity) {
        int err = vector_reserve(vector, !capacity ? 1 : capacity+capacity);
        if(err) {
            return err;
        }
    }

    memmove(vec->data + elementSize * size, element, elementSize);
    ++(vec->size);
    return STATUS_OK;
}

int vector_pop_back(Vector *vector, const_ptr *element)
{
    int err = is_vector_initialized(vector);
    if (err)  {
        return err;
    }
    
    size_t size        = vector->size;
    if(!size) {
        return STATUS_ERROR_BAD_ARG;
    }
    
    ProxyVector * vec  = vector;
    byte * data        = vec->data;
    size_t elementSize = vec->elementSize;
    memmove(element, (data + (size-1)*elementSize), elementSize);
   
    --(vec->size);
    
    return STATUS_OK;
}

int vector_insert(Vector *vector, const size_t index, const_ptr element)
{
    int err = is_vector_initialized(vector);
    if (err) {
        return err;
    }

    ProxyVector * vec  = vector;
    size_t size        = vec->size;
    
    if(index >= size){
        return STATUS_ERROR_BAD_ARG;
    }
    
    size_t elementSize = vec->elementSize;
    memmove((vec->data) + index*elementSize, element, elementSize);
    
    return STATUS_OK;
}

int vector_shrink_to_fit(Vector *vector)
{
    int err = is_vector_initialized(vector);
    if (err) {
        return err;
    }
    
    ProxyVector * vec  = vector;
    size_t size        = vec->size;
    if(vec->capacity == size) {
        return STATUS_OK;
    }
    byte * new_data    = (byte *)realloc(vec->data, size * vec->elementSize);
    vec->data          = new_data;
    vec->capacity      = size;
    
    return STATUS_OK;
}

int vector_capacity(Vector *vector, size_t *capacity)
{
    int err = is_vector_initialized(vector);
    if (err) {
        return err;
    }
    *capacity = ((ProxyVector *)vector)->capacity;
    return STATUS_OK;
}
