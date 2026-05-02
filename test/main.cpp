#include <iostream>
#include <sstream>
#define FCF_TEST_IMPLEMENTATION
#include <fcfTest/test.hpp>

int main(int a_argc, char* a_argv[]){
  {
    std::cout << "Input:" << std::endl;
    for(size_t i = 0; i < a_argc; ++i){
      std::cout << "  |" << a_argv[i] << "|" << std::endl;
    }
    auto vec = fcf::NTest::NDetails::parseArgs(a_argc, a_argv);
    std::cout << "Output:" << std::endl;
    for(size_t i = 0; i < vec.size(); ++i){
      std::cout << "  |" << vec[i] << "|" << std::endl;
    }
  } 
  {
    const char* argv[] = {"--test=12"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    if (vec.size() != 2)
      throw std::runtime_error((std::stringstream() << "Invalid check [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[0] != "--test")
      throw std::runtime_error((std::stringstream() << "Invalid check [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[1] != "12")
      throw std::runtime_error((std::stringstream() << "Invalid check [" << __FILE__ << ":" << __LINE__ << "]").str());
  }
  {
    const char* argv[] = {"--test", "12"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    if (vec.size() != 2)
      throw std::runtime_error((std::stringstream() << "Invalid check (size:" << vec.size() << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[0] != "--test")
      throw std::runtime_error((std::stringstream() << "Invalid check [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[1] != "12")
      throw std::runtime_error((std::stringstream() << "Invalid check [" << __FILE__ << ":" << __LINE__ << "]").str());
  }
  {
    const char* argv[] = {"--test=12"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    if (vec.size() != 2)
      throw std::runtime_error((std::stringstream() << "Invalid check (size:" << vec.size() << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[0] != "--test")
      throw std::runtime_error((std::stringstream() << "Invalid check [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[1] != "12")
      throw std::runtime_error((std::stringstream() << "Invalid check [" << __FILE__ << ":" << __LINE__ << "]").str());
  }
  {
    const char* argv[] = {"--test-test=dynamic container access"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    if (vec.size() != 2)
      throw std::runtime_error((std::stringstream() << "Invalid check (size:" << vec.size() << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[0] != "--test-test")
      throw std::runtime_error((std::stringstream() << "Invalid check (vec[0]:" << vec[0] << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[1] != "dynamic container access")
      throw std::runtime_error((std::stringstream() << "Invalid check (vec[1]:" << vec[1] << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
  }
  {
    const char* argv[] = {"--test-group=Variant", "--test-test=dynamic container access"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    if (vec.size() != 4)
      throw std::runtime_error((std::stringstream() << "Invalid check (size:" << vec.size() << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
    size_t i = 0;
    if (vec[i] != "--test-group")
      throw std::runtime_error((std::stringstream() << "Invalid check (vec[0]:" << vec[i] << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
    ++i;
    if (vec[i] != "Variant")
      throw std::runtime_error((std::stringstream() << "Invalid check (vec[1]:" << vec[i] << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
    ++i;
    if (vec[i] != "--test-test")
      throw std::runtime_error((std::stringstream() << "Invalid check (vec[2]:" << vec[i] << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
    ++i;
    if (vec[i] != "dynamic container access")
      throw std::runtime_error((std::stringstream() << "Invalid check (vec[3]:" << vec[i] << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
    ++i;
  }

  {
    const char* argv[] = {"--test=", "12"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    if (vec.size() != 2)
      throw std::runtime_error((std::stringstream() << "Invalid check (size:" << vec.size() << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[0] != "--test")
      throw std::runtime_error((std::stringstream() << "Invalid check [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[1] != "12")
      throw std::runtime_error((std::stringstream() << "Invalid check [" << __FILE__ << ":" << __LINE__ << "]").str());
  }
  {
    const char* argv[] = {"--test", "\"12\""};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    if (vec.size() != 2)
      throw std::runtime_error((std::stringstream() << "Invalid check (size:" << vec.size() << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[0] != "--test")
      throw std::runtime_error((std::stringstream() << "Invalid check [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[1] != "\"12\"")
      throw std::runtime_error((std::stringstream() << "Invalid check [" << __FILE__ << ":" << __LINE__ << "]").str());
  }
  {
    const char* argv[] = {"--test", "=12"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    if (vec.size() != 2)
      throw std::runtime_error((std::stringstream() << "Invalid check (size:" << vec.size() << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[0] != "--test")
      throw std::runtime_error((std::stringstream() << "Invalid check [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[1] != "12")
      throw std::runtime_error((std::stringstream() << "Invalid check [" << __FILE__ << ":" << __LINE__ << "]").str());
  }
  {
    const char* argv[] = {"--test", "=", "12"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    if (vec.size() != 2)
      throw std::runtime_error((std::stringstream() << "Invalid check (size:" << vec.size() << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[0] != "--test")
      throw std::runtime_error((std::stringstream() << "Invalid check [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[1] != "12")
      throw std::runtime_error((std::stringstream() << "Invalid check [" << __FILE__ << ":" << __LINE__ << "]").str());
  }
  {
    const char* argv[] = {"--test", "=12"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    /*
    for(auto itm : vec){
      std::cout << itm << std::endl;
    }
    */
    if (vec.size() != 2)
      throw std::runtime_error((std::stringstream() << "Invalid check (size:" << vec.size() << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[0] != "--test")
      throw std::runtime_error((std::stringstream() << "Invalid check (vec[1]: " << vec[0] << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[1] != "12")
      throw std::runtime_error((std::stringstream() << "Invalid check (vec[1]: " << vec[1] << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
  }
  {
    const char* argv[] = {"--test", "=1\"2"};
    int   argc = sizeof(argv) / sizeof(argv[0]);
    auto vec = fcf::NTest::NDetails::parseArgs(argc, argv);
    if (vec.size() != 2)
      throw std::runtime_error((std::stringstream() << "Invalid check (size:" << vec.size() << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[0] != "--test")
      throw std::runtime_error((std::stringstream() << "Invalid check (vec[1]: " << vec[0] << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
    if (vec[1] != "1\"2")
      throw std::runtime_error((std::stringstream() << "Invalid check (vec[1]: " << vec[1] << ") [" << __FILE__ << ":" << __LINE__ << "]").str());
  }

  return 0;
}

