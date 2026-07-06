#include <fcfTest/test.hpp>

#define STR1(x) #x
#define STR0(x) STR1(x)
#define STR(x) STR0(x)


FCF_TEST_DECLARE("fcfTest", "macro", "FCF_TEST"){
  {
    std::string macro   = STR(_FCF_TEST__APPEND_TO_LIST(list, ));
    std::string expeced = "";
    FCF_TEST(macro == expeced, macro, expeced);
  }
  {
    std::string macro   = STR(_FCF_TEST__APPEND_TO_LIST(list, "1"));
    std::string expeced = "list.push_back(\"\\\"1\\\"\");";
    FCF_TEST(macro == expeced, macro, expeced);
  }
  {
    std::string macro   = STR(_FCF_TEST__APPEND_TO_LIST(list,1,2));
    std::string expeced = "list.push_back(\"1\"); list.push_back(\"2\");";
    FCF_TEST(macro == expeced, macro, expeced);
  }
  {
    std::string macro   = STR(_FCF_TEST__APPEND_TO_LIST(list, (some<i,1>), 2));
    std::string expeced = "list.push_back(\"some<i,1>\"); list.push_back(\"2\");";
    FCF_TEST(macro == expeced, macro, expeced);
  }
}


