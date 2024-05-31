#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

// Function prototypes
void runTests();
void runTest(const char *testName, void (*testFunction)());
void redirectInput(const char *input);
void restoreInput();
void redirectOutput();
void restoreOutput();
char *getOutput();

FILE *originalInput;
FILE *originalOutput;
FILE *testInput;
FILE *testOutput;
char outputBuffer[1024];

// Test cases prototypes
void testAllocateEqualResources();
void testAllocateResourcesBasedOnShortestPath();
void testPatientEntry();
void testPatientDeparture();
void testDisplayPatientAllocations();
void testShowDepartmentResources();

int main()
{
    runTests();
    return 0;
}

void runTests()
{
    runTest("Allocate Equal Resources", testAllocateEqualResources);
    runTest("Allocate Resources Based on Shortest Path", testAllocateResourcesBasedOnShortestPath);
    runTest("Patient Entry", testPatientEntry);
    runTest("Patient Departure", testPatientDeparture);
    runTest("Display Patient Allocations", testDisplayPatientAllocations);
    runTest("Show Department Resources", testShowDepartmentResources);
}

void runTest(const char *testName, void (*testFunction)())
{
    printf("Running test: %s\n", testName);
    redirectOutput();
    testFunction();
    restoreOutput();
    printf("Output:\n%s\n", outputBuffer);
    printf("End of test: %s\n\n", testName);
}

void redirectInput(const char *input)
{
    testInput = fmemopen((void *)input, strlen(input), "r");
    originalInput = stdin;
    stdin = testInput;
}

void restoreInput()
{
    stdin = originalInput;
    fclose(testInput);
}

void redirectOutput()
{
    testOutput = open_memstream(&outputBuffer, (size_t[1]){1024});
    originalOutput = stdout;
    stdout = testOutput;
}

void restoreOutput()
{
    fclose(testOutput);
    stdout = originalOutput;
}

char *getOutput()
{
    fflush(testOutput);
    return outputBuffer;
}
