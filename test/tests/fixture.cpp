#include <fcfTest/test.hpp>
#include "helpers.hpp"


/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "fixture", "fixture-global"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "fixture", "fixture-global") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"fixture-global"}, {"fixture-global"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "  > fixture up\n" +
                            "Performing the test: \"subrun\" -> \"fixture-global\" -> \"fixture-global\" ...\n" +
                            "  > fixture test\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > fixture down\n"
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

FCF_TEST_BEFORE_DEFINE("subrun", "*", "fixture-global", fcf::NTest::FL_GLOBAL) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("fixture-global");
  if (p) {
    ++*p.cast<int>();
  } else {
    fcf::NTest::state().data("fixture-global", fcf::NTest::SharedPtrAny::make<int>(0) );
  }
  fcf::NTest::log() << "fixture up" << std::endl;
}

FCF_TEST_AFTER_DEFINE("subrun", "*", "fixture-global", fcf::NTest::FL_GLOBAL) {
  fcf::NTest::log() << "fixture down" << std::endl;
}

FCF_TEST_DEFINE("subrun", "fixture-global", "fixture-global") {
  fcf::NTest::log() << "fixture test" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-global").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 0, *statePtr);
}



/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "fixture", "fixture-part"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "fixture", "fixture-part") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun-fixture-part-001", "subrun-fixture-part-002", "subrun-fixture-part-003"}, {"*"}, {"*"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun-fixture-part-001\" -> \"fixture\" -> \"fixture\" ...\n" +
                            "  > subrun-fixture-part-001\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > subrun-fixture-part-002 up\n" +
                            "Performing the test: \"subrun-fixture-part-002\" -> \"fixture\" -> \"fixture\" ...\n" +
                            "  > subrun-fixture-part-002\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > subrun-fixture-part-002 down\n" +
                            "Performing the test: \"subrun-fixture-part-003\" -> \"fixture\" -> \"fixture\" ...\n" +
                            "  > subrun-fixture-part-003\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "\n" +
                            "[SUCCESS] All tests were completed.\n" +
                            "Tests: 3 passed, 0 failed, 0 skipped, 3 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(!error);
  }
}

FCF_TEST_BEFORE_DEFINE("subrun-fixture-part-002", "*", "*", fcf::NTest::FL_PART) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("fixture-part");
  if (p) {
    ++*p.cast<int>();
  } else {
    fcf::NTest::state().data("fixture-part", fcf::NTest::SharedPtrAny::make<int>(1) );
  }
  fcf::NTest::log() << "subrun-fixture-part-002 up" << std::endl;
}

FCF_TEST_AFTER_DEFINE("subrun-fixture-part-002", "*", "*", fcf::NTest::FL_PART) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("fixture-part");
  *p.cast<int>() += 1000;
  fcf::NTest::log() << "subrun-fixture-part-002 down" << std::endl;
}


FCF_TEST_DEFINE("subrun-fixture-part-001", "fixture", "fixture") {
  fcf::NTest::log() << "subrun-fixture-part-001" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-part").cast<int>();
  FCF_TEST(!statePtr);
}

FCF_TEST_DEFINE("subrun-fixture-part-002", "fixture", "fixture") {
  fcf::NTest::log() << "subrun-fixture-part-002" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-part").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1, *statePtr);
}

FCF_TEST_DEFINE("subrun-fixture-part-003", "fixture", "fixture") {
  fcf::NTest::log() << "subrun-fixture-part-003" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-part").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1001, *statePtr);
}



/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "fixture", "fixture-group"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "fixture", "fixture-group") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"fixture-group-001", "fixture-group-002", "fixture-group-003"}, {"*"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"fixture-group-001\" -> \"fixture\" ...\n" +
                            "  > fixture-group-001\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > fixture-group-002 up\n" +
                            "Performing the test: \"subrun\" -> \"fixture-group-002\" -> \"fixture\" ...\n" +
                            "  > fixture-group-002\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > fixture-group-002 down\n" +
                            "Performing the test: \"subrun\" -> \"fixture-group-003\" -> \"fixture\" ...\n" +
                            "  > fixture-group-003\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "\n" +
                            "[SUCCESS] All tests were completed.\n" +
                            "Tests: 3 passed, 0 failed, 0 skipped, 3 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(!error);
  }
}

FCF_TEST_BEFORE_DEFINE("subrun", "fixture-group-002", "*", fcf::NTest::FL_GROUP) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("fixture-group");
  if (p) {
    ++*p.cast<int>();
  } else {
    fcf::NTest::state().data("fixture-group", fcf::NTest::SharedPtrAny::make<int>(1) );
  }
  fcf::NTest::log() << "fixture-group-002 up" << std::endl;
}

FCF_TEST_AFTER_DEFINE("subrun", "fixture-group-002", "*", fcf::NTest::FL_GROUP) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("fixture-group");
  *p.cast<int>() += 1000;
  fcf::NTest::log() << "fixture-group-002 down" << std::endl;
}

FCF_TEST_DEFINE("subrun", "fixture-group-001", "fixture") {
  fcf::NTest::log() << "fixture-group-001" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-group").cast<int>();
  FCF_TEST(!statePtr);
}

FCF_TEST_DEFINE("subrun", "fixture-group-002", "fixture") {
  fcf::NTest::log() << "fixture-group-002" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-group").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1, *statePtr);
}

FCF_TEST_DEFINE("subrun", "fixture-group-003", "fixture") {
  fcf::NTest::log() << "fixture-group-003" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-group").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1001, *statePtr);
}



/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "fixture", "fixture-test"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "fixture", "fixture-test") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"fixture-test"}, {"*"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"fixture-test\" -> \"test-001\" ...\n" +
                            "  > test-001\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > fixture-test-002 up\n" +
                            "Performing the test: \"subrun\" -> \"fixture-test\" -> \"test-002\" ...\n" +
                            "  > test-002\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > fixture-test-002 down\n" +
                            "Performing the test: \"subrun\" -> \"fixture-test\" -> \"test-003\" ...\n" +
                            "  > test-003\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "\n" +
                            "[SUCCESS] All tests were completed.\n" +
                            "Tests: 3 passed, 0 failed, 0 skipped, 3 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(!error);
  }
}

FCF_TEST_BEFORE_DEFINE("subrun", "fixture-test", "test-002", fcf::NTest::FL_TEST) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("fixture-test");
  if (p) {
    ++*p.cast<int>();
  } else {
    fcf::NTest::state().data("fixture-test", fcf::NTest::SharedPtrAny::make<int>(1) );
  }
  fcf::NTest::log() << "fixture-test-002 up" << std::endl;
}

FCF_TEST_AFTER_DEFINE("subrun", "fixture-test", "test-002", fcf::NTest::FL_TEST) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("fixture-test");
  *p.cast<int>() += 1000;
  fcf::NTest::log() << "fixture-test-002 down" << std::endl;
}


FCF_TEST_DEFINE("subrun", "fixture-test", "test-001") {
  fcf::NTest::log() << "test-001" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-test").cast<int>();
  FCF_TEST(!statePtr);
}

FCF_TEST_DEFINE("subrun", "fixture-test", "test-002") {
  fcf::NTest::log() << "test-002" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-test").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1);
}

FCF_TEST_DEFINE("subrun", "fixture-test", "test-003") {
  fcf::NTest::log() << "test-003" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-test").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1001, *statePtr);
}



/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "fixture", "fixture-group-more"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "fixture", "fixture-group-more") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"fixture-group-more-001", "fixture-group-more-002"}, {"*"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"fixture-group-more-001\" -> \"fixture-test-000\" ...\n" +
                            "  > fixture-test-000\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > fixture-group-more-001 up\n" +
                            "Performing the test: \"subrun\" -> \"fixture-group-more-001\" -> \"fixture-test-001\" ...\n" +
                            "  > fixture-test-001\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "Performing the test: \"subrun\" -> \"fixture-group-more-001\" -> \"fixture-test-002\" ...\n" +
                            "  > fixture-test-002\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "Performing the test: \"subrun\" -> \"fixture-group-more-001\" -> \"fixture-test-003\" ...\n" +
                            "  > fixture-test-003\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > fixture-group-more-002 down\n" +
                            "Performing the test: \"subrun\" -> \"fixture-group-more-001\" -> \"fixture-test-004\" ...\n" +
                            "  > fixture-test-004\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "Performing the test: \"subrun\" -> \"fixture-group-more-002\" -> \"fixture-test\" ...\n" +
                            "  > fixture-test\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "\n" +
                            "[SUCCESS] All tests were completed.\n" +
                            "Tests: 6 passed, 0 failed, 0 skipped, 6 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(!error);
  }
}

FCF_TEST_BEFORE_DEFINE("subrun", "fixture-group-more-001", "fixture-test-003|fixture-test-001", fcf::NTest::FL_GROUP) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("fixture-group-more");
  if (p) {
    ++*p.cast<int>();
  } else {
    fcf::NTest::state().data("fixture-group-more", fcf::NTest::SharedPtrAny::make<int>(1) );
  }
  fcf::NTest::log() << "fixture-group-more-001 up" << std::endl;
}

FCF_TEST_AFTER_DEFINE("subrun", "fixture-group-more-001", "fixture-test-003|fixture-test-001", fcf::NTest::FL_GROUP) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("fixture-group-more");
  *p.cast<int>() += 1000;
  fcf::NTest::log() << "fixture-group-more-002 down" << std::endl;
}


FCF_TEST_DEFINE("subrun", "fixture-group-more-001", "fixture-test-000") {
  fcf::NTest::log() << "fixture-test-000" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-group-more").cast<int>();
  FCF_TEST(!statePtr);
}

FCF_TEST_DEFINE("subrun", "fixture-group-more-001", "fixture-test-001") {
  fcf::NTest::log() << "fixture-test-001" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-group-more").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1);
}

FCF_TEST_DEFINE("subrun", "fixture-group-more-001", "fixture-test-002") {
  fcf::NTest::log() << "fixture-test-002" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-group-more").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1);
}

FCF_TEST_DEFINE("subrun", "fixture-group-more-001", "fixture-test-003") {
  fcf::NTest::log() << "fixture-test-003" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-group-more").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1);
}

FCF_TEST_DEFINE("subrun", "fixture-group-more-001", "fixture-test-004") {
  fcf::NTest::log() << "fixture-test-004" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-group-more").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1001);
}

FCF_TEST_DEFINE("subrun", "fixture-group-more-002", "fixture-test") {
  fcf::NTest::log() << "fixture-test" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-group-more").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1001, *statePtr);
}




/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "fixture", "fixture-test-multi"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "fixture", "fixture-test-multi") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"fixture-test-multi-001", "fixture-test-multi-002", "fixture-test-multi-003"}, {"*"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"fixture-test-multi-001\" -> \"test-001\" ...\n" +
                            "  > fixture-test-multi-001 test-001\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > fixture-test-multi-002 up\n" +
                            "Performing the test: \"subrun\" -> \"fixture-test-multi-002\" -> \"test-001\" ...\n" +
                            "  > fixture-test-multi-002 test-001\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > fixture-test-multi-002 down\n" +
                            "  > fixture-test-multi-002 up\n" +
                            "Performing the test: \"subrun\" -> \"fixture-test-multi-002\" -> \"test-002\" ...\n" +
                            "  > fixture-test-multi-002 test-002\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > fixture-test-multi-002 down\n" +
                            "  > fixture-test-multi-002 up\n" +
                            "Performing the test: \"subrun\" -> \"fixture-test-multi-002\" -> \"test-003\" ...\n" +
                            "  > fixture-test-multi-002 test-003\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > fixture-test-multi-002 down\n" +
                            "Performing the test: \"subrun\" -> \"fixture-test-multi-003\" -> \"test-001\" ...\n" +
                            "  > fixture-test-multi-003 test-001\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "Performing the test: \"subrun\" -> \"fixture-test-multi-003\" -> \"test-002\" ...\n" +
                            "  > fixture-test-multi-003 test-002\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "\n" +
                            "[SUCCESS] All tests were completed.\n" +
                            "Tests: 6 passed, 0 failed, 0 skipped, 6 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(!error);
  }
}

FCF_TEST_BEFORE_DEFINE("subrun", "fixture-test-multi-002", "*", fcf::NTest::FL_TEST) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("fixture-test-multi");
  if (p) {
    ++*p.cast<int>();
  } else {
    fcf::NTest::state().data("fixture-test-multi", fcf::NTest::SharedPtrAny::make<int>(0) );
  }
  fcf::NTest::log() << "fixture-test-multi-002 up" << std::endl;
}

FCF_TEST_AFTER_DEFINE("subrun", "fixture-test-multi-002", "*", fcf::NTest::FL_TEST) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("fixture-test-multi");
  *p.cast<int>() += 1000;
  fcf::NTest::log() << "fixture-test-multi-002 down" << std::endl;
}

FCF_TEST_DEFINE("subrun", "fixture-test-multi-001", "test-001") {
  fcf::NTest::log() << "fixture-test-multi-001 test-001" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-test-multi").cast<int>();
  FCF_TEST(!statePtr);
}

FCF_TEST_DEFINE("subrun", "fixture-test-multi-002", "test-001") {
  fcf::NTest::log() << "fixture-test-multi-002 test-001" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-test-multi").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 0);
}

FCF_TEST_DEFINE("subrun", "fixture-test-multi-002", "test-002") {
  fcf::NTest::log() << "fixture-test-multi-002 test-002" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-test-multi").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1001, *statePtr);
}

FCF_TEST_DEFINE("subrun", "fixture-test-multi-002", "test-003") {
  fcf::NTest::log() << "fixture-test-multi-002 test-003" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-test-multi").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 2002, *statePtr);
}

FCF_TEST_DEFINE("subrun", "fixture-test-multi-003", "test-001") {
  fcf::NTest::log() << "fixture-test-multi-003 test-001" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-test-multi").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 3002, *statePtr);
}

FCF_TEST_DEFINE("subrun", "fixture-test-multi-003", "test-002") {
  fcf::NTest::log() << "fixture-test-multi-003 test-002" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-test-multi").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 3002, *statePtr);
}









/// -----------------------------------------------------------------------------------------------
///
/// Test: "fcfTest", "fixture", "fixture-group-multi"
///
/// -----------------------------------------------------------------------------------------------
FCF_TEST_DEFINE("fcfTest", "fixture", "fixture-group-multi") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun-fixture-group-multi-001", "subrun-fixture-group-multi-002", "subrun-fixture-group-multi-003"}, {"*"}, {"*"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun-fixture-group-multi-001\" -> \"group-001\" -> \"test-001\" ...\n" +
                            "  > subrun-fixture-group-multi-001 group-001 test-001\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > subrun-fixture-group-multi-002 up\n" +
                            "Performing the test: \"subrun-fixture-group-multi-002\" -> \"group-001\" -> \"test-001\" ...\n" +
                            "  > subrun-fixture-group-multi-002 group-001 test-001\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > subrun-fixture-group-multi-002 down\n" +
                            "  > subrun-fixture-group-multi-002 up\n" +
                            "Performing the test: \"subrun-fixture-group-multi-002\" -> \"group-002\" -> \"test-001\" ...\n" +
                            "  > subrun-fixture-group-multi-002 group-002 test-001\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > subrun-fixture-group-multi-002 down\n" +
                            "  > subrun-fixture-group-multi-002 up\n" +
                            "Performing the test: \"subrun-fixture-group-multi-002\" -> \"group-003\" -> \"test-001\" ...\n" +
                            "  > subrun-fixture-group-multi-002 group-003 test-001\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "  > subrun-fixture-group-multi-002 down\n" +
                            "Performing the test: \"subrun-fixture-group-multi-003\" -> \"group-001\" -> \"test-001\" ...\n" +
                            "  > subrun-fixture-group-multi-003 group-001 test-001\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "\n" +
                            "[SUCCESS] All tests were completed.\n" +
                            "Tests: 5 passed, 0 failed, 0 skipped, 5 total\n" +
                            "Duration: XXX sec\n"
                            ;

    expected = uniout(expected, true);

    FCF_TEST(content == expected, content, expected);
    FCF_TEST(!error);
  }
}


FCF_TEST_BEFORE_DEFINE("subrun-fixture-group-multi-002", "*", "*", fcf::NTest::FL_GROUP) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("fixture-group-multi");
  if (p) {
    ++*p.cast<int>();
  } else {
    fcf::NTest::state().data("fixture-group-multi", fcf::NTest::SharedPtrAny::make<int>(0) );
  }
  fcf::NTest::log() << "subrun-fixture-group-multi-002 up" << std::endl;
}

FCF_TEST_AFTER_DEFINE("subrun-fixture-group-multi-002", "*", "*", fcf::NTest::FL_GROUP) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("fixture-group-multi");
  *p.cast<int>() += 1000;
  fcf::NTest::log() << "subrun-fixture-group-multi-002 down" << std::endl;
}

FCF_TEST_DEFINE("subrun-fixture-group-multi-001", "group-001", "test-001") {
  fcf::NTest::log() << "subrun-fixture-group-multi-001 group-001 test-001" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-group-multi").cast<int>();
  FCF_TEST(!statePtr);
}

FCF_TEST_DEFINE("subrun-fixture-group-multi-002", "group-001", "test-001") {
  fcf::NTest::log() << "subrun-fixture-group-multi-002 group-001 test-001" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-group-multi").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 0);
}

FCF_TEST_DEFINE("subrun-fixture-group-multi-002", "group-002", "test-001") {
  fcf::NTest::log() << "subrun-fixture-group-multi-002 group-002 test-001" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-group-multi").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1001, *statePtr);
}

FCF_TEST_DEFINE("subrun-fixture-group-multi-002", "group-003", "test-001") {
  fcf::NTest::log() << "subrun-fixture-group-multi-002 group-003 test-001" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-group-multi").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 2002, *statePtr);
}

FCF_TEST_DEFINE("subrun-fixture-group-multi-003", "group-001", "test-001") {
  fcf::NTest::log() << "subrun-fixture-group-multi-003 group-001 test-001" << std::endl;
  int* statePtr = fcf::NTest::state().data("fixture-group-multi").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 3002, *statePtr);
}

