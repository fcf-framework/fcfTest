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

#define FCF_TEST__STRINGIFY_2(a_arg) #a_arg
#define FCF_TEST__STRINGIFY_1(a_arg) FCF_TEST__STRINGIFY_2(a_arg)
#define FCF_TEST__STRINGIFY(a_arg)  FCF_TEST__STRINGIFY_1(a_arg)
#define FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_value) a_list.push_back(a_value);
#define FCF_TEST__APPEND_TO_LIST__EXPAND1(expand1, ...) #expand1
#define FCF_TEST__APPEND_TO_LIST__EXPAND2(expand1, expand2, ...) #expand2
#define FCF_TEST__APPEND_TO_LIST__EXPAND3(expand1, expand2, expand3, ...) #expand3
#define FCF_TEST__APPEND_TO_LIST__EXPAND4(expand1, expand2, expand3, expand4, ...) #expand4
#define FCF_TEST__APPEND_TO_LIST__EXPAND5(expand1, expand2, expand3, expand4, expand5, ...) #expand5
#define FCF_TEST__APPEND_TO_LIST__EXPAND6(expand1, expand2, expand3, expand4, expand5, expand6, ...) #expand6
#define FCF_TEST__APPEND_TO_LIST__EXPAND7(expand1, expand2, expand3, expand4, expand5, expand6, expand7, ...) #expand7
#define FCF_TEST__APPEND_TO_LIST__EXPAND8(expand1, expand2, expand3, expand4, expand5, expand6, expand7, expand8, ...) #expand8
#define FCF_TEST__APPEND_TO_LIST__EXPAND9(expand1, expand2, expand3, expand4, expand5, expand6, expand7, expand8, expand9, ...) #expand9
#define FCF_TEST__APPEND_TO_LIST__EXPAND10(expand1, expand2, expand3, expand4, expand5, expand6, expand7, expand8, expand9, expand10, ...) #expand10
#define FCF_TEST__APPEND_TO_LIST__EXECUTOR(a_list, a_a1, a_a2, a_a3, a_a4, a_a5, a_a6, a_a7, a_a8, a_a9, a_a10, ...) \
                      FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a1) \
                      FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a2) \
                      FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a3) \
                      FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a4) \
                      FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a5) \
                      FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a6) \
                      FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a7) \
                      FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a8) \
                      FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a9) \
                      FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a10)
#define FCF_TEST__APPEND_TO_LIST(a_list, ...)  FCF_TEST__APPEND_TO_LIST__EXECUTOR(a_list, \
                                                            FCF_TEST__APPEND_TO_LIST__EXPAND1(__VA_ARGS__, , , , , , , , , , , ),\
                                                            FCF_TEST__APPEND_TO_LIST__EXPAND2(__VA_ARGS__, , , , , , , , , , , ),\
                                                            FCF_TEST__APPEND_TO_LIST__EXPAND3(__VA_ARGS__, , , , , , , , , , , ),\
                                                            FCF_TEST__APPEND_TO_LIST__EXPAND4(__VA_ARGS__, , , , , , , , , , , ),\
                                                            FCF_TEST__APPEND_TO_LIST__EXPAND5(__VA_ARGS__, , , , , , , , , , , ),\
                                                            FCF_TEST__APPEND_TO_LIST__EXPAND6(__VA_ARGS__, , , , , , , , , , , ),\
                                                            FCF_TEST__APPEND_TO_LIST__EXPAND7(__VA_ARGS__, , , , , , , , , , , ),\
                                                            FCF_TEST__APPEND_TO_LIST__EXPAND8(__VA_ARGS__, , , , , , , , , , , ),\
                                                            FCF_TEST__APPEND_TO_LIST__EXPAND9(__VA_ARGS__, , , , , , , , , , , ),\
                                                            FCF_TEST__APPEND_TO_LIST__EXPAND10(__VA_ARGS__, , , , , , , , , , , )\
                                                            );
#define FCF_TEST(exp, ...) \
  if (!(exp)){ \
    std::list<std::string> names;\
    FCF_TEST__APPEND_TO_LIST(names, __VA_ARGS__)\
    fcf::Test::Details::PrintArgs<std::list<std::string>::iterator> p;\
    p.begin = names.begin();\
    p.end = names.end();\
    std::string messge = std::string() + \
                         "Test error: " + #exp + "  [FILE: " + __FILE__ + ":" + FCF_TEST__STRINGIFY(__LINE__) + "]\n" + \
                         p(__VA_ARGS__);\
    throw std::runtime_error(messge);\
  }

#endif // #ifndef ___FCF_TEST__TEST_HPP___
