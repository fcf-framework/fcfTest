#include <fcfTest/test.hpp>
#include "helpers.hpp"

/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "fixture-error", "fixture-global_error"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "fixture-error", "fixture-global_error") {
  {
    fcf::NTest::Options options;
    //options.noBreak = true;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun-fixture"}, {"fixture-global_error-all"}, {"*"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "  > fixture-global_error-all begin\n" +
                            "Performing the test: \"subrun-fixture\" -> \"fixture-global_error-all\" -> \"test 001\" ...\n" +
                            "    Fixture error [FILE: XXX]:\n" +
                            "      error\n" +
                            "    [FAILED] Test failed (XXX sec)\n" +
                            "  > fixture-global_error-all end\n" +
                            "\n" +
                            "[FAILED] Testing completed with failures.\n" +
                            "Tests: 0 passed, 1 failed, 1 skipped, 2 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(error);
  }

}

FCF_TEST_BEFORE_DEFINE("subrun-fixture", "fixture-global_error-all", "*", fcf::NTest::FL_GLOBAL) {
  fcf::NTest::log() << "fixture-global_error-all begin" << std::endl;
  throw std::runtime_error("error");
}

FCF_TEST_AFTER_DEFINE("subrun-fixture", "fixture-global_error-all", "*", fcf::NTest::FL_GLOBAL) {
  fcf::NTest::log() << "fixture-global_error-all end" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-global_error-all", "test 001") {
  fcf::NTest::log() << "test 001" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-global_error-all", "test 002") {
  fcf::NTest::log() << "test 002" << std::endl;
}

/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "fixture-error-nobreak", "fixture-global_error-nobreak"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "fixture-error", "fixture-global_error-nobreak") {
  {
    fcf::NTest::Options options;
    options.noBreak = true;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun-fixture"}, {"fixture-global_error-all-nobreak", "fixture-global_error-all-nobreak-002"}, {"*"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "  > fixture-global_error-all begin\n" +
                            "Performing the test: \"subrun-fixture\" -> \"fixture-global_error-all-nobreak\" -> \"test 001\" ...\n" +
                            "    Fixture error [FILE: XXX]:\n" +
                            "      error\n" +
                            "    [FAILED] Test failed (XXX sec)\n" +
                            "Performing the test: \"subrun-fixture\" -> \"fixture-global_error-all-nobreak\" -> \"test 002\" ...\n" +
                            "    Fixture error [FILE: XXX]\n" +
                            "    [FAILED] Test failed (XXX sec)\n" +
                            "Performing the test: \"subrun-fixture\" -> \"fixture-global_error-all-nobreak-002\" -> \"test 003\" ...\n" +
                            "  > test 003\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > fixture-global_error-all end\n" +
                            "\n" +
                            "[FAILED] Testing completed with failures.\n" +
                            "Tests: 1 passed, 2 failed, 0 skipped, 3 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(error);
  }

}

FCF_TEST_BEFORE_DEFINE("subrun-fixture", "fixture-global_error-all-nobreak", "*", fcf::NTest::FL_GLOBAL) {
  fcf::NTest::log() << "fixture-global_error-all begin" << std::endl;
  throw std::runtime_error("error");
}

FCF_TEST_AFTER_DEFINE("subrun-fixture", "fixture-global_error-all-nobreak", "*", fcf::NTest::FL_GLOBAL) {
  fcf::NTest::log() << "fixture-global_error-all end" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-global_error-all-nobreak", "test 001") {
  fcf::NTest::log() << "test 001" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-global_error-all-nobreak", "test 002") {
  fcf::NTest::log() << "test 002" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-global_error-all-nobreak-002", "test 003") {
  fcf::NTest::log() << "test 003" << std::endl;
}


/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "fixture-error", "fixture-part_error"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "fixture-error", "fixture-part_error") {
  {
    fcf::NTest::Options options;
    options.noBreak = false;
    options.selectors.push_back( fcf::NTest::Options::Selector{
                                    {
                                      "subrun-fixture-part_error-all-001",
                                      "subrun-fixture-part_error-all-002",
                                      "subrun-fixture-part_error-all-003"
                                    },
                                    {"*"},
                                    {"*"}
                                  }
                              );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "  > fixture-part_error-all begin\n" +
                            "Performing the test: \"subrun-fixture-part_error-all-001\" -> \"group 001\" -> \"test 001\" ...\n" +
                            "    Fixture error [FILE: XXX]:\n" +
                            "      error\n" +
                            "    [FAILED] Test failed (XXX sec)\n" +
                            "  > fixture-part_error-all end\n" +
                            "\n" +
                            "[FAILED] Testing completed with failures.\n" +
                            "Tests: 0 passed, 1 failed, 3 skipped, 4 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(error);
  }
}

FCF_TEST_BEFORE_DEFINE("subrun-fixture-part_error-all-001|subrun-fixture-part_error-all-002", "*", "*", fcf::NTest::FL_GROUP) {
  fcf::NTest::log() << "fixture-part_error-all begin" << std::endl;
  throw std::runtime_error("error");
}

FCF_TEST_AFTER_DEFINE("subrun-fixture-part_error-all-001|subrun-fixture-part_error-all-002", "*", "*", fcf::NTest::FL_GROUP) {
  fcf::NTest::log() << "fixture-part_error-all end" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture-part_error-all-001", "group 001", "test 001") {
  fcf::NTest::log() << "test 001" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture-part_error-all-001", "group 002", "test 002") {
  fcf::NTest::log() << "test 002" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture-part_error-all-002", "group 003", "test 003") {
  fcf::NTest::log() << "test 003" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture-part_error-all-003", "group 004", "test 004") {
  fcf::NTest::log() << "test 004" << std::endl;
}


/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "fixture-error", "fixture-part_error-nobreak"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "fixture-error", "fixture-part_error-nobreak") {
  {
    fcf::NTest::Options options;
    options.noBreak = true;
    options.selectors.push_back( fcf::NTest::Options::Selector{{
                                                                  "subrun-fixture-part_error-nobreak-all-001",
                                                                  "subrun-fixture-part_error-nobreak-all-002",
                                                                  "subrun-fixture-part_error-nobreak-all-003",
                                                                  "subrun-fixture-part_error-nobreak-all-004"
                                                               },
                                                               {"*"},
                                                               {"*"}
                                                              });
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun-fixture-part_error-nobreak-all-001\" -> \"group 001\" -> \"test 001\" ...\n" +
                            "  > test 001\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "Performing the test: \"subrun-fixture-part_error-nobreak-all-001\" -> \"group 002\" -> \"test 002\" ...\n" +
                            "  > test 002\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > fixture-part_error-nobreak-all begin\n" +
                            "Performing the test: \"subrun-fixture-part_error-nobreak-all-002\" -> \"group 003\" -> \"test 003\" ...\n" +
                            "    Fixture error [FILE: XXX]:\n" +
                            "      error\n" +
                            "    [FAILED] Test failed (XXX sec)\n" +
                            "Performing the test: \"subrun-fixture-part_error-nobreak-all-002\" -> \"group 004\" -> \"test 004\" ...\n" +
                            "    Fixture error [FILE: XXX]\n" +
                            "    [FAILED] Test failed (XXX sec)\n" +
                            "  > fixture-part_error-nobreak-all end\n" +
                            "  > fixture-part_error-nobreak-all begin\n" +
                            "Performing the test: \"subrun-fixture-part_error-nobreak-all-003\" -> \"group 005\" -> \"test 005\" ...\n" +
                            "    Fixture error [FILE: XXX]:\n" +
                            "      error\n" +
                            "    [FAILED] Test failed (XXX sec)\n" +
                            "Performing the test: \"subrun-fixture-part_error-nobreak-all-003\" -> \"group 006\" -> \"test 006\" ...\n" +
                            "    Fixture error [FILE: XXX]\n" +
                            "    [FAILED] Test failed (XXX sec)\n" +
                            "  > fixture-part_error-nobreak-all end\n" +
                            "Performing the test: \"subrun-fixture-part_error-nobreak-all-004\" -> \"group 007\" -> \"test 007\" ...\n" +
                            "  > test 007\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "Performing the test: \"subrun-fixture-part_error-nobreak-all-004\" -> \"group 008\" -> \"test 008\" ...\n" +
                            "  > test 008\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "\n" +
                            "[FAILED] Testing completed with failures.\n" +
                            "Tests: 4 passed, 4 failed, 0 skipped, 8 total\n" +
                            "Duration: XXX sec\n"
                          ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(error);
  }

}

FCF_TEST_BEFORE_DEFINE("subrun-fixture-part_error-nobreak-all-002|subrun-fixture-part_error-nobreak-all-003", "*", "*", fcf::NTest::FL_PART) {
  fcf::NTest::log() << "fixture-part_error-nobreak-all begin" << std::endl;
  throw std::runtime_error("error");
}

FCF_TEST_AFTER_DEFINE("subrun-fixture-part_error-nobreak-all-002|subrun-fixture-part_error-nobreak-all-003", "*", "*", fcf::NTest::FL_PART) {
  fcf::NTest::log() << "fixture-part_error-nobreak-all end" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture-part_error-nobreak-all-001", "group 001", "test 001") {
  fcf::NTest::log() << "test 001" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture-part_error-nobreak-all-001", "group 002", "test 002") {
  fcf::NTest::log() << "test 002" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture-part_error-nobreak-all-002", "group 003", "test 003") {
  fcf::NTest::log() << "test 003" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture-part_error-nobreak-all-002", "group 004", "test 004") {
  fcf::NTest::log() << "test 004" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture-part_error-nobreak-all-003", "group 005", "test 005") {
  fcf::NTest::log() << "test 005" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture-part_error-nobreak-all-003", "group 006", "test 006") {
  fcf::NTest::log() << "test 006" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture-part_error-nobreak-all-004", "group 007", "test 007") {
  fcf::NTest::log() << "test 007" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture-part_error-nobreak-all-004", "group 008", "test 008") {
  fcf::NTest::log() << "test 008" << std::endl;
}


/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "fixture-error", "fixture-group_error"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "fixture-error", "fixture-group_error") {
  {
    fcf::NTest::Options options;
    options.noBreak = false;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun-fixture"}, {"fixture-group_error-all-001", "fixture-group_error-all-002", "fixture-group_error-all-003"}, {"*"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "  > fixture-group_error-all begin\n" +
                            "Performing the test: \"subrun-fixture\" -> \"fixture-group_error-all-001\" -> \"test 001\" ...\n" +
                            "    Fixture error [FILE: XXX]:\n" +
                            "      error\n" +
                            "    [FAILED] Test failed (XXX sec)\n" +
                            "  > fixture-group_error-all end\n" +
                            "\n" +
                            "[FAILED] Testing completed with failures.\n" +
                            "Tests: 0 passed, 1 failed, 3 skipped, 4 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(error);
  }
}

FCF_TEST_BEFORE_DEFINE("subrun-fixture", "fixture-group_error-all-001|fixture-group_error-all-002", "*", fcf::NTest::FL_GROUP) {
  fcf::NTest::log() << "fixture-group_error-all begin" << std::endl;
  throw std::runtime_error("error");
}

FCF_TEST_AFTER_DEFINE("subrun-fixture", "fixture-group_error-all-001|fixture-group_error-all-002", "*", fcf::NTest::FL_GROUP) {
  fcf::NTest::log() << "fixture-group_error-all end" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-group_error-all-001", "test 001") {
  fcf::NTest::log() << "test 001" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-group_error-all-001", "test 002") {
  fcf::NTest::log() << "test 002" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-group_error-all-002", "test 003") {
  fcf::NTest::log() << "test 003" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-group_error-all-003", "test 004") {
  fcf::NTest::log() << "test 004" << std::endl;
}

/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "fixture-error", "fixture-group_error-nobreak"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "fixture-error", "fixture-group_error-nobreak") {
  {
    fcf::NTest::Options options;
    options.noBreak = true;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun-fixture"},
                                                               {
                                                                  "fixture-group_error-nobreak-all-000",
                                                                  "fixture-group_error-nobreak-all-001",
                                                                  "fixture-group_error-nobreak-all-002",
                                                                  "fixture-group_error-nobreak-all-003"
                                                               },
                                                               {"*"}
                                                              });
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                          "Performing the test: \"subrun-fixture\" -> \"fixture-group_error-nobreak-all-000\" -> \"test 000-002\" ...\n" +
                          "  > test 000-002\n" +
                          "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                          "Performing the test: \"subrun-fixture\" -> \"fixture-group_error-nobreak-all-000\" -> \"test 001-001\" ...\n" +
                          "  > test 000-001\n" +
                          "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                          "  > fixture-group_error-nobreak-all begin\n" +
                          "Performing the test: \"subrun-fixture\" -> \"fixture-group_error-nobreak-all-001\" -> \"test 001\" ...\n" +
                          "    Fixture error [FILE: XXX]:\n" +
                          "      error\n" +
                          "    [FAILED] Test failed (XXX sec)\n" +
                          "Performing the test: \"subrun-fixture\" -> \"fixture-group_error-nobreak-all-001\" -> \"test 002\" ...\n" +
                          "    Fixture error [FILE: XXX]\n" +
                          "    [FAILED] Test failed (XXX sec)\n" +
                          "  > fixture-group_error-nobreak-all end\n" +
                          "  > fixture-group_error-nobreak-all begin\n" +
                          "Performing the test: \"subrun-fixture\" -> \"fixture-group_error-nobreak-all-002\" -> \"test 003\" ...\n" +
                          "    Fixture error [FILE: XXX]:\n" +
                          "      error\n" +
                          "    [FAILED] Test failed (XXX sec)\n" +
                          "Performing the test: \"subrun-fixture\" -> \"fixture-group_error-nobreak-all-002\" -> \"test 004\" ...\n" +
                          "    Fixture error [FILE: XXX]\n" +
                          "    [FAILED] Test failed (XXX sec)\n" +
                          "  > fixture-group_error-nobreak-all end\n" +
                          "Performing the test: \"subrun-fixture\" -> \"fixture-group_error-nobreak-all-003\" -> \"test 005\" ...\n" +
                          "  > test 005\n" +
                          "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                          "Performing the test: \"subrun-fixture\" -> \"fixture-group_error-nobreak-all-003\" -> \"test 006\" ...\n" +
                          "  > test 006\n" +
                          "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                          "\n" +
                          "[FAILED] Testing completed with failures.\n" +
                          "Tests: 4 passed, 4 failed, 0 skipped, 8 total\n" +
                          "Duration: XXX sec\n"
                          ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(error);
  }

}

FCF_TEST_BEFORE_DEFINE("subrun-fixture", "fixture-group_error-nobreak-all-001|fixture-group_error-nobreak-all-002", "*", fcf::NTest::FL_GROUP) {
  fcf::NTest::log() << "fixture-group_error-nobreak-all begin" << std::endl;
  throw std::runtime_error("error");
}

FCF_TEST_AFTER_DEFINE("subrun-fixture", "fixture-group_error-nobreak-all-001|fixture-group_error-nobreak-all-002", "*", fcf::NTest::FL_GROUP) {
  fcf::NTest::log() << "fixture-group_error-nobreak-all end" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-group_error-nobreak-all-000", "test 001-001") {
  fcf::NTest::log() << "test 000-001" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-group_error-nobreak-all-000", "test 000-002") {
  fcf::NTest::log() << "test 000-002" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-group_error-nobreak-all-001", "test 001") {
  fcf::NTest::log() << "test 001" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-group_error-nobreak-all-001", "test 002") {
  fcf::NTest::log() << "test 002" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-group_error-nobreak-all-002", "test 003") {
  fcf::NTest::log() << "test 003" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-group_error-nobreak-all-002", "test 004") {
  fcf::NTest::log() << "test 004" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-group_error-nobreak-all-003", "test 005") {
  fcf::NTest::log() << "test 005" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-group_error-nobreak-all-003", "test 006") {
  fcf::NTest::log() << "test 006" << std::endl;
}



/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "fixture-error", "fixture-test_error"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "fixture-error", "fixture-test_error") {
  {
    fcf::NTest::Options options;
    options.noBreak = false;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun-fixture"}, {"fixture-test_error-all"}, {"*"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun-fixture\" -> \"fixture-test_error-all\" -> \"fixture-test_error-all-001\" ...\n" +
                            "  > test 001\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > fixture-test_error-all begin\n" +
                            "Performing the test: \"subrun-fixture\" -> \"fixture-test_error-all\" -> \"fixture-test_error-all-002\" ...\n" +
                            "    Fixture error [FILE: XXX]:\n" +
                            "      error\n" +
                            "    [FAILED] Test failed (XXX sec)\n" +
                            "  > fixture-test_error-all end\n" +
                            "\n" +
                            "[FAILED] Testing completed with failures.\n" +
                            "Tests: 1 passed, 1 failed, 2 skipped, 4 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(error);
  }
}

FCF_TEST_BEFORE_DEFINE("subrun-fixture", "*", "fixture-test_error-all-002", fcf::NTest::FL_TEST) {
  fcf::NTest::log() << "fixture-test_error-all begin" << std::endl;
  throw std::runtime_error("error");
}

FCF_TEST_AFTER_DEFINE("subrun-fixture", "*", "fixture-test_error-all-002", fcf::NTest::FL_TEST) {
  fcf::NTest::log() << "fixture-test_error-all end" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-test_error-all", "fixture-test_error-all-001") {
  fcf::NTest::log() << "test 001" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-test_error-all", "fixture-test_error-all-002") {
  fcf::NTest::log() << "test 002" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-test_error-all", "fixture-test_error-all-003") {
  fcf::NTest::log() << "test 003" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-test_error-all", "fixture-test_error-all-004") {
  fcf::NTest::log() << "test 004" << std::endl;
}


/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "fixture-error", "fixture-test_error-nobreak"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "fixture-error", "fixture-test_error-nobreak") {
  {
    fcf::NTest::Options options;
    options.noBreak = true;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun-fixture"},
                                                               {"*"},
                                                               {
                                                                  "fixture-test_error-nobreak-all-001",
                                                                  "fixture-test_error-nobreak-all-002",
                                                                  "fixture-test_error-nobreak-all-003",
                                                                  "fixture-test_error-nobreak-all-004"
                                                               },
                                                              });
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun-fixture\" -> \"fixture-test_error-nobreak-all\" -> \"fixture-test_error-nobreak-all-001\" ...\n" +
                            "  > test 001\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > fixture-test_error-nobreak-all begin\n" +
                            "Performing the test: \"subrun-fixture\" -> \"fixture-test_error-nobreak-all\" -> \"fixture-test_error-nobreak-all-002\" ...\n" +
                            "    Fixture error [FILE: XXX]:\n" +
                            "      error\n" +
                            "    [FAILED] Test failed (XXX sec)\n" +
                            "  > fixture-test_error-nobreak-all end\n" +
                            "  > fixture-test_error-nobreak-all begin\n" +
                            "Performing the test: \"subrun-fixture\" -> \"fixture-test_error-nobreak-all\" -> \"fixture-test_error-nobreak-all-003\" ...\n" +
                            "    Fixture error [FILE: XXX]:\n" +
                            "      error\n" +
                            "    [FAILED] Test failed (XXX sec)\n" +
                            "  > fixture-test_error-nobreak-all end\n" +
                            "Performing the test: \"subrun-fixture\" -> \"fixture-test_error-nobreak-all\" -> \"fixture-test_error-nobreak-all-004\" ...\n" +
                            "  > test 004\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "\n" +
                            "[FAILED] Testing completed with failures.\n" +
                            "Tests: 2 passed, 2 failed, 0 skipped, 4 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(error);
  }

}

FCF_TEST_BEFORE_DEFINE("subrun-fixture", "*", "fixture-test_error-nobreak-all-002|fixture-test_error-nobreak-all-003", fcf::NTest::FL_TEST) {
  fcf::NTest::log() << "fixture-test_error-nobreak-all begin" << std::endl;
  throw std::runtime_error("error");
}

FCF_TEST_AFTER_DEFINE("subrun-fixture", "*", "fixture-test_error-nobreak-all-002|fixture-test_error-nobreak-all-003", fcf::NTest::FL_TEST) {
  fcf::NTest::log() << "fixture-test_error-nobreak-all end" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-test_error-nobreak-all", "fixture-test_error-nobreak-all-001") {
  fcf::NTest::log() << "test 001" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-test_error-nobreak-all", "fixture-test_error-nobreak-all-002") {
  fcf::NTest::log() << "test 002" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-test_error-nobreak-all", "fixture-test_error-nobreak-all-003") {
  fcf::NTest::log() << "test 003" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture", "fixture-test_error-nobreak-all", "fixture-test_error-nobreak-all-004") {
  fcf::NTest::log() << "test 004" << std::endl;
}


