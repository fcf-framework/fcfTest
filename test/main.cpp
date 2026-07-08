#include <sstream>
#include <regex>

#define Z__FCF_TEST_RECURCIVE_RUN_DISABLE
#define FCF_TEST_IMPLEMENTATION
#include <fcfTest/test.hpp>


FCF_TEST_DECLARE("fcfTest", "some", "asd"){

  //FCF_TEST_CHECK(1==2, 2);

}


FCF_TEST_DECLARE("subrun", "default", "subrun ok"){

}

FCF_TEST_DECLARE("subrun", "default", "subrun error 0"){
  int v1 = 1;
  int v2 = 2;
  FCF_TEST(v1 == v2);
}

FCF_TEST_DECLARE("subrun", "default", "subrun error 1"){
  int v1 = 1;
  int v2 = 2;
  FCF_TEST(v1 == v2, v1);
}

FCF_TEST_DECLARE("subrun", "default", "subrun error 2"){
  int v1 = 1;
  int v2 = 2;
  FCF_TEST(v1 == v2, v1, v2);
}

std::string uniout(std::string a_string, bool a_wrap = false){
  std::string result;
  std::regex pattern1(R"(\d+\.\d+`\d+`\d+)");
  std::regex pattern2(R"(time=\"\d+\.\d+\")");
  std::regex pattern3(R"(\[FILE:[^\]]*)");
  std::regex pattern4(R"(\x1b\[[0-9;]*m)");
  result = std::regex_replace(a_string, pattern1, "XXX");
  result = std::regex_replace(result, pattern2, "time=\"XXX\"");
  result = std::regex_replace(result, pattern3, "[FILE: XXX");
  result = std::regex_replace(result, pattern4, "");
  if (a_wrap) {
    return std::string() + "<<<" + result + ">>>";
  } else {
    return result;
  }
}

class InnerTestRunner{
  public:
    InnerTestRunner()
      : _targets(fcf::NTest::logger().targets())
    {
    }
    void operator()(fcf::NTest::Options& a_options, std::stringstream& a_sstream, int a_argc, const char** a_argv){
      fcf::NTest::LogOutputTargets targets = { {"default", &a_sstream, "", {}, {}} };
      fcf::NTest::logger().targets(targets);
      bool error = false;
      fcf::NTest::cmdRun(a_options, a_argc, a_argv, fcf::NTest::CRM_RUN, &error);
    }
    ~InnerTestRunner(){
      fcf::NTest::logger().targets(_targets);
    }
  private:
    fcf::NTest::LogOutputTargets  _targets;

};



FCF_TEST_DECLARE("fcfTest", "cmdRun", "simple run"){
  {
    std::stringstream ss;
    fcf::NTest::Options options;
    options.tests   = {"subrun ok"};
    InnerTestRunner()(options, ss, 0, 0);
    std::string expected = std::string()+
                          "Performing the test: \"subrun\" -> \"default\" -> \"subrun ok\" ...\n"+
                          "   [SUCCESS] Test completed successfully (XXX sec)\n"+
                          "\n"+
                          "[SUCCESS] All tests were completed.\n"+
                          "Tests: 1 passed, 0 failed, 0 skipped, 1 total\n"+
                          "Duration: XXX sec\n";
    FCF_TEST(expected == uniout(ss.str()), uniout(ss.str(), true), uniout(expected, true));
  }
  {
    std::stringstream ss;
    fcf::NTest::Options options;
    options.tests   = {"subrun ok"};
    const char* argv[] = {"--test-format=junit"};
    InnerTestRunner()(options, ss, 1, &argv[0]);
    std::string expected = std::string()+
                          "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"+
                          "<testsuites tests=\"1\" failure=\"0\" skipped=\"0\" time=\"XXX\">\n"+
                          "  <testsuite name=\"subrun/default\" tests=\"1\" failure=\"0\" skipped=\"0\" time=\"XXX\">\n"+
                          "    <testcase classname=\"subrun/default\" name=\"subrun ok\" time=\"XXX\"/>\n"+
                          "  </testsuite>\n"+
                          "</testsuites>\n";
    FCF_TEST(expected == uniout(ss.str()), uniout(ss.str(), true));
  }

  {
    std::stringstream ss;
    fcf::NTest::Options options;
    options.tests   = {"subrun error 0"};
    const char* argv[] = {""};
    InnerTestRunner()(options, ss, 1, &argv[0]);
    std::string expected = std::string()+
                            "Performing the test: \"subrun\" -> \"default\" -> \"subrun error 0\" ...\n"+
                            "   Test error: v1 == v2  [FILE: XXX]\n"+
                            "   [FAILED] Test failed (XXX sec)\n"+
                            "\n"+
                            "[FAILED] Testing completed with failures.\n"+
                            "Tests: 1 passed, 1 failed, 0 skipped, 1 total\n"+
                            "Duration: XXX sec\n";
    FCF_TEST(expected == uniout(ss.str()), uniout(expected, true), uniout(ss.str(), true));
  }
  {
    std::stringstream ss;
    fcf::NTest::Options options;
    options.tests   = {"subrun error 1"};
    const char* argv[] = {""};
    InnerTestRunner()(options, ss, 1, &argv[0]);
    std::string expected = std::string()+
                            "Performing the test: \"subrun\" -> \"default\" -> \"subrun error 1\" ...\n"+
                            "   Test error: v1 == v2  [FILE: XXX]\n"+
                            "     Values:\n"+
                            "       v1: 1\n"+
                            "   [FAILED] Test failed (XXX sec)\n"+
                            "\n"+
                            "[FAILED] Testing completed with failures.\n"+
                            "Tests: 1 passed, 1 failed, 0 skipped, 1 total\n"+
                            "Duration: XXX sec\n";
    FCF_TEST(expected == uniout(ss.str()), uniout(expected, true), uniout(ss.str(), true));
  }
  {
    std::stringstream ss;
    fcf::NTest::Options options;
    options.tests   = {"subrun error 2"};
    const char* argv[] = {""};
    InnerTestRunner()(options, ss, 1, &argv[0]);
    std::string expected = std::string()+
                            "Performing the test: \"subrun\" -> \"default\" -> \"subrun error 2\" ...\n"+
                            "   Test error: v1 == v2  [FILE: XXX]\n"+
                            "     Values:\n"+
                            "       v1: 1\n"+
                            "       v2: 2\n"+
                            "   [FAILED] Test failed (XXX sec)\n"+
                            "\n"+
                            "[FAILED] Testing completed with failures.\n"+
                            "Tests: 1 passed, 1 failed, 0 skipped, 1 total\n"+
                            "Duration: XXX sec\n";
    FCF_TEST(expected == uniout(ss.str()), uniout(expected, true), uniout(ss.str(), true));
  }

}



FCF_TEST_GROUP_ORDER("macro", 1);
FCF_TEST_GROUP_ORDER("cmd",   2);
FCF_TEST_GROUP_ORDER("cmdRun", 3);

int main(int a_argc, char* a_argv[]) {
  bool error = false;
  fcf::NTest::Options options;
  options.ignoreParts = {"subrun"};
  fcf::NTest::cmdRun(options, a_argc, a_argv, fcf::NTest::CRM_RUN, &error);
  return error ? 1 : 0;
}

