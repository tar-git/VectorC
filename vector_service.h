#ifndef VECTOR_SERVICE_H
#define VECTOR_SERVICE_H

#include "vector.h"

void ** get_vector_data(Vector * vector);

#ifdef vector_data
    #undef vector_data
#define vector_data(vector, type) (\
    (type**)(get_vector_data(vector))\
)
#endif

#ifdef vector_at
    #undef vector_at
    #define vector_at(vector, type, pos) ()
#endif

#ifdef vector_front
    #undef vector_front
    #define vector_front(vector, type) ()
#endif

#ifdef vector_back
    #undef vector_back
    #define vector_back(vector, type) ()
#endif

#endif // VECTOR_SERVICE_H
