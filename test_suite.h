#ifndef TEST_SUITE_H
#define TEST_SUITE_H

#include <stdlib.h>

#define MAX_TEST_CASES 256

#define TS_ASSERT(condition) { \
    if (condition) { \
    } \
    else { \
        triggerAssertion(__FUNCTION__, __FILE__, __LINE__, #condition); \
    } \
}

#define EXPECT_EQ(v1, v2) TS_ASSERT(v1 == v2)
#define EXPECT_NEQ(v1, v2) TS_ASSERT(v1 != v2)
#define EXPECT_NULL(v) EXPECT_EQ(v, NULL)
#define EXPECT_NOT_NULL(v) EXPECT_NEQ(v, NULL)

typedef void (*TestFunction)(void);

typedef struct {
    const char* name;
    TestFunction testFunction;
} TestCase;

typedef struct {
    size_t totalTests;
    size_t failedTests;
    TestCase testCases[MAX_TEST_CASES];
} TestSuite;

extern TestSuite* newTestSuite(void);

extern void runTestSuite(TestSuite* testSuite);

extern void addTestCase(TestSuite* testSuite, const char* testName, TestFunction test);

extern void destroyTestSuite(TestSuite* testSuite);

void triggerAssertion(const char* functionName, const char* fileName, int line,
                      const char* condition);

#endif  // !TEST_SUITE_H
