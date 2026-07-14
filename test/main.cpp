#define Z__FCF_TEST_RECURCIVE_RUN_DISABLE
#define FCF_TEST_IMPLEMENTATION
#include <fcfTest/test.hpp>

#include "tests/helpers.hpp"

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


FCF_TEST_DECLARE("subrun", "default", "subrun error check 1"){
  fcf::NTest::log() << "1" << std::endl;
  int v1 = 1;
  int v2 = 2;
  if (!FCF_TEST_CHECK(v1 == v2, v1)) {
    fcf::NTest::err() << "error" << std::endl;
  }
  fcf::NTest::log() << "2" << std::endl;
}

/*
FCF_TEST_DECLARE("MyLib", "Base", "Simple test"){
  fcf::NTest::log() << "Test started" << std::endl;
  int v1 = 1;
  int v2 = 2;
  if (!FCF_TEST_CHECK(v1 == v2, v1)) {
    fcf::NTest::err() << "Simple comparison failed" << std::endl;
  }
  fcf::NTest::log() << "Test resumed" << std::endl;
}
*/
FCF_TEST_DECLARE("subrun", "default", "subrun check 1"){
  fcf::NTest::log() << "1" << std::endl;
  int v1 = 1;
  int v2 = 1;
  if (FCF_TEST_CHECK(v1 == v2, v1)) {
    fcf::NTest::err() << "ok" << std::endl;
  } else {
    fcf::NTest::err() << "error" << std::endl;
  }
  fcf::NTest::log() << "2" << std::endl;
}




FCF_TEST_DECLARE("fcfTest", "cmdRun", "simple run"){
  {
    std::stringstream ss;
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{}, {}, {"subrun ok"}} );
    InnerTestRunner()(options, ss, 0, 0);
    std::string expected = std::string()+
                          "Performing the test: \"subrun\" -> \"default\" -> \"subrun ok\" ...\n"+
                          "    [SUCCESS] Test completed successfully (XXX sec)\n"+
                          "\n"+
                          "[SUCCESS] All tests were completed.\n"+
                          "Tests: 1 passed, 0 failed, 0 skipped, 1 total\n"+
                          "Duration: XXX sec\n";
    FCF_TEST(expected == uniout(ss.str()), uniout(ss.str(), true), uniout(expected, true));
  }
  {
    std::stringstream ss;
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{}, {}, {"subrun ok"}} );
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
    options.selectors.push_back( fcf::NTest::Options::Selector{{}, {}, {"subrun error 0"}} );
    const char* argv[] = {""};
    InnerTestRunner()(options, ss, 1, &argv[0]);
    std::string expected = std::string()+
                            "Performing the test: \"subrun\" -> \"default\" -> \"subrun error 0\" ...\n"+
                            "    Test error: v1 == v2  [FILE: XXX]\n"+
                            "    [FAILED] Test failed (XXX sec)\n"+
                            "\n"+
                            "[FAILED] Testing completed with failures.\n"+
                            "Tests: 0 passed, 1 failed, 0 skipped, 1 total\n"+
                            "Duration: XXX sec\n";
    FCF_TEST(expected == uniout(ss.str()), uniout(expected, true), uniout(ss.str(), true));
  }
  {
    std::stringstream ss;
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{}, {}, {"subrun error 1"}} );
    const char* argv[] = {""};
    InnerTestRunner()(options, ss, 1, &argv[0]);
    std::string expected = std::string()+
                            "Performing the test: \"subrun\" -> \"default\" -> \"subrun error 1\" ...\n"+
                            "    Test error: v1 == v2  [FILE: XXX]\n"+
                            "      Values:\n"+
                            "        v1: 1\n"+
                            "    [FAILED] Test failed (XXX sec)\n"+
                            "\n"+
                            "[FAILED] Testing completed with failures.\n"+
                            "Tests: 0 passed, 1 failed, 0 skipped, 1 total\n"+
                            "Duration: XXX sec\n";
    FCF_TEST(expected == uniout(ss.str()), uniout(expected, true), uniout(ss.str(), true));
  }
  {
    std::stringstream ss;
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{}, {}, {"subrun error 2"}} );
    const char* argv[] = {""};
    InnerTestRunner()(options, ss, 1, &argv[0]);
    std::string expected = std::string()+
                            "Performing the test: \"subrun\" -> \"default\" -> \"subrun error 2\" ...\n"+
                            "    Test error: v1 == v2  [FILE: XXX]\n"+
                            "      Values:\n"+
                            "        v1: 1\n"+
                            "        v2: 2\n"+
                            "    [FAILED] Test failed (XXX sec)\n"+
                            "\n"+
                            "[FAILED] Testing completed with failures.\n"+
                            "Tests: 0 passed, 1 failed, 0 skipped, 1 total\n"+
                            "Duration: XXX sec\n";
    FCF_TEST(expected == uniout(ss.str()), uniout(expected, true), uniout(ss.str(), true));
  }
  {
    std::stringstream ss;
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{}, {}, {"subrun error check 1"}} );
    const char* argv[] = {""};
    InnerTestRunner()(options, ss, 1, &argv[0]);
    std::string expected = std::string()+
                            "Performing the test: \"subrun\" -> \"default\" -> \"subrun error check 1\" ...\n" +
                            "  > 1\n" +
                            "  > error\n" +
                            "  > 2\n" +
                            "    Test error: v1 == v2  [FILE: XXX]\n" +
                            "      Values:\n" +
                            "        v1: 1\n" +
                            "    [FAILED] Test failed (XXX sec)\n" +
                            "\n" +
                            "[FAILED] Testing completed with failures.\n" +
                            "Tests: 0 passed, 1 failed, 0 skipped, 1 total\n" +
                            "Duration: XXX sec\n"
                            ;
    FCF_TEST(expected == uniout(ss.str()), uniout(expected, true), uniout(ss.str(), true));
  }
  {
    std::stringstream ss;
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{}, {}, {"subrun check 1"}} );
    const char* argv[] = {""};
    InnerTestRunner()(options, ss, 1, &argv[0]);
    std::string expected = std::string()+
                            "Performing the test: \"subrun\" -> \"default\" -> \"subrun check 1\" ...\n" +
                            "  > 1\n" +
                            "  > ok\n" +
                            "  > 2\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n"+
                            "\n"+
                            "[SUCCESS] All tests were completed.\n"+
                            "Tests: 1 passed, 0 failed, 0 skipped, 1 total\n"+
                            "Duration: XXX sec\n";
                            ;
    FCF_TEST(expected == uniout(ss.str()), uniout(expected, true), uniout(ss.str(), true));
  }

}

FCF_TEST_GROUP_ORDER("macro", 1);
FCF_TEST_GROUP_ORDER("cmd",   1000001);
FCF_TEST_GROUP_ORDER("cmdRun", 1000002);

int main(int a_argc, char* a_argv[]) {
  bool error = false;
  fcf::NTest::Options options;
  options.ignoreSelectors.push_back(
    fcf::NTest::Options::Selector{
      {"subrun", "subrun-export", "subrun-order 1", "subrun-order 2", "subrun-order 3"}, 
      {}, 
      {}
    }
  );
  fcf::NTest::cmdRun(options, a_argc, a_argv, fcf::NTest::CRM_RUN, &error);
  return error ? 1 : 0;
}

