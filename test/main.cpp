#include <sstream>
#include <regex>

#define _FCF_TEST_RECURCIVE_RUN_DISABLE
#define FCF_TEST_IMPLEMENTATION
#include <fcfTest/test.hpp>


std::string uniout(std::string a_string){
  std::string result;
  std::regex pattern1(R"(\d+\.\d+`\d+`\d+)");
  std::regex pattern2(R"(time=\"\d+\.\d+\")");
  result = std::regex_replace(a_string, pattern1, "XXX");
  result = std::regex_replace(result, pattern2, "time=\"XXX\"");
  return result;
}

FCF_TEST_GROUP_ORDER("cmd", 1);

FCF_TEST_DECLARE("fcfTest", "cmdRun", "simple run"){
  {
    std::stringstream ss;
    fcf::NTest::Options options;
    options.parts   = {"subrun"};
    options.stream  = &ss;
    fcf::NTest::cmdRun(options, 0, 0, fcf::NTest::CRM_RUN);
    std::string expected = std::string()+ 
                          "Performing the test: \"subrun\" -> \"default\" -> \"ok\" ...\n"+
                          "  Test completed successfully (XXX sec)\n"+
                          "\n"+
                          "All tests were completed.\n"+
                          "Tests: 1 passed, 0 failed, 0 skiped, 1 total\n"+
                          "Duration: XXX sec\n";
    FCF_TEST(expected == uniout(ss.str()));
  }
  {
    std::stringstream ss;
    fcf::NTest::Options options;
    options.parts   = {"subrun"};
    options.stream  = &ss;
    const char* argv[] = {"--test-format=junit"};
    fcf::NTest::cmdRun(options, 1, argv, fcf::NTest::CRM_RUN);
    std::string expected = std::string()+ 
                          "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"+
                          "<testsuites tests=\"1\" failure=\"0\" skipped=\"0\" time=\"XXX\">\n"+
                          "  <testsuite name=\"subrun/default\" tests=\"1\" failure=\"0\" skipped=\"0\" time=\"XXX\">\n"+
                          "    <testcase classname=\"subrun/default\" name=\"ok\" time=\"XXX\"/>\n"+
                          "  </testsuite>\n"+
                          "</testsuites>\n";
    FCF_TEST(expected == uniout(ss.str()));
  }
}

FCF_TEST_DECLARE("run1", "default", "ok"){
  FCF_TEST(1 == 1, "1");
}


FCF_TEST_DECLARE("subrun", "default", "ok"){

}
/*
FCF_TEST_DECLARE("run", "junit", "ok"){

}
*/


int main(int a_argc, char* a_argv[]) {
  bool error = false;
  fcf::NTest::Options options;
  options.ignoreParts = {"subrun"};
  fcf::NTest::cmdRun(options, a_argc, a_argv, fcf::NTest::CRM_RUN, &error);
  return error ? 1 : 0;
}

