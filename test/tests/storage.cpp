#include <fcfTest/test.hpp>

namespace {
  std::string testToStr(const fcf::NTest::Test& a_test){
    return std::string() + a_test.part + "->" + a_test.group + "->" + a_test.test;
  }

  std::string testsToStr(const std::set<fcf::NTest::Test>& a_tests) {
    std::string res;
    for(auto test : a_tests) {
      res += res.empty() ? "" : " / ";
      res += testToStr(test);
    }
    return res;
  }
}

FCF_TEST_DEFINE("fcfTest", "storage", "select") {
  fcf::NTest::Storage storage;

  fcf::NTest::Test testP1G1T1 = {"p1", 0, "g1", 0, "t1", 0, nullptr};
  storage.appendTest(testP1G1T1);

  fcf::NTest::Test testP1G1T2 = {"p1", 0, "g1", 0, "t2", 0, nullptr};
  storage.appendTest(testP1G1T2);

  fcf::NTest::Test testP2G1T1 = {"p2", 0, "g1", 0, "t1", 0, nullptr};
  storage.appendTest(testP2G1T1);

  fcf::NTest::Test testP2G1T2 = {"p2", 0, "g1", 0, "t2", 0, nullptr};
  storage.appendTest(testP2G1T2);

  fcf::NTest::Test testP2G2T1 = {"p2", 0, "g2", 0, "t1", 0, nullptr};
  storage.appendTest(testP2G2T1);

  fcf::NTest::Test testP2G2T2 = {"p2", 0, "g2", 0, "t2", 0, nullptr};
  storage.appendTest(testP2G2T2);

  {
    std::set<fcf::NTest::Test> tests;
    std::set<fcf::NTest::Test> expected = { testP1G1T1, testP1G1T2, testP2G1T1, testP2G1T2, testP2G2T1, testP2G2T2 };
    fcf::NTest::Options options;
    options.selectors.push_back(fcf::NTest::Options::Selector{ });
    tests = storage.selectTests(options);
    FCF_TEST(tests == expected, testsToStr(tests), testsToStr(expected));
  }
  {
    std::set<fcf::NTest::Test> tests;
    std::set<fcf::NTest::Test> expected = { testP1G1T1, testP1G1T2, testP2G1T1, testP2G1T2, testP2G2T1, testP2G2T2 };
    fcf::NTest::Options options;
    options.selectors.push_back(fcf::NTest::Options::Selector{ {}, {}, {} });
    tests = storage.selectTests(options);
    FCF_TEST(tests == expected, testsToStr(tests), testsToStr(expected));
  }
  {
    std::set<fcf::NTest::Test> tests;
    std::set<fcf::NTest::Test> expected = { testP1G1T1, testP1G1T2, testP2G1T1, testP2G1T2, testP2G2T1, testP2G2T2 };
    fcf::NTest::Options options;
    options.selectors.push_back(fcf::NTest::Options::Selector{ {""}, {}, {} });
    tests = storage.selectTests(options);
    FCF_TEST(tests == expected, testsToStr(tests), testsToStr(expected));
  }
  {
    std::set<fcf::NTest::Test> tests;
    std::set<fcf::NTest::Test> expected = { testP1G1T1 };
    fcf::NTest::Options options;
    options.selectors.push_back(fcf::NTest::Options::Selector{ {"p1"}, {"g1"}, {"t1"} });
    tests = storage.selectTests(options);
    FCF_TEST(tests == expected, testsToStr(tests), testsToStr(expected));
  }
  {
    std::set<fcf::NTest::Test> tests;
    std::set<fcf::NTest::Test> expected = { testP1G1T1, testP1G1T2 };
    fcf::NTest::Options options;
    options.selectors.push_back(fcf::NTest::Options::Selector{ {"p1"}, {"g1"}, {"t1", "t2"} });
    tests = storage.selectTests(options);
    FCF_TEST(tests == expected, testsToStr(tests), testsToStr(expected));
  }
  {
    std::set<fcf::NTest::Test> tests;
    std::set<fcf::NTest::Test> expected = { testP1G1T1, testP1G1T2 };
    fcf::NTest::Options options;
    options.selectors.push_back(fcf::NTest::Options::Selector{ {"p1"}, {}, {"*"} });
    tests = storage.selectTests(options);
    FCF_TEST(tests == expected, testsToStr(tests), testsToStr(expected));
  }
  {
    std::set<fcf::NTest::Test> tests;
    std::set<fcf::NTest::Test> expected = { testP1G1T1, testP1G1T2 };
    fcf::NTest::Options options;
    options.selectors.push_back(fcf::NTest::Options::Selector{ {"p1"}, {"*"}, {} });
    tests = storage.selectTests(options);
    FCF_TEST(tests == expected, testsToStr(tests), testsToStr(expected));
  }
  {
    std::set<fcf::NTest::Test> tests;
    std::set<fcf::NTest::Test> expected = { testP1G1T1, testP1G1T2 };
    fcf::NTest::Options options;
    options.selectors.push_back(fcf::NTest::Options::Selector{ {"p1"}, {"g1"}, {"t1"} });
    options.selectors.push_back(fcf::NTest::Options::Selector{ {"p1"}, {"g1"}, {"t2"} });
    tests = storage.selectTests(options);
    FCF_TEST(tests == expected, testsToStr(tests), testsToStr(expected));
  }
  {
    std::set<fcf::NTest::Test> tests;
    std::set<fcf::NTest::Test> expected = { testP1G1T1 };
    fcf::NTest::Options options;
    options.selectors.push_back(fcf::NTest::Options::Selector{ {"p1"}, {}, {"t1"} });
    tests = storage.selectTests(options);
    FCF_TEST(tests == expected, testsToStr(tests), testsToStr(expected));
  }
  {
    std::set<fcf::NTest::Test> tests;
    std::set<fcf::NTest::Test> expected = { testP1G1T1,testP1G1T2 };
    fcf::NTest::Options options;
    options.selectors.push_back(fcf::NTest::Options::Selector{ {"p1"}, {}, {} });
    tests = storage.selectTests(options);
    FCF_TEST(tests == expected, testsToStr(tests), testsToStr(expected));
  }
  {
    std::set<fcf::NTest::Test> tests;
    std::set<fcf::NTest::Test> expected = { testP1G1T1, testP2G1T1, testP2G2T1 };
    fcf::NTest::Options options;
    options.selectors.push_back(fcf::NTest::Options::Selector{ {}, {}, {"t1"} });
    tests = storage.selectTests(options);
    FCF_TEST(tests == expected, testsToStr(tests), testsToStr(expected));
  }
}



FCF_TEST_DEFINE("fcfTest", "storage", "select (ignore)") {
  fcf::NTest::Storage storage;

  fcf::NTest::Test testP1G1T1 = {"p1", 0, "g1", 0, "t1", 0, nullptr};
  storage.appendTest(testP1G1T1);

  fcf::NTest::Test testP1G1T2 = {"p1", 0, "g1", 0, "t2", 0, nullptr};
  storage.appendTest(testP1G1T2);

  fcf::NTest::Test testP2G1T1 = {"p2", 0, "g1", 0, "t1", 0, nullptr};
  storage.appendTest(testP2G1T1);

  fcf::NTest::Test testP2G1T2 = {"p2", 0, "g1", 0, "t2", 0, nullptr};
  storage.appendTest(testP2G1T2);

  fcf::NTest::Test testP2G2T1 = {"p2", 0, "g2", 0, "t1", 0, nullptr};
  storage.appendTest(testP2G2T1);

  fcf::NTest::Test testP2G2T2 = {"p2", 0, "g2", 0, "t2", 0, nullptr};
  storage.appendTest(testP2G2T2);

  {
    std::set<fcf::NTest::Test> tests;
    std::set<fcf::NTest::Test> expected = { testP1G1T1, testP1G1T2 };
    fcf::NTest::Options options;
    options.ignoreSelectors.push_back(fcf::NTest::Options::Selector{ {"p2"}, {}, {} });
    tests = storage.selectTests(options);
    FCF_TEST(tests == expected, testsToStr(tests), testsToStr(expected));
  }
  {
    std::set<fcf::NTest::Test> tests;
    std::set<fcf::NTest::Test> expected = { testP1G1T1, testP1G1T2 };
    fcf::NTest::Options options;
    options.selectors.push_back(fcf::NTest::Options::Selector{ {}, {}, {"t1", "t2"} });
    options.ignoreSelectors.push_back(fcf::NTest::Options::Selector{ {"p2"}, {}, {} });
    tests = storage.selectTests(options);
    FCF_TEST(tests == expected, testsToStr(tests), testsToStr(expected));
  }
  {
    std::set<fcf::NTest::Test> tests;
    std::set<fcf::NTest::Test> expected = { };
    fcf::NTest::Options options;
    options.selectors.push_back(fcf::NTest::Options::Selector{ {}, {}, {"t1", "t2"} });
    options.ignoreSelectors.push_back(fcf::NTest::Options::Selector{ {"*"}, {}, {} });
    tests = storage.selectTests(options);
    FCF_TEST(tests == expected, testsToStr(tests), testsToStr(expected));
  }
  {
    std::set<fcf::NTest::Test> tests;
    std::set<fcf::NTest::Test> expected = { testP1G1T1, testP2G1T1 };
    fcf::NTest::Options options;
    options.ignoreSelectors.push_back(fcf::NTest::Options::Selector{ {}, {"g2"}, {"t2"} });
    tests = storage.selectTests(options);
    FCF_TEST(tests == expected, testsToStr(tests), testsToStr(expected));
  }
}


FCF_TEST_DEFINE("fcfTest", "storage", "select (throw)") {
  fcf::NTest::Storage storage;
  fcf::NTest::Test testP1G1T1 = {"p1", 0, "g1", 0, "t1", 0, nullptr};
  storage.appendTest(testP1G1T1);
  {
    std::set<fcf::NTest::Test> tests;
    fcf::NTest::Options options;
    options.selectors.push_back(fcf::NTest::Options::Selector{ {"w"}, {}, {} });
    FCF_TEST_THROW(storage.selectTests(options), std::exception, testsToStr(tests));
  }
  {
    std::set<fcf::NTest::Test> tests;
    fcf::NTest::Options options;
    options.selectors.push_back(fcf::NTest::Options::Selector{ {}, {"w"}, {} });
    FCF_TEST_THROW(storage.selectTests(options), std::exception, testsToStr(tests));
  }
  {
    std::set<fcf::NTest::Test> tests;
    fcf::NTest::Options options;
    options.selectors.push_back(fcf::NTest::Options::Selector{ {}, {}, {"w"} });
    FCF_TEST_THROW(storage.selectTests(options), std::exception, testsToStr(tests));
  }
  {
    std::set<fcf::NTest::Test> tests;
    fcf::NTest::Options options;
    options.selectors.push_back(fcf::NTest::Options::Selector{ {"w"}, {}, {"w"} });
    FCF_TEST_THROW(storage.selectTests(options), std::exception, testsToStr(tests));
  }

}
