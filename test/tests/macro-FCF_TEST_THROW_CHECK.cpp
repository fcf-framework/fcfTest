#include <fcfTest/test.hpp>
#include "helpers.hpp"

namespace {
  void callNoThrow(){
  }
  template <typename ...TArgPack>
  void callNoThrowWithParams(TArgPack ...a_argPack){
  }
  void callThrow(){
    throw std::runtime_error("1");
  }
  void callThrowOutOfRange(){
    throw std::out_of_range("Info");
  }
}

FCF_TEST_DECLARE("fcfTest", "macro", "FCF_TEST_THROW_CHECK") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"throw-check"}, {"no except"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);

    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"throw-check\" -> \"no except\" ...\n" +
                            "  > 1\n" +
                            "  > 2\n" +
                            "  > err\n" +
                            "  > 3\n" +
                            "    Test error: 'callNoThrow()' did not throw  [FILE: XXX]\n" +
                            "    Test error: 'callNoThrow()' did not throw  [FILE: XXX]\n" +
                            "    [FAILED] Test failed (XXX sec)\n" +
                            "\n" +
                            "[FAILED] Testing completed with failures.\n" +
                            "Tests: 0 passed, 1 failed, 0 skipped, 1 total\n" +
                            "Duration: XXX sec\n"
                            ;
    expected = uniout(expected, true);
    FCF_TEST(content == expected, content, expected);
    FCF_TEST(error);
  }
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"throw-check"}, {"except"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);

    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"throw-check\" -> \"except\" ...\n" +
                            "  > 1\n" +
                            "  > 2\n" +
                            "  > 3\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                            "\n" +
                            "[SUCCESS] All tests were completed.\n"+
                            "Tests: 1 passed, 0 failed, 0 skipped, 1 total\n" +
                            "Duration: XXX sec\n"
                            ;
    expected = uniout(expected, true);
    FCF_TEST(content == expected, content, expected);
    FCF_TEST(!error);
  }
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"throw-check"}, {"no except a2"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);

    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"throw-check\" -> \"no except a2\" ...\n" +
                            "  > 1\n" +
                            "  > 2\n" +
                            "  > err\n" +
                            "  > 3\n" +
                            "    Test error: 'callNoThrowWithParams(a1)' did not throw  [FILE: XXX]\n" +
                            "      Values:\n"+
                            "        a1: 1\n"+
                            "    Test error: 'callNoThrowWithParams(a1, a2)' did not throw  [FILE: XXX]\n" +
                            "      Values:\n"+
                            "        a1: 1\n"+
                            "        a2: 2\n"+
                            "    [FAILED] Test failed (XXX sec)\n" +
                            "\n" +
                            "[FAILED] Testing completed with failures.\n" +
                            "Tests: 0 passed, 1 failed, 0 skipped, 1 total\n" +
                            "Duration: XXX sec\n"
                            ;
    expected = uniout(expected, true);
    FCF_TEST(content == expected, content, expected);
    FCF_TEST(error);
  }
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"FCF_TEST_THROW_CHECK"}, {"except std::out_of_range != std::runtime_error"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);

    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"FCF_TEST_THROW_CHECK\" -> \"except std::out_of_range != std::runtime_error\" ...\n" +
                            "  > end\n" +
                            "    Test error: 'callThrow()' threw an exception that does not match 'std::out_of_range'  [FILE: XXX]\n" +
                            "      Values:\n" +
                            "        v: 999\n" +
                            "    [FAILED] Test failed (XXX sec)\n" +
                            "\n" +
                            "[FAILED] Testing completed with failures.\n" +
                            "Tests: 0 passed, 1 failed, 0 skipped, 1 total\n" +
                            "Duration: XXX sec\n"
                            ;
    expected = uniout(expected, true);
    FCF_TEST(content == expected, content, expected);
    FCF_TEST(error);
  }
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"FCF_TEST_THROW_CHECK"}, {"except std::out_of_range == std::out_of_range"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);

    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"FCF_TEST_THROW_CHECK\" -> \"except std::out_of_range == std::out_of_range\" ...\n" +
                            "  > end\n" +
                            "    [SUCCESS] Test completed successfully (XXX sec)\n"+
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


FCF_TEST_DECLARE("subrun", "throw-check", "no except") {
  fcf::NTest::log() << "1" << std::endl;
  FCF_TEST_THROW_CHECK(callNoThrow(), ...);
  fcf::NTest::log() << "2" << std::endl;
  if(!FCF_TEST_THROW_CHECK(callNoThrow(), ...) ){
    fcf::NTest::log() << "err" << std::endl;
  }
  fcf::NTest::log() << "3" << std::endl;
}

FCF_TEST_DECLARE("subrun", "throw-check", "except") {
  fcf::NTest::log() << "1" << std::endl;
  FCF_TEST_THROW_CHECK(callThrow(), ...);
  fcf::NTest::log() << "2" << std::endl;
  if(!FCF_TEST_THROW_CHECK(callThrow(), ...)){
    fcf::NTest::log() << "err" << std::endl;
  }
  fcf::NTest::log() << "3" << std::endl;
}

FCF_TEST_DECLARE("subrun", "throw-check", "no except a2") {
  int a1 = 1;
  int a2 = 2;
  fcf::NTest::log() << "1" << std::endl;
  FCF_TEST_THROW_CHECK(callNoThrowWithParams(a1), ..., a1);
  fcf::NTest::log() << "2" << std::endl;
  if(!FCF_TEST_THROW_CHECK(callNoThrowWithParams(a1, a2), ..., a1, a2)){
    fcf::NTest::log() << "err" << std::endl;
  }
  fcf::NTest::log() << "3" << std::endl;
}

FCF_TEST_DECLARE("subrun", "FCF_TEST_THROW_CHECK", "except std::out_of_range != std::runtime_error") {
  int v = 999;
  FCF_TEST_THROW_CHECK(callThrow(), std::out_of_range, v);
  fcf::NTest::log() << "end" << std::endl;
}

FCF_TEST_DECLARE("subrun", "FCF_TEST_THROW_CHECK", "except std::out_of_range == std::out_of_range") {
  int v = 888;
  FCF_TEST_THROW_CHECK(callThrowOutOfRange(), std::out_of_range, v);
  fcf::NTest::log() << "end" << std::endl;
}

