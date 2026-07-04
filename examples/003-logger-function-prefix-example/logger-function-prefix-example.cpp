#define FCF_TEST_IMPLEMENTATION
#include <fcfTest/test.hpp>

FCF_TEST_DECLARE("Library", "Math", "sum"){
  fcf::NTest::log() << "Example asda\nasdas" << std::endl;
}

int main(int a_argc, char* a_argv[]) {
  bool error = false;
  fcf::NTest::cmdRun(a_argc, a_argv, fcf::NTest::CRM_RUN, &error);
  return error ? 1 : 0;
}
