#include <fcfTest/test.hpp>
#include "helpers.hpp"

FCF_TEST_DEFINE("fcfTest", "fixture-run", "fixture") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun-fixture"}, {"fixture"}, {"*"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "  > fixture begin\n" + 
                            "Performing the test: \"subrun-fixture\" -> \"fixture\" -> \"fixture\" ...\n" + 
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" + 
                            "Performing the test: \"subrun-fixture\" -> \"fixture\" -> \"fixture-001\" ...\n" + 
                            "    error\n" + 
                            "    [FAILED] Test failed (XXX sec)\n" + 
                            "  > fixture end\n" + 
                            "\n" + 
                            "[FAILED] Testing completed with failures.\n" + 
                            "Tests: 1 passed, 1 failed, 1 skipped, 3 total\n" + 
                            "Duration: XXX sec\n";

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(error);
  }
}

FCF_TEST_BEFORE_DEFINE("subrun-fixture", "fixture", "*", fcf::NTest::FL_GROUP) {
  fcf::NTest::log() << "fixture begin" << std::endl;
}

FCF_TEST_AFTER_DEFINE("subrun-fixture", "fixture", "*", fcf::NTest::FL_GROUP) {
  fcf::NTest::log() << "fixture end" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture", "fixture") {
}

FCF_TEST_DEFINE("subrun-fixture", "fixture", "fixture-001") {
  throw std::runtime_error("error");
}

FCF_TEST_DEFINE("subrun-fixture", "fixture", "fixture-002") {
}

