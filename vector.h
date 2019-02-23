#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>
#include <stdlib.h>

#define STATUS_OK (0)                // Successfully completed
#define STATUS_ERROR_NO_MEMORY (-1)  // Insufficient memory to complete operation
#define STATUS_ERROR_BAD_ARG (-2)    // Bad argument value E.G. null pointer

typedef const void* const_ptr;

typedef uint8_t byte;

/* Interface of the delete_function, it's purpose is described below. */
typedef void (*delete_function)(void*);

/* Contains the public fields of a Vector. !!! This structure shouldn't be changed.
P.S. In source file Vector may have an additional structure with the
additional fields to support all operations.
https://en.wikipedia.org/wiki/Opaque_pointer */
typedef struct {
    byte* data;
    size_t size;
} Vector;

/* Returns pointer to the vector data of specified type. */
#define vector_data(vector, type) ()

 /* Returns the element at specified location pos, with bounds checking. */
#define vector_at(vector, type, pos) ()

/* Returns the element at the begging of the vector. */
#define vector_front(vector, type) ()

/* Returns the last element of the vector. */
#define vector_back(vector, type) ()

/* Creates a new Vector for elements with size `elementSize`. */
extern int vector_new(Vector** vector, const size_t elementSize);

/* Creates a new Vector with `count` number of elements preallocated */
extern int vector_sized_new(Vector** vector, const size_t elementSize,
                            const size_t count);

/* Sets delete function for an element of vector. The deleter will be called when the
Vector is freed and memory deallocated as well.*/
extern int vector_set_delete_function(Vector* vector, delete_function deleter);

/* Frees the memory allocated for the Vector.
If vetor elements contain dynamically-allocated memory, they should be freed by passed `delete_function`.
If `delete_function` is not set it leads to memory leak.*/
extern int vector_free(Vector* vector);

/* Replaces the content of the vector with value.  */
extern int vector_fill(Vector* vector, const size_t count, const_ptr value);

/* Increase the capacity of the vector to a value that's greater or equal to new_cap.
If new_cap is greater than the current capacity, new storage is allocated, otherwise the method does nothing. */
extern int vector_reserve(Vector* vector, const size_t newCapacity);

/* Adds the value pointed by the `element` to the end of the array.
The vector will grow in size automatically if necessary. */
extern int vector_push_back(Vector* vector, const_ptr element);

/* Removes the last element of the vector.
Calling this function on empty container should lead to error.*/
extern int vector_pop_back(Vector* vector, const_ptr* element);

/* Inserts an element in a vector at the given index. */
extern int vector_insert(Vector* vector, const size_t index, const_ptr element);

/* Requests the removal of unused capacity. */
extern int vector_shrink_to_fit(Vector* vector);

/* Returns the number of elements that the vector has currently allocated space for. */
extern int vector_capacity(Vector* vector, size_t* capacity);

#endif  // !VECTOR_H
