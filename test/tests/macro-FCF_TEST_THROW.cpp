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
}

FCF_TEST_DECLARE("fcfTest", "macro", "FCF_TEST_THROW") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"FCF_TEST_THROW"}, {"no except"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"FCF_TEST_THROW\" -> \"no except\" ...\n" +
                            "  > 1\n" +
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
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"FCF_TEST_THROW"}, {"no except (parenthesis)"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"FCF_TEST_THROW\" -> \"no except (parenthesis)\" ...\n" +
                            "  > 1\n" +
                            "    Test error: 'callNoThrowWithParams<int, int>(std::map<int,int>().size(), 1)' did not throw  [FILE: XXX]\n" +
                            "      Values:\n" +
                            "        std::map<int,int>().size(): 0\n" +
                            "        1: 1\n" +
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
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"FCF_TEST_THROW"}, {"except"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);


    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"FCF_TEST_THROW\" -> \"except\" ...\n" +
                            "  > 1\n" +
                            "  > 2\n" +
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
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun"}, {"FCF_TEST_THROW"}, {"no except a2"}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);

    std::string content = uniout(ss.str(), true);
    std::string expected =  std::string() +
                            "Performing the test: \"subrun\" -> \"FCF_TEST_THROW\" -> \"no except a2\" ...\n" +
                            "  > 1\n" +
                            "    Test error: 'callNoThrowWithParams(a1)' did not throw  [FILE: XXX]\n" +
                            "      Values:\n"+
                            "        a1: 1\n"+
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
}

FCF_TEST_DECLARE("subrun", "FCF_TEST_THROW", "no except") {
  fcf::NTest::log() << "1" << std::endl;
  FCF_TEST_THROW(callNoThrow());
  fcf::NTest::log() << "2" << std::endl;
}

FCF_TEST_DECLARE("subrun", "FCF_TEST_THROW", "no except (parenthesis)") {
  fcf::NTest::log() << "1" << std::endl;
  FCF_TEST_THROW((callNoThrowWithParams<int, int>(std::map<int,int>().size(), 1)), (std::map<int,int>().size()), 1);
  fcf::NTest::log() << "2" << std::endl;
}

FCF_TEST_DECLARE("subrun", "FCF_TEST_THROW", "except") {
  fcf::NTest::log() << "1" << std::endl;
  FCF_TEST_THROW(callThrow());
  fcf::NTest::log() << "2" << std::endl;
}

FCF_TEST_DECLARE("subrun", "FCF_TEST_THROW", "no except a2") {
  int a1 = 1;
  int a2 = 2;
  fcf::NTest::log() << "1" << std::endl;
  FCF_TEST_THROW(callNoThrowWithParams(a1), a1);
  fcf::NTest::log() << "2" << std::endl;
}


