#ifndef ___FCF_TEST__TEST_HPP___
#define ___FCF_TEST__TEST_HPP___

#include <stdexcept>
#include <string>
#include <sstream>
#include <list>

namespace fcf {
  namespace Test {
    namespace Details {

      template <typename... TPack>
      struct PrintPack {
        template <typename TIterator, typename TArg, typename... TPack2>
        std::string operator()(TIterator a_begName, TIterator a_endName, const TArg& a_arg, TPack2... a_pack){
          std::stringstream ss;
          std::string name = a_begName != a_endName ? *a_begName : "arg";
          ss << "    " << name << ": " << a_arg << std::endl;
          if (a_begName != a_endName){
            ++a_begName;
          }
          return ss.str() + PrintPack<TPack2...>()(a_begName, a_endName, a_pack...);
        }
      };

      template <>
      struct PrintPack<> {
        template <typename TIterator>
        std::string operator()(TIterator a_begName, TIterator a_endName){
          return std::string();
        }
      };

      template <typename TIterator>
      struct PrintArgs{
        TIterator begin;
        TIterator end;
        template <typename... TArgPack>
        std::string operator()(TArgPack... a_pack){
          std::string result;
          if (begin != end){
            result += "  Values:\n";
          }
          result += PrintPack<TArgPack...>()(begin, end, a_pack...);
          return result;
        }
      };
    } // Details namespace
  } // Test namespace
} // fcf namespace

#define FCF_TEST_STR_ARGS_S2(a_arg) #a_arg
#define FCF_TEST_STR_ARGS_S1(a_arg) FCF_TEST_STR_ARGS_S2(a_arg)
#define FCF_TEST_STR_ARGS_S(a_arg)  FCF_TEST_STR_ARGS_S1(a_arg)
#define FCF_TEST_STR_ARGS_P(a_list, a_value) if (FCF_TEST_STR_ARGS_S(a_value)[0]) { a_list.push_back(FCF_TEST_STR_ARGS_S(a_value)); };
#define FCF_TEST_STR_ARGS_A10(a_list, a_arg, ...) FCF_TEST_STR_ARGS_P(a_list, a_arg);
#define FCF_TEST_STR_ARGS_A9(a_list, a_arg, ...) FCF_TEST_STR_ARGS_P(a_list, a_arg); FCF_TEST_STR_ARGS_A10(a_list, __VA_ARGS__)
#define FCF_TEST_STR_ARGS_A8(a_list, a_arg, ...) FCF_TEST_STR_ARGS_P(a_list, a_arg); FCF_TEST_STR_ARGS_A9(a_list, __VA_ARGS__)
#define FCF_TEST_STR_ARGS_A7(a_list, a_arg, ...) FCF_TEST_STR_ARGS_P(a_list, a_arg); FCF_TEST_STR_ARGS_A8(a_list, __VA_ARGS__)
#define FCF_TEST_STR_ARGS_A6(a_list, a_arg, ...) FCF_TEST_STR_ARGS_P(a_list, a_arg); FCF_TEST_STR_ARGS_A7(a_list, __VA_ARGS__)
#define FCF_TEST_STR_ARGS_A5(a_list, a_arg, ...) FCF_TEST_STR_ARGS_P(a_list, a_arg); FCF_TEST_STR_ARGS_A6(a_list, __VA_ARGS__)
#define FCF_TEST_STR_ARGS_A4(a_list, a_arg, ...) FCF_TEST_STR_ARGS_P(a_list, a_arg); FCF_TEST_STR_ARGS_A5(a_list, __VA_ARGS__)
#define FCF_TEST_STR_ARGS_A3(a_list, a_arg, ...) FCF_TEST_STR_ARGS_P(a_list, a_arg); FCF_TEST_STR_ARGS_A4(a_list, __VA_ARGS__)
#define FCF_TEST_STR_ARGS_A2(a_list, a_arg, ...) FCF_TEST_STR_ARGS_P(a_list, a_arg); FCF_TEST_STR_ARGS_A3(a_list, __VA_ARGS__)
#define FCF_TEST_STR_ARGS_A1(a_list, a_arg, ...) FCF_TEST_STR_ARGS_P(a_list, a_arg); FCF_TEST_STR_ARGS_A2(a_list, __VA_ARGS__)
#define FCF_TEST_STR_ARGS(a_list, ...)\
  FCF_TEST_STR_ARGS_A1(a_list, __VA_ARGS__)
#define FCF_TEST(exp, ...) \
  if (!(exp)){ \
    std::list<std::string> names;\
    FCF_TEST_STR_ARGS(names, __VA_ARGS__);\
    fcf::Test::Details::PrintArgs<std::list<std::string>::iterator> p;\
    p.begin = names.begin();\
    p.end = names.end();\
    std::string messge = std::string() + \
                         "Test error: " + #exp + "  [FILE: " + __FILE__ + ":" + FCF_TEST_STR_ARGS_S(__LINE__) + "]\n" + \
                         p(__VA_ARGS__);\
    throw std::runtime_error(messge);\
  }

#endif // #ifndef ___FCF_TEST__TEST_HPP___
