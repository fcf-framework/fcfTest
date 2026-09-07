#include <fcfTest/test.hpp>
#include "helpers.hpp"
#include <regex>

struct DurationTester {
  void junit(const std::string& a_string) {
    std::regex patternSuites("<testsuites.*time=\"\\d+\\.(\\d+)\"");
    std::smatch matches;
    unsigned long long totalDuration = 0;
    if (std::regex_search(a_string, matches, patternSuites)) {
      totalDuration = std::stoull(matches[1]);
    } else {
      FCF_TEST(false,
              "Invaid xml",
              a_string);
    }
    FCF_TEST(totalDuration);

    unsigned long long totalDurationSum = 0;
    std::regex patternSuite("<testsuite .*time=\"\\d+\\.(\\d+)\"");
    std::sregex_iterator suiteMatchesBegin = std::sregex_iterator(a_string.begin(), a_string.end(), patternSuite);
    std::sregex_iterator suiteMatchesEnd = std::sregex_iterator();
    for(; suiteMatchesBegin != suiteMatchesEnd; ++suiteMatchesBegin){
      totalDurationSum += std::stoull(suiteMatchesBegin->str(1));
    }

    FCF_TEST(totalDurationSum == totalDuration, totalDurationSum, totalDuration, a_string);
  }

};

namespace {
  struct TestParam {
    bool        noBreak;
    std::string format;
    std::string expected;
  };
}

/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "case-error", "case-error 3-e2"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_BEFORE_DEFINE("fcfTest", "case-error", "case-error 3-e2", fcf::NTest::FL_GLOBAL) {
  fcf::NTest::TestPath path = {"fcfTest", "case-error", "case-error 3-e2"};
  fcf::NTest::storage().appendParamValue(
    path,
    TestParam{
      false,

      "default",

      std::string() +
      "Performing the test: \"subrun\" -> \"case-error 3-e2\" -> \"a first test\" ...\n" +
      " == Parameter set: 1\n" +
      "    Parameter status: success (duration: XXX sec)\n"+
      " == Parameter set: 2\n" +
      "    Parameter status: success (duration: XXX sec)\n"+
      "    [SUCCESS] Test completed successfully (XXX sec)\n" +
      "Performing the test: \"subrun\" -> \"case-error 3-e2\" -> \"main test\" ...\n" +
      " == Parameter set: 1\n" +
      "    Parameter status: success (duration: XXX sec)\n"+
      " == Parameter set: 2\n" +
      "    Test error: 1 == 2  [FILE: XXX]\n" +
      "    Parameter status: failed (duration: XXX sec)\n"+
      "    [FAILED] Test failed (XXX sec)\n" +
      "\n" +
      "[FAILED] Testing completed with failures.\n" +
      "Tests: 1 passed, 1 failed, 1 skipped, 3 total\n" +
      "Duration: XXX sec\n"
    }
  );
  fcf::NTest::storage().appendParamValue(
    path,
    TestParam{
      true,

      "default",

      "Performing the test: \"subrun\" -> \"case-error 3-e2\" -> \"a first test\" ...\n"
      " == Parameter set: 1\n"
      "    Parameter status: success (duration: XXX sec)\n"
      " == Parameter set: 2\n"
      "    Parameter status: success (duration: XXX sec)\n"
      "    [SUCCESS] Test completed successfully (XXX sec)\n"
      "Performing the test: \"subrun\" -> \"case-error 3-e2\" -> \"main test\" ...\n"
      " == Parameter set: 1\n"
      "    Parameter status: success (duration: XXX sec)\n"
      " == Parameter set: 2\n"
      "    Test error: 1 == 2  [FILE: XXX]\n"
      "    Parameter status: failed (duration: XXX sec)\n"
      " == Parameter set: 3\n"
      "    Parameter status: success (duration: XXX sec)\n"
      "    [FAILED] Test failed (XXX sec)\n"
      "Performing the test: \"subrun\" -> \"case-error 3-e2\" -> \"w last test\" ...\n"
      " == Parameter set: 1\n"
      "    Parameter status: success (duration: XXX sec)\n"
      " == Parameter set: 2\n"
      "    Parameter status: success (duration: XXX sec)\n"
      "    [SUCCESS] Test completed successfully (XXX sec)\n"
      "\n"
      "[FAILED] Testing completed with failures.\n"
      "Tests: 2 passed, 1 failed, 0 skipped, 3 total\n"
      "Duration: XXX sec\n"
    }
  );
  fcf::NTest::storage().appendParamValue(
    path,
    TestParam{
      false,

      "junit",

      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<testsuites tests=\"7\" failure=\"1\" skipped=\"3\" time=\"XXX\">\n"
      "  <testsuite name=\"subrun/case-error 3-e2\" tests=\"7\" failure=\"1\" skipped=\"3\" time=\"XXX\">\n"
      "    <testcase classname=\"subrun/case-error 3-e2\" name=\"a first test[1]\" time=\"XXX\"/>\n"
      "    <testcase classname=\"subrun/case-error 3-e2\" name=\"a first test[2]\" time=\"XXX\"/>\n"
      "    <testcase classname=\"subrun/case-error 3-e2\" name=\"main test[1]\" time=\"XXX\"/>\n"
      "    <testcase classname=\"subrun/case-error 3-e2\" name=\"main test[2]\" time=\"XXX\">\n"
      "      <failure message=\"Test error: 1 == 2\" type=\"AssertionError\">\n"
      "Test error: 1 == 2  [FILE: XXX]\n"
      "      </failure>\n"
      "    </testcase>\n"
      "    <testcase classname=\"subrun/case-error 3-e2\" name=\"main test[3]\" time=\"XXX\">\n"
      "      <skipped message=\"The test was skipped because the fail-on-error mode was enabled.\"/>\n"
      "    </testcase>\n"
      "    <testcase classname=\"subrun/case-error 3-e2\" name=\"w last test[1]\" time=\"XXX\">\n"
      "      <skipped message=\"The test was skipped because the fail-on-error mode was enabled.\"/>\n"
      "    </testcase>\n"
      "    <testcase classname=\"subrun/case-error 3-e2\" name=\"w last test[2]\" time=\"XXX\">\n"
      "      <skipped message=\"The test was skipped because the fail-on-error mode was enabled.\"/>\n"
      "    </testcase>\n"
      "  </testsuite>\n"
      "</testsuites>\n"
    }
  );
  fcf::NTest::storage().appendParamValue(
    path,
    TestParam{
      true,

      "junit",

      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<testsuites tests=\"7\" failure=\"1\" skipped=\"0\" time=\"XXX\">\n"
      "  <testsuite name=\"subrun/case-error 3-e2\" tests=\"7\" failure=\"1\" skipped=\"0\" time=\"XXX\">\n"
      "    <testcase classname=\"subrun/case-error 3-e2\" name=\"a first test[1]\" time=\"XXX\"/>\n"
      "    <testcase classname=\"subrun/case-error 3-e2\" name=\"a first test[2]\" time=\"XXX\"/>\n"
      "    <testcase classname=\"subrun/case-error 3-e2\" name=\"main test[1]\" time=\"XXX\"/>\n"
      "    <testcase classname=\"subrun/case-error 3-e2\" name=\"main test[2]\" time=\"XXX\">\n"
      "      <failure message=\"Test error: 1 == 2\" type=\"AssertionError\">\n"
      "Test error: 1 == 2  [FILE: XXX]\n"
      "      </failure>\n"
      "    </testcase>\n"
      "    <testcase classname=\"subrun/case-error 3-e2\" name=\"main test[3]\" time=\"XXX\"/>\n"
      "    <testcase classname=\"subrun/case-error 3-e2\" name=\"w last test[1]\" time=\"XXX\"/>\n"
      "    <testcase classname=\"subrun/case-error 3-e2\" name=\"w last test[2]\" time=\"XXX\"/>\n"
      "  </testsuite>\n"
      "</testsuites>\n"
    }
  );
}

FCF_TEST_DEFINE("fcfTest", "case-error", "case-error 3-e2") {
  TestParam testParam = *fcf::NTest::state().param().cast<TestParam>();

  fcf::NTest::Options options;
  options.noBreak = testParam.noBreak;
  options.format = testParam.format;
  options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"case-error 3-e2"}, {"*"}} );
  std::stringstream ss;
  bool error = InnerTestRunner().run(options, ss);

  std::string content = uniout(ss.str(), true);
  std::string expected = uniout(testParam.expected, true);

  FCF_TEST(content == expected, content, expected);
  FCF_TEST(error);

  if (testParam.format == "junit"){
    DurationTester dt;
    dt.junit(ss.str());
  }
}

FCF_TEST_BEFORE_DEFINE("subrun", "case-error 3-e2", "*", fcf::NTest::FL_GLOBAL) {
  fcf::NTest::TestPath path = {"subrun", "case-error 3-e2", "main test"};
  if (fcf::NTest::storage().params(path).empty()) {
    fcf::NTest::storage().appendParamValue(path, (int)1, (int)2, (int)3);
    fcf::NTest::storage().appendParamValue("subrun", "case-error 3-e2", "a first test", (int)1, (int)2);
    fcf::NTest::storage().appendParamValue("subrun", "case-error 3-e2", "w last test", (int)1, (int)2);
  }
}

FCF_TEST_DEFINE("subrun", "case-error 3-e2", "a first test") {
}

FCF_TEST_DEFINE("subrun", "case-error 3-e2", "main test") {
  if (fcf::NTest::state().paramIndex() == 1){
    FCF_TEST(1 == 2);
  }
}

FCF_TEST_DEFINE("subrun", "case-error 3-e2", "w last test") {
}

