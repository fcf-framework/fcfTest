#include <vector>
#include <cmath>

// It is necessary to define the `FCF_TEST_IMPLEMENTATION` macro so that the 
// implementations are exposed when the header file is included. 
// If the `fcfTest/test.hpp` file is included multiple times within a project, 
// this macro should be defined in only one `.cpp` file.
//
// When working with DLLs, you must define both the `FCF_TEST_IMPLEMENTATION` 
// and `FCF_TEST_EXPORT` macros within the main library that exports 
// the functions; conversely, in libraries that import these functions, 
// you need to define only the `FCF_TEST_IMPORT` macro.
#define FCF_TEST_IMPLEMENTATION
#include <fcfTest/test.hpp>


// --- Test Declarations ---
FCF_TEST_DECLARE("Math" /*PART NAME*/, "BasicArithmetic" /*GROUP NAME*/, "Addition" /*TEST NAME*/) {
  // We create an object to measure execution duration
  // over 10,000 iterations.
  fcf::NTest::Duration bench(10000);

  // Set the starting time point for measuring execution time.
  bench.begin();
  for(size_t i = 0; i < bench.iterations(); ++i) {
    int a = 2;
    int b = 3;
    // Performing a check of the unit test execution.
    FCF_TEST(a + b == 5, a, b);
  }
  // We set the final time point for measuring execution time.
  bench.end();

  // Outputting the execution time measurement result at the 'info' logging level.
  fcf::NTest::inf() << "  Itertion count: " << bench.iterations() << std::endl;
  fcf::NTest::inf() << "  Total: " << bench.totalDuration().count() << " ns" << std::endl;
  fcf::NTest::inf() << "  Avg: " << bench.duration().count() << " ns" << std::endl;
}

FCF_TEST_DECLARE("Math" /*PART NAME*/, "BasicArithmetic" /*GROUP NAME*/, "Subtraction" /*TEST NAME*/) {
  // We create an object to measure execution duration
  // over 10,000 iterations.
  fcf::NTest::Duration bench(10000);

  // We perform the task 10,000 times.
  bench([](){
    int a = 10;
    int b = 4;
    // Performing a check of the unit test execution.
    FCF_TEST(a - b == 6, a, b);
  });

  // Outputting the execution time measurement result at the 'info' logging level.
  fcf::NTest::inf() << "  Itertion count: " << bench.iterations() << std::endl;
  fcf::NTest::inf() << "  Total: " << bench.totalDuration().count() << " ns" << std::endl;
  fcf::NTest::inf() << "  Avg: " << bench.duration().count() << " ns" << std::endl;
}

FCF_TEST_DECLARE("Vector" /*PART NAME*/, "SizeCheck" /*GROUP NAME*/, "EmptyVector" /*TEST NAME*/) {
    std::vector<int> v;
    FCF_TEST(v.size() == 0, v.size());
}

// --- Order Registration ---
// Run Math tests before Vector tests
FCF_TEST_PART_ORDER("Math", 1);
FCF_TEST_PART_ORDER("Vector", 2);

// Run "BasicArithmetic" group first within Math part
FCF_TEST_GROUP_ORDER("BasicArithmetic", 1);

// Run Addition test first
FCF_TEST_TEST_ORDER("Addition", 1);

int main(int a_argc, char* a_argv[]) {
  // Use CRM_RUN for standard execution
  bool error;
  fcf::NTest::cmdRun(a_argc, a_argv, fcf::NTest::CRM_RUN, &error);
  return error ? 1 : 0;
}
