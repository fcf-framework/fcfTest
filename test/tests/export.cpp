#include <fcfTest/test.hpp>
#include "helpers.hpp"

FCF_TEST_DECLARE("fcfTest", "export", "run"){
  {
    fcf::NTest::Options options;
    options.parts.push_back("subrun-export");
    options.files.push_back({"test.console.log", "default"});
    options.files.push_back({"test.results.xml", "junit"});
    std::stringstream ss;
    bool error = InnerTestRunner().run(options, ss);

    FCF_TEST(!error);

    {
      std::string content = uniout(ss.str(), true);
      std::string expected =  std::string() +
                              "Performing the test: \"subrun-export\" -> \"Files\" -> \"MultiLogTest\" ...\n"+
                              "    [SUCCESS] Test completed successfully (XXX sec)\n"+
                              "\n"+
                              "[SUCCESS] All tests were completed.\n"+
                              "Tests: 1 passed, 0 failed, 0 skipped, 1 total\n"+
                              "Duration: XXX sec\n"
                              ;
      expected = uniout(expected, true);
      FCF_TEST(content == expected, content, expected);
    }
    {
      std::ifstream file("test.console.log", std::ios::in | std::ios::binary);
      std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
      content = uniout(content, true);
      std::string expected =  std::string() +
                              "Performing the test: \"subrun-export\" -> \"Files\" -> \"MultiLogTest\" ...\n"+
                              "    [SUCCESS] Test completed successfully (XXX sec)\n"+
                              "\n"+
                              "[SUCCESS] All tests were completed.\n"+
                              "Tests: 1 passed, 0 failed, 0 skipped, 1 total\n"+
                              "Duration: XXX sec\n"
                              ;
      expected = uniout(expected, true);
      FCF_TEST(content == expected, content, expected);
    }
    {
      std::ifstream file("test.results.xml", std::ios::in | std::ios::binary);
      std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
      content = uniout(content, true);
      std::string expected =  std::string() +
                              "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"+
                              "<testsuites tests=\"1\" failure=\"0\" skipped=\"0\" time=\"XXX\">\n"+
                              "  <testsuite name=\"subrun-export/Files\" tests=\"1\" failure=\"0\" skipped=\"0\" time=\"XXX\">\n"+
                              "    <testcase classname=\"subrun-export/Files\" name=\"MultiLogTest\" time=\"XXX\"/>\n"+
                              "  </testsuite>\n"+
                              "</testsuites>\n";
      expected = uniout(expected, true);
      FCF_TEST(content == expected, content, expected);
    }
  }
}


// A simple test case for demonstration
FCF_TEST_DECLARE("subrun-export", "Files", "MultiLogTest") {
    FCF_TEST(1 + 1 == 2);
}


