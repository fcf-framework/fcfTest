#include <fcfTest/test.hpp>
#include "helpers.hpp"


FCF_TEST_TEST_ORDER("Setting a value within a fixture", 1);
FCF_TEST_TEST_ORDER("Setting 2 values within a fixture", 2);
FCF_TEST_TEST_ORDER("Setting 3 values within a fixture", 3);
FCF_TEST_TEST_ORDER("Setting a 1 shared value within a test", 4);
FCF_TEST_TEST_ORDER("Setting a 1 value within a test", 5);
FCF_TEST_TEST_ORDER("Setting 2 values within a test", 6);
FCF_TEST_TEST_ORDER("Setting 3 values within a test", 7);

/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "param", "Setting a value within a fixture"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "param", "Setting a value within a fixture") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"param"}, {"value 1 in fixture"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"param\" -> \"value 1 in fixture\" ...\n" +
                            " == Parameter set: 1\n" +
                            "  > Test index: 0\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "\n" +
                            "[SUCCESS] All tests were completed.\n" +
                            "Tests: 1 passed, 0 failed, 0 skipped, 1 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(!error);
  }
}

FCF_TEST_BEFORE_DEFINE("subrun", "param", "value 1 in fixture", fcf::NTest::FL_GLOBAL) {
  fcf::NTest::storage().appendParamValue("subrun", "param", "value 1 in fixture", (int)1);
}

FCF_TEST_DEFINE("subrun", "param", "value 1 in fixture") {
  fcf::NTest::log() << "Test index: " << fcf::NTest::state().paramIndex() << std::endl;
  FCF_TEST(((int)fcf::NTest::state().paramIndex()+1 == *fcf::NTest::state().param().cast<int>()),
           fcf::NTest::state().paramIndex(),
           (*fcf::NTest::state().param().cast<int>())
          );
}


/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "param", "Setting 2 values within a fixture"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "param", "Setting 2 values within a fixture") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"param"}, {"value 2 in fixture"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"param\" -> \"value 2 in fixture\" ...\n" +
                            " == Parameter set: 1\n" +
                            "  > Test index: 0\n" +
                            " == Parameter set: 2\n" +
                            "  > Test index: 1\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "\n" +
                            "[SUCCESS] All tests were completed.\n" +
                            "Tests: 1 passed, 0 failed, 0 skipped, 1 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(!error);
  }
}

FCF_TEST_BEFORE_DEFINE("subrun", "param", "value 2 in fixture", fcf::NTest::FL_GLOBAL) {
  fcf::NTest::Options options;
  options.selectors.push_back({{"subrun"}, {"param"}, {"value 2 in fixture"}});
  fcf::NTest::Test test = *fcf::NTest::storage().selectTests(options).begin();
  fcf::NTest::storage().appendParamValue(test, (int)1);
  fcf::NTest::storage().appendParamValue(test, (int)2);
}

FCF_TEST_DEFINE("subrun", "param", "value 2 in fixture") {
  fcf::NTest::log() << "Test index: " << fcf::NTest::state().paramIndex() << std::endl;
  FCF_TEST(((int)fcf::NTest::state().paramIndex()+1 == *fcf::NTest::state().param().cast<int>()),
           fcf::NTest::state().paramIndex(),
           (*fcf::NTest::state().param().cast<int>())
          );
}

/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "param", "Setting 3 values within a fixture"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "param", "Setting 3 values within a fixture") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"param"}, {"value 3 in fixture"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"param\" -> \"value 3 in fixture\" ...\n" +
                            " == Parameter set: 1\n" +
                            "  > Test index: 0\n" +
                            " == Parameter set: 2\n" +
                            "  > Test index: 1\n" +
                            " == Parameter set: 3\n" +
                            "  > Test index: 2\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "\n" +
                            "[SUCCESS] All tests were completed.\n" +
                            "Tests: 1 passed, 0 failed, 0 skipped, 1 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(!error);
  }
}

FCF_TEST_BEFORE_DEFINE("subrun", "param", "value 3 in fixture", fcf::NTest::FL_GLOBAL) {
  fcf::NTest::TestPath path = {"subrun", "param", "value 3 in fixture"};
  fcf::NTest::storage().appendParamValue(path, (int)1);
  fcf::NTest::storage().appendParamValue(path, (int)2);
  fcf::NTest::storage().appendParamValue(path, (int)3);
}

FCF_TEST_DEFINE("subrun", "param", "value 3 in fixture") {
  fcf::NTest::log() << "Test index: " << fcf::NTest::state().paramIndex() << std::endl;
  FCF_TEST(((int)fcf::NTest::state().paramIndex()+1 == *fcf::NTest::state().param().cast<int>()),
           fcf::NTest::state().paramIndex(),
           (*fcf::NTest::state().param().cast<int>())
          );
}


/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "param", "Setting a 1 shared value within a test"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "param", "Setting a 1 shared value within a test") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"param"}, {"shared value 1 in test"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"param\" -> \"shared value 1 in test\" ...\n" +
                            " == Parameter set: 1\n" +
                            "  > Test index: 0\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "\n" +
                            "[SUCCESS] All tests were completed.\n" +
                            "Tests: 1 passed, 0 failed, 0 skipped, 1 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(!error);
  }
}

FCF_TEST_DEFINE("subrun", "param", "shared value 1 in test") {
  if (!fcf::NTest::state().paramIndex()) {
    fcf::NTest::storage().appendParamValue(fcf::NTest::state().test(), (int)1);
  }
  fcf::NTest::log() << "Test index: " << fcf::NTest::state().paramIndex() << std::endl;
  FCF_TEST(((int)fcf::NTest::state().paramIndex()+1 == *fcf::NTest::state().param().cast<int>()),
           fcf::NTest::state().paramIndex(),
           (*fcf::NTest::state().param().cast<int>())
          );
}

/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "param", "Setting a 1 value within a test"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "param", "Setting a 1 value within a test") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"param"}, {"value 1 in test"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"param\" -> \"value 1 in test\" ...\n" +
                            " == Parameter set: 1\n" +
                            "  > Test index: 0\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "\n" +
                            "[SUCCESS] All tests were completed.\n" +
                            "Tests: 1 passed, 0 failed, 0 skipped, 1 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(!error);
  }
}

FCF_TEST_DEFINE("subrun", "param", "value 1 in test") {
  if (!fcf::NTest::state().paramIndex()) {
    fcf::NTest::storage().appendParamValue(fcf::NTest::state().test(), (int)1);
  }
  fcf::NTest::log() << "Test index: " << fcf::NTest::state().paramIndex() << std::endl;
  FCF_TEST(((int)fcf::NTest::state().paramIndex()+1 == *fcf::NTest::state().param().cast<int>()),
           fcf::NTest::state().paramIndex(),
           (*fcf::NTest::state().param().cast<int>())
          );
}

/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "param", "Setting 2 values within a test"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "param", "Setting 2 values within a test") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"param"}, {"value 2 in test"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"param\" -> \"value 2 in test\" ...\n" +
                            " == Parameter set: 1\n" +
                            "  > Test index: 0\n" +
                            " == Parameter set: 2\n" +
                            "  > Test index: 1\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "\n" +
                            "[SUCCESS] All tests were completed.\n" +
                            "Tests: 1 passed, 0 failed, 0 skipped, 1 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(!error);
  }
}

FCF_TEST_DEFINE("subrun", "param", "value 2 in test") {
  if (!fcf::NTest::state().paramIndex()) {
    fcf::NTest::storage().appendParamValue(fcf::NTest::state().test(), (int)1);
    fcf::NTest::storage().appendParamValue(fcf::NTest::state().test(), (int)2);
  }
  fcf::NTest::log() << "Test index: " << fcf::NTest::state().paramIndex() << std::endl;
  FCF_TEST(((int)fcf::NTest::state().paramIndex()+1 == *fcf::NTest::state().param().cast<int>()),
           fcf::NTest::state().paramIndex(),
           (*fcf::NTest::state().param().cast<int>())
          );
}

/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "param", "Setting 3 values within a test"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "param", "Setting 3 values within a test") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"param"}, {"value 3 in test"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"param\" -> \"value 3 in test\" ...\n" +
                            " == Parameter set: 1\n" +
                            "  > Test index: 0\n" +
                            " == Parameter set: 2\n" +
                            "  > Test index: 1\n" +
                            " == Parameter set: 3\n" +
                            "  > Test index: 2\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "\n" +
                            "[SUCCESS] All tests were completed.\n" +
                            "Tests: 1 passed, 0 failed, 0 skipped, 1 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(!error);
  }
}

FCF_TEST_DEFINE("subrun", "param", "value 3 in test") {
  if (!fcf::NTest::state().paramIndex()) {
    fcf::NTest::storage().appendParamValue(fcf::NTest::state().test(), (int)1);
    fcf::NTest::storage().appendParamValue(fcf::NTest::state().test(), (int)2);
    fcf::NTest::storage().appendParamValue(fcf::NTest::state().test(), (int)3);
  }
  fcf::NTest::log() << "Test index: " << fcf::NTest::state().paramIndex() << std::endl;
  FCF_TEST(((int)fcf::NTest::state().paramIndex()+1 == *fcf::NTest::state().param().cast<int>()),
           fcf::NTest::state().paramIndex(),
           (*fcf::NTest::state().param().cast<int>())
          );
}

