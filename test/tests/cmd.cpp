#include <fcfTest/test.hpp>

FCF_TEST_DECLARE("fcfTest", "cmd", "parser"){
  {
    const char* argv[] = {"--test=12"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    FCF_TEST(vec.size() == 2, vec.size());
    FCF_TEST(vec[0] == "--test", vec[0]);
    FCF_TEST(vec[1] == "12", vec[1]);
  }
  {
    const char* argv[] = {"--test=12"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    FCF_TEST(vec.size() == 2, vec.size());
    FCF_TEST(vec[0] == "--test", vec[0]);
    FCF_TEST(vec[1] == "12", vec[1]);
  }
  {
    const char* argv[] = {"--test", "12"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    FCF_TEST(vec.size() == 2, vec.size());
    FCF_TEST(vec[0] == "--test", vec[0]);
    FCF_TEST(vec[1] == "12", vec[1]);
  }
  {
    const char* argv[] = {"--test=", "12"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    FCF_TEST(vec.size() == 2, vec.size());
    FCF_TEST(vec[0] == "--test", vec[0]);
    FCF_TEST(vec[1] == "12", vec[1]);
  }
  {
    const char* argv[] = {"--test", "=12"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    FCF_TEST(vec.size() == 2, vec.size());
    FCF_TEST(vec[0] == "--test", vec[0]);
    FCF_TEST(vec[1] == "12", vec[1]);
  }
  {
    const char* argv[] = {"--test", "=", "12"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    FCF_TEST(vec.size() == 2, vec.size());
    FCF_TEST(vec[0] == "--test", vec[0]);
    FCF_TEST(vec[1] == "12", vec[1]);
  }
  {
    const char* argv[] = {"--test", "\"12\""};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    FCF_TEST(vec.size() == 2, vec.size());
    FCF_TEST(vec[0] == "--test", vec[0]);
    FCF_TEST(vec[1] == "\"12\"", vec[1]);
  }
  {
    const char* argv[] = {"--test", "=\"12\""};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    FCF_TEST(vec.size() == 2, vec.size());
    FCF_TEST(vec[0] == "--test", vec[0]);
    FCF_TEST(vec[1] == "\"12\"", vec[1]);
  }
  {
    const char* argv[] = {"--test=", "\"12\""};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    FCF_TEST(vec.size() == 2, vec.size());
    FCF_TEST(vec[0] == "--test", vec[0]);
    FCF_TEST(vec[1] == "\"12\"", vec[1]);
  }
  {
    const char* argv[] = {"--test", "=", "\"12\""};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    FCF_TEST(vec.size() == 2, vec.size());
    FCF_TEST(vec[0] == "--test", vec[0]);
    FCF_TEST(vec[1] == "\"12\"", vec[1]);
  }
  {
    const char* argv[] = {"--test", "=1\"2"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    FCF_TEST(vec.size() == 2, vec.size());
    FCF_TEST(vec[0] == "--test", vec[0]);
    FCF_TEST(vec[1] == "1\"2", vec[1]);
  }
  {
    const char* argv[] = {"--test-test=dynamic container access"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    FCF_TEST(vec.size() == 2, vec.size());
    FCF_TEST(vec[0] == "--test-test", vec[0]);
    FCF_TEST(vec[1] == "dynamic container access", vec[1]);
  }
  {
    const char* argv[] = {"--test-group=Variant", "--test-test=dynamic container access"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    FCF_TEST(vec.size() == 4, vec.size());
    FCF_TEST(vec[0] == "--test-group", vec[0]);
    FCF_TEST(vec[1] == "Variant", vec[1]);
    FCF_TEST(vec[2] == "--test-test", vec[2]);
    FCF_TEST(vec[3] == "dynamic container access", vec[3]);
  }
}

