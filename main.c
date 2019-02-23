#include <signal.h>
#include <time.h>

#include "test_suite.h"
#include "vector.h"
#include "vector_service.h"

void delFun(void * ptr);

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

void vectorFreeSuccess(void);

void vectorFillSuccess(void);

void vectorReservSuccess(void);

void vectorPopBackFail(void);

void vectorPushBackSuccess(void);

void vectorPopBackSuccess(void);

void vectorSetDeleteFunctionSuccess(void);

void vectorInsertFail(void);


void vectorDataSuccess(void);

void vectorAtSuccess(void);

/*----------------TESTS-----------------*/

int main() {
    srand((unsigned int)time(NULL));
    TestSuite* testSuite = newTestSuite();
//    addTestCase(testSuite, "expect_equal_fail", expectEqualFail);
//    addTestCase(testSuite, "expect_not_null_fail", expectNotNullFail);
//    addTestCase(testSuite, "expect_null_success", expectNullSuccess);
//    addTestCase(testSuite, "expect_not_equal_success", expectNotEqualSuccess);
//    addTestCase(testSuite, "sigsegv_fail", failOnSigSegvSignal);
//    addTestCase(testSuite, "sigabrt_fail", failOnSigAbrtSignal);
//    addTestCase(testSuite, "empty_test_success", passOnEmptyTest);
    addTestCase(testSuite, "vector_new_success", vectorNewSuccess);
    addTestCase(testSuite, "vector_free_success", vectorFreeSuccess);
    addTestCase(testSuite, "vector_pop_back_fail", vectorPopBackFail);
    addTestCase(testSuite, "vector_push_back_success", vectorPushBackSuccess);
    addTestCase(testSuite, "vector_pop_back_success", vectorPopBackSuccess);
    addTestCase(testSuite, "vector_set_delete_function_success", vectorSetDeleteFunctionSuccess);
    addTestCase(testSuite, "vector_insert_fail", vectorInsertFail);
    
    addTestCase(testSuite, "vector_data_success", vectorDataSuccess);
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
    vector_set_delete_function(vec, delFun);
    EXPECT_EQ(vector_sized_new_result, STATUS_OK);
    vector_free(vec);
}

void vectorFreeSuccess(void) {
    Vector * vec;
    vector_sized_new(&vec, sizeof(double), 10);
    double d = 5.5;
    vector_insert(vec, 5, &d);
           d = 4.4;
     vector_insert(vec, 4, &d);      
    vector_set_delete_function(vec, delFun);
    int vector_free_result = vector_free(vec);
    EXPECT_EQ(vector_free_result, STATUS_OK);
}

void vectorFillSuccess(void) {
//    Vector * vec;
//    vector_sized_new(&vec, sizeof(double), 10);   
//    double d = 5.5;
    
}

void vectorReservSuccess(void) {
    Vector * vec;
    
}

void vectorPushBackSuccess(void) {
    Vector * v;
    int res = vector_new(&v, sizeof(int));
    int i = 10;
    int push_in_empty = vector_push_back(v, &i);
    EXPECT_EQ(push_in_empty, STATUS_OK);
    i = 20;
    int push_in_not_empty = vector_push_back(v, &i);
    EXPECT_EQ(push_in_not_empty, STATUS_OK);
    vector_free(v);
}

void vectorPopBackSuccess(void) {
    Vector * v;
    vector_new(&v, 4);
    int i = 10;
    vector_push_back(v, &i);
        i = 20;
    vector_push_back(v, &i);
    int * x;
    int pop_2_elem = vector_pop_back(v, &x);
    EXPECT_EQ(pop_2_elem, STATUS_OK);
    int pop_1_elem = vector_pop_back(v, &x);
    EXPECT_EQ(pop_1_elem, STATUS_OK);
    vector_free(v);
}

void vectorPopBackFail(void) {
    Vector * v;
    vector_new(&v, 4);
    void * x;
    int pop_back_from_empty = vector_pop_back(v, x);
    EXPECT_EQ(pop_back_from_empty, STATUS_ERROR_BAD_ARG);
    vector_free(v);
}

void vectorSetDeleteFunctionSuccess(void) {
    Vector * v;
    vector_new(&v, sizeof(int));
    int delete_result = vector_set_delete_function(v, delFun);
    EXPECT_EQ(delete_result, STATUS_OK);
    for(int i = 1; i <= 10; ++i){
        int push_val = i * 10;
        vector_push_back(v, &push_val);
    }
    vector_free(v);
}

void vectorInsertFail(void) {
    Vector * v;
    vector_new(&v, sizeof(int));
    int i = 3;
    int empty_insert = vector_insert(v, 0, &i);
    EXPECT_EQ(empty_insert, STATUS_ERROR_BAD_ARG);    
}


void vectorDataSuccess(void) {
    #define ARRAY_SIZE 10
    Vector * vec;
    vector_new(&vec, sizeof(int));
    int push_val = 0;
    for(int i = 1; i <= ARRAY_SIZE; ++i){
        push_val = i * 10;
        vector_push_back(vec, &push_val);
    }
    int ** array = vector_data(vec, int);
    for(int i = 0; i < ARRAY_SIZE; ++i){
        EXPECT_EQ((*array[i]), (i+1)*10);
    }
}

void vectorAtSuccess(void) {
    Vector * vec;
    vector_sized_new(&vec, sizeof(int), 5);
    int val = 19;
    vector_insert(vec, 3, val);
}
