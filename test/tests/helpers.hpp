#pragma once

#include <sstream>
#include <regex>


inline std::string uniout(std::string a_string, bool a_wrap = false){
  std::string result;
  std::regex pattern1(R"(\d+\.\d+`\d+`\d+)");
  std::regex pattern2(R"(time=\"\d+\.\d+\")");
  std::regex pattern3(R"(\[FILE:[^\]]*)");
  std::regex pattern4(R"(\x1b\[[0-9;]*m)");
  result = std::regex_replace(a_string, pattern1, "XXX");
  result = std::regex_replace(result, pattern2, "time=\"XXX\"");
  result = std::regex_replace(result, pattern3, "[FILE: XXX");
  result = std::regex_replace(result, pattern4, "");
  if (a_wrap) {
    return std::string() + "<<<" + result + ">>>";
  } else {
    return result;
  }
}

class InnerTestRunner{
  public:
    InnerTestRunner()
      : _targets(fcf::NTest::logger().targets())
    {
    }
    bool run(fcf::NTest::Options& a_options, std::stringstream& a_sstream){
      fcf::NTest::LogOutputTargets targets = { {"default", &a_sstream, "", {}, {}} };
      fcf::NTest::logger().targets(targets);
      bool error = false;
      fcf::NTest::run(a_options, &error);
      return error;
    }
    void operator()(fcf::NTest::Options& a_options, std::stringstream& a_sstream, int a_argc, const char** a_argv){
      fcf::NTest::LogOutputTargets targets = { {"default", &a_sstream, "", {}, {}} };
      fcf::NTest::logger().targets(targets);
      bool error = false;
      fcf::NTest::cmdRun(a_options, a_argc, a_argv, fcf::NTest::CRM_RUN, &error);
    }
    ~InnerTestRunner(){
      fcf::NTest::logger().targets(_targets);
    }
  private:
    fcf::NTest::LogOutputTargets  _targets;

};


