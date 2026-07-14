#include <fcfTest/test.hpp>
#include "helpers.hpp"



FCF_TEST_DECLARE("fcfTest", "order", "macro") {
  {
    fcf::NTest::Options options;
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun-order 1"}, {}, {}} );
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun-order 2"}, {}, {}} );
    options.selectors.push_back( fcf::NTest::Options::Selector{{"subrun-order 3"}, {}, {}} );
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);

    FCF_TEST(!error);

    {
      std::string content = uniout(ss.str(), true);
      std::string expected =  std::string() +
                              "Performing the test: \"subrun-order 2\" -> \"subrun-order group 1\" -> \"subrun-order test\" ...\n" +
                              "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                              "Performing the test: \"subrun-order 3\" -> \"subrun-order group 1\" -> \"subrun-order test\" ...\n" +
                              "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                              "Performing the test: \"subrun-order 1\" -> \"subrun-order group 3\" -> \"subrun-order group 1 test\" ...\n" +
                              "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                              "Performing the test: \"subrun-order 1\" -> \"subrun-order group 1\" -> \"Mathematical functions subrun-order\" ...\n" +
                              "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                              "Performing the test: \"subrun-order 1\" -> \"subrun-order group 1\" -> \"Algorithm (sort) subrun-order\" ...\n" +
                              "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                              "Performing the test: \"subrun-order 1\" -> \"subrun-order group 1\" -> \"Algorithm (analysis) subrun-order\" ...\n" +
                              "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                              "Performing the test: \"subrun-order 1\" -> \"subrun-order group 2\" -> \"subrun-order group 1 test\" ...\n" +
                              "    [SUCCESS] Test completed successfully (XXX sec)\n" +
                              "\n" +
                              "[SUCCESS] All tests were completed.\n" +
                              "Tests: 7 passed, 0 failed, 0 skipped, 7 total\n" +
                              "Duration: XXX sec\n";
      expected = uniout(expected, true);
      FCF_TEST(content == expected, content, expected);
    }
  }

}

FCF_TEST_DECLARE("subrun-order 1", "subrun-order group 1", "Mathematical functions subrun-order"){
}

FCF_TEST_DECLARE("subrun-order 1", "subrun-order group 1", "Algorithm (sort) subrun-order"){
}

FCF_TEST_DECLARE("subrun-order 1", "subrun-order group 1", "Algorithm (analysis) subrun-order"){
}

FCF_TEST_DECLARE("subrun-order 1", "subrun-order group 2", "subrun-order group 1 test"){
}

FCF_TEST_DECLARE("subrun-order 1", "subrun-order group 3", "subrun-order group 1 test"){
}

FCF_TEST_DECLARE("subrun-order 2", "subrun-order group 1", "subrun-order test"){
}

FCF_TEST_DECLARE("subrun-order 3", "subrun-order group 1", "subrun-order test"){
}


FCF_TEST_PART_ORDER("subrun-order 2", 1);
FCF_TEST_PART_ORDER("subrun-order 3", 2);
FCF_TEST_PART_ORDER("subrun-order 1", 3);


FCF_TEST_GROUP_ORDER("subrun-order group 3", 1);
FCF_TEST_GROUP_ORDER("subrun-order group 1", 2);
FCF_TEST_GROUP_ORDER("subrun-order group 2", 3);

FCF_TEST_TEST_ORDER("Mathematical functions subrun-order", 1);
FCF_TEST_TEST_ORDER("Algorithm (sort) subrun-order", 2);
FCF_TEST_TEST_ORDER("Algorithm (analysis) subrun-order", 3);

