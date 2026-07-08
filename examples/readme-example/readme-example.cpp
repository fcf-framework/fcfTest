#define FCF_TEST_IMPLEMENTATION
#include <fcfTest/test.hpp>
#include <vector>

// Declare a test case
FCF_TEST_DECLARE("MyLibraryPartName", "ExamplesGroupName", "VectorSizeTestName"){
  std::vector<std::string> vec;
  vec.push_back("test");

  // Perform an assertion. If false, throws std::runtime_error with details.
  FCF_TEST(vec.size() == 2, vec.size());
}

int main(int a_argc, char* a_argv[]){
  // Run the test suite via command line interface
  bool error;
  fcf::NTest::cmdRun(a_argc, a_argv, fcf::NTest::CRM_RUN, &error);
  return error ? 1 : 0;
}
