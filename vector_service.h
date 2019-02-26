#ifndef VECTOR_SERVICE_H
#define VECTOR_SERVICE_H

#include "vector.h"

void * get_vector_data(Vector * vector);

void * get_vector_at(Vector * vector, size_t pos);

void * get_vector_back(Vector * vector);

#ifdef vector_data
    #undef vector_data
#define vector_data(vector, type) (\
    (type*)get_vector_data(vector)\
)
#endif

#ifdef vector_at
    #undef vector_at
    #define vector_at(vector, type, pos) (\
        (type*)get_vector_at(vector, pos)\
    )
#endif

#ifdef vector_front
    #undef vector_front
    #define vector_front(vector, type) (\
    (type*)get_vector_data(vector)\
)
#endif

#ifdef vector_back
    #undef vector_back
    #define vector_back(vector, type) (\
    (type*)get_vector_back(vector)\
)
#endif

#endif // VECTOR_SERVICE_H
