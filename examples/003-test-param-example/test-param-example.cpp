#define FCF_TEST_IMPLEMENTATION
#include <fcfTest/test.hpp>

// In the fixture we set parameters that will always be added
FCF_TEST_BEFORE_DEFINE("*", "*", "*", fcf::NTest::FL_GLOBAL){
  fcf::NTest::TestPath path = {"Library", "Math", "sum"};
  fcf::NTest::storage().appendParamValue(path, std::string("one")
                                                , std::string("two")
                                                , std::string("three"));
}

FCF_TEST_DEFINE("Library", "Math", "sum"){
  // Get the test parameter value
  fcf::NTest::log() << "Current test parameter: " << *fcf::NTest::state().param().cast<std::string>() << std::endl;
}

int main(int a_argc, char* a_argv[]) {
  // Add test parameters passed via the command line
  for(int argIndex = 1; argIndex < a_argc; ++argIndex) {
    if (std::strcmp(a_argv[argIndex], "--param") == 0 && (argIndex + 1)  < a_argc) {
      fcf::NTest::storage().appendParamValue("Library", "Math", "sum", std::string(a_argv[argIndex+1]));
      ++argIndex;
    }
  }
  // Start testing execution
  bool error = false;
  fcf::NTest::cmdRun(a_argc, a_argv, fcf::NTest::CRM_RUN, &error);
  return error ? 1 : 0;
}
