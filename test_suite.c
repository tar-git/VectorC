#include "test_suite.h"

#include <setjmp.h>
#include <signal.h>
#include <stdio.h>

#define TEST_PASSED 0
#define TEST_FAILED 1
#define TEST_RUN 2

volatile sig_atomic_t testStatus = TEST_PASSED;
jmp_buf buffer;

void signalHandler(int receivedSignal) {
    signal(receivedSignal, signalHandler);
    printf("[.........] Signal %d received.\n", receivedSignal);
    testStatus = TEST_FAILED;
    longjmp(buffer, TEST_FAILED);
}

static void generateReport(TestSuite* testSuite);

static void shuffleTests(TestSuite* testSuite);

static int runTestCase(TestCase* testCase) {
    printf("[RUN......] %s\n", testCase->name);
    testStatus = TEST_RUN;
    setjmp(buffer);
    if (testStatus == TEST_RUN) {
        testCase->testFunction();
        testStatus = TEST_PASSED;
    }
    if (testStatus == TEST_FAILED) {
        printf("[...FAILED] %s\n", testCase->name);
        return TEST_FAILED;
    } else {
        printf("[...PASSED] %s\n", testCase->name);
        return TEST_PASSED;
    }
}

TestSuite* newTestSuite(void) {
    void* memory = malloc(sizeof(TestSuite));
    if (memory == NULL) {
        printf("Can't allocate memory for test suite creation");
        abort();
    }
    TestSuite* suite = (TestSuite*)memory;
    suite->totalTests = 0u;
    suite->failedTests = 0u;
    return suite;
}

void addTestCase(TestSuite* testSuite, const char* testName, TestFunction test) {
    if (testSuite == NULL || testName == NULL || test == NULL) {
        printf("Test suite and test can't be NULL\n");
        abort();
    }
    if (testSuite->totalTests == MAX_TEST_CASES) {
        printf("Test suite can't handle more than %d\n", MAX_TEST_CASES);
        abort();
    }
    testSuite->testCases[testSuite->totalTests].name = testName;
    testSuite->testCases[testSuite->totalTests].testFunction = test;
    testSuite->totalTests += 1;
}

void runTestSuite(TestSuite* testSuite) {
    if (testSuite == NULL) {
        printf("Test suite can't be NULL\n");
        abort();
    }
    printf("---------- Tests Run Start ----------\n");
    shuffleTests(testSuite);
    signal(SIGABRT, signalHandler);
    signal(SIGSEGV, signalHandler);
    for (size_t i = 0; i < testSuite->totalTests; ++i) {
        testSuite->failedTests += runTestCase(&testSuite->testCases[i]);
    }
    signal(SIGABRT, SIG_DFL);
    signal(SIGSEGV, SIG_DFL);
    generateReport(testSuite);
}

void destroyTestSuite(TestSuite* testSuite) { free(testSuite); }

static void shuffleTests(TestSuite* testSuite) {
    const size_t n = testSuite->totalTests;
    if (n > 1) {
        for (size_t i = 0; i < n - 1; ++i) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            TestCase tmp;
            tmp.name = testSuite->testCases[i].name;
            tmp.testFunction = testSuite->testCases[i].testFunction;
            testSuite->testCases[i].name = testSuite->testCases[j].name;
            testSuite->testCases[i].testFunction = testSuite->testCases[j].testFunction;
            testSuite->testCases[j].name = tmp.name;
            testSuite->testCases[j].testFunction = tmp.testFunction;
        }
    }
}

static void generateReport(TestSuite* testSuite) {
    printf("--------- Tests Run Complete ---------\n");
    printf("Total %ld | Passed %ld | Failed %ld\n", testSuite->totalTests,
           testSuite->totalTests - testSuite->failedTests, testSuite->failedTests);
    printf("--------------------------------------\n");
}

void triggerAssertion(const char* functionName, const char* fileName, int line,
                      const char* condition) {
    printf("[.........] Assertion failure test %s in %s line %d. Condition: %s\n", functionName,
           fileName, line, condition);
    testStatus = TEST_FAILED;
    longjmp(buffer, TEST_FAILED);
}
