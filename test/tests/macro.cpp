#include <fcfTest/test.hpp>

#define STR1(...) #__VA_ARGS__ ""
#define STR0(...) STR1(__VA_ARGS__)
#define STR(...) STR0(__VA_ARGS__)



FCF_TEST_DECLARE("fcfTest", "macro", "FCF_TEST"){
  {
    std::string macro   = STR( Z__FCF_TEST__APPEND_TO_LIST(list, ));
    std::string expeced = "";
    FCF_TEST(macro == expeced, macro, expeced);
  }
  {
    std::string macro = STR(Z__FCF_TEST__APPEND_TO_LIST(list, a));
    std::string expeced = ", \"a\"";
    std::string expecedMSVC = ", \" a\"";
    FCF_TEST(macro == expeced || macro == expecedMSVC, macro, expeced, expecedMSVC);
  }
  {
    std::string macro   = STR(Z__FCF_TEST__APPEND_TO_LIST(list, "1"));
    std::string expeced = ", \"\\\"1\\\"\"";
    std::string expecedMSVC = ", \" \\\"1\\\"\"";
    FCF_TEST(macro == expeced || macro == expecedMSVC, macro, expeced, expecedMSVC);
  }
  {
    std::string macro   = STR(Z__FCF_TEST__APPEND_TO_LIST(list,1,2));
    std::string expeced = ", \"1\" , \"2\"";
    std::string expecedMSVC = ", \" 1\" , \" 2\"";
    FCF_TEST(macro == expeced || macro == expecedMSVC, macro, expeced, expecedMSVC);
  }
  {
    std::string macro   = STR(Z__FCF_TEST__APPEND_TO_LIST(list, (some<i,1>), 2));
    std::string expeced = ", \"some<i,1>\" , \"2\"";
    std::string expecedMSVC = ", \" some<i,1>\" , \" 2\"";
    FCF_TEST(macro == expeced || macro == expecedMSVC, macro, expeced, expecedMSVC);
  }
  
}


