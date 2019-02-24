#include <signal.h>
#include <time.h>

#include "test_suite.h"
#include "vector.h"
#include "vector_service.h"

void delFun(void * ptr);

typedef struct {
    byte * mem;
} byteField;

byteField * CreateByteField(int val);

int ByteFieldEqual(byteField * a, byteField * b);

void DestroyByteField(void * bf);

/*+++++++++++++++TESTS+++++++++++++++*/

void expectEqualFail(void) { EXPECT_EQ(5, 10); }

void expectNotNullFail(void) {
    int* nullIntPtr = NULL;
    EXPECT_NOT_NULL(nullIntPtr);
}

void expectNullSuccess(void) {
    double* nullDblPtr = NULL;
    EXPECT_NULL(nullDblPtr);
}

void expectNotEqualSuccess(void) { EXPECT_NEQ(1.0, -2.4); }

void failOnSigSegvSignal(void) { raise(SIGSEGV); }

void passOnEmptyTest(void){}

void failOnSigAbrtSignal(void) { raise(SIGABRT); }


void vectorNewSuccess(void);

void vectorNewFail(void);

void vectorDeleteFunctionSuccess(void);

void vectorFreeSuccess(void);

void vectorFillSuccess(void);

void vectorReserveSuccess(void);

void vectorPopBackFail(void);

void vectorPushBackSuccess(void);

void vectorPopBackSuccess(void);

void vectorInsertSuccess(void);

void vectorInsertFail(void);

void vectorShrinkToFitSuccess(void);

void vectorCapacitySuccess(void);


void vectorDataSuccess(void);

void vectorAtSuccess(void);

void vectorAtFail(void);

void vectorFrontSuccess(void);

void vectorFrontFail(void);

void vectorBackSuccess(void);

void vectorBackFail(void);

/*----------------TESTS-----------------*/

int main() {
    srand((unsigned int)time(NULL));
    TestSuite* testSuite = newTestSuite();
    addTestCase(testSuite, "expect_equal_fail", expectEqualFail);
    addTestCase(testSuite, "expect_not_null_fail", expectNotNullFail);
    addTestCase(testSuite, "expect_null_success", expectNullSuccess);
    addTestCase(testSuite, "expect_not_equal_success", expectNotEqualSuccess);
    addTestCase(testSuite, "sigsegv_fail", failOnSigSegvSignal);
    addTestCase(testSuite, "sigabrt_fail", failOnSigAbrtSignal);
    addTestCase(testSuite, "empty_test_success", passOnEmptyTest);
    
    addTestCase(testSuite, "vector_new_success", vectorNewSuccess);
    addTestCase(testSuite, "vector_new_fail", vectorNewFail);
    addTestCase(testSuite, "vector_delete_function_success", vectorDeleteFunctionSuccess);
    addTestCase(testSuite, "vector_free_success", vectorFreeSuccess);
    addTestCase(testSuite, "vector_fill_success", vectorFillSuccess);
    addTestCase(testSuite, "vector_reserve_success", vectorReserveSuccess);
    addTestCase(testSuite, "vector_push_back_success", vectorPushBackSuccess);
    addTestCase(testSuite, "vector_pop_back_fail", vectorPopBackFail);
    addTestCase(testSuite, "vector_pop_back_success", vectorPopBackSuccess);
    addTestCase(testSuite, "vector_insert_success", vectorInsertSuccess);
    addTestCase(testSuite, "vector_insert_fail", vectorInsertFail);
    addTestCase(testSuite, "vector_shrink_to_fit_success", vectorShrinkToFitSuccess);
    addTestCase(testSuite, "vector_capacity_success", vectorCapacitySuccess);
    
    addTestCase(testSuite, "vector_data_success", vectorDataSuccess);
    addTestCase(testSuite, "vector_at_success", vectorAtSuccess);
    addTestCase(testSuite, "vector_at_fail", vectorAtFail);
    addTestCase(testSuite, "vector_front_success", vectorFrontSuccess);
    addTestCase(testSuite, "vector_front_fail", vectorFrontFail);
    addTestCase(testSuite, "vector_back_success", vectorBackSuccess);
    addTestCase(testSuite, "vector_back_fail", vectorBackFail);
    runTestSuite(testSuite);
    destroyTestSuite(testSuite);
}

/**************DEFINITIONS******************/

void delFun(void * ptr) {
    free(ptr);
}

void vectorNewSuccess(void) {
    Vector * vec;
    int vector_new_result = vector_new(&vec, sizeof(int));
    
    EXPECT_EQ(vector_new_result, STATUS_OK);
    vector_free(vec);
    
    int vector_sized_new_result = vector_sized_new(&vec, sizeof(char), 10);
    EXPECT_EQ(vector_sized_new_result, STATUS_OK);
    vector_free(vec);
}

void vectorNewFail(void) {
    Vector * vec;
    int vector_new_result = vector_new(&vec, 0);
    EXPECT_EQ(vector_new_result, STATUS_OK);
    vector_free(vec);
}

void vectorDeleteFunctionSuccess(void){
    byteField * bf =  CreateByteField(1);
    Vector * vec;
    vector_new(&vec, sizeof(byteField));
    int set_del_fun = vector_set_delete_function(vec, DestroyByteField);
    EXPECT_EQ(set_del_fun, STATUS_OK);
    
    vector_push_back(vec, bf);
    bf =  CreateByteField(2);
    vector_push_back(vec, bf);
    
    int vectorFree = vector_free(vec);
    EXPECT_EQ(vectorFree, STATUS_OK);
    
    vector_sized_new(&vec, sizeof(byteField), 3);
    vector_set_delete_function(vec, DestroyByteField);
    bf =  CreateByteField(1);
    vector_insert(vec, 0, bf);
    bf =  CreateByteField(2);
    vector_insert(vec, 1, bf);
    bf =  CreateByteField(3);
    vector_insert(vec, 2, bf);
    
    byteField * box = malloc(sizeof(byteField));
    for(int i = 2; i >= 0; --i){
        bf = CreateByteField(i+1);
        int pop_res = vector_pop_back(vec, box);
        EXPECT_EQ(pop_res, STATUS_OK);
        TS_ASSERT(ByteFieldEqual(bf, box));
    }
    vector_free(vec);
}

void vectorFreeSuccess(void) {
    Vector * vec;
    vector_sized_new(&vec, sizeof(double), 10);
    
    double d = 5.5;
    vector_insert(vec, 5, &d);
           d = 4.4;
    vector_insert(vec, 4, &d);  
    
    int vector_free_result = vector_free(vec);
    EXPECT_EQ(vector_free_result, STATUS_OK);
}

void vectorFillSuccess(void) {
    const int ARRAY_SIZE = 10;
    Vector * vec;
    vector_new(&vec, sizeof(double)); 
    
    double d = 5.5;
    double pop_val;
    int fill_empty = vector_fill(vec, ARRAY_SIZE, &d);
    EXPECT_EQ(fill_empty, STATUS_OK);
    
    for(int i = 0; i < ARRAY_SIZE; ++i) {
        vector_pop_back(vec, &pop_val);
        EXPECT_EQ(pop_val, d);
    }
    
    double x = 9;
    vector_fill(vec, ARRAY_SIZE, &d);
    int fill_not_empty = vector_fill(vec, 1, &x);
    EXPECT_EQ(fill_not_empty, STATUS_OK);
    
    double * at_val = vector_at(vec, double, 0);
    EXPECT_EQ(*at_val, x);
    
    for(int i = 1; i < ARRAY_SIZE; ++i) {
        vector_pop_back(vec, &pop_val);
        EXPECT_EQ(pop_val, d);
    }
    
    vector_free(vec);
}

void vectorReserveSuccess(void) {
    Vector * vec;
    vector_sized_new(&vec, sizeof(char), 15);
    
    char let = 'A';
    for(int i = 0; i < 15; ++i, ++let){
        vector_insert(vec, i, &let);
    }
    size_t cap_before;
    vector_capacity(vec, &cap_before);
    
    size_t new_cap      = 20;
    int reserve_result  = vector_reserve(vec, new_cap);
    EXPECT_EQ(reserve_result, STATUS_OK);
    
    let = 'A';
    char * elem;
    for(int i = 0; i < 15; ++i, ++let){
        elem = vector_at(vec, char, i);
        EXPECT_EQ(let, *elem);
    }
    
    size_t cap_after;
    vector_capacity(vec, &cap_after);
    EXPECT_EQ(new_cap, cap_after);
    
    new_cap             = 7;
    reserve_result      = vector_reserve(vec, new_cap);
    EXPECT_EQ(reserve_result, STATUS_OK);
    
    vector_capacity(vec, &cap_after);
    EXPECT_NEQ(new_cap, cap_after);

    vector_free(vec);
}

void vectorPushBackSuccess(void) {
    Vector * vec;
    size_t cap;
    const size_t ARR_SIZE = 3;
    vector_new(&vec, sizeof(int));
    
    int iarr[ARR_SIZE];
    iarr[0]     = 10;
    iarr[1]     = 20;
    iarr[2]     = 30;
    
    int push_in_empty = vector_push_back(vec, iarr+0);
    vector_capacity(vec, &cap);
    EXPECT_EQ(push_in_empty, STATUS_OK);
    EXPECT_EQ(vec->size, cap);
    EXPECT_EQ(vec->size, 1);
    
    int push_in_not_empty = vector_push_back(vec, iarr+1);
    vector_capacity(vec, &cap);
    EXPECT_EQ(push_in_not_empty, STATUS_OK);
    EXPECT_EQ(vec->size, cap);
    EXPECT_EQ(vec->size, 2);
    
    push_in_not_empty = vector_push_back(vec, iarr+2);
    vector_capacity(vec, &cap);
    EXPECT_EQ(push_in_not_empty, STATUS_OK);
    EXPECT_NEQ(vec->size, cap);
    EXPECT_EQ(vec->size, 3);
    EXPECT_EQ(cap, 4);
    
    
    int box;
    for(int i = ARR_SIZE-1; i >= 0; --i){
        vector_pop_back(vec, &box);
        EXPECT_EQ(box, iarr[i]);
    }
    EXPECT_EQ(vec->size, 0);
    
    vector_free(vec);
}

void vectorPopBackSuccess(void) {
    Vector * vec;
    vector_new(&vec, 4);
    
    int i = 10;
    vector_push_back(vec, &i);
        i = 20;
    vector_push_back(vec, &i);
    
    int * x;
    int pop_2_elem = vector_pop_back(vec, &x);
    EXPECT_EQ(pop_2_elem, STATUS_OK);
    
    int pop_1_elem = vector_pop_back(vec, &x);
    EXPECT_EQ(pop_1_elem, STATUS_OK);
    EXPECT_EQ(vec->size, 0);
    
    vector_free(vec);
}

void vectorPopBackFail(void) {
    Vector * vec;
    vector_new(&vec, 4);
    
    void * x = NULL;
    int pop_back_from_empty = vector_pop_back(vec, x);
    EXPECT_EQ(pop_back_from_empty, STATUS_OK);
    
    vector_free(vec);
}

void vectorInsertSuccess(void){
    Vector * vec;
    vector_sized_new(&vec, sizeof(int), 1);
    
    int i = 3;
    int one_elem_insert = vector_insert(vec, 0, &i);
    EXPECT_EQ(one_elem_insert, STATUS_OK);
    
    int tmp = 4;
    vector_push_back(vec, &tmp);
    
    int j = 5;
    int two_elem_insert = vector_insert(vec, 1, &j);
    EXPECT_EQ(two_elem_insert, STATUS_OK);
    
    int * iptr = vector_at(vec, int, 0);
    int * jptr = vector_at(vec, int, 1);
    
    EXPECT_EQ((*iptr), i);
    EXPECT_EQ((*jptr), j);
    
    vector_free(vec);
}

void vectorInsertFail(void) {
    Vector * vec;
    vector_new(&vec, sizeof(int));
    
    int i = 3;
    int empty_insert = vector_insert(vec, 0, &i);
    EXPECT_EQ(empty_insert, STATUS_OK); 
    
    vector_free(vec);
}

void vectorShrinkToFitSuccess(void){
    Vector * vec;
    vector_sized_new(&vec, sizeof(int), 2);
    
    int val = 7;
    vector_push_back(vec, &val);
    
    int ** data = vector_data(vec, int);
    
    size_t cap;
    vector_capacity(vec, &cap);
    EXPECT_NEQ(vec->size, cap);
    
    vector_shrink_to_fit(vec);
    data = vector_data(vec, int);
    vector_capacity(vec, &cap);
    EXPECT_EQ(vec->size, cap);
    
    vector_shrink_to_fit(vec);
    vector_capacity(vec, &cap);
    EXPECT_EQ(vec->size, cap);
    
    vector_free(vec);
}

void vectorCapacitySuccess(void){
    size_t cap_befor = 15, cap_after;
    Vector * vec;
    vector_sized_new(&vec, sizeof(char), cap_befor);
    
    int cap_result = vector_capacity(vec, &cap_after);
    EXPECT_EQ(cap_result, STATUS_OK);
    EXPECT_EQ(cap_befor, cap_after);
    
    vector_free(vec);
}


void vectorDataSuccess(void) {
    const int ARRAY_SIZE = 10;
    Vector * vec;
    vector_new(&vec, sizeof(int));
    
    int push_val = 0;
    for(int i = 1; i <= ARRAY_SIZE; ++i){
        push_val = i * 10;
        vector_push_back(vec, &push_val);
    }
    
    int * array = vector_data(vec, int);
    EXPECT_NOT_NULL(array);
    for(int i = 0; i < ARRAY_SIZE; ++i){
        EXPECT_EQ(array[i], (i+1)*10);
    }
    
    vector_free(vec);
}

void vectorAtSuccess(void) {
    Vector * vec;
    vector_sized_new(&vec, sizeof(int), 5);
    
    int val = 19;
    vector_insert(vec, 3, &val);
    
    int * vector_element = vector_at(vec, int, 3);
    EXPECT_NOT_NULL(vector_element);
    EXPECT_EQ((*vector_element), val);
    
    vector_free(vec);
}

void vectorAtFail(void) {
    Vector * vec;
    vector_sized_new(&vec, sizeof(int), 2);
    
    int val = 19;
    int insert_res = vector_insert(vec, 1, &val);
    EXPECT_EQ(insert_res, STATUS_OK);
    
    int * vector_element = vector_at(vec, int, 3); //FAIL
    EXPECT_NOT_NULL(vector_element);
    EXPECT_EQ((*vector_element), val);
    
    vector_free(vec);
}

void vectorFrontSuccess(void) {
    Vector * vec;
    vector_new(&vec, sizeof(long));
    
    long l1 = 12345;
    vector_push_back(vec, &l1);
    long l2 = 54321;
    vector_push_back(vec, &l2);
    
    long * vectorFront = vector_front(vec, long);
    EXPECT_NOT_NULL(vectorFront);
    EXPECT_EQ((*vectorFront), l1);
    
    vector_free(vec);
}

void vectorFrontFail(void) {
    Vector * vec;
    vector_new(&vec, sizeof(long));
    
    long * vectorFront = vector_front(vec, long); //FAIL
    EXPECT_NOT_NULL(vectorFront);
    
    vector_free(vec);   
}

void vectorBackSuccess(void) {
    Vector * vec;
    vector_new(&vec, sizeof(long));
    
    long l1 = 12345;
    vector_push_back(vec, &l1);
    long l2 = 54321;
    vector_push_back(vec, &l2);
    
    long * vectorBack = vector_back(vec, long);
    EXPECT_NOT_NULL(vectorBack);
    EXPECT_EQ((*vectorBack), l2);
    
    vector_free(vec);
}

void vectorBackFail(void) {
    Vector * vec;
    vector_new(&vec, sizeof(long));
    
    long * vectorBack = vector_back(vec, long); //FAIL
    EXPECT_NOT_NULL(vectorBack);
    
    vector_free(vec);
}

byteField * CreateByteField(int val){
    byteField * bf  = (byteField*) malloc(sizeof(byteField));
    bf->mem         = (byte*) malloc(sizeof(byte)*10);
    for(int i = 0; i < 10; ++i){
        bf->mem[i] = i+val;
    }
    return bf;
}

int ByteFieldEqual(byteField * a, byteField * b){
    for(int i = 0; i < 10; ++i){
        if(a->mem[i] != b->mem[i])
            return 0;
    }
    return 1;
}

void DestroyByteField(void * bf){
    free(((byteField*)bf)->mem);
}
