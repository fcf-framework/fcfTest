#include <fcfTest/test.hpp>
#include "helpers.hpp"

#define STR1(...) #__VA_ARGS__ ""
#define STR0(...) STR1(__VA_ARGS__)
#define STR(...) STR0(__VA_ARGS__)

FCF_TEST_DECLARE("fcfTest", "macro", "FCF_TEST_DECLARE") {
  {
    std::string macro = STR(Z_FCF_TEST_DECLARE__IMPL__MACRO_NAME());
    FCF_TEST(macro == "Z_FCF_TEST_DECLARE__IMPL__DEFINITIONAUTO", macro);
  }
  {
    std::string macro = STR(Z_FCF_TEST_DECLARE__IMPL__MACRO_NAME(a));
    FCF_TEST(macro == "Z_FCF_TEST_DECLARE__IMPL__DEFINITION", macro);
  }
}

