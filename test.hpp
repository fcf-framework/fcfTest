/**
 * fcfTest - Lightweight C++ Unit Testing Library (>= C++11)
 *
 * Homepage: https://fcf-framework.is-a-fullstack.dev/cpp-libraries/fcfTest
 * Repository: https://github.com/fcf-framework/fcfTest
 *
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2025 Vladimir Markin (v.o.markin221@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#ifndef Z___FCF_TEST__TEST_HPP___
#define Z___FCF_TEST__TEST_HPP___

#define FCF_TEST_VERSION_MAJOR        1
#define FCF_TEST_VERSION_MINOR        2
#define FCF_TEST_VERSION_MAINTENANCE  3

/*
 * fcfTest is a lightweight C++ (>= C++11) unit testing framework.
 * Modern, zero-dependency, header-only cpp unittest library for TDD.
 * It provides a simple single-macro interface (FCF_TEST) for seamless
 * assertion checks and automatic variable tracking using standard library components.
 * Designed as an easy-to-integrate, standalone C++ test library,
 * it includes built-in test registration, a comprehensive command-line test runner (CLI),
 * a native logger, and benchmarking tools for precise execution time measurement.
 * */

#include <stdexcept>
#include <algorithm>
#include <utility>
#include <memory>
#include <atomic>
#include <cstddef>
#include <typeinfo>
#include <string>
#include <functional>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <streambuf>
#include <cctype>
#include <chrono>
#include <list>
#include <map>
#include <vector>
#include <set>
#include <mutex>



/**
 * @brief FCF_TEST_IMPLEMENTATION/FCF_IMPLEMENTATION. Defines the implementation macro for FCF test functionality.
 * This block ensures that the implementation is only included when the main
 * FCF library is implemented, preventing multiple definition errors.
 */
#ifdef FCF_IMPLEMENTATION
  #ifndef FCF_TEST_IMPLEMENTATION
    #define FCF_TEST_IMPLEMENTATION
  #endif // #ifndef FCF_TEST_IMPLEMENTATION
#endif // #ifdef FCF_IMPLEMENTATION

/**
 * @brief FCF_EXPORT. Defines the export macro for FCF test symbols.
 * Inherits the behavior of the main library's `FCF_EXPORT` macro to ensure
 * consistent symbol visibility across different build configurations.
 */
#ifdef FCF_EXPORT
  #ifndef FCF_TEST_EXPORT
    #define FCF_TEST_EXPORT
  #endif // #ifndef FCF_TEST_EXPORT
#endif // #ifdef FCF_EXPORT

/**
 * @brief FCF_IMPORT. Defines the import macro for FCF test symbols.
 * Inherits the behavior of the main library's `FCF_IMPORT` macro to ensure
 * consistent symbol visibility when importing from a DLL.
 */
#ifdef FCF_IMPORT
  #ifndef FCF_TEST_IMPORT
    #define FCF_TEST_IMPORT
  #endif // #ifndef FCF_TEST_IMPORT
#endif // #ifdef FCF_IMPORT

/**
 * @brief Defines the external linkage macro for FCF test symbols.
 * Automatically set to empty when implementing the library (internal use)
 * and set to `extern` when only declaring it (header-only usage).
 */
#ifndef FCF_TEST_LINKAGE
  #ifdef FCF_TEST_IMPLEMENTATION
    #define FCF_TEST_LINKAGE
  #else
    #define FCF_TEST_LINKAGE extern
  #endif // #ifdef FCF_TEST_IMPLEMENTATION
#endif // #ifndef FCF_TEST_LINKAGE

/**
 * @brief Defines the export/declaration macro for Windows environments.
 * Uses `__declspec(dllexport)` or `__declspec(dllimport)` based on
 * whether symbols are being exported or imported, otherwise defaults to empty.
 */
#ifndef FCF_TEST_API
  #ifdef WIN32
    #if defined(FCF_TEST_EXPORT)
      #define FCF_TEST_API __declspec(dllexport)
    #elif defined(FCF_TEST_IMPORT)
      #define FCF_TEST_API __declspec(dllimport)
    #else
      #define FCF_TEST_API
    #endif // #if defined(FCF_TEST_EXPORT)
  #else
    #define FCF_TEST_API
  #endif // #ifdef WIN32
#endif // #ifndef FCF_TEST_API


#ifndef Z__FCF_TEST__CONCAT3
  #define Z__FCF_TEST__CONCAT3_2(am_x, am_y, am_z)\
              am_x##am_y##am_z
  #define Z__FCF_TEST__CONCAT3(am_x, am_y, am_z)\
              Z__FCF_TEST__CONCAT3_2(am_x, am_y, am_z)
#endif

#ifndef Z__FCF_TEST__CONCAT2
  #define Z__FCF_TEST__CONCAT2_2(am_x, am_y)\
              am_x##am_y
  #define Z__FCF_TEST__CONCAT2(am_x, am_y)\
              Z__FCF_TEST__CONCAT2_2(am_x, am_y)
#endif



/**
 * @brief Defines the default priority order for tests, groups, and parts.
 *
 * A higher value indicates a lower priority (executed later).
 * This constant is used when no specific order is provided via
 * FCF_TEST_PART_ORDER, FCF_TEST_GROUP_ORDER, or FCF_TEST_TEST_ORDER.
 */
#ifndef FCF_TEST_ORDER_DEFAULT
  #define FCF_TEST_ORDER_DEFAULT 1000000
#endif


/**
 * @brief FCF_TEST_DEFINE(const char* am_part, const char* am_group, const char* am_test)
 * @brief FCF_TEST_DEFINE(const char* am_part, const char* am_group, const char* am_test, TYPE am_testerClassName)
 * Macro to declare a new test case.
 * Registers the test function with the storage system and assigns it to a group and part.
 *
 * @param am_part The name of the part (logical grouping level).
 * @param am_group The name of the group (sub-grouping level).
 * @param am_test The name or identifier of the test function.
 * @param am_testerClassName The name of the tester class. Can be used to specify a 'friend' test.
 */
#ifndef FCF_TEST_DEFINE
  #define Z_FCF_TEST_DEFINE__EXPAND(am_arg) am_arg
  #define Z_FCF_TEST_DEFINE__IMPL__SELECT_LIST() , AUTO
  #define Z_FCF_TEST_DEFINE__IMPL__SELECT_ARG(am_a1, am_a2, ...) am_a2
  #define Z_FCF_TEST_DEFINE__IMPL__SELECT(...) \
                      Z_FCF_TEST_DEFINE__EXPAND(Z_FCF_TEST_DEFINE__IMPL__SELECT_ARG(__VA_ARGS__))
  #define Z_FCF_TEST_DEFINE__IMPL__MACRO_NAME(...)\
                      Z_FCF_TEST_DEFINE__EXPAND(Z__FCF_TEST__CONCAT2(Z_FCF_TEST_DEFINE__IMPL__DEFINITION, Z_FCF_TEST_DEFINE__IMPL__SELECT(Z_FCF_TEST_DEFINE__IMPL__SELECT_LIST __VA_ARGS__ () , )))
  #define Z_FCF_TEST_DEFINE__IMPL__DEFINITION(am_part,  am_group, am_test, am_autoTesterClassName, am_testerClassName)\
    class  am_testerClassName { \
      public:\
      am_testerClassName() {\
        ::fcf::NTest::storage().appendTest( ::fcf::NTest::Test{ am_part, FCF_TEST_ORDER_DEFAULT, am_group, FCF_TEST_ORDER_DEFAULT, am_test, FCF_TEST_ORDER_DEFAULT, am_testerClassName::test } );\
      }\
      static void test();\
    };\
    namespace {\
      am_testerClassName Z__FCF_TEST__CONCAT3(am_testerClassName, _reg_, __COUNTER__);\
    }\
    void am_testerClassName::test()

  #define Z_FCF_TEST_DEFINE__IMPL__DEFINITIONAUTO(am_part,  am_group, am_test, am_autoTesterClassName, ...) \
    namespace {\
      class  am_autoTesterClassName { \
        public:\
        am_autoTesterClassName() {\
          ::fcf::NTest::storage().appendTest( ::fcf::NTest::Test{ am_part, FCF_TEST_ORDER_DEFAULT, am_group, FCF_TEST_ORDER_DEFAULT, am_test, FCF_TEST_ORDER_DEFAULT, am_autoTesterClassName::test } );\
        }\
        static void test();\
      };\
      am_autoTesterClassName Z__FCF_TEST__CONCAT3(am_autoTesterClassName, _reg_, __COUNTER__);\
    }\
    void am_autoTesterClassName::test()


  #define Z_FCF_TEST_DEFINE__IMPL(am_macro, am_part,  am_group, am_test, am_autoTesterClassName, ...)\
    am_macro(am_part,  am_group, am_test, am_autoTesterClassName, __VA_ARGS__)

  #define FCF_TEST_DEFINE(am_part, am_group, am_test, ...)\
    Z_FCF_TEST_DEFINE__IMPL(Z_FCF_TEST_DEFINE__IMPL__MACRO_NAME(__VA_ARGS__), am_part,  am_group, am_test, Z__FCF_TEST__CONCAT2(_fcf_test_,__COUNTER__), __VA_ARGS__)

  // This macro is retained for compatibility with older versions.
  // This is the old name of FCF_TEST_DEFINE.
  #define FCF_TEST_DECLARE(am_part, am_group, am_test, ...)\
    FCF_TEST_DEFINE(am_part, am_group, am_test, __VA_ARGS__)
#endif



#ifndef FCF_TEST_BEFORE_DEFINE
  #define Z_FCF_TEST_BEFORE_DEFINE__EXPAND(am_arg) am_arg
  #define Z_FCF_TEST_BEFORE_DEFINE__IMPL__SELECT_LIST() , AUTO
  #define Z_FCF_TEST_BEFORE_DEFINE__IMPL__SELECT_ARG(am_a1, am_a2, ...) am_a2
  #define Z_FCF_TEST_BEFORE_DEFINE__IMPL__SELECT(...) \
                      Z_FCF_TEST_BEFORE_DEFINE__EXPAND(Z_FCF_TEST_BEFORE_DEFINE__IMPL__SELECT_ARG(__VA_ARGS__))
  #define Z_FCF_TEST_BEFORE_DEFINE__IMPL__MACRO_NAME(...)\
                      Z_FCF_TEST_BEFORE_DEFINE__EXPAND(Z__FCF_TEST__CONCAT2(Z_FCF_TEST_BEFORE_DEFINE__IMPL__DEFINITION, Z_FCF_TEST_BEFORE_DEFINE__IMPL__SELECT(Z_FCF_TEST_BEFORE_DEFINE__IMPL__SELECT_LIST __VA_ARGS__ () , )))
  #define Z_FCF_TEST_BEFORE_DEFINE__IMPL__DEFINITION(am_part,  am_group, am_test, am_start, am_level, am_autoFixtureClassName, am_fixtureClassName)\
    class  am_fixtureClassName { \
      public:\
      am_fixtureClassName() {\
        ::fcf::NTest::storage().appendFixture( ::fcf::NTest::Fixture{ ::fcf::NTest::NDetails::splitSelector(am_part), \
                                                                  ::fcf::NTest::NDetails::splitSelector(am_group), \
                                                                  ::fcf::NTest::NDetails::splitSelector(am_test), \
                                                                  am_start, am_level, am_fixtureClassName::fixture,\
                                                                  __FILE__, __LINE__ } );\
      }\
      static void fixture();\
    };\
    namespace {\
      am_fixtureClassName Z__FCF_TEST__CONCAT3(am_fixtureClassName, _reg_, __COUNTER__);\
    }\
    void am_fixtureClassName::fixture()

  #define Z_FCF_TEST_BEFORE_DEFINE__IMPL__DEFINITIONAUTO(am_part,  am_group, am_test, am_start, am_level, am_autoFixtureClassName, ...) \
    namespace {\
      class  am_autoFixtureClassName { \
        public:\
        am_autoFixtureClassName() {\
          ::fcf::NTest::storage().appendFixture( ::fcf::NTest::Fixture{ ::fcf::NTest::NDetails::splitSelector(am_part), \
                                                                    ::fcf::NTest::NDetails::splitSelector(am_group), \
                                                                    ::fcf::NTest::NDetails::splitSelector(am_test), \
                                                                    am_start, am_level, am_autoFixtureClassName::fixture,\
                                                                    __FILE__, __LINE__ } );\
        }\
        static void fixture();\
      };\
      am_autoFixtureClassName Z__FCF_TEST__CONCAT3(am_autoFixtureClassName, _reg_, __COUNTER__);\
    }\
    void am_autoFixtureClassName::fixture()

  #define Z_FCF_TEST_BEFORE_DEFINE__IMPL(am_macro, am_part,  am_group, am_test, am_start, am_level, am_autoFixtureClassName, ...)\
    am_macro(am_part,  am_group, am_test, am_start, am_level, am_autoFixtureClassName, __VA_ARGS__)

  #define FCF_TEST_BEFORE_DEFINE(am_part, am_group, am_test, am_level, ...)\
    Z_FCF_TEST_BEFORE_DEFINE__IMPL(Z_FCF_TEST_BEFORE_DEFINE__IMPL__MACRO_NAME(__VA_ARGS__), am_part,  am_group, am_test, true, am_level, Z__FCF_TEST__CONCAT2(_fcf_fixture_,__COUNTER__), __VA_ARGS__)
#endif

#ifndef FCF_TEST_AFTER_DEFINE
  #define FCF_TEST_AFTER_DEFINE(am_part, am_group, am_test, am_level, ...)\
    Z_FCF_TEST_BEFORE_DEFINE__IMPL(Z_FCF_TEST_BEFORE_DEFINE__IMPL__MACRO_NAME(__VA_ARGS__), am_part,  am_group, am_test, false, am_level, Z__FCF_TEST__CONCAT2(_fcf_fixture_,__COUNTER__), __VA_ARGS__)
#endif


#ifndef Z__FCF_TEST__REMOVE_PARENTHESIS
  #define Z__FCF_TEST__REMOVE_PARENTHESIS_SELECTORZ__FCF_TEST__REMOVE_PARENTHESIS_ARGUMENT
  #define Z__FCF_TEST__REMOVE_PARENTHESIS_ARGUMENT(...) Z__FCF_TEST__REMOVE_PARENTHESIS_ARGUMENT __VA_ARGS__
  #define Z__FCF_TEST__REMOVE_PARENTHESIS_0(...) Z__FCF_TEST__REMOVE_PARENTHESIS_SELECTOR##__VA_ARGS__
  #define Z__FCF_TEST__REMOVE_PARENTHESIS(...) Z__FCF_TEST__REMOVE_PARENTHESIS_0(__VA_ARGS__)
#endif


#ifndef Z__FCF_TEST__STRINGIFY
  #define Z__FCF_TEST__STRINGIFY_2(a_arg) #a_arg ""
  #define Z__FCF_TEST__STRINGIFY_1(a_arg) Z__FCF_TEST__STRINGIFY_2(a_arg)
  #define Z__FCF_TEST__STRINGIFY(a_arg)  Z__FCF_TEST__STRINGIFY_1(a_arg)
#endif


#ifndef Z__FCF_TEST__EXPAND
  #define  Z__FCF_TEST__EXPAND(...) __VA_ARGS__
#endif


/**
 * @brief FCF_TEST. Macro to assert a condition and throw an error if it fails.
 * Generates a detailed error message including the failing expression and failed arguments.
 *
 * @param exp The boolean condition to check.
 * @param ... Variable list of arguments whose values will be included in the error message if 'exp' is false.
 */
#ifndef FCF_TEST
  #define Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS__COMMA_SELECTOR__ARG_A20(am_a1, am_a2, am_a3, am_a4, am_a5, am_a6, am_a7, am_a8, am_a9, am_a10, \
                                               am_a11, am_a12, am_a13, am_a14, am_a15, am_a16, am_a17, am_a18, am_a19, am_a20, \
                                               ...) am_a20
  #define Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS__COMMA_SELECTOR__ARG_LIST(...) \
            Z__FCF_TEST__EXPAND(Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS__COMMA_SELECTOR__ARG_A20(__VA_ARGS__, CM, CM, CM, CM, CM, CM, CM, CM, CM, \
                                                              CM, CM, CM, CM, CM, CM, CM, CM, CM, CM))
  #define Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS__COMMA_SELECTOR__PARENTHESIS(...) , , , , , , , , , , \
                                                        , , , , , , , , , ,

  #define Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS__COMMA_SELECTOR(...) \
            Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS__COMMA_SELECTOR__ARG_LIST ( Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS__COMMA_SELECTOR__PARENTHESIS  __VA_ARGS__ ( ))

  #define Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS__RESULT(am_list, ...)
  #define Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS__RESULTCM(am_list, ...) , #__VA_ARGS__

  #define Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS__CALL_RESULT1(am_macro, am_argument, ...) am_macro(am_argument, __VA_ARGS__)
  #define Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS__CALL_RESULT0(am_macro, am_argument, ...) \
            Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS__CALL_RESULT1(Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS__RESULT##am_macro, am_argument, __VA_ARGS__)
  #define Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS__CALL_RESULT(am_macro, am_argument, ...) \
            Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS__CALL_RESULT0(am_macro, am_argument, __VA_ARGS__)

  #define Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS(am_error, ...) \
            Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS__CALL_RESULT(Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS__COMMA_SELECTOR(__VA_ARGS__), am_error, __VA_ARGS__)

  #define Z__FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, ...) Z__FCF_TEST__APPEND_TO_LIST__CONCAT_ARGS(a_list, Z__FCF_TEST__REMOVE_PARENTHESIS(Z__FCF_TEST__REMOVE_PARENTHESIS_ARGUMENT __VA_ARGS__))
  #define Z__FCF_TEST__APPEND_TO_LIST__EXECUTOR_0(a_list, a_a1, a_a2, a_a3, a_a4, a_a5, a_a6, a_a7, a_a8, a_a9, a_a10, ...) \
                        Z__FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a1) \
                        Z__FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a2) \
                        Z__FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a3) \
                        Z__FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a4) \
                        Z__FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a5) \
                        Z__FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a6) \
                        Z__FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a7) \
                        Z__FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a8) \
                        Z__FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a9) \
                        Z__FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a10)
  #define Z__FCF_TEST__APPEND_TO_LIST__EXECUTOR(a_list, ...)  Z__FCF_TEST__EXPAND(Z__FCF_TEST__APPEND_TO_LIST__EXECUTOR_0(a_list,  __VA_ARGS__))
  #define Z__FCF_TEST__APPEND_TO_LIST(a_list, ...)  Z__FCF_TEST__APPEND_TO_LIST__EXECUTOR(a_list, __VA_ARGS__, , , , , , , , , , , )

  #define FCF_TEST(exp, ...) \
    if (!(exp)) { \
      ::fcf::NTest::NDetails::Printer _fcf_test_error_printer(Z__FCF_TEST__STRINGIFY(exp), \
                                         __FILE__, \
                                         Z__FCF_TEST__STRINGIFY(__LINE__)\
                                         Z__FCF_TEST__APPEND_TO_LIST(_fcf_test_names, __VA_ARGS__)\
                                         );\
      std::runtime_error exception(_fcf_test_error_printer(__VA_ARGS__));\
      ::fcf::NTest::state().error(exception.what(), false);\
      throw exception;\
    }

  #define FCF_TEST_CHECK(exp, ...) \
    ::fcf::NTest::NDetails::Printer(Z__FCF_TEST__STRINGIFY(exp), \
                                       __FILE__, \
                                       Z__FCF_TEST__STRINGIFY(__LINE__)\
                                       Z__FCF_TEST__APPEND_TO_LIST(_fcf_test_names, __VA_ARGS__)\
                                       ).inlineCheck((exp), __VA_ARGS__)
#endif

/**
 * @brief FCF_TEST_THROW. Macro to assert that a code block throws an exception.
 * If the provided code does not throw any exception, it generates a detailed
 * error message including the expression, file, line number, and any provided
 * arguments, then logs the error and throws a std::runtime_error.
 *
 * @param am_code The code block or expression to be executed and checked for an exception.
 * @param am_exception The name of the expected exception.
 *                     To catch all exceptions, this parameter must be ...
 * @param ... Variable list of arguments whose values will be included in the error message if no exception is thrown.
 */
#ifndef FCF_TEST_THROW
  #define FCF_TEST_THROW(am_code, am_exception, ...)\
    {\
      bool _fcf_test_throw_flag = false;\
      bool _fcf_test_throw_uncatch_flag = false;\
      try { \
        try {\
          am_code;\
        } catch( const Z__FCF_TEST__REMOVE_PARENTHESIS(Z__FCF_TEST__REMOVE_PARENTHESIS_ARGUMENT am_exception)& ) {\
          _fcf_test_throw_flag = true;\
        }\
      } catch(...) {\
        _fcf_test_throw_uncatch_flag = true;\
      }\
      if (!_fcf_test_throw_flag || _fcf_test_throw_uncatch_flag) {\
        ::fcf::NTest::NDetails::Printer _fcf_test_error_printer(\
            std::pair<const char*, const char*> {\
              Z__FCF_TEST__STRINGIFY(am_code), \
              _fcf_test_throw_uncatch_flag ? Z__FCF_TEST__STRINGIFY(am_exception) : nullptr\
            }, \
            __FILE__, \
            Z__FCF_TEST__STRINGIFY(__LINE__)\
            Z__FCF_TEST__APPEND_TO_LIST(_fcf_test_names, __VA_ARGS__)\
            );\
        std::runtime_error _fcf_test_throw_exception(_fcf_test_error_printer(__VA_ARGS__));\
        ::fcf::NTest::state().error(_fcf_test_throw_exception.what(), false);\
        throw _fcf_test_throw_exception;\
      }\
    }
#endif

/**
 * @brief FCF_TEST_THROW_CHECK. Non-throwing macro to assert that a code block throws an exception.
 * Evaluates the provided code block. If no exception is thrown, it logs a detailed
 * error message to the internal state but does not throw an exception.
 *
 * @return true if an exception was caught, false if no exception was thrown.
 *
 * @param am_code The code block or expression to be executed and checked for an exception.
 * @param am_exception The name of the expected exception.
 *                     To catch all exceptions, this parameter must be ...
 * @param ... Variable list of arguments whose values will be included in the error message if no exception is thrown.
 */
#ifndef FCF_TEST_THROW_CHECK
  #define FCF_TEST_THROW_CHECK(am_code, am_exception, ...)\
    [&]() -> bool{\
      bool _fcf_test_throw_uncatch_flag = false;\
      try {\
        try {\
          am_code;\
        } catch( const Z__FCF_TEST__REMOVE_PARENTHESIS(Z__FCF_TEST__REMOVE_PARENTHESIS_ARGUMENT am_exception)&  ) {\
          return true;\
        }\
      } catch(...) {\
        _fcf_test_throw_uncatch_flag = true;\
      }\
      ::fcf::NTest::NDetails::Printer _fcf_test_error_printer(\
          std::pair<const char*, const char*> {\
            Z__FCF_TEST__STRINGIFY(am_code), \
            _fcf_test_throw_uncatch_flag ? Z__FCF_TEST__STRINGIFY(am_exception) : nullptr\
          }, \
          __FILE__, \
          Z__FCF_TEST__STRINGIFY(__LINE__)\
          Z__FCF_TEST__APPEND_TO_LIST(_fcf_test_names, __VA_ARGS__)\
          );\
      ::fcf::NTest::state().error(std::runtime_error(_fcf_test_error_printer(__VA_ARGS__)).what(), false);\
      return false;\
    }()
#endif

/**
 * @brief FCF_TEST_PART_ORDER. Macro to register the order of a test part.
 * Ensures deterministic execution order for parts.
 *
 * @param am_part The name of the part.
 * @param am_order The desired execution order (integer).
 */
#define FCF_TEST_PART_ORDER(am_part, am_order)\
  namespace {\
    ::fcf::NTest::NDetails::PartOrderRegistrator Z__FCF_TEST__CONCAT2(fcf_test_order_registrator_, __COUNTER__)(am_part, am_order);\
  }

/**
 * @brief FCF_TEST_GROUP_ORDER. Macro to register the order of a test group.
 * Ensures deterministic execution order for groups within a part.
 *
 * @param am_group The name of the group.
 * @param am_order The desired execution order (integer).
 */
#define FCF_TEST_GROUP_ORDER(am_group, am_order)\
  namespace {\
    ::fcf::NTest::NDetails::GroupOrderRegistrator Z__FCF_TEST__CONCAT2(fcf_test_order_registrator_, __COUNTER__)(am_group, am_order);\
  }

/**
 * @brief FCF_TEST_TEST_ORDER. Macro to register the order of a specific test.
 * Ensures deterministic execution order for tests within a group.
 *
 * @param am_test The name of the test.
 * @param am_order The desired execution order (integer).
 */
#define FCF_TEST_TEST_ORDER(am_test, am_order)\
  namespace {\
    ::fcf::NTest::NDetails::TestOrderRegistrator Z__FCF_TEST__CONCAT2(fcf_test_order_registrator_, __COUNTER__)(am_test, am_order);\
  }

#ifndef Z__FCF_TEST_ANSI_SUCCESS
  #define Z__FCF_TEST_ANSI_SUCCESS  "\033[1;32m"
#endif
#ifndef Z__FCF_TEST_ANSI_FAILED
  #define Z__FCF_TEST_ANSI_FAILED   "\033[1;31m"
#endif
#ifndef Z__FCF_TEST_ANSI_RESET
  #define Z__FCF_TEST_ANSI_RESET    "\033[0m"
#endif

namespace fcf {
  namespace NTest {

    struct Options;

    /**
     * @brief Enumerates command modes for the test runner.
     */
    enum ECmdMode {
      CM_NONE,   ///< No specific mode set.
      CM_RUN,    ///< Run tests mode.
      CM_LIST,   ///< List tests mode.
      CM_HELP,   ///< Help mode.
    };

    /**
     * @brief Enumerates modes for command line argument parsing and execution.
     */
    enum ECmdRunMode {
      CRM_PARSE,    ///< cmdRun() only parses the command line.
      CRM_EXECUTE,  ///< cmdRun() parses the command line and runs tests if the --test-run flag was passed.
      CRM_RUN,      ///< cmdRun() parses the command line and runs the tests unless the --test-help or --test-list run flags were specified on the command line.
    };

   /**
     * @brief Enumerates the available log levels.
     */
    enum ELogLevel {
      LL_DEF = -1,  ///< is used to indicate the use of the meaning by default.
      LL_OFF = 0,   ///< No logging.
      LL_FTL = 1,   ///< Fatal level.
      LL_ERR = 2,   ///< Error level.
      LL_WRN = 3,   ///< Warning level.
      LL_ATT = 4,   ///< Attention level.
      LL_LOG = 5,   ///< Log level.
      LL_INF = 6,   ///< Information level.
      LL_DBG = 7,   ///< Debug level.
      LL_TRC = 8,   ///< Trace level.
      LL_ALL = 9,   ///< All levels.
    };


    /**
     * @brief Enumerates message categories for the logger.
     * Used for filtering and prefixing log messages.
     *
     * The fcf::NTest::ELogMessageCategory enumeration is used to
     * categorize every log message produced by the framework.
     * It is designed as a bitmask, which allows a single log message
     * to belong to multiple categories simultaneously,
     * or for a logger to filter messages based on a combination of categories.
     *
     * The high two bytes are the message category/type.
     * When determining whether a message matches a given type,
     * a check is performed using the & operator.
     *
     * The low two bytes are the message number.
     * And the equality check is performed if it is not zero.
     * If the low-order bytes are zero, then all common
     * bytes from the specified group are selected.
     *
     * The first two bytes can be set by the user to
     * identify the message. Example: LMC_USER_GROUP | 0x0001
     */
    enum ELogMessageCategory {
      LMC_ROOT_GROUP                = 0x00010000,
      LMC_LAUNCH_GROUP              = 0x00020000,
      LMC_TEST_GROUP                = 0x00040000,
      LMC_USER_GROUP                = 0x00080000,                                     ///< User-defined messages.
      LMC_SYSTEM_GROUP              = 0x80000000,                                     ///< Messages are not displayed and are system messages.
      LMC_ROOT_START                = LMC_SYSTEM_GROUP | LMC_ROOT_GROUP | 0x0001,     ///< [no output] Start of the test execution process.
      LMC_ROOT_END                  = LMC_SYSTEM_GROUP | LMC_ROOT_GROUP | 0x0002,     ///< [no output] End of the test execution process.
      LMC_ROOT_COMPLETE             = LMC_ROOT_GROUP | 0x0003,                        ///< Overall completion status.
      LMC_ROOT_ERROR                = LMC_ROOT_GROUP | 0x0004,                        ///< General error messages.
      LMC_ROOT_SUMMARY              = LMC_ROOT_GROUP | 0x0005,                        ///< Test results summary.
      LMC_ROOT_DURATION             = LMC_ROOT_GROUP | 0x0006,                        ///< Execution time/duration information.
      LMC_ROOT_RUN_ERROR            = LMC_ROOT_GROUP | 0x0007,                        ///< Error during the test runner execution.
      LMC_ROOT_NEW_LINE             = LMC_ROOT_GROUP | 0x0008,                        ///< Error during the test runner execution.
      LMC_LAUNCH_START              = LMC_SYSTEM_GROUP | LMC_LAUNCH_GROUP  | 0x0001,  ///< Start of an individual test case.
      LMC_LAUNCH_START_MESSAGE      = LMC_LAUNCH_GROUP | 0x0002,                      ///< Descriptive message for test start.
      LMC_LAUNCH_START_CASE_MESSAGE = LMC_LAUNCH_GROUP | 0x0003,                      ///< Descriptive running for a specific parameter case start.
      LMC_LAUNCH_END                = LMC_SYSTEM_GROUP | LMC_LAUNCH_GROUP  | 0x0004,  ///< End of an individual test case.
      LMC_TEST_COMPLETE             = LMC_TEST_GROUP | 0x0001,                        ///< Successful completion of a test case.
      LMC_TEST_ERROR                = LMC_TEST_GROUP | 0x0002,                        ///< Failure of a test case.
      LMC_TEST_ERROR_MESSAGE        = LMC_TEST_GROUP | 0x0003,                        ///< Detailed error description for a test.
      LMC_ALL                       = 0xFFFF0000,                                     ///< All message categories.
    };

    enum EFixtureLevel{
      FL_GLOBAL,
      FL_PART,
      FL_GROUP,
      FL_TEST
    };

  } // NTest namespace
} // fcf namespace


/* ========================================================== */
/* ===                                                    === */
/* ===              Declaration of the main               === */
/* ===             functions of unit testing              === */
/* ===                                                    === */
/* ========================================================== */

namespace fcf {
  namespace NTest {

    /**
     * @brief Parses command line arguments and executes the appropriate action.
     *
     * @param a_dstOptions Reference to the options structure to populate with parsed arguments.
     * @param a_argc Number of command line arguments.
     * @param a_argv Array of command line arguments.
     * @param a_runMode Current mode of execution (parse, execute, or run).
     )* @param a_errorPtr (default = (bool*)0) A pointer to a variable that receives information about a test error.
     *                                        If an error occurs, the value is set to true.
     *                                        If a null pointer is passed, the function throws an exception.
     * @return The determined command mode after processing.
     */
    inline ECmdMode cmdRun(Options& a_dstOptions, int a_argc, const char* const* a_argv, ECmdRunMode a_runMode, bool* a_errorPtr);

    inline ECmdMode cmdRun(Options& a_dstOptions, int a_argc, const char* const* a_argv, ECmdRunMode a_runMode);

    template <typename Ty>
    inline ECmdMode cmdRun(Options& a_dstOptions, int a_argc, Ty a_argv, ECmdRunMode a_runMode, bool* a_errorPtr);

    template <typename Ty>
    inline ECmdMode cmdRun(Options& a_dstOptions, int a_argc, Ty a_argv, ECmdRunMode a_runMode);

    template <typename Ty>
    inline ECmdMode cmdRun(int a_argc, Ty a_argv, ECmdRunMode a_runMode, bool* a_errorPtr);

    template <typename Ty>
    inline ECmdMode cmdRun(int a_argc, Ty a_argv, ECmdRunMode a_runMode);

    /**
     * @brief Declaration for displaying a list of all registered tests.
     */
    FCF_TEST_API void cmdHelp();


    /**
     * @brief Declaration for displaying a list of all registered tests.
     */
    FCF_TEST_API void cmdList();



    /**
     * @brief Executes the selected tests based on provided options.
     *
     * @param a_options Configuration options specifying which tests to run and logging level.
     * @param a_errorPtr (default = (bool*)0) A pointer to a variable receiving error information.
     *                                        If an error occurs, the value is set to true.
     *                                        If a null pointer is passed, the function throws an exception.
     */
    inline void run(const Options& a_options, bool* a_errorPtr);

    inline void run(const Options& a_options);

    class Storage;

    /**
     * @brief Declaration for the global storage instance.
     * @return Reference to the Storage instance.
     */
    FCF_TEST_API Storage& storage();

  } // NTest namespace
} // fcf namespace


/* ========================================================== */
/* ===                                                    === */
/* ===              Declaration of the basic              === */
/* ===             structures of unit testing             === */
/* ===                                                    === */
/* ========================================================== */

namespace fcf {
  namespace NTest {
    /**
     * @brief Represents a single test case with metadata.
     */
    struct Test {
      std::string part;            ///< Name of the test part.
      int         partOrder;       ///< Execution order within the part.
      std::string group;           ///< Name of the test group.
      int         groupOrder;      ///< Execution order within the group.
      std::string test;            ///< Name of the test function.
      int         testOrder;       ///< Execution order within the test.
      void (*testFunction)();      ///< Pointer to the test function to execute.

      Test()
        : partOrder(FCF_TEST_ORDER_DEFAULT)
        , groupOrder(FCF_TEST_ORDER_DEFAULT)
        , testOrder(FCF_TEST_ORDER_DEFAULT)
        , testFunction(nullptr)
      {}

      Test(std::string a_part, int a_partOrder, std::string a_group, int a_groupOrder,
           std::string a_test, int a_testOrder, void (*a_testFunction)())
        : part(a_part)
        , partOrder(a_partOrder)
        , group(a_group)
        , groupOrder(a_groupOrder)
        , test(a_test)
        , testOrder(a_testOrder)
        , testFunction(a_testFunction)
      {}

      /**
       * @brief Comparison operator for sorting tests by hierarchy and order.
       * Sorting priority: Part Order -> Part Name -> Group Order -> Group Name -> Test Order -> Test Name.
       *
       * @param a_test The test instance to compare against.
       * @return true if this test should precede the other in execution order.
       */
      bool operator<(const Test& a_test) const {
        return partOrder < a_test.partOrder ? true :
               partOrder > a_test.partOrder ? false :
               part < a_test.part ? true :
               part > a_test.part ? false :
               groupOrder < a_test.groupOrder ? true :
               groupOrder > a_test.groupOrder ? false :
               group < a_test.group ? true :
               group > a_test.group ? false :
               testOrder < a_test.testOrder ? true :
               testOrder > a_test.testOrder ? false :
               test < a_test.test ? true :
               test > a_test.test ? false :
                                    false;
      }

      bool operator==(const Test& a_test) const {
        return partOrder == a_test.partOrder &&
               partOrder == a_test.partOrder &&
               part == a_test.part &&
               part == a_test.part &&
               groupOrder == a_test.groupOrder &&
               groupOrder == a_test.groupOrder &&
               group == a_test.group &&
               group == a_test.group &&
               testOrder == a_test.testOrder &&
               testOrder == a_test.testOrder &&
               test == a_test.test &&
               test == a_test.test;
      }
    };

    struct TestPath {
      std::string part;
      std::string group;
      std::string test;
    };

    class FCF_TEST_API SharedPtrAny;

    /**
     * @brief Represents a test fixture used for setup and teardown operations.
     *
     * A fixture can be applied at different levels of the test hierarchy
     * (Global, Part, Group, or Test) and is executed before and after
     * the target tests.
     */
    struct Fixture {
      std::vector<std::string>  parts;    ///< List of part names to which this fixture applies. Empty means all parts.
      std::vector<std::string>  groups;   ///< List of group names to which this fixture applies. Empty means all groups.
      std::vector<std::string>  tests;    ///< List of specific test names to which this fixture applies. Empty means all tests.
      bool                      before;   ///< True if this is a setup fixture (runs before), false if it is a teardown fixture (runs after).
      EFixtureLevel             level;   ///< The scope/level of the fixture (FL_GLOBAL, FL_PART, FL_GROUP, or FL_TEST).
      void (*fixtureFunction)();         ///< Pointer to the function containing the fixture logic.
      std::string               file;    ///< Source file where the fixture was defined.
      unsigned int              line;    ///< Source line number where the fixture was defined.

      Fixture()
        : before(true)
        , level(FL_GLOBAL)
        , fixtureFunction(nullptr)
        , line(0)
      {}

      Fixture(std::vector<std::string>  a_parts,
              std::vector<std::string>  a_groups,
              std::vector<std::string>  a_tests,
              bool                      a_before,
              EFixtureLevel             a_level,
              void (*a_fixtureFunction)(),
              std::string               a_file,
              unsigned int              a_line)
        : parts(a_parts),
          groups(a_groups),
          tests(a_tests),
          before(a_before),
          level(a_level),
          fixtureFunction(a_fixtureFunction),
          file(a_file),
          line(a_line)
      {}
    };

    /**
     * @brief Configuration options for the test runner.
     *
     * This structure holds all the settings used to filter tests,
     * configure logging, and specify output files.
     */
    struct Options {
      /**
       * @brief Represents a file output target.
       */
      struct File {
        std::string  file;   ///< Path to the output file.
        std::string  format; ///< Output format to use for this specific file (e.g., "default", "junit").
      };

      /**
       * @brief A selector used to filter tests based on hierarchy.
       *
       * A selector can match tests by their part, group, or test.
       * If the vector is empty, all elements from the given level are selected.
       * If the element is "*" or an empty string, all elements from the specified level are selected.
       */
      struct Selector {
        std::vector<std::string>  parts;  ///< List of part names to include.
        std::vector<std::string>  groups; ///< List of group names to include.
        std::vector<std::string>  tests;  ///< List of specific test names to include.
      };

      std::vector<Selector>    selectors;       ///< List of selectors to include specific tests.
      std::vector<Selector>    ignoreSelectors; ///< List of selectors to exclude specific tests.
      ELogLevel                logLevel;        ///< Desired verbosity level of the logger.
      std::string              format;          ///< Default output format (e.g., "default", "junit").
      bool                     noBreak;         ///< If true, the runner will continue testing even after a failure.
      std::vector<File>        files;           ///< List of files where logs should be written.

      Options()
        : logLevel(LL_DEF)
        , format("default")
        , noBreak(false) {
      }
    };

    /**
     * @brief Container for a collection of tests within a group.
     */
    struct Tests {
      typedef std::map<std::string, Test> Map;
      Map values; ///< Map of test names to Test objects.
    };

    /**
     * @brief Container for a collection of groups within a part.
     */
    struct Groups {
      typedef std::map<std::string, Tests> Map;
      Map values; ///< Map of group names to Tests containers.
    };

    /**
     * @brief Container for a collection of groups across all parts.
     */
    struct Parts {
      typedef std::map<std::string, Groups> Map;
      Map values; ///< Map of part names to Groups containers.
    };



    /**
     * @brief Central storage for registered tests, parts, groups, and fixtures.
     *
     * This class acts as a thread-safe repository for all test metadata,
     * including test functions, hierarchical organization (parts and groups),
     * execution orders, and setup/teardown fixtures.
     */
    class FCF_TEST_API Storage {
      public:

        /**
         * @brief Sets the execution order for a specific part.
         * @param a_name The name of the part.
         * @param a_order The priority order (lower values run first).
         */
        void partOrder(const char* a_name, int a_order);

        /**
         * @brief Gets the execution order of a specific part.
         * @param a_name The name of the part.
         * @return The assigned order, or FCF_TEST_ORDER_DEFAULT if not set.
         */
        int partOrder(const char* a_name) const;

        /**
         * @brief Sets the execution order for a specific group.
         * @param a_name The name of the group.
         * @param a_order The priority order (lower values run first).
         */
        void groupOrder(const char* a_name, int a_order);

        /**
         * @brief Gets the execution order of a specific group.
         * @param a_name The name of the group.
         * @return The assigned order, or FCF_TEST_ORDER_DEFAULT if not set.
         */
        int groupOrder(const char* a_name) const;

        /**
         * @brief Sets the execution order for a specific test.
         * @param a_name The name of the test.
         * @param a_order The priority order (lower values run first).
         */
        void testOrder(const char* a_name, int a_order);

        /**
         * @brief Gets the execution order of a specific test.
         * @param a_name The name of the test.
         * @return The assigned order, or FCF_TEST_ORDER_DEFAULT if not set.
         */
        int testOrder(const char* a_name)  const;

        /**
         * @brief Recursively selects tests based on the provided options and filters.
         * @param a_dst Destination set where selected tests will be inserted.
         * @param a_options Configuration options containing selectors and ignore rules.
         */
        std::set<Test> selectTests(const Options& a_options) const;

        /**
         * @brief Adds a new test to the storage, organizing it into parts and groups.
         * @param a_test The Test object containing metadata and function pointer.
         */
        void appendTest(const Test& a_test);

        /**
         * @brief Returns a constant reference to the collection of registered fixtures.
         * @return A vector containing all registered Fixture objects.
         */
        std::vector<Fixture> fixtures() const;

        /**
         * @brief Adds a new fixture to the storage, organizing it into parts and groups.
         * @param a_fixture The Fixture object containing metadata and function pointer.
         */
        void appendFixture(const Fixture& a_fixture);

        std::vector<SharedPtrAny> params(const std::string& a_part, const std::string& a_group, const std::string& a_test) const;

        void params(const std::string& a_part, const std::string& a_group, const std::string& a_test, const std::vector<SharedPtrAny>& a_params);

        template <typename ...TParamPack>
        void appendParam(const std::string& a_partName, const std::string& a_groupName, const std::string& a_testName, TParamPack... a_parameterPack);
        template <typename ...TParamPack>
        void appendParam(const Test& a_a_test, TParamPack... a_parameterPack);
        template <typename ...TParamPack>
        void appendParam(const TestPath& a_testPath, TParamPack... a_parameterPack);

        template <typename ...TValuePack>
        void appendParamValue(const std::string& a_partName, const std::string& a_groupName, const std::string& a_testName, const TValuePack& ...a_parameterPack);
        template <typename ...TValuePack>
        void appendParamValue(const Test& a_test, const TValuePack& ...a_parameterPack);
        template <typename ...TValuePack>
        void appendParamValue(const TestPath& a_testPath, const TValuePack& ...a_parameterPack);

      private:
        typedef std::map<std::string, int> OrderMap;

        struct TestParams {
          std::string                       part;
          std::string                       group;
          std::string                       test;
          mutable std::vector<SharedPtrAny> params;

          bool operator<(const TestParams& a_test) const {
            return part < a_test.part   ? true :
                   part > a_test.part   ? false :
                   group < a_test.group ? true :
                   group > a_test.group ? false :
                   test < a_test.test   ? true :
                   test > a_test.test   ? false :
                                          false;
          }
        };

        bool _suitability(const std::vector<std::string>& a_items, const std::string& a_rule, bool& a_dstSuitability) const;

        template<typename ...TParamPack>
        void _appendParam(const std::string& a_partName, const std::string& a_groupName, const std::string& a_testName, SharedPtrAny a_parameter, const TParamPack& ...a_parameterPack);
        inline void _appendParam(const std::string& a_partName, const std::string& a_groupName, const std::string& a_testName);

        template <typename TValue, typename ...TValuePack>
        void _appendParamValue(const std::string& a_partName, const std::string& a_groupName, const std::string& a_testName, const TValue& a_parameter, const TValuePack& ...a_parameterPack);
        inline void _appendParamValue(const std::string& a_partName, const std::string& a_groupName, const std::string& a_testName);


        std::vector<Test>         _tests;
        OrderMap                  _partOrders;
        OrderMap                  _groupOrders;
        OrderMap                  _testOrders;
        std::vector<Fixture>      _fixtures;
        std::set<TestParams>      _params;
        mutable std::mutex        _mutex;
    };


  } // NTest namespace
} // fcf namespace


/* ========================================================== */
/* ===                                                    === */
/* ===                 Benchmarking class                 === */
/* ===                                                    === */
/* ========================================================== */

namespace fcf {
  namespace NTest {

    /**
     * @class Duration
     * @brief High-precision timing class for code benchmarking and performance testing.
     *
     * Provides capabilities to measure total accumulated execution time with pause/resume support,
     * calculate average time per iteration, and isolatedly evaluate the duration of the
     * last active time segment (between the last resume/begin and end).
     * Supports dynamic "on-the-fly" metric calculation while the timer is actively running.
     */
    class Duration {
      public:
        /**
         * @brief Constructs a Duration object with a specified number of iterations.
         *
         * Protects division logic from zero errors: if 0 is passed,
         * the iteration count is automatically clamped to 1.
         *
         * @param a_iterations The number of planned executions for the tested code block.
         */
        Duration(unsigned long long a_iterations)
          : _iterations(a_iterations ? a_iterations : 1), _pause(true) {
        }

        /**
         * @brief Default constructor.
         *
         * Initializes the object with a default value of 1 iteration.
         */
        Duration()
          : _iterations(1), _pause(true) {
        }

        /**
         * @brief Returns the configured number of iterations.
         * @return The number of iterations.
         */
        unsigned long long iterationCount() const {
          return _iterations;
        }

        /**
         * @brief Starts the global time measurement.
         *
         * Resets all previously accumulated intervals and captures the current
         * time point for both the global tracker and the local segment.
         */
        void begin() {
          _start     = std::chrono::steady_clock::now();
          _end       = _start;
          _lastStart = _start;
          _lastEnd   = _start;
          _pause     = false;
        }

        /**
         * @brief Stops the current time measurement (pauses the timer).
         *
         * Records the ending timestamp for both the total accumulation and the current segment.
         * If the timer is already paused, subsequent calls are ignored.
         */
        void end() {
          if (_pause) {
            return;
          }
          _end = std::chrono::steady_clock::now();
          _lastEnd = _end;
          _pause = true;
        }

        /**
         * @brief Resumes time measurement after a pause.
         *
         * If called for the very first time (before begin()), it automatically falls back
         * to act as begin() for seamless code integration.
         * On subsequent calls, it mathematically shifts the global before timestamp forward
         * to exclude the paused duration from the total score, and opens a new local segment.
         */
        void resume() {
          if (!_pause) {
            return;
          }
          if (_start == std::chrono::steady_clock::time_point{}) {
            begin();
            return;
          }
          _lastStart = std::chrono::steady_clock::now();
          _start = _lastStart - (_end - _start);
          _pause = false;
        }

        /**
         * @brief Executes a functor multiple times and measures the total execution duration.
         *
         * Automatically triggers begin() before entering the loop and end() immediately after.
         *
         * @tparam TFunctor Type of the callable object (lambda, function pointer, functor).
         * @param a_functor The callable target to be benchmarked.
         */
        template <typename TFunctor>
        void operator()(TFunctor&& a_functor) {
          begin();
          for(unsigned long long i = 0; i < _iterations; ++i) {
            a_functor();
          }
          end();
        }

        /**
         * @brief Returns the total accumulated duration across all iterations (excluding pauses).
         *
         * If the timer is active when invoked, the result is dynamically calculated
         * "on-the-fly" relative to the current time point.
         *
         * @return Total duration in nanoseconds.
         */
        std::chrono::nanoseconds totalDuration() const {
          if (!_pause) {
            auto cur = std::chrono::steady_clock::now();
            return std::chrono::duration_cast<std::chrono::nanoseconds>(cur - _start);
          }
          return std::chrono::duration_cast<std::chrono::nanoseconds>(_end - _start);
        }

        /**
         * @brief Returns the duration of the last active execution segment only.
         *
         * A segment is defined as the time interval between the last resume() (or begin()) and end().
         * If the timer is active when invoked, returns the time elapsed since the current segment started.
         *
         * @return Duration of the last segment in nanoseconds.
         */
        std::chrono::nanoseconds lastTotalDuration() const {
          if (!_pause) {
            auto cur = std::chrono::steady_clock::now();
            return std::chrono::duration_cast<std::chrono::nanoseconds>(cur - _lastStart);
          }
          return std::chrono::duration_cast<std::chrono::nanoseconds>(_lastEnd - _lastStart);
        }

        /**
         * @brief Returns a string representation of the total accumulated duration.
         * @return Formatted string: "SECONDS.MILLIS`MICROS`NANOS".
         */
        std::string totalDurationStr(bool a_friendly = false) const {
          return nsToStr( totalDuration().count(), a_friendly);
        }

        /**
         * @brief Returns a string representation of the last active segment duration.
         * @return Formatted string: "SECONDS.MILLIS`MICROS`NANOS".
         */
        std::string lastTotalDurationStr(bool a_friendly = false) const {
          return nsToStr( lastTotalDuration().count(), a_friendly);
        }

        /**
         * @brief Calculates the average duration of a single iteration based on total time.
         * @return Average duration of one iteration in nanoseconds.
         */
        std::chrono::nanoseconds duration() const {
          return totalDuration() / _iterations;
        }

        /**
         * @brief Calculates the average duration of a single iteration based on the last segment.
         * @return Average duration of one iteration within the last segment in nanoseconds.
         */
        std::chrono::nanoseconds lastDuration() const {
          return lastTotalDuration() / _iterations;
        }

        /**
         * @brief Returns a string representation of the average iteration duration based on total time.
         * @return Formatted string: "SECONDS.MILLIS`MICROS`NANOS".
         */
        std::string durationStr(bool a_friendly = false) const {
          return nsToStr(duration().count(), a_friendly);
        }

        /**
         * @brief Returns a string representation of the average iteration duration based on the last segment.
         * @return Formatted string: "SECONDS.MILLIS`MICROS`NANOS".
         */
        std::string lastDurationStr(bool a_friendly = false) const {
          return nsToStr( lastDuration().count(), a_friendly);
        }

        /**
         * @brief Formats raw nanoseconds into a custom string representation.
         *
         * Converts raw nanosecond counts into a human-readable format separated by backticks.
         * Const-correctness guarantees safe execution within the class's const getters.
         *
         * @param a_ns The total number of nanoseconds to format.
         * @return Formatted string matching "SEC.MIL`MICRO`NS".
         */
        static std::string nsToStr(unsigned long long a_ns, bool a_friendly) {
          std::stringstream ss;
          if (a_friendly) {
            ss << (a_ns / 1000000000) << '.'
               << std::setfill('0') << std::setw(3) << ((a_ns / 1000000) % 1000) << '`'
               << std::setw(3)                      << ((a_ns / 1000) % 1000) << '`'
               << std::setw(3)                      << (a_ns % 1000);
          } else {
            ss << (a_ns / 1000000000) << '.'
               << std::setfill('0') << std::setw(9) << (a_ns % 1000000000);
          }
          return ss.str();
        }

      private:
        unsigned long long                    _iterations;  ///< Total number of iterations for test execution and average metrics calculation.
        bool                                  _pause;       ///< State flag: true if the timer is paused/stopped, false if actively running.
        std::chrono::steady_clock::time_point _start;       ///< Modifiable starting time point of the global (accumulated) tracker.
        std::chrono::steady_clock::time_point _end;         ///< Ending time point of the global (accumulated) tracker.
        std::chrono::steady_clock::time_point _lastStart;   ///< Immutable starting time point of the last opened interval segment.
        std::chrono::steady_clock::time_point _lastEnd;     ///< Ending time point of the last completed interval segment.
    };

  } // NTest namespace
} // fcf namespace



/* ========================================================== */
/* ===                                                    === */
/* ===         Helper types for storing user data         === */
/* ===                                                    === */
/* ========================================================== */

namespace fcf {
  namespace NTest {


    /**
     * @brief A type-erased smart pointer that manages the lifetime of an object of any type.
     *
     * SharedPtrAny provides a way to store and manage objects of different types
     * within a single container, using reference counting to ensure proper
     * memory management. It is designed to be used for storing arbitrary
     * user data within the testing framework.
     */
    class FCF_TEST_API SharedPtrAny {
      private:
        struct ControlBlockBase {
          std::atomic<int> refCount;

          ControlBlockBase();
          virtual ~ControlBlockBase() = default;

          virtual void* ptr() = 0;
          virtual const std::type_info& type() const noexcept = 0;
        };

        template <typename Ty>
        struct ControlBlockDerived : public ControlBlockBase {
          Ty data;

          template <typename... ArgPack>
          ControlBlockDerived(ArgPack&&... args);

          void* ptr() override;

          const std::type_info& type() const noexcept override;
        };
      public:
        /**
         * @brief Creates a new SharedPtrAny managing an object of type Ty.
         *
         * This is the equivalent of std::make_shared for arbitrary types.
         *
         * @tparam Ty The type of the object to be managed.
         * @tparam ArgPack The types of the arguments to pass to the constructor of Ty.
         * @param a_args The arguments to be forwarded to the constructor of Ty.
         * @return A new SharedPtrAny instance.
         */
        template <typename Ty, typename... ArgPack>
        static SharedPtrAny make(ArgPack&&... a_args);

        /**
         * @brief Default constructor.
         * Initializes an empty SharedPtrAny.
         */
        SharedPtrAny() noexcept;

        /**
         * @brief Nullptr constructor.
         * @param a_source std::nullptr_t.
         */
        SharedPtrAny(std::nullptr_t) noexcept;

        /**
         * @brief Copy constructor.
         * Increments the reference count of the managed object.
         * @param a_source The source SharedPtrAny instance.
         */
        SharedPtrAny(const SharedPtrAny& a_source) noexcept;

        /**
         * @brief Move constructor.
         * Transfers ownership from the source instance without changing the reference count.
         * @param a_source The source SharedPtrAny instance.
         */
        SharedPtrAny(SharedPtrAny&& a_source) noexcept;

        /**
         * @brief Destructor.
         * Decrements the reference count and deletes the object if it reaches zero.
         */
        ~SharedPtrAny();

        /**
         * @brief Copy assignment operator.
         * @param a_source The source SharedPtrAny instance.
         * @return Reference to this instance.
         */
        SharedPtrAny& operator=(const SharedPtrAny& a_source) noexcept;

        /**
         * @brief Move assignment operator.
         * @param a_source The source SharedPtrAny instance.
         * @return Reference to this instance.
         */
        SharedPtrAny& operator=(SharedPtrAny&& a_source) noexcept;

        /**
         * @brief Attempts to cast the managed object to a specific type.
         *
         * @throw std::bad_cast If the conversion failed
         * @tparam Ty The target type to cast to.
         * @return A pointer to the managed object if the type matches, otherwise nullptr.
         */
        template <typename Ty>
        Ty* cast();

        /**
         * @brief Check a stored type
         *
         * @return Returns true if a stored type equal to Ty type
         */
        template <typename Ty>
        bool is() const noexcept;

        /**
         * @brief Gets the current reference count of the managed object.
         * @return The number of SharedPtrAny instances pointing to the same object.
         */
        int count() const noexcept;

        /**
         * @brief Checks if the SharedPtrAny is not empty.
         * @return true if it manages an object, false otherwise.
         */
        explicit operator bool() const noexcept;

        /**
         * @brief Releases ownership of the managed object.
         * Decrements the reference count.
         */
        void release();

      private:
        explicit SharedPtrAny(ControlBlockBase* a_block) noexcept;
        ControlBlockBase* _block;
    };

  } // NTest namespace
} // fcf namespace



/* ========================================================== */
/* ===                                                    === */
/* ===                    Testing status                  === */
/* ===                                                    === */
/* ========================================================== */

namespace fcf {
  namespace NTest {

    class SharedPtrAny;

    namespace NDetails {
      FCF_TEST_API void runImpl(const Options& a_options, bool a_enableThrow, bool* a_errorPtr);
    } // NDetails namespace

    /**
     * @brief Central repository for the current execution state of the test runner.
     *
     * The State class maintains real-time information about the ongoing test execution,
     * including the currently active test, the set of all tests being run,
     * accumulated duration, recorded errors, and a thread-safe storage for
     * arbitrary user-defined data.
     */
    class FCF_TEST_API State {
        friend void NDetails::runImpl(const Options& a_options, bool a_enableThrow, bool* a_errorPtr);
        friend class Storage;

      public:
        State();

        /**
         * @brief Gets the currently active test.
         * @return The Test object representing the test currently being executed.
         */
        Test                      test() const;

        /**
         * @brief Sets the currently active test.
         * @param a_test The Test object to set as active.
         */
        void                      test(const Test& a_test);

        /**
         * @brief Gets the set of all tests currently in the execution queue.
         * @return A set containing all registered Test objects.
         */
        std::set<Test>            tests() const;

        /**
         * @brief Sets the set of all tests in the execution queue.
         * @param a_tests The set of Test objects to set.
         */
        void                      tests(const std::set<Test>& a_tests);

        /**
         * @brief Gets the total number of tests in the current execution.
         * @return The count of tests.
         */
        size_t                    testCount() const;

        /**
         * @brief Gets the total accumulated duration of the test execution.
         * @return The Duration object containing timing information.
         */
        Duration                  duration() const;

        /**
         * @brief Sets the total accumulated duration.
         * @param a_duration The Duration object to set.
         */
        void                      duration(const Duration& a_duration);

        /**
         * @brief Records an error message.
         * @param a_error The error message string.
         * @param a_ignoreExists If true, the error will not be added if it is already present in the list.
         */
        void                      error(const char* a_error, bool a_ignoreExists);

        /**
         * @brief Gets the list of all recorded error messages.
         * @return A list of error message strings.
         */
        std::vector<std::string>  errors() const;

        /**
         * @brief Sets the list of recorded error messages.
         * @param a_errors The list of error message strings to set.
         */
        void                      errors(const std::vector<std::string>& a_errors);

        /**
         * @brief Stores arbitrary user data associated with a key.
         * @param a_key The unique identifier for the data.
         * @param a_data The SharedPtrAny object containing the data.
         */
        void                      data(const char* a_key, SharedPtrAny a_data);

        /**
         * @brief Retrieves arbitrary user data associated with a key.
         * @param a_key The unique identifier for the data.
         * @return The SharedPtrAny object containing the data, or an empty object if not found.
         */
        SharedPtrAny              data(const char* a_key);

        /**
         * @brief Removes user data associated with a specific key.
         * @param a_key The unique identifier of the data to erase.
         */
        void                      eraseData(const char* a_key);

        /**
         * @brief Gets all keys currently present in the user data storage.
         * @return A set of strings containing all data keys.
         */
        std::set<std::string>     dataKeys() const;

        SharedPtrAny              param();

        size_t                    paramIndex() const;

        bool                      active() const;

      private:
        void                                _setParam(SharedPtrAny a_parameter);
        void                                _setParamIndex(size_t a_paramIndex);
        void                                _setActive(bool a_active);

        void                                _resumeDuration();
        void                                _endDuration();
        Test                                _test;
        std::set<Test>                      _tests;
        Duration                            _duration;
        std::vector<std::string>            _errors;
        mutable std::mutex                  _mutex;
        std::map<std::string, SharedPtrAny> _data;
        SharedPtrAny                        _parameter;
        size_t                              _paramIndex;
        bool                                _active;
    };

    /**
     * @brief Provides access to the global singleton instance of the State class.
     *
     * This function returns a reference to the central repository that manages
     * the current execution state, including test results, errors, and user data.
     * The instance is lazily initialized upon the first call.
     *
     * @return A reference to the global State instance.
     */
    FCF_TEST_API State& state();

  } // NTest namespace
} // fcf namespace


/* ========================================================== */
/* ===                                                    === */
/* ===               Logging and formatting               === */
/* ===                                                    === */
/* ========================================================== */

class LogPrefixTester;

namespace fcf {
  namespace NTest {

    /**
     * @class Logger
     * @brief Thread-safe logging system for the fcfTest framework.
     *
     * The Logger provides a flexible way to output messages at various severity levels
     * (Fatal, Error, Warning, Attention, Log, Info, Debug, Trace). It supports:
     * - **Multiple Output Targets**: Simultaneously log to multiple streams (e.g., std::cout, files).
     * - **Custom Formatting**: Apply different formatting rules (like JUnit XML) to specific targets.
     * - **Dynamic Prefixes**: Use static or functional prefixes to add context (like timestamps
     *   or log levels) to every message.
     * - **Category Filtering**: Categorize messages using bitmask-based categories for granular control.
     * - **Thread Safety**: Uses recursive mutexes to ensure safe logging from multithreaded tests.
     *
     * The Logger uses a `Writer` pattern to allow for fluent, chained syntax:
     * @code
     * fcf::NTest::log() << "Message with level: " << level << std::endl;
     * @endcode
     */
    class FCF_TEST_API Logger {
        friend void NDetails::runImpl(const Options& a_options, bool a_enableThrow, bool* a_errorPtr);
        friend class ::LogPrefixTester;
      public:
        struct MessageContext;

        /**
         * @brief Function type for generating dynamic log prefixes.
         * @param a_logger Reference to the logger instance.
         * @param a_context The context of the current message.
         * @return A string representing the generated prefix.
         */
        typedef std::function<std::string(Logger&, MessageContext&)> PrefixFunction;

        /**
         * @brief Function type for handling log message formatting.
         * @param a_logger Reference to the logger instance.
         * @param a_context The context of the current message.
         */
        typedef std::function<void(Logger&, MessageContext&)>        FormatFunction;

        struct Format;

        /**
         * @brief Collection of log formats.
         */
        typedef std::vector<Format> Formats;

        struct Prefix;

        /**
         * @brief Collection of log prefixes.
         */
        typedef std::vector<Prefix> Prefixes;

        /**
         * @brief Map used to store data associated with handlers.
         */
        typedef std::map<std::string, SharedPtrAny> HandlerDataMap;

        struct OutputTarget;

        /**
         * @brief Collection of output targets.
         */
        typedef std::vector<OutputTarget> OutputTargets;

        /**
         * @brief Function type for creating data for targets or handlers.
         * @param a_logger Reference to the logger instance.
         * @param a_target The target being processed.
         * @return A SharedPtrAny containing the created data.
         */
        typedef std::function<SharedPtrAny(Logger&, OutputTarget&)>  DataFactory;

        class FCF_TEST_API Writer;

        /**
         * @brief Default constructor for the Logger.
         */
        Logger();

        /**
         * @brief Returns a Writer for fatal level messages.
         * @param a_category The message category.
         * @return A Writer object for fatal logs.
         */
        Writer ftl(unsigned int a_category = LMC_USER_GROUP);

        /**
         * @brief Returns a Writer for error level messages.
         * @param a_category The message category.
         * @return A Writer object for error logs.
         */
        Writer err(unsigned int a_category = LMC_USER_GROUP);

        /**
         * @brief Returns a Writer for warning level messages.
         * @param a_category The message category.
         * @return A Writer object for warning logs.
         */
        Writer wrn(unsigned int a_category = LMC_USER_GROUP);

        /**
         * @brief Returns a Writer for attention level messages.
         * @param a_category The message category.
         * @return A Writer object for attention logs.
         */
        Writer att(unsigned int a_category = LMC_USER_GROUP);

        /**
         * @brief Returns a Writer for standard log level messages.
         * @param a_category The message category.
         * @return A Writer object for log messages.
         */
        Writer log(unsigned int a_category = LMC_USER_GROUP);

        /**
         * @brief Returns a Writer for information level messages.
         * @param a_category The message category.
         * @return A Writer object for info logs.
         */
        Writer inf(unsigned int a_category = LMC_USER_GROUP);

        /**
         * @brief Returns a Writer for debug level messages.
         * @param a_category The message category.
         * @return A Writer object for debug logs.
         */
        Writer dbg(unsigned int a_category = LMC_USER_GROUP);

        /**
         * @brief Returns a Writer for trace level messages.
         * @param a_category The message category.
         * @return A Writer object for trace logs.
         */
        Writer trc(unsigned int a_category = LMC_USER_GROUP);

        /**
         * @brief Gets the current log level as a string.
         * @return A constant pointer to the level string.
         */
        const char* levelStr() const;

        /**
         * @brief Sets the current log level from a string.
         * @param a_level The level string.
         */
        void levelStr(const char* a_level);

        /**
         * @brief Gets the current log level.
         * @return The current ELogLevel.
         */
        ELogLevel level() const;

        /**
         * @brief Sets the current log level.
         * @param a_level The new ELogLevel.
         */
        void level(ELogLevel a_level);

        /**
         * @brief Converts a string to an ELogLevel.
         * @param a_level The input string.
         * @param a_default The default level if conversion fails.
         * @return The converted ELogLevel.
         */
        static ELogLevel toLevel(std::string a_level, ELogLevel a_default = LL_LOG);

        /**
         * @brief Converts an ELogLevel to a string.
         * @param a_level The level to convert.
         * @return A constant pointer to the level string.
         */
        static const char* toLevelStr(ELogLevel a_level);

        /**
         * @brief Gets the current list of prefixes.
         * @return A list of prefixes.
         */
        Prefixes prefixes() const;

        /**
         * @brief Replaces all current prefixes with the provided list.
         * @param a_prefixes The new list of prefixes.
         */
        void prefixes(const Prefixes& a_prefixes);

        /**
         * @brief Removes all current prefixes.
         * @param a_defaultState If true, applies default prefixes.
         */
        void clearPrefixes(bool a_defaultState = false);

        /**
         * @brief Adds a new prefix to the current list.
         * @param a_prefix The prefix to append.
         */
        void appendPrefix(const Prefix& a_prefix);

        /**
         * @brief Gets the current list of formats.
         * @return A list of formats.
         */
        Formats formats() const;

        /**
         * @brief Replaces all current formats with the provided list.
         * @param a_formats The new list of formats.
         */
        void formats(Formats& a_formats);

        /**
         * @brief Clears all current formats.
         * @param a_defaultState If true, applies default formats.
         */
        void clearFormats(bool a_defaultState = false);

        /**
         * @brief Adds a new format to the current list.
         * @param a_format The format to append.
         */
        void appendFormat(const Format& a_format);

        /**
         * @brief Gets the current list of output targets.
         * @return A list of output targets.
         */
        OutputTargets targets() const;

        /**
         * @brief Replaces all current targets with the provided list.
         * @param a_targets The new list of targets.
         */
        void targets(const OutputTargets& a_targets);

        /**
         * @brief Clears all current targets.
         * @param a_defaultState If true, applies default targets.
         */
        void clearTargets(bool a_defaultState = false);

        /**
         * @brief Adds a new output target to the current list.
         * @param a_stream The target to append.
         */
        void appendTarget(const OutputTarget& a_stream);

        /**
         * @brief A thread-safe wrapper for writing log messages to a stream.
         */
        class FCF_TEST_API Writer {
          public:
            /**
             * @brief Default constructor for Writer.
             */
            Writer();

            /**
             * @brief Deleted copy constructor to prevent accidental copies.
             */
            Writer(const Writer& a_output)  = delete;

            /**
             * @brief Move constructor for Writer.
             */
            Writer(Writer&& a_output);

            /**
             * @brief Parameterized constructor for Writer.
             * @param a_logger Reference to the parent logger.
             * @param a_level The log level for this writer.
             * @param a_loggerMessageCategory The category for this writer.
             */
            Writer(Logger& a_logger, ELogLevel a_level, unsigned int a_loggerMessageCategory);

            /**
             * @brief Destructor for Writer. Triggers the actual write operation.
             */
            ~Writer();

            /**
             * @brief Appends a value to the log stream.
             * @tparam Ty Type of the value.
             * @param a_value The value to append.
             * @return Reference to the Writer for chaining.
             */
            template <typename Ty>
            Writer& operator<<(const Ty& a_value);

            /**
             * @brief Appends a stream manipulator to the log stream.
             * @param a_manipulator The manipulator function.
             * @return Reference to the Writer for chaining.
             */
            Writer& operator<<(std::ostream& (*a_manipulator)(std::ostream&));

          private:
            Logger*             _logger;
            ELogLevel           _level;
            unsigned int _loggerMessageCategory;
            std::stringstream   _sstream;
        };

        /**
         * @brief Contextual information for a single log message.
         */
        struct MessageContext {
          unsigned int        category;
          bool                system;
          const std::string   origin;
          std::string         message;
          size_t              line;
          ELogLevel           level;
          std::ostream*       stream;
          SharedPtrAny*       data;

          MessageContext() = delete;
          MessageContext(const MessageContext&) = delete;
          MessageContext& operator=(const MessageContext&) = delete;

          /**
           * @brief Constructs a context with an origin message.
           * @param a_message The origin message string.
           */
          MessageContext(const std::string& a_message)
            : origin(a_message) {
          }
        };

        /**
         * @brief Configuration for a log format.
         */
        struct Format {
          std::string     name;
          DataFactory     dataFactory;
          FormatFunction  handler;

          Format()
            : dataFactory(nullptr)
            , handler(nullptr)
          {}

          Format(std::string a_name, DataFactory a_dataFactory, FormatFunction  a_handler)
            : name(a_name)
            , dataFactory(a_dataFactory)
            , handler(a_handler)
          {}
        };

        /**
         * @brief Configuration for a log prefix.
         */
        struct Prefix {
          std::string            name;
          bool                   multiLine;
          unsigned int           category;
          DataFactory            dataFactory;
          std::string            prefix;
          Logger::PrefixFunction handler;

          /**
           * @brief Default constructor for Prefix.
           */
          Prefix()
            : multiLine(false)
            , category(LMC_ALL)
          { }

          /**
           * @brief Parameterized constructor for Prefix.
           * @param a_name Name of the prefix.
           * @param a_multiLine Whether the prefix applies to multiple lines.
           * @param a_category The category this prefix applies to.
           * @param a_dataFactory Factory for prefix-specific data.
           * @param a_prefix The actual prefix string.
           * @param a_handler The handler function for dynamic prefixes.
           */
          Prefix(const std::string& a_name,
                 bool a_multiLine,
                 unsigned int a_category,
                 const DataFactory& a_dataFactory,
                 std::string a_prefix,
                 Logger::PrefixFunction a_handler)
            : name(a_name)
            , multiLine(a_multiLine)
            , category(a_category)
            , dataFactory(a_dataFactory)
            , prefix(a_prefix)
            , handler(a_handler)
          { }

        };

        /**
         * @brief Configuration for a log output target.
         */
        struct OutputTarget {
          std::string     name;
          std::ostream*   stream;
          std::string     format;
          HandlerDataMap  prefixData;
          HandlerDataMap  formatData;

          /**
           * @brief Default constructor for OutputTarget.
           */
          OutputTarget()
            : stream(nullptr)
          {}

          /**
           * @brief Parameterized constructor for OutputTarget.
           * @param a_name Name of the target.
           * @param a_stream Pointer to the output stream.
           * @param a_format Format name for this target.
           * @param a_prefixData Data map for prefixes.
           * @param a_formatData Data map for formats.
           */
          OutputTarget(const std::string& a_name,
                       std::ostream* a_stream,
                       const std::string& a_format,
                       const HandlerDataMap&  a_prefixData,
                       const HandlerDataMap&  a_formatData)
            : name(a_name)
            , stream(a_stream)
            , format(a_format)
            , prefixData(a_prefixData)
            , formatData(a_formatData)
          {
          }
        };

      protected:

        struct Environment {
          ELogLevel        level;
          std::string      format;
          OutputTargets    targets;
        };

        static void _appendTarget(const OutputTarget& a_stream, Environment& a_environment );

        Environment _getEnvironment();

        void _setEnvironment(const Environment& a_environment);

        void _write(fcf::NTest::ELogLevel a_level, unsigned int a_messageCategory, const std::string& a_message);

        Writer _log(ELogLevel a_level, unsigned int a_messageCategory);

        Environment           _environment;
        Prefixes              _prefixes;
        Formats               _formats;
        mutable std::recursive_mutex  _mutex;
        bool                  _newLine;
    };





    class FCF_TEST_API LogJunitFormatter {
      public:
        static void format(Logger& a_logger, Logger::MessageContext& a_messageContext);
        static std::string suiteName(const Test& a_test);
        static std::string xmlAttribute(const std::string& a_string);
        static std::string xmlText(const std::string& a_string);

      private:
        struct ProcessedInfo {
          bool                error;
          std::string         message;
          unsigned long long  duration;
        };
        std::map<Test, ProcessedInfo> _processed;
    };

    /**
    * @brief Declaration for the global logger instance.
    * @return Reference to the Logger instance.
    */
    FCF_TEST_API Logger& logger();

    /**
     * @brief Returns the output stream for fatal messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline Logger::Writer ftl(unsigned int a_category = LMC_USER_GROUP);

    /**
     * @brief Returns the output stream for error messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline Logger::Writer err(unsigned int a_category = LMC_USER_GROUP);

    /**
     * @brief Returns the output stream for warning messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline Logger::Writer wrn(unsigned int a_category = LMC_USER_GROUP);

    /**
     * @brief Returns the output stream for attention messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline Logger::Writer att(unsigned int a_category = LMC_USER_GROUP);

    /**
     * @brief Returns the output stream for log messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline Logger::Writer log(unsigned int a_category = LMC_USER_GROUP);

    /**
     * @brief Returns the output stream for informational messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline Logger::Writer inf(unsigned int a_category = LMC_USER_GROUP);

    /**
     * @brief Returns the output stream for debug messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline Logger::Writer dbg(unsigned int a_category = LMC_USER_GROUP);

    /**
     * @brief Returns the output stream for trace messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline Logger::Writer trc(unsigned int a_category = LMC_USER_GROUP);


  } // NTest namespace
} // fcf namespace



/* ========================================================== */
/* ===                                                    === */
/* ===           Auxiliary elements from NDetails         === */
/* ===                                                    === */
/* ===               Logging and formatting               === */
/* ===                                                    === */
/* ========================================================== */

namespace fcf {
  namespace NTest {
    namespace NDetails {

      struct Registrator {
        Registrator(const Test& a_test) {
          storage().appendTest(a_test);
        }
      };

      struct PartOrderRegistrator {
        PartOrderRegistrator(const char* a_name, int a_order) {
          storage().partOrder(a_name, a_order);
        }
      };

      struct GroupOrderRegistrator {
        GroupOrderRegistrator(const char* a_name, int a_order) {
          storage().groupOrder(a_name, a_order);
        }
      };

      struct TestOrderRegistrator {
        TestOrderRegistrator(const char* a_name, int a_order) {
          storage().testOrder(a_name, a_order);
        }
      };

    } // NDetails namespace
  } // NTest namespace
} // fcf namespace



/* ========================================================== */
/* ===                                                    === */
/* ===                   Implementation                   === */
/* ===                                                    === */
/* ===           Basic functions of unit testing          === */
/* ===                                                    === */
/* ========================================================== */

namespace fcf {
  namespace NTest {

    namespace NDetails {
      FCF_TEST_API ECmdMode cmdRunImpl(Options& a_dstOptions, int a_argc, const char* const* a_argv, ECmdRunMode a_runMode, bool a_enableThrow, bool* a_errorPtr);
    } // NDetails namespace

    #ifdef FCF_TEST_IMPLEMENTATION
      FCF_TEST_API void cmdHelp() {
        std::cout << "Test options:" << std::endl;
        std::cout << "  --test-run  - Run tests" << std::endl;
        std::cout << "  --test-list - Displays a list of all tests" << std::endl;
        std::cout << "  --test-part  PART_NAME - Run only tests from the part. The parameter can be used multiple times" << std::endl;
        std::cout << "  --test-group GROUP_NAME - Run only tests from the group. The parameter can be used multiple times" << std::endl;
        std::cout << "  --test-test  TEST_NAME - Run only this test. The parameter can be used multiple times" << std::endl;
        std::cout << "  --test-select PART GROUP TEST - Runs only tests that satisfy the selector specified by the three parameters." << std::endl
                  << "                                  If a parameter is an empty string or '*', it is assumed that " << std::endl
                  << "                                  the selector selects all elements from the group." << std::endl
                  << "                                  Multiple values can be provided in a parameter, separated by the '|' symbol." << std::endl
                  << "                                    Example: test --test-select Library \"\" \"func2|func2\"" << std::endl
                  << "                                  The parameter can be used multiple times" << std::endl;
        std::cout << "  --test-ignore-part PART_NAME - Exclude tests in the specified part(s). The parameter can be used multiple times" << std::endl;
        std::cout << "  --test-ignore-group GROUP_NAME - Exclude tests in the specified group(s). The parameter can be used multiple times" << std::endl;
        std::cout << "  --test-ignore-test TEST_NAME - Exclude the specified test(s). The parameter can be used multiple times" << std::endl;
        std::cout << "  --test-ignore-select PART GROUP TEST - Exclude tests that satisfy the selector specified by the three parameters." << std::endl
                  << "                                  If a parameter is an empty string or '*', it is assumed that " << std::endl
                  << "                                  the selector selects all elements from the group." << std::endl
                  << "                                  Multiple values can be provided in a parameter, separated by the '|' symbol." << std::endl
                  << "                                  The parameter can be used multiple times" << std::endl;
        std::cout << "  --test-log-level LEVEL - Logging level (VALUES: def, off, ftl, err, wrn, att, log, inf, dbg, trc, all)" << std::endl;
        std::cout << "  --test-no-break - In case of an error, testing does not stop" << std::endl;
        std::string formats;
        for(auto format : logger().formats()) {
          formats += ", ";
          formats += format.name;
        }
        std::cout << "  --test-format FORMAT - Output format (default" + formats + ")." << std::endl;

        std::cout << "  --test-file FILE_PATH - Log file" << std::endl
                  << "                          Use the default format or specify the --test-format parameter" << std::endl;
        std::cout << "  --test-file-default FILE_PATH - Log file (format: default)." << std::endl;
        for(auto format : logger().formats()) {
          std::cout << "  --test-file-" << format.name << " FILE_PATH - Log file (format: " << format.name << ")." << std::endl;
        }
        std::cout << "  --test-help  - Help message" << std::endl;
        std::cout << std::endl;
        std::cout << "Explanatory details:" << std::endl;
        std::cout << "  1. The --test-part, --test-group, --test-test, --test-select commands are combined using the OR operation" << std::endl;
        std::cout << "  2. The --test-ignore-part, --test-ignore-group, --test-ignore-test, --test-ignore-select commands" << std::endl
                  << "     are combined using the OR operation" << std::endl;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      FCF_TEST_API void cmdList() {
        Options options;
        std::set<Test> tests = storage().selectTests(options);
        std::cout << "List of tests:" << std::endl;
        for(const Test& test : tests) {
          std::cout << "  \"" << test.part << "\" -> \"" << test.group << "\" -> \"" << test.test  << "\""<< std::endl;
        }
      }
    #endif

    /**
     * @brief Parses command line arguments and executes the appropriate action.
     *
     * @param a_dstOptions Reference to the options structure to populate with parsed arguments.
     * @param a_argc Number of command line arguments.
     * @param a_argv Array of command line arguments.
     * @param a_runMode Current mode of execution (parse, execute, or run).
     )* @param a_errorPtr (default = (bool*)0) A pointer to a variable that receives information about a test error.
     *                                        If an error occurs, the value is set to true.
     *                                        If a null pointer is passed, the function throws an exception.
     * @return The determined command mode after processing.
     */
    inline ECmdMode cmdRun(Options& a_dstOptions, int a_argc, const char* const* a_argv, ECmdRunMode a_runMode, bool* a_errorPtr) {
      return NDetails::cmdRunImpl(a_dstOptions, a_argc, a_argv, a_runMode, false, a_errorPtr);
    }

    inline ECmdMode cmdRun(Options& a_dstOptions, int a_argc, const char* const* a_argv, ECmdRunMode a_runMode) {
      return NDetails::cmdRunImpl(a_dstOptions, a_argc, a_argv, a_runMode, true, nullptr);
    }

    template <typename Ty>
    inline ECmdMode cmdRun(Options& a_dstOptions, int a_argc, Ty a_argv, ECmdRunMode a_runMode, bool* a_errorPtr) {
      return cmdRun(a_dstOptions, a_argc, (const char* const*)a_argv, a_runMode, a_errorPtr);
    }

    template <typename Ty>
    inline ECmdMode cmdRun(Options& a_dstOptions, int a_argc, Ty a_argv, ECmdRunMode a_runMode) {
      return cmdRun(a_dstOptions, a_argc, (const char* const*)a_argv, a_runMode);
    }

    template <typename Ty>
    inline ECmdMode cmdRun(int a_argc, Ty a_argv, ECmdRunMode a_runMode, bool* a_errorPtr) {
      Options options;
      return cmdRun(options, a_argc, (const char* const*)a_argv, a_runMode, a_errorPtr);
    }

    template <typename Ty>
    inline ECmdMode cmdRun(int a_argc, Ty a_argv, ECmdRunMode a_runMode) {
      Options options;
      return cmdRun(options, a_argc, (const char* const*)a_argv, a_runMode);
    }


    inline void run(const Options& a_options, bool* a_errorPtr) {
      NDetails::runImpl(a_options, false, a_errorPtr);
    }

    inline void run(const Options& a_options) {
      NDetails::runImpl(a_options, true, 0);
    }

    #ifdef FCF_TEST_IMPLEMENTATION
      FCF_TEST_API Storage& storage() {
        static Storage* storage = nullptr;
        static std::once_flag flag;

        std::call_once(flag, []() {
          storage = new Storage();
        });

        return *storage;
      }
    #endif


    #ifdef FCF_TEST_IMPLEMENTATION
      Test State::test() const {
        std::lock_guard<std::mutex> lock(_mutex);
        return _test;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void State::test(const Test& a_test){
        std::lock_guard<std::mutex> lock(_mutex);
        _test = a_test;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      std::set<Test> State::tests() const {
        std::lock_guard<std::mutex> lock(_mutex);
        return _tests;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void State::tests(const std::set<Test>& a_tests){
        std::lock_guard<std::mutex> lock(_mutex);
        _tests = a_tests;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      size_t State::testCount() const {
        std::lock_guard<std::mutex> lock(_mutex);
        return _tests.size();
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Duration State::duration() const{
        std::lock_guard<std::mutex> lock(_mutex);
        return _duration;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      State::State()
        : _paramIndex (0)
        , _active(false) {
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void State::duration(const Duration& a_duration){
        std::lock_guard<std::mutex> lock(_mutex);
        _duration = a_duration;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void State::error(const char* a_error, bool a_ignoreExists){
        std::lock_guard<std::mutex> lock(_mutex);
        if (a_ignoreExists) {
          if (std::find(_errors.begin(), _errors.end(), a_error) != _errors.end()){
            return;
          }
        }
        _errors.push_back(a_error);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      std::vector<std::string> State::errors() const{
        std::lock_guard<std::mutex> lock(_mutex);
        return _errors;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void State::errors(const std::vector<std::string>& a_errors){
        std::lock_guard<std::mutex> lock(_mutex);
        _errors = a_errors;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void State::data(const char* a_key, SharedPtrAny a_data) {
        std::lock_guard<std::mutex> lock(_mutex);
        _data[a_key] = a_data;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      SharedPtrAny State::data(const char* a_key) {
        std::lock_guard<std::mutex> lock(_mutex);
        return SharedPtrAny(_data[a_key]);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      std::set<std::string> State::dataKeys() const {
        std::lock_guard<std::mutex> lock(_mutex);
        std::set<std::string> result;
        for(std::map<std::string, SharedPtrAny>::const_iterator it = _data.begin(); it != _data.end(); ++it) {
          result.insert(it->first);
        }
        return result;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      SharedPtrAny State::param(){
        std::lock_guard<std::mutex> lock(_mutex);
        return _parameter;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      size_t State::paramIndex() const {
        return _paramIndex;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      bool State::active() const {
        return _active;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void State::_setParam(SharedPtrAny a_parameter){
        std::lock_guard<std::mutex> lock(_mutex);
        _parameter = a_parameter;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void State::_setParamIndex(size_t a_paramIndex){
        _paramIndex = a_paramIndex;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void State::_setActive(bool a_active){
        _active = a_active;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
     void State::eraseData(const char* a_key){
        std::lock_guard<std::mutex> lock(_mutex);
        _data.erase(a_key);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void State::_resumeDuration(){
        std::lock_guard<std::mutex> lock(_mutex);
        _duration.resume();
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void State::_endDuration(){
        std::lock_guard<std::mutex> lock(_mutex);
        _duration.end();
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      FCF_TEST_API State& state(){
        static State* state = nullptr;
        static std::once_flag flag;

        std::call_once(flag, []() {
          state = new State();
        });

        return *state;
      }
    #endif

    namespace NDetails {
      std::vector<std::string> splitSelector(const std::string& a_str);
    } // NDetails namespace

    namespace NDetails {
      #ifdef FCF_TEST_IMPLEMENTATION

        inline std::vector<std::string> parseArgs(int a_argc, const char* const* a_argv);

        FCF_TEST_API ECmdMode cmdRunImpl(Options& a_dstOptions, int a_argc, const char* const* a_argv, ECmdRunMode a_runMode, bool a_enableThrow, bool* a_errorPtr) {
          ECmdMode mode = CM_NONE;

          try {
            std::vector<std::string> args = NDetails::parseArgs(a_argc, (const char* const*)a_argv);

            for(size_t i = 0; i < args.size(); ++i) {
              if (args[i] == "--test-run") {
                mode = CM_RUN;
              } else if (args[i] == "--test-help") {
                mode = CM_HELP;
                if (a_runMode == CRM_EXECUTE || a_runMode == CRM_RUN) {
                  cmdHelp();
                  return mode;
                }
              } else if (args[i] == "--test-log-level") {
                if ((i+1) < args.size()) {
                  a_dstOptions.logLevel = Logger::toLevel(args[i+1], logger().level());
                  ++i;
                } else {
                  throw std::runtime_error("One parameter are required for the --test-log-level LOG_LEVEL argument");
                }
              } else if (args[i] == "--test-list") {
                mode = CM_LIST;
                if (a_runMode == CRM_EXECUTE || a_runMode == CRM_RUN) {
                  cmdList();
                  return mode;
                }
              } else if (args[i] == "--test-format") {
                if ((i+1) < args.size()) {
                  a_dstOptions.format = args[i+1];
                  ++i;
                } else {
                  throw std::runtime_error("One parameter are required for the --test-format FORMAT argument");
                }
              } else if (args[i] == "--test-part") {
                if ((i+1) < args.size()) {
                  a_dstOptions.selectors.push_back( Options::Selector{{args[i+1]}, {}, {}}  );
                  ++i;
                } else {
                  throw std::runtime_error("One parameter are required for the --test-part PART argument");
                }
              } else if (args[i] == "--test-group") {
                if ((i+1) < args.size()) {
                  a_dstOptions.selectors.push_back( Options::Selector{{}, {args[i+1]}, {}}  );
                  ++i;
                } else {
                  throw std::runtime_error("One parameter are required for the --test-group GROUP argument");
                }
              } else if (args[i] == "--test-test") {
                if ((i+1) < args.size()) {
                  a_dstOptions.selectors.push_back( Options::Selector{{}, {}, {args[i+1]}}  );
                  ++i;
                } else {
                  throw std::runtime_error("One parameter are required for the --test-test TEST argument");
                }
              } else if (args[i] == "--test-select") {
                if ((i+3) < args.size()) {
                  Options::Selector selector{
                    NDetails::splitSelector(args[i+1]),
                    NDetails::splitSelector(args[i+2]),
                    NDetails::splitSelector(args[i+3])
                  };
                  a_dstOptions.selectors.push_back(selector);
                  i += 3;
                } else {
                  throw std::runtime_error("Three parameters are required for the --test-select PART GROUP TEST argument");
                }
              } else if (args[i] == "--test-ignore-part") {
                if ((i+1) < args.size()) {
                  a_dstOptions.ignoreSelectors.push_back( Options::Selector{{args[i+1]}, {}, {}}  );
                  ++i;
                } else {
                  throw std::runtime_error("One parameter are required for the --test-ignore-part PART argument");
                }
              } else if (args[i] == "--test-ignore-group") {
                if ((i+1) < args.size()) {
                  a_dstOptions.ignoreSelectors.push_back( Options::Selector{{}, {args[i+1]}, {}}  );
                  ++i;
                } else {
                  throw std::runtime_error("One parameter are required for the --test-ignore-group GROUP argument");
                }
              } else if (args[i] == "--test-ignore-test") {
                if ((i+1) < args.size()) {
                  a_dstOptions.ignoreSelectors.push_back( Options::Selector{{}, {}, {args[i+1]}}  );
                  ++i;
                } else {
                  throw std::runtime_error("One parameter are required for the --test-ignore-test TEST argument");
                }
              } else if (args[i] == "--test-ignore-select") {
                if ((i+3) < args.size()) {
                  Options::Selector selector{
                    NDetails::splitSelector(args[i+1]),
                    NDetails::splitSelector(args[i+2]),
                    NDetails::splitSelector(args[i+3])
                  };
                  a_dstOptions.ignoreSelectors.push_back(selector);
                  i += 3;
                } else {
                  throw std::runtime_error("Three parameters are required for the --test-ignore-select PART GROUP TEST argument");
                }
              } else if (args[i] == "--test-no-break") {
                a_dstOptions.noBreak = true;
              } else if (args[i] == "--test-file") {
                if ((i+1) < args.size()) {
                  a_dstOptions.files.push_back({args[i+1], ""});
                  ++i;
                } else {
                  throw std::runtime_error("One parameter are required for the --test-file FILE  argument");
                }
              } else if (args[i] == "--test-file-default") {
                if ((i+1) < args.size()) {
                  a_dstOptions.files.push_back({args[i+1], "default"});
                  ++i;
                } else {
                  throw std::runtime_error("One parameter are required for the --test-file-default FILE  argument");
                }
              }
              for(auto format : logger().formats()) {
                std::string param = "--test-file-" + format.name;
                if (args[i] == param) {
                  if ((i+1) < args.size()) {
                    a_dstOptions.files.push_back({args[i+1], format.name});
                    ++i;
                    break;
                  } else {
                    throw std::runtime_error("One parameter are required for the --test-file-FORMAT FILE  argument");
                  }
                }
              }
            }
          } catch (const std::exception& e) {
            log(LMC_ROOT_RUN_ERROR) << "Error: " << e.what() << std::endl;
            if (a_enableThrow) {
              throw;
            } else if (a_errorPtr){
              *a_errorPtr = true;
            }
            return CM_NONE;
          }

          if ((mode == CM_RUN && a_runMode == CRM_EXECUTE) || a_runMode == CRM_RUN) {
            runImpl(a_dstOptions, a_enableThrow, a_errorPtr);
          }

          return mode;
        }
      #endif
    } // NDetails namespace

    namespace NDetails {
      #ifdef FCF_TEST_IMPLEMENTATION
        std::vector<std::string> splitSelector(const std::string& a_str) {
          std::vector<std::string> result;
          std::string line;
          std::stringstream tokenStream(a_str);
          while (std::getline(tokenStream, line, '|')) {
            result.push_back(line.length() ? line : std::string("*"));
          }
          return result;
        }
      #endif
    } // NDetails namespace

    namespace NDetails {
      #ifdef FCF_TEST_IMPLEMENTATION
        class FixtureHandler {
          private:
            struct FixtureError {
              std::string message;
              size_t      index;
              size_t      count;
            };

            struct FixtureInfo {
              Fixture                                 fixture;
              mutable int                             active;
              mutable std::shared_ptr< int >          close;
              mutable std::shared_ptr< FixtureError > errors;
            };

            struct FixtureTestGraph {
              std::vector<FixtureInfo> fixtures;
            };

            struct FixtureGroupGraph {
              std::map<std::string, FixtureTestGraph> tests;
              std::vector<FixtureInfo>                fixtures;
            };

            struct FixturePartGraph {
              std::map<std::string, FixtureGroupGraph> groups;
              std::vector<FixtureInfo>                 fixtures;
            };

            struct FixtureGraph {
              std::map<std::string, FixturePartGraph> parts;
              std::vector<FixtureInfo>                fixtures;
            };

          public:
            struct Error {
              std::string   message;
              std::string   file;
              unsigned int  line;
              size_t        count;
              size_t        index;
            };
            typedef std::vector<Error> Errors;

            FixtureHandler()
              : _graph(_build())
              , _errorCounter(0) {
            }

            void start(const std::set<Test>& a_tests) {
              for(std::set<Test>::const_iterator it = a_tests.begin(); it != a_tests.end(); ++it) {
                _call(std::set<Test>::const_iterator(), std::set<Test>::const_iterator(), std::set<Test>::const_iterator(), it->part, it->group, it->test, true, true);
              }
            }

            void end(const std::set<Test>& a_tests) {
              for(std::set<Test>::const_iterator it = a_tests.begin(); it != a_tests.end(); ++it) {
                _call(std::set<Test>::const_iterator(), std::set<Test>::const_iterator(), std::set<Test>::const_iterator(), it->part, it->group, it->test, false, true);
              }
            }

            void call(std::set<Test>::const_iterator a_beginIt, std::set<Test>::const_iterator a_currentIt, std::set<Test>::const_iterator a_endIt,
                      const std::string& a_part, const std::string& a_group, const std::string& a_test,
                      bool a_before) {
              _call(a_beginIt, a_currentIt, a_endIt, a_part, a_group, a_test, a_before, false);
            }

            Errors errors(const Test& a_test) {
              Errors errors;

              _fillErrors(_graph.fixtures, errors);

              std::map<std::string, FixturePartGraph>::iterator pitv[2] = {_graph.parts.find(a_test.part), _graph.parts.find("*")};
              for(size_t i = 0; i < 2; ++i) {
                if (pitv[i] == _graph.parts.end()) {
                  continue;
                }

                _fillErrors(pitv[i]->second.fixtures, errors);

                std::map<std::string, FixtureGroupGraph>::iterator gitv[2] = { pitv[i]->second.groups.find(a_test.group), pitv[i]->second.groups.find("*")};

                for(size_t j = 0; j < 2; ++j) {
                  if (gitv[j] == pitv[i]->second.groups.end()) {
                    continue;
                  }

                  _fillErrors(gitv[j]->second.fixtures, errors);

                  std::map<std::string, FixtureTestGraph>::iterator titv[2] = { gitv[j]->second.tests.find(a_test.test), gitv[j]->second.tests.find("*")};

                  for(size_t k = 0; k < 2; ++k) {
                    if (titv[k] == gitv[j]->second.tests.end()) {
                      continue;
                    }
                    _fillErrors(titv[k]->second.fixtures, errors);
                  }
                }
              }

              return errors;
            }

          private:
            void _fillErrors(std::vector<FixtureInfo>& a_fixtures, Errors& a_dst) {
              for(const FixtureInfo& info : a_fixtures) {
                if (!info.errors->message.empty()) {
                  a_dst.push_back( Error{info.errors->message, info.fixture.file, info.fixture.line, info.errors->count, info.errors->index} );
                  ++info.errors->count;
                }
              }
            }

            void _call(std::set<Test>::const_iterator a_beginIt, std::set<Test>::const_iterator a_currentIt, std::set<Test>::const_iterator a_endIt,
                       const std::string& a_part, const std::string& a_group, const std::string& a_test,
                       bool a_before, int a_onlyGlobal) {

              _callFixtures(a_beginIt, a_currentIt, a_endIt, a_part, a_group, _graph.fixtures, a_before, a_onlyGlobal);

              std::map<std::string, FixturePartGraph>::iterator pitv[2] = {_graph.parts.find(a_part), _graph.parts.find("*")};
              for(size_t i = 0; i < 2; ++i) {
                if (pitv[i] == _graph.parts.end()) {
                  continue;
                }

                _callFixtures(a_beginIt, a_currentIt, a_endIt, a_part, a_group, pitv[i]->second.fixtures, a_before, a_onlyGlobal);

                std::map<std::string, FixtureGroupGraph>::iterator gitv[2] = { pitv[i]->second.groups.find(a_group), pitv[i]->second.groups.find("*")};

                for(size_t j = 0; j < 2; ++j) {
                  if (gitv[j] == pitv[i]->second.groups.end()) {
                    continue;
                  }
                  _callFixtures(a_beginIt, a_currentIt, a_endIt, a_part, a_group, gitv[j]->second.fixtures, a_before, a_onlyGlobal);

                  std::map<std::string, FixtureTestGraph>::iterator titv[2] = { gitv[j]->second.tests.find(a_test), gitv[j]->second.tests.find("*")};

                  for(size_t k = 0; k < 2; ++k) {
                    if (titv[k] == gitv[j]->second.tests.end()) {
                      continue;
                    }
                    _callFixtures(a_beginIt, a_currentIt, a_endIt, a_part, a_group, titv[k]->second.fixtures, a_before, a_onlyGlobal);
                  }
                }
              }
            }

            bool _fixureMatch(const Fixture& a_fixture, const std::string& a_part, const std::string& a_group, const std::string& a_test){
              bool match;
              match = a_fixture.parts.empty() ||
                      std::find_if(a_fixture.parts.begin(), a_fixture.parts.end(), [&](const std::string& a_name){ return a_name == "*" || a_name == a_part; }) != a_fixture.parts.end();
              if (!match) {
                return false;
              }
              match = a_fixture.groups.empty() ||
                      std::find_if(a_fixture.groups.begin(), a_fixture.groups.end(), [&](const std::string& a_name){ return a_name == "*" || a_name == a_group; }) != a_fixture.groups.end();
              if (!match) {
                return false;
              }
              match = a_fixture.tests.empty() ||
                      std::find_if(a_fixture.tests.begin(), a_fixture.tests.end(), [&](const std::string& a_name){ return a_name == "*" || a_name == a_test; }) != a_fixture.tests.end();
              return match;
            }

            void _callFixtures(std::set<Test>::const_iterator a_beginIt, std::set<Test>::const_iterator a_currentIt, std::set<Test>::const_iterator a_endIt,
                               const std::string& a_part, const std::string& a_group,
                               std::vector<FixtureInfo>& a_fixtures, bool a_before, bool a_onlyGlobal) {
              for(const FixtureInfo& fi : a_fixtures) {
                int enable = false;
                if (a_onlyGlobal) {
                  enable = ((fi.fixture.level == FL_GLOBAL && fi.active == 0) || ( fi.fixture.level != FL_GLOBAL && !a_before && *fi.close)) &&
                           a_before == fi.fixture.before;
                } else if (fi.fixture.level == FL_GLOBAL) {
                  enable = false;
                } else if (fi.fixture.level == FL_TEST) {
                  enable = true;
                } else if (a_before) {
                  if (fi.fixture.level == FL_GROUP) {
                    bool match = false;
                    std::set<Test>::const_iterator it = a_currentIt;
                    while (a_beginIt != it && !match) {
                      --it;
                      if (it->part != a_part || it->group != a_group){
                        break;
                      }
                      match |= _fixureMatch(fi.fixture, it->part, it->group, it->test);
                    };
                    if (!match) {
                      enable = true;
                    }
                  } else if (fi.fixture.level == FL_PART) {
                    bool match = false;
                    std::set<Test>::const_iterator it = a_currentIt;
                    while (a_beginIt != it && !match) {
                      --it;
                      if (it->part != a_part) {
                        break;
                      }
                      match |= _fixureMatch(fi.fixture, it->part, it->group, it->test);
                    };
                    if (!match) {
                      enable = true;
                    }
                  }
                } else {
                  if (fi.fixture.level == FL_GROUP) {
                    bool match = false;
                    std::set<Test>::const_iterator it = a_currentIt;
                    if (it != a_endIt) {
                      ++it;
                    }
                    for (; it != a_endIt; ++it) {
                      if (it->part != a_part || it->group != a_group){
                        break;
                      }
                      match |= _fixureMatch(fi.fixture, it->part, it->group, it->test);
                    }
                    if (!match) {
                      enable = true;
                    }
                  } else if (fi.fixture.level == FL_PART) {
                    bool match = false;
                    std::set<Test>::const_iterator it = a_currentIt;
                    if (it != a_endIt) {
                      ++it;
                    }
                    for (; it != a_endIt; ++it) {
                      if (it->part != a_part) {
                        break;
                      }
                      match |= _fixureMatch(fi.fixture, it->part, it->group, it->test);
                    }
                    if (!match) {
                      enable = true;
                    }
                  }
                }

                if (!enable) {
                  continue;
                }

                if (a_before) {
                  if (!a_onlyGlobal && !fi.fixture.before){
                    ++*fi.close;
                  }
                } else {
                  if (!fi.fixture.before){
                    if (a_onlyGlobal) {
                      *fi.close = 0;
                    } else {
                      --*fi.close;
                    }
                  }
                }

                if (fi.fixture.before != a_before) {
                  continue;
                }

                ++fi.active;

                fi.errors->message.clear();
                fi.errors->count = 0;
                fi.errors->index = 0;

                try {
                  fi.fixture.fixtureFunction();
                } catch(const std::exception& e) {
                  fi.errors->message = e.what();
                  fi.errors->index = ++_errorCounter;
                }
              }
            }

            FixtureGraph _build() {
              FixtureGraph graph;
              int level = 3;
              std::vector<Fixture> fixtures(storage().fixtures());
              for(const Fixture& fixture : fixtures) {
                std::shared_ptr< FixtureError > errors( new FixtureError{"", 0, 0} );
                std::shared_ptr< int > count( new int{0} );
                for(const std::string& part : fixture.parts) {
                  for(const std::string& group : fixture.groups) {
                    for(const std::string& test : fixture.tests) {
                      level = 3;
                      if (test == "*") {
                        level = 2;
                        if (group == "*") {
                          level = 1;
                          if (part == "*") {
                            level = 0;
                          }
                        }
                      }

                      std::vector<FixtureInfo>* set = &graph.fixtures;
                      std::map<std::string, FixturePartGraph>::iterator  pit;
                      std::map<std::string, FixtureGroupGraph>::iterator git;
                      std::map<std::string, FixtureTestGraph>::iterator  tit;
                      if (level >= 1) {
                        pit = graph.parts.find(part);
                        if (pit == graph.parts.end()){
                          pit = graph.parts.insert({ part, FixturePartGraph() }).first;
                        }
                        set = &pit->second.fixtures;
                      }
                      if (level >= 2) {
                        git = pit->second.groups.find(group);
                        if (git == pit->second.groups.end()){
                          git = pit->second.groups.insert({ group, FixtureGroupGraph() }).first;
                        }
                        set = &git->second.fixtures;
                      }
                      if (level >= 3) {
                        tit = git->second.tests.find(test);
                        if (tit == git->second.tests.end()) {
                          tit = git->second.tests.insert({ test, FixtureTestGraph() }).first;
                        }
                        set = &tit->second.fixtures;
                      }

                      if (set) {
                        set->push_back( { fixture, 0,  count, errors } );
                      }

                    }
                  }
                }
              }
              return graph;
            }

            FixtureGraph _graph;
            size_t       _errorCounter;
        };
      #endif
    }

    namespace NDetails {
      #ifdef FCF_TEST_IMPLEMENTATION
        inline void attachFixtureErrors(const FixtureHandler::Errors* a_errors, const FixtureHandler::Errors* a_startErrors) {
          std::set<size_t> processed;
          const FixtureHandler::Errors* verrors[] = {a_startErrors, a_errors};
          for(size_t i = 0; i < 2; ++i) {
            if (!verrors[i]){
              continue;
            }
            for(const FixtureHandler::Error& error : *verrors[i]) {
              if (processed.count(error.index)) {
                continue;
              }
              std::string message = "Fixture error [FILE: " + error.file + ":" + std::to_string(error.line)  + "]";
              if (error.count == 0){
                message += ":\n";
                std::stringstream ss(error.message);
                std::string line;
                while(std::getline(ss, line)){
                  message += "  ";
                  message += line;
                }
              }
              state().error(message.c_str(), false);
              processed.insert(error.index);
            }
          }
        }
      #endif
    } // NDetails namespace

    namespace NDetails {
      inline void printCaseMessage() {
        log(LMC_LAUNCH_START_CASE_MESSAGE) << "Parameter set: " << (state().paramIndex() + 1) << std::endl;
      }
    }

    namespace NDetails {
      #ifdef FCF_TEST_IMPLEMENTATION
        FCF_TEST_API void runImpl(const Options& a_options, bool a_enableThrow, bool* a_errorPtr) {
          static std::recursive_mutex mutex;
          static bool globalRunState = false;

          {
            std::lock_guard<std::recursive_mutex> lock(mutex);
            #ifndef Z__FCF_TEST_RECURCIVE_RUN_DISABLE
              if (globalRunState) {
                if (a_enableThrow) {
                  throw std::runtime_error("The tests have already been launched");
                } else if (a_errorPtr){
                  *a_errorPtr = true;
                }
                return;
              }
            #endif
            globalRunState = true;
          }

          bool totalErrorFlag = false;

          FixtureHandler fixtureHandler;
          std::set<Test> tests;

          Logger::Environment lastEnv = logger()._getEnvironment();
          Logger::Environment newEnv {
                                a_options.logLevel != LL_DEF ? a_options.logLevel : lastEnv.level,
                                a_options.format.length() ? a_options.format : lastEnv.format,
                                lastEnv.targets
                              };
          std::list<std::ofstream> ofstreams;
          for(const Options::File& file : a_options.files) {
            std::string streamName = file.format.length() ? (std::string() + "file-" + file.format)
                                                          : std::string("file");
            ofstreams.push_back(std::ofstream(file.file, std::ios::binary));
            Logger::_appendTarget({streamName, &ofstreams.back(), file.format, {}, {}}, newEnv);
          }

          std::set<Test>           lastTests = state().tests();
          Test                     lastTest = state().test();
          Duration                 lastDuration = state().duration();
          std::vector<std::string> lastErrors = state().errors();
          bool                     lastActive = state().active();
          size_t                   lastParamIndex = state().paramIndex();
          SharedPtrAny             lastParam = state().param();

          std::set< std::string > lastTestData;
          try {
            logger()._setEnvironment(newEnv);

            std::set<Test> tests = storage().selectTests(a_options);

            state().duration({});
            state().tests(tests);
            state().test({});
            state().errors({});
            state()._setParamIndex(0);
            state()._setParam(SharedPtrAny());
            state()._setActive(false);

            lastTestData = state().dataKeys();

            log(LMC_ROOT_START);

            unsigned int errorCounter  = 0;
            unsigned int passedCounter = 0;

            if (tests.size()) {
              state().test(*tests.begin());

              fixtureHandler.start(tests);

              FixtureHandler::Errors startFixtureErrors = fixtureHandler.errors(*tests.begin());

              std::set<Test>::const_iterator testIt = tests.begin();
              for(; testIt != tests.end(); ++testIt) {
                const Test& test = *testIt;
                state().test(test);
                state().errors({});

                fixtureHandler.call(tests.begin(), testIt, tests.end(), test.part, test.group, test.test, true);

                FixtureHandler::Errors fixtureErrors = fixtureHandler.errors(*testIt);

                attachFixtureErrors(&fixtureErrors, (testIt == tests.begin() ? &startFixtureErrors : (FixtureHandler::Errors*) nullptr ));
                state()._setActive(true);
                log(LMC_LAUNCH_START);
                log(LMC_LAUNCH_START_MESSAGE) << "Performing the test: \"" + test.part + "\" -> \"" + test.group + "\" -> \"" + test.test + "\" ..." << std::endl;

                std::vector<SharedPtrAny> params = storage().params(testIt->part, testIt->group, testIt->test);

                state()._resumeDuration();

                if (fixtureErrors.empty()) {
                  std::vector<SharedPtrAny> params = storage().params(testIt->part, testIt->group, testIt->test);
                  for(size_t paramIndex = 0; !paramIndex || paramIndex < params.size(); ++paramIndex) {
                    try {
                      state()._setParamIndex(paramIndex);
                      state()._setParam( paramIndex < params.size() ? params[paramIndex] : SharedPtrAny() );
                      if (params.size()) {
                        printCaseMessage();
                      }
                      test.testFunction();
                    } catch(const std::exception& e){
                      state().error(e.what(), true);
                    }
                    params = storage().params(testIt->part, testIt->group, testIt->test);
                    if (!a_options.noBreak && state().errors().size()) {
                      break;
                    }
                  }
                }

                state()._endDuration();

                state()._setActive(false);

                std::vector<std::string> errors = state().errors();
                if (!errors.size()) {
                  ++passedCounter;
                  log(LMC_TEST_COMPLETE) << Z__FCF_TEST_ANSI_SUCCESS << "[SUCCESS]" << Z__FCF_TEST_ANSI_RESET
                                         << " Test completed successfully (" << state().duration().lastTotalDurationStr(true) << " sec)" << std::endl;
                  log(LMC_LAUNCH_END);
                  fixtureHandler.call(tests.begin(), testIt, tests.end(), test.part, test.group, test.test, false);
                } else {
                  totalErrorFlag = true;
                  ++errorCounter;
                  for(std::string errorMesssage : errors) {
                    errorMesssage.erase(errorMesssage.find_last_not_of(" \t\n\r\f\v") + 1);
                    log(LMC_TEST_ERROR_MESSAGE) << errorMesssage << std::endl;
                  }
                  log(LMC_TEST_ERROR) << Z__FCF_TEST_ANSI_FAILED << "[FAILED]" << Z__FCF_TEST_ANSI_RESET << " Test failed (" << state().duration().lastTotalDurationStr(true) << " sec)" << std::endl;
                  log(LMC_LAUNCH_END);

                  fixtureHandler.call(tests.begin(), testIt, tests.end(), test.part, test.group, test.test, false);
                  if (a_options.noBreak) {
                    continue;
                  } else {
                    break;
                  }
                }
              }

              fixtureHandler.end(tests);
            }

            unsigned int skippedCounter = tests.size() - passedCounter - errorCounter;

            if (!errorCounter) {
              log(LMC_ROOT_NEW_LINE) << std::endl;
              log(LMC_ROOT_COMPLETE) << Z__FCF_TEST_ANSI_SUCCESS << "[SUCCESS]" << Z__FCF_TEST_ANSI_RESET << " All tests were completed." << std::endl;
            } else {
              log(LMC_ROOT_NEW_LINE) << std::endl;
              log(LMC_ROOT_ERROR) << Z__FCF_TEST_ANSI_FAILED << "[FAILED]" << Z__FCF_TEST_ANSI_RESET << " Testing completed with failures." << std::endl;
            }

            log(LMC_ROOT_SUMMARY)   << "Tests: " << passedCounter << " passed, " << errorCounter << " failed, " << skippedCounter << " skipped, " << tests.size() << " total" << std::endl;
            log(LMC_ROOT_DURATION) << "Duration: " << state().duration().totalDurationStr(true) << " sec" << std::endl;

            log(LMC_ROOT_END);

            state()._setActive(lastActive);
            state()._setParam(lastParam);
            state()._setParamIndex(lastParamIndex);
            state().tests(lastTests);
            state().test(lastTest);
            state().duration(lastDuration);
            state().errors(lastErrors);


            std::set< std::string > currentTestData = state().dataKeys();
            for(const std::string& key : currentTestData) {
              if ( lastTestData.find(key) == lastTestData.end() ){
                state().eraseData(key.c_str());
              }
            }

            logger()._setEnvironment(lastEnv);

            {
              std::lock_guard<std::recursive_mutex> lock(mutex);
              globalRunState = false;
            }
          } catch(const std::exception& a_error) {
            log(LMC_ROOT_RUN_ERROR) << "Error: " << a_error.what() << std::endl;

            state()._setActive(lastActive);
            state()._setParam(lastParam);
            state()._setParamIndex(lastParamIndex);
            state().tests(lastTests);
            state().test(lastTest);
            state().duration(lastDuration);
            state().errors(lastErrors);

            std::set< std::string > currentTestData = state().dataKeys();
            for(const std::string& key : currentTestData) {
              if ( lastTestData.find(key) == lastTestData.end() ) {
                state().eraseData(key.c_str());
              }
            }

            logger()._setEnvironment(lastEnv);

            {
              std::lock_guard<std::recursive_mutex> lock(mutex);
              globalRunState = false;
            }

            if (a_enableThrow) {
              throw;
            }
            if (a_errorPtr) {
              *a_errorPtr = true;
            }
            return;
          }


          if (totalErrorFlag && a_enableThrow) {
            throw std::runtime_error("Testing completed with failures");
          }
          if (a_errorPtr) {
            *a_errorPtr = totalErrorFlag;
          }
        }

      #endif
    } // NDetails namespace

    namespace NDetails {

      template <typename... TPack>
      struct PrintPack {
        template <typename TIterator, typename TArg, typename... TPack2>
        std::string operator()(TIterator a_begName, TIterator a_endName, const TArg& a_arg, const TPack2&... a_pack) {
          std::stringstream ss;
          std::string name = a_begName != a_endName ? *a_begName : "arg";
          name = name.length() && (unsigned char)name[0] <= (unsigned char)' ' ? name.substr(1, std::string::npos) : name;
          if (name.length() && name[0]=='\"') {
            ss << "    " << name << std::endl;
          } else {
            ss << "    " << name << ": " << a_arg << std::endl;
          }
          if (a_begName != a_endName) {
            ++a_begName;
          }
          return ss.str() + PrintPack<TPack2...>()(a_begName, a_endName, a_pack...);
        }
      };

      template <>
      struct PrintPack<> {
        template <typename TIterator>
        std::string operator()(TIterator /*a_begName*/, TIterator /*a_endName*/) {
          return std::string();
        }
      };

      struct Printer {
        std::string             expr;
        const char*             file;
        const char*             line;
        std::list<const char*>  values;

        template <typename ...TPack>
        Printer(const char* a_expr, const char* a_file, const char* a_line, TPack... a_argPack)
          : expr (_trim(a_expr, true))
          , file(a_file)
          , line(a_line) {
          _appendValue(a_argPack...);
        }

        template <typename ...TPack>
        Printer(std::pair<const char*, const char*> a_exceptionInfo, const char* a_file, const char* a_line, TPack... a_argPack)
          : file(a_file)
          , line(a_line) {
          if (a_exceptionInfo.second) {
            expr = "'";
            expr += _trim(a_exceptionInfo.first, true);
            expr += "' threw an exception that does not match '";
            expr += _trim(a_exceptionInfo.second, true);
            expr += "'";
          } else {
            expr = "'";
            expr += _trim(a_exceptionInfo.first, true);
            expr += "' did not throw";
          }
          _appendValue(a_argPack...);
        }

        template <typename... TArgPack>
        std::string operator()(const TArgPack&... a_pack) {
          std::string result = std::string() + \
                               "Test error: " + expr + "  [FILE: " + file + ":" + line + "]\n";
          if (sizeof...(TArgPack) && values.size()) {
            result += "  Values:\n";
          }
          result += PrintPack<TArgPack...>()(values.begin(), values.end(), a_pack...);
          return result;
        }

        template <typename... TArgPack>
        bool inlineCheck(bool a_expression, const TArgPack&... a_pack) {
          if (!a_expression) {
            std::string result = std::string() + \
                                 "Test error: " + expr + "  [FILE: " + file + ":" + line + "]\n";
            if (sizeof...(TArgPack) && values.size()) {
              result += "  Values:\n";
            }
            result += PrintPack<TArgPack...>()(values.begin(), values.end(), a_pack...);
            result = std::runtime_error(result).what();
            ::fcf::NTest::state().error(result.c_str(), false);\
          }
          return a_expression;
        }

        private:
          template <typename TValue, typename ...TPack>
          void _appendValue(TValue a_value, TPack... a_valuePack) {
            values.push_back(a_value);
            _appendValue(a_valuePack...);
          }
          void _appendValue() {
          }
          std::string _trim(std::string a_str, bool a_removeParenthesis) {
            auto is_space = [](unsigned char ch) { return !std::isspace(ch); };
            a_str.erase(a_str.begin(), std::find_if(a_str.begin(), a_str.end(), is_space));
            a_str.erase(std::find_if(a_str.rbegin(), a_str.rend(), is_space).base(), a_str.end());
            if (a_removeParenthesis && a_str.length() && a_str.front() == '(' && a_str.back() == ')'){
              a_str.erase(0, 1);
              a_str.erase(a_str.length()-1, a_str.length());
            }
            return a_str;
          }
      };
    } // NDetails namespace
  } // NTest namespace
} // fcf namespace


namespace fcf {
  namespace NTest {
    namespace NDetails {

      /**
       * @brief Parses a single string into a vector of arguments (flags and values).
       * @param a_dstVector The vector to populate with parsed arguments.
       * @param a_input The input string to parse.
       */
      inline void parseArgs(std::vector<std::string>& a_dstVector, std::string a_input) {
        if (a_input == "=") {
          return;
        }
        if (!a_input.empty()) {
          size_t pos = a_input.find("=");
          if (pos != std::string::npos) {
            size_t len = pos;
            if (len) {
              a_dstVector.push_back(a_input.substr(0, len));
            }
            len = a_input.length() - (pos+1);
            if (len) {
              a_dstVector.push_back(a_input.substr(pos+1, len));
            }
            return;
          }
        }
        a_dstVector.push_back(a_input);
      }

      /**
       * @brief Parses an array of C-style strings into a vector of strings.
       * @param a_argc Number of arguments.
       * @param a_argv Array of argument strings.
       * @return A vector containing all parsed arguments.
       */
      inline std::vector<std::string> parseArgs(int a_argc, const char* const* a_argv) {
        std::vector<std::string> result;
        for(int i = 0; i < a_argc; ++i) {
          parseArgs(result, (std::string)a_argv[i]);
        }
        return result;
      }

    } // NDetails namespace
  } // NTest namespace
} // fcf namespace



/* ========================================================== */
/* ===                                                    === */
/* ===                   Implementation                   === */
/* ===                                                    === */
/* ===              Declaration of the basic              === */
/* ===             structures of unit testing             === */
/* ===                                                    === */
/* ========================================================== */

namespace fcf {
  namespace NTest {

    #ifdef FCF_TEST_IMPLEMENTATION
      void Storage::partOrder(const char* a_name, int a_order) {
        std::lock_guard<std::mutex> lock(_mutex);
        _partOrders[a_name] = a_order;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      int Storage::partOrder(const char* a_name) const {
        std::lock_guard<std::mutex> lock(_mutex);
        OrderMap::const_iterator it = _partOrders.find(a_name);
        return it != _partOrders.end() ? it->second : FCF_TEST_ORDER_DEFAULT;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Storage::groupOrder(const char* a_name, int a_order) {
        std::lock_guard<std::mutex> lock(_mutex);
        _groupOrders[a_name] = a_order;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      int Storage::groupOrder(const char* a_name) const {
        std::lock_guard<std::mutex> lock(_mutex);
        OrderMap::const_iterator it = _groupOrders.find(a_name);
        return it != _groupOrders.end() ? it->second : FCF_TEST_ORDER_DEFAULT;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Storage::testOrder(const char* a_name, int a_order) {
        std::lock_guard<std::mutex> lock(_mutex);
        _testOrders[a_name] = a_order;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      int Storage::testOrder(const char* a_name) const {
        std::lock_guard<std::mutex> lock(_mutex);
        OrderMap::const_iterator it = _testOrders.find(a_name);
        return it != _testOrders.end() ? it->second : FCF_TEST_ORDER_DEFAULT;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Storage::appendTest(const Test& a_test) {
        std::lock_guard<std::mutex> lock(_mutex);
        _tests.push_back(a_test);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Storage::appendFixture(const Fixture& a_fixture) {
        Fixture fixture(a_fixture);
        std::lock_guard<std::mutex> lock(_mutex);
        if (!fixture.parts.size()) {
          fixture.parts.push_back("*");
        }
        for(size_t i = 0; i < fixture.parts.size(); ++i) {
          if (!fixture.parts[i].length()) {
            fixture.parts[i] = "*";
          }
        }
        if (!fixture.groups.size()) {
          fixture.groups.push_back("*");
        }
        for(size_t i = 0; i < fixture.groups.size(); ++i) {
          if (!fixture.groups[i].length()) {
            fixture.groups[i] = "*";
          }
        }
        if (!fixture.tests.size()) {
          fixture.tests.push_back("*");
        }
        for(size_t i = 0; i < fixture.tests.size(); ++i) {
          if (!fixture.tests[i].length()) {
            fixture.tests[i] = "*";
          }
        }
        _fixtures.push_back(fixture);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      std::vector<SharedPtrAny> Storage::params(const std::string& a_part, const std::string& a_group, const std::string& a_test) const {
        std::lock_guard<std::mutex> lock(_mutex);
        TestParams tp{a_part, a_group, a_test, {}};
        std::set<TestParams>::const_iterator it = _params.find(tp);
        if (it == _params.end()){
          return {};
        }
        return it->params;
      }

    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Storage::params(const std::string& a_part, const std::string& a_group, const std::string& a_test, const std::vector<SharedPtrAny>& a_params){
        std::lock_guard<std::mutex> lock(_mutex);
        TestParams tp{a_part, a_group, a_test, {}};
        std::set<TestParams>::iterator it = _params.insert(tp).first;

        bool isCurrentTest = state().active() &&
                      state().test().part == a_part &&
                      state().test().group == a_group &&
                      state().test().test == a_test;
        bool upcase = isCurrentTest &&
                      it->params.empty() &&
                      !a_params.empty() &&
                      state().paramIndex() == 0;

        it->params = a_params;

        if (isCurrentTest) {
          SharedPtrAny spa = state().paramIndex() < it->params.size() 
                                ? it->params[state().paramIndex()]
                                : SharedPtrAny();
          state()._setParam(spa);
          if (upcase) {
            NDetails::printCaseMessage();
          }
        }
      }
    #endif

    template <typename ...TParamPack>
    void Storage::appendParam(const std::string& a_partName, const std::string& a_groupName, const std::string& a_testName, TParamPack... a_parameterPack){
      std::lock_guard<std::mutex> lock(_mutex);
      _appendParam(a_partName, a_groupName, a_testName, a_parameterPack...);
    }

    template <typename ...TParamPack>
    void Storage::appendParam(const Test& a_test, TParamPack... a_parameterPack){
      std::lock_guard<std::mutex> lock(_mutex);
      _appendParam(a_test.part, a_test.group, a_test.test, a_parameterPack...);
    }

    template <typename ...TParamPack>
    void Storage::appendParam(const TestPath& a_testPath, TParamPack... a_parameterPack){
      std::lock_guard<std::mutex> lock(_mutex);
      _appendParam(a_testPath.part, a_testPath.group, a_testPath.test, a_parameterPack...);
    }

    template<typename ...TParamPack>
    void Storage::_appendParam(const std::string& a_partName, const std::string& a_groupName, const std::string& a_testName, SharedPtrAny a_parameter, const TParamPack& ...a_parameterPack){
      TestParams tp{a_partName, a_groupName, a_testName, {}};
      std::set<TestParams>::iterator it = _params.insert(tp).first;

      bool isCurrentTest = state().active() &&
                    state().test().part == a_partName &&
                    state().test().group == a_groupName &&
                    state().test().test == a_testName;

      bool upcase = isCurrentTest &&
                    it->params.empty() &&
                    state().paramIndex() == 0;

      it->params.push_back(a_parameter);

      if (upcase) {
        state()._setParam(a_parameter);
        NDetails::printCaseMessage();
      }

      _appendParam(a_partName, a_groupName, a_testName, a_parameterPack...);
    }

    inline void Storage::_appendParam(const std::string& a_partName, const std::string& a_groupName, const std::string& a_testName){
    }

    template <typename ...TValuePack>
    void Storage::appendParamValue(const std::string& a_partName, const std::string& a_groupName, const std::string& a_testName, const TValuePack& ...a_parameterPack){
      std::lock_guard<std::mutex> lock(_mutex);
      _appendParamValue(a_partName, a_groupName, a_testName, a_parameterPack...);
    }

    template <typename ...TValuePack>
    void Storage::appendParamValue(const Test& a_test, const TValuePack& ...a_parameterPack){
      std::lock_guard<std::mutex> lock(_mutex);
      _appendParamValue(a_test.part, a_test.group, a_test.test, a_parameterPack...);
    }

    template <typename ...TValuePack>
    void Storage::appendParamValue(const TestPath& a_testPath, const TValuePack& ...a_parameterPack){
      std::lock_guard<std::mutex> lock(_mutex);
      _appendParamValue(a_testPath.part, a_testPath.group, a_testPath.test, a_parameterPack...);
    }

    template <typename TValue, typename ...TValuePack>
    void Storage::_appendParamValue(const std::string& a_partName, const std::string& a_groupName, const std::string& a_testName, const TValue& a_parameter, const TValuePack& ...a_parameterPack){
      _appendParam(a_partName, a_groupName, a_testName, SharedPtrAny::make<TValue>(a_parameter));
      _appendParamValue(a_partName, a_groupName, a_testName, a_parameterPack...);
    }

    inline void Storage::_appendParamValue(const std::string& a_partName, const std::string& a_groupName, const std::string& a_testName){
    }


    #ifdef FCF_TEST_IMPLEMENTATION
      std::set<Test> Storage::selectTests(const Options& a_options) const{
        std::set<Test> result;
        std::lock_guard<std::mutex> lock(_mutex);
        std::map<std::string, bool> exists[3];

        for(const Options::Selector& selector : a_options.selectors) {
          const std::vector<std::string>* selectors[3] = {&selector.parts, &selector.groups, &selector.tests};
          for(size_t i = 0; i < 3; ++i) {
            for(const std::string& element : *selectors[i]) {
              if (element.length() && element != "*" && element != "") {
                exists[i].insert({element, false});
              }
            }
          }
        }

        for(const Test& test : _tests) {
          const std::string* levels[3] = {&test.part, &test.group, &test.test};
          for(size_t i = 0; i < 3; ++i) {
            std::map<std::string, bool>::iterator existsIt = exists[i].find(*levels[i]);
            if (existsIt != exists[i].end()){
              existsIt->second = true;
            }
          }

          bool suitability = true;
          for(const Options::Selector& selector : a_options.selectors) {
            suitability = true;
            if (!selector.parts.empty() && !_suitability(selector.parts, test.part, suitability)){
              continue;
            }
            if (!selector.groups.empty() && !_suitability(selector.groups, test.group, suitability)){
              continue;
            }
            if (!selector.tests.empty() && !_suitability(selector.tests, test.test, suitability)){
              continue;
            }
            break;
          }
          if (!suitability) {
            continue;
          }

          bool ignore = false;
          for(const Options::Selector& selector : a_options.ignoreSelectors) {
            if (!selector.parts.empty() && _suitability(selector.parts, test.part, ignore)){
              break;
            }
            if (!selector.groups.empty() && _suitability(selector.groups, test.group, ignore)){
              break;
            }
            if (!selector.tests.empty() && _suitability(selector.tests, test.test, ignore)){
              break;
            }
          }
          if (ignore) {
            continue;
          }

          Test resultTest(test);
          {
            Storage::OrderMap::const_iterator it = _partOrders.find(test.part);
            if (it != _partOrders.end()) {
              resultTest.partOrder = it->second;
            }
          }
          {
            Storage::OrderMap::const_iterator it = _groupOrders.find(test.group);
            if (it != _groupOrders.end()) {
              resultTest.groupOrder = it->second;
            }
          }
          {
            Storage::OrderMap::const_iterator it = _testOrders.find(test.test);
            if (it != _testOrders.end()) {
              resultTest.testOrder = it->second;
            }
          }
          result.insert(resultTest);
        }

        const char* throwPrefixes[3] = { "parts ", "groups ", "" };
        for(size_t i = 0; i < 3; ++i) {
          for(std::pair<const std::string, bool>& existsElement : exists[i]) {
            if (!existsElement.second) {
              throw std::runtime_error(std::string() + "The test " + throwPrefixes[i] + "named '" + existsElement.first + "' cannot be found");
            }
          }
        }

        return result;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      std::vector<Fixture> Storage::fixtures() const{
        std::lock_guard<std::mutex> lock(_mutex);
        std::vector<Fixture> fixtures(_fixtures);
        return fixtures;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      bool Storage::_suitability(const std::vector<std::string>& a_items, const std::string& a_rule, bool& a_dstSuitability) const{
        std::vector<std::string>::const_iterator foundIt = std::find(a_items.begin(), a_items.end(), "*");
        if (foundIt == a_items.end()) {
          foundIt = std::find(a_items.begin(), a_items.end(), "");
        }
        if (foundIt == a_items.end()) {
          foundIt = std::find_if(a_items.begin(), a_items.end(), [&a_rule](const std::string& a_name){ return a_name == a_rule; });
        }
        a_dstSuitability = foundIt != a_items.end();
        return a_dstSuitability;
      }
    #endif

  } // NTest namespace
} // fcf namespace

/* ========================================================== */
/* ===                                                    === */
/* ===                   Implementation                   === */
/* ===                                                    === */
/* ===         Helper types for storing user data         === */
/* ===                                                    === */
/* ========================================================== */

namespace fcf {
  namespace NTest {

    #ifdef FCF_TEST_IMPLEMENTATION
      SharedPtrAny::ControlBlockBase::ControlBlockBase()
        : refCount(1)
      {
      }
    #endif



    template <typename Ty>
    template <typename... ArgPack>
    SharedPtrAny::ControlBlockDerived<Ty>::ControlBlockDerived(ArgPack&&... a_args)
      : ControlBlockBase(), data(std::forward<ArgPack>(a_args)...) {
    }

    template <typename Ty>
    void* SharedPtrAny::ControlBlockDerived<Ty>::ptr() {
      return &data;
    }

    template <typename Ty>
    const std::type_info& SharedPtrAny::ControlBlockDerived<Ty>::type() const noexcept {
      return typeid(Ty);
    }



    template <typename Ty, typename... ArgPack>
    SharedPtrAny SharedPtrAny::make(ArgPack&&... a_args) {
      return SharedPtrAny(new ControlBlockDerived<Ty>(std::forward<ArgPack>(a_args)...));
    }

    #ifdef FCF_TEST_IMPLEMENTATION
      SharedPtrAny::SharedPtrAny() noexcept
        : _block(nullptr)
      {
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      SharedPtrAny::SharedPtrAny(std::nullptr_t) noexcept
        : _block(nullptr)
      {
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      SharedPtrAny::SharedPtrAny(const SharedPtrAny& a_source) noexcept
        : _block(a_source._block)
      {
        if (_block) {
          _block->refCount.fetch_add(1, std::memory_order_relaxed);
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      SharedPtrAny::SharedPtrAny(SharedPtrAny&& a_source) noexcept
        : _block(a_source._block)
      {
        a_source._block = nullptr;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      SharedPtrAny::~SharedPtrAny() {
        release();
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      SharedPtrAny& SharedPtrAny::operator=(const SharedPtrAny& a_source) noexcept {
        if (this != &a_source) {
          release();
          _block = a_source._block;
          if (_block) {
            _block->refCount.fetch_add(1, std::memory_order_relaxed);
          }
        }
        return *this;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      SharedPtrAny& SharedPtrAny::operator=(SharedPtrAny&& a_source) noexcept {
        if (this != &a_source) {
          release();
          _block = a_source._block;
          a_source._block = nullptr;
        }
        return *this;
      }
    #endif

    template <typename Ty>
    Ty* SharedPtrAny::cast() {
      if (_block && _block->type() == typeid(Ty)) {
        return static_cast<Ty*>(_block->ptr());
      }
      throw std::bad_cast();
    }

    template <typename Ty>
    bool SharedPtrAny::is() const noexcept{
      return _block && _block->type() == typeid(Ty);
    }


    #ifdef FCF_TEST_IMPLEMENTATION
      int SharedPtrAny::count() const noexcept {
        return _block ? _block->refCount.load(std::memory_order_relaxed) : 0;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      SharedPtrAny::operator bool() const noexcept {
        return _block != nullptr;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      SharedPtrAny::SharedPtrAny(ControlBlockBase* a_block) noexcept
        : _block(a_block) {
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void SharedPtrAny::release() {
        if (_block) {
          if (_block->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            std::atomic_thread_fence(std::memory_order_acquire);
            delete _block;
          }
          _block = nullptr;
        }
      }
    #endif

  } // NTest namespace
} // fcf namespace


/* ========================================================== */
/* ===                                                    === */
/* ===                   Implementation                   === */
/* ===                                                    === */
/* ===               Logging and formatting               === */
/* ===                                                    === */
/* ========================================================== */


/* ----------------------------- */
/* --      Implementation    --- */
/* ---   fcf::NTest::Logger  --- */
/* ----------------------------- */

namespace fcf {
  namespace NTest {

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Logger()
        : _environment{LL_LOG, "default", {}}
        , _newLine(true)
      {
        clearPrefixes(true);
        clearFormats(true);
        clearTargets(true);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Writer Logger::ftl(unsigned int a_category) {
        return _log(LL_FTL, a_category);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Writer Logger::err(unsigned int a_category) {
        return _log(LL_ERR, a_category);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Writer Logger::wrn(unsigned int a_category) {
        return _log(LL_WRN, a_category);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Writer Logger::att(unsigned int a_category) {
        return _log(LL_ATT, a_category);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Writer Logger::log(unsigned int a_category) {
        return _log(LL_LOG, a_category);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Writer Logger::inf(unsigned int a_category) {
        return _log(LL_INF, a_category);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Writer Logger::dbg(unsigned int a_category) {
        return _log(LL_DBG, a_category);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Writer Logger::trc(unsigned int a_category) {
        return _log(LL_TRC, a_category);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      const char* Logger::levelStr() const {
        return toLevelStr(_environment.level);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::levelStr(const char* a_level) {
        level(toLevel(a_level));
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      ELogLevel Logger::level() const {
        return _environment.level;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::level(ELogLevel a_level) {
        if (a_level == LL_DEF) {
          throw std::runtime_error("LL_DEF value cannot be set as primary value");
        }
        _environment.level = a_level;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      ELogLevel Logger::toLevel(std::string a_level, ELogLevel a_default) {
        static const char* levels[] = {"def", "off", "ftl", "err", "wrn", "att", "log", "inf", "dbg", "trc", "all"};
        a_default = a_default == LL_DEF ? LL_LOG : a_default;
        int size = sizeof(levels) / sizeof(levels[0]);
        for(int i = 0; i < size; ++i) {
          int l = i - 1;
          if (a_level == levels[i]) {
            return l == LL_DEF ? a_default : (ELogLevel)l;
          }
        }
        return a_default;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      const char* Logger::toLevelStr(ELogLevel a_level) {
        static const char* levels[] = {"def", "off", "ftl", "err", "wrn", "att", "log", "inf", "dbg", "trc", "all"};
        int size  = sizeof(levels) / sizeof(levels[0]);
        int level = (int)a_level + 1;
        level = level < 0     ? 0 :
                level >= size ? size - 1 :
                               level;
        return levels[level];
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Prefixes Logger::prefixes() const {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        Prefixes prefixes = _prefixes;
        return prefixes;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::prefixes(const Prefixes& a_prefixes) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        clearPrefixes(false);
        for(const Prefix& prefix : a_prefixes) {
          appendPrefix(prefix);
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::clearPrefixes(bool a_defaultState) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        _prefixes.clear();
        if (a_defaultState) {
          Prefix prefix;
          prefix.name        = "test-offset";
          prefix.prefix      = "    ";
          prefix.multiLine   = true;
          prefix.category    = LMC_TEST_GROUP;
          appendPrefix(prefix);

          prefix.name        = "user-offset";
          prefix.prefix      = "  > ";
          prefix.multiLine   = true;
          prefix.category    = LMC_USER_GROUP;
          appendPrefix(prefix);

          prefix.name        = "case-offset";
          prefix.prefix      = " == ";
          prefix.multiLine   = true;
          prefix.category    = LMC_LAUNCH_START_CASE_MESSAGE;
          appendPrefix(prefix);

        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::appendPrefix(const Prefix& a_prefix) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        auto existIt = std::find_if(_prefixes.begin(), _prefixes.end(), [&a_prefix](const Prefix& a_item) { return a_prefix.name == a_item.name; });
        if (existIt != _prefixes.end()) {
          *existIt = a_prefix;
        } else {
          _prefixes.push_back(a_prefix);
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Formats Logger::formats() const {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        Formats formats = _formats;
        return formats;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::formats(Formats& a_formats) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        _formats.clear();
        for(const Format& format : a_formats){
          appendFormat(format);
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::clearFormats(bool a_defaultState) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        _formats.clear();
        if (a_defaultState){
          Format format;
          format.name = "junit";
          format.handler = LogJunitFormatter::format;
          appendFormat(format);
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::appendFormat(const Format& a_format) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        auto existIt = std::find_if(_formats.begin(), _formats.end(), [&a_format](const Format& a_item) { return a_format.name == a_item.name; });
        if (existIt != _formats.end()) {
          *existIt = a_format;
        } else {
          _formats.push_back(a_format);
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::OutputTargets Logger::targets() const {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        OutputTargets result(_environment.targets);
        return result;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::targets(const OutputTargets& a_targets) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        clearTargets(false);
        for(const OutputTarget& stream : a_targets) {
          appendTarget(stream);
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::clearTargets(bool a_defaultState) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        _environment.targets.clear();
        if (a_defaultState) {
          _environment.targets.push_back({"default", &std::cout, "", {}, {}});
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::appendTarget(const OutputTarget& a_stream) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        _appendTarget(a_stream, _environment);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::_appendTarget(const OutputTarget& a_stream, Environment& a_environment ) {
        OutputTargets::iterator it = std::find_if(
                                           a_environment.targets.begin(),
                                           a_environment.targets.end(),
                                           [&a_stream](OutputTarget& a_item){
                                             return a_item.name == a_stream.name;
                                           });
        if (it == a_environment.targets.end()) {
          a_environment.targets.push_back(a_stream);
        } else {
          *it = a_stream;
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Environment Logger::_getEnvironment() {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        Environment environment = _environment;
        return environment;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::_setEnvironment(const Environment& a_environment) {
        _environment = a_environment;
        if (_environment.format.empty()) {
          _environment.format = "default";
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::_write(fcf::NTest::ELogLevel a_level, unsigned int a_messageCategory, const std::string& a_message) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);

        for(OutputTarget& stream : _environment.targets) {

          MessageContext lms(a_message);
          lms.category      = a_messageCategory;
          lms.system        = a_messageCategory & LMC_SYSTEM_GROUP;
          lms.message       = lms.origin;
          lms.line          = 0;
          lms.level         = a_level;
          lms.stream        = stream.stream ? stream.stream : &std::cout;
          lms.data          = nullptr;

          std::vector<size_t> prefposv;

          const std::string& currentFormatName = stream.format.length()
                                                  ? stream.format
                                                  : _environment.format;

          if (lms.origin.length()) {
            for(Prefix prefix : _prefixes) {
              const unsigned int hmask = 0xffff0000 & prefix.category;
              const unsigned int lmask = 0x0000ffff & prefix.category;
              if (!(a_messageCategory & hmask) || ( lmask && lmask !=  (0x0000ffff & a_messageCategory)) ) {
                continue;
              }
              if (!_newLine) {
                continue;
              }

              lms.line          = 0;

              size_t lastPos = 0;
              std::string currentMessage = lms.message;
              std::string resultMessage;
              while(lastPos < currentMessage.length()) {
                size_t prefLength = 0;
                size_t pos = prefix.multiLine ? currentMessage.find("\n", lastPos)
                                              : currentMessage.length()-1;
                if (pos == std::string::npos) {
                  pos = currentMessage.length();
                } else {
                  ++pos;
                }
                lms.message = currentMessage.substr(lastPos, pos - lastPos);

                if (prefix.handler) {
                  const char* prefixName = prefix.name.empty() ? "default" : prefix.name.c_str();
                  HandlerDataMap::iterator dataIt = stream.prefixData.find(prefixName);
                  if (dataIt == stream.prefixData.end()) {
                    SharedPtrAny data = prefix.dataFactory ? prefix.dataFactory(*this, stream) : SharedPtrAny();
                    dataIt = stream.prefixData.insert({prefixName, data}).first;
                  }
                  lms.data = &dataIt->second;

                  std::string prefixPart = prefix.handler(*this, lms);
                  if (prefixPart.length()) {
                    size_t lastPrefEnd = lms.line < prefposv.size() ? prefposv[lms.line] : 0;
                    lms.message = lms.message.substr(0, lastPrefEnd) +  prefixPart + lms.message.substr(lastPrefEnd, lms.message.length() - lastPrefEnd);
                  }
                  prefLength += prefixPart.length();
                }
                if (prefix.prefix.length()) {
                  size_t lastPrefEnd = lms.line < prefposv.size() ? prefposv[lms.line] : 0;
                  lms.message = lms.message.substr(0, lastPrefEnd) +  prefix.prefix + lms.message.substr(lastPrefEnd, lms.message.length() - lastPrefEnd);
                  prefLength += prefix.prefix.length();
                }

                if (prefposv.size() <= lms.line) {
                  prefposv.push_back(prefLength);
                } else {
                  prefposv[lms.line] += prefLength;
                }

                resultMessage += lms.message;
                lastPos = pos;
                ++lms.line;
              }
              std::swap(lms.message, resultMessage);
            }
          }

          lms.line = 0;

          for(Format format : _formats) {
            if (!format.handler){
              continue;
            }
            const char* formatName = format.name.empty() ? "default" : format.name.c_str();
            if (formatName == currentFormatName) {
              HandlerDataMap::iterator dataIt = stream.formatData.find(formatName);
              if (dataIt == stream.formatData.end()) {
                SharedPtrAny data = format.dataFactory ? format.dataFactory(*this, stream) : SharedPtrAny();
                dataIt = stream.formatData.insert({formatName, data}).first;
              }
              lms.data = &dataIt->second;
              format.handler(*this, lms);
            }
          }
          if (lms.message.length() && !lms.system) {
            (*lms.stream) << lms.message;
          }

          if (a_message.length()) {
            _newLine = a_message[a_message.length()-1] == '\n';
          }
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Writer Logger::_log(ELogLevel a_level, unsigned int a_messageCategory) {
        if (_environment.level >= a_level || a_messageCategory != LMC_USER_GROUP) {
          return Logger::Writer(*this, a_level, a_messageCategory);
        } else {
          return Logger::Writer();
        }
      }
    #endif

  } // NTest namespace
} // fcf namespace



/* -------------------------------- */
/* --       Implementation      --- */
/* ---   fcf::NTest::Logger::Writer  --- */
/* -------------------------------- */

namespace fcf {
  namespace NTest {

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Writer::Writer()
        : _logger(0) {
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Writer::Writer(Writer&& a_output)
        : _logger((Logger*)a_output._logger)
        , _level(a_output._level)
        , _loggerMessageCategory(a_output._loggerMessageCategory)
        , _sstream(std::move(a_output._sstream)) {
            a_output._logger = nullptr;
        }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Writer::Writer(Logger& a_logger, ELogLevel a_level, unsigned int a_loggerMessageCategory)
        : _logger(&a_logger), _level(a_level), _loggerMessageCategory(a_loggerMessageCategory) {
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Writer::~Writer() {
        if (_logger) {
          _logger->_write(_level, _loggerMessageCategory, _sstream.str());
        }
      }
    #endif

    template <typename Ty>
    Logger::Writer& Logger::Writer::operator<<(const Ty& a_value) {
      _sstream << a_value;
      return *this;
    }

    #ifdef FCF_TEST_IMPLEMENTATION
      Logger::Writer& Logger::Writer::operator<<(std::ostream& (*a_manipulator)(std::ostream&)) {
        a_manipulator(_sstream);
        return *this;
      }
    #endif

  } // NTest namespace
} // fcf namespace



/* ---------------------------------------- */
/* --            Implementation         --- */
/* ---   fcf::NTest::LogJunitFormatter  --- */
/* ---------------------------------------- */

namespace fcf {
  namespace NTest {

    #ifdef FCF_TEST_IMPLEMENTATION
      std::string LogJunitFormatter::suiteName(const Test& a_test) {
        return a_test.part + "/" + a_test.group;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      std::string LogJunitFormatter::xmlAttribute(const std::string& a_string) {
        std::string result;
        result.reserve(a_string.size());
        for (char ch : a_string) {
          if (ch == '\\' || ch == '"') {
            result += '\\';
          }
          result += ch;
        }
        return result;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      std::string LogJunitFormatter::xmlText(const std::string& a_string) {
        std::string result;
        result.reserve(a_string.size());
        for (char ch : a_string) {
          if (ch == '<') {
            result += "&lt;";
          } else if (ch == '>') {
            result += "&gt;";
          } else if (ch == '&') {
            result += "&amp;";
          } else {
            result += ch;
          }
        }
        return result;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void LogJunitFormatter::format(Logger& /*a_logger*/, Logger::MessageContext& a_messageContext) {
        std::ostringstream output;

        switch (a_messageContext.category) {
          case LMC_ROOT_START:
            {
              *a_messageContext.data = SharedPtrAny::make<LogJunitFormatter>();
            }
            break;
          case LMC_TEST_COMPLETE:
          case LMC_TEST_ERROR_MESSAGE:
            {
              LogJunitFormatter* formatHandler = a_messageContext.data->cast<LogJunitFormatter>();
              if (formatHandler) {
                std::map<Test, ProcessedInfo>::iterator it = formatHandler->_processed.find(state().test());
                if (it != formatHandler->_processed.end()) {
                  it->second.message += "\n";
                  it->second.message += a_messageContext.origin;
                } else {
                  ProcessedInfo pi;
                  pi.error = a_messageContext.category == LMC_TEST_ERROR_MESSAGE;
                  pi.message = a_messageContext.origin;
                  pi.duration = state().duration().lastTotalDuration().count();
                  formatHandler->_processed.insert({state().test(), pi});
                }
              }
            }
            break;
          case LMC_ROOT_END:
            {
              LogJunitFormatter* formatHandler = a_messageContext.data->cast<LogJunitFormatter>();
              if (formatHandler) {
                a_messageContext.system = false;

                size_t totalTestCount   = state().testCount();
                size_t totalTestFailure = std::count_if(formatHandler->_processed.begin(),
                                                        formatHandler->_processed.end(),
                                                        [](const std::pair<Test, ProcessedInfo>& a_item) {
                                                          return a_item.second.error;
                                                        });
                size_t totalTestSkipped  = totalTestCount - formatHandler->_processed.size();

                std::map<std::string, std::set<Test> > suites;
                std::set<Test>                         tests( state().tests() );
                for(const Test& test : tests) {
                  std::string currentSuiteName = suiteName(test);
                  std::map<std::string, std::set<Test> >::iterator it = suites.find(currentSuiteName);
                  if (it == suites.end()) {
                    it = suites.insert({currentSuiteName, {}}).first;
                  }
                  it->second.insert(test);
                }

                output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
                output << "<testsuites "
                       << "tests=\"" << totalTestCount << "\" "
                       << "failure=\"" << totalTestFailure << "\" "
                       << "skipped=\"" << totalTestSkipped << "\" "
                       << "time=\"" << state().duration().totalDurationStr(false) << "\""
                       << ">\n";
                for(const std::pair< const std::string, std::set<Test> >& currentSuite : suites ) {
                  const std::string& currentSuiteName = currentSuite.first;
                  const std::set<Test>& currentTests = currentSuite.second;
                  size_t currentTestCount = currentTests.size();
                  size_t currentFailureCount = std::count_if(currentTests.begin(),
                                                             currentTests.end(),
                                                          [&formatHandler](const Test& a_test) {
                                                            auto it = formatHandler->_processed.find(a_test);
                                                            if (it == formatHandler->_processed.end()) {
                                                              return false;
                                                            }
                                                            return it->second.error;
                                                          });
                  size_t currentSkippedCount = std::count_if(currentTests.begin(),
                                                          currentTests.end(),
                                                          [&formatHandler](const Test& a_test) {
                                                            auto it = formatHandler->_processed.find(a_test);
                                                            return it == formatHandler->_processed.end();
                                                          });
                  unsigned long long time = 0;
                  for(const Test& test : currentTests) {
                    auto it = formatHandler->_processed.find(test);
                    if (it != formatHandler->_processed.end()) {
                      time += it->second.duration;
                    }
                  }
                  output << "  <testsuite "
                         << "name=\""<< xmlAttribute(currentSuiteName) << "\" "
                         << "tests=\"" << currentTestCount << "\" "
                         << "failure=\"" << currentFailureCount <<"\" "
                         << "skipped=\"" << currentSkippedCount << "\" "
                         << "time=\"" << Duration::nsToStr(time, false) << "\""
                         << ">\n";
                  for(const Test& currentTest : currentTests) {
                    auto processedIt = formatHandler->_processed.find(currentTest);
                    bool isSkipped = processedIt == formatHandler->_processed.end();
                    if (isSkipped) {
                      output << "    <testcase classname=\"" << xmlAttribute(currentSuiteName) << "\" "
                             << "name=\"" << xmlAttribute(currentTest.test) << "\" "
                             << "time=\"" << Duration::nsToStr(0, false) << "\""
                             << ">\n";
                      output << "      <skipped message=\"The test was skipped because the fail-on-error mode was enabled.\"/>\n";
                      output << "    </testcase>\n";
                    } else if (processedIt->second.error) {
                      std::string message = processedIt->second.message;
                      message.erase(message.find_last_not_of(" \t\n\r\f\v") + 1);
                      std::string shortMessage = message.substr(0, message.find("\n"));
                      shortMessage = shortMessage.substr(0, shortMessage.find("[FILE:"));
                      shortMessage.erase(shortMessage.find_last_not_of(" \t\n\r\f\v") + 1);
                      output << "    <testcase "
                             << "classname=\"" << xmlAttribute(currentSuiteName) << "\" "
                             << "name=\"" << xmlAttribute(currentTest.test) << "\" "
                             << "time=\"" << Duration::nsToStr(processedIt->second.duration, false) << "\""
                             << ">\n";
                      output << "      <failure message=\"" << xmlAttribute(shortMessage) << "\" type=\"AssertionError\">\n";
                      output << xmlText(message) << "\n";
                      output << "      </failure>\n";
                      output << "    </testcase>\n";
                    } else {
                      output << "    <testcase "
                             << "classname=\"" << xmlAttribute(currentSuiteName) << "\" "
                             << "name=\"" << xmlAttribute(currentTest.test) << "\" "
                             << "time=\"" << Duration::nsToStr(processedIt->second.duration, false) << "\""
                             << "/>\n";
                    }
                  }
                  output << "  </testsuite>\n";
                }
                output << "</testsuites>\n";

                a_messageContext.data->release();
              }
            }
            break;
            default:
              break;
        }

        a_messageContext.message = output.str();
      }
    #endif

  } // NTest namespace
} // fcf namespace



/* ----------------------------- */
/* --     Implementation     --- */
/* ---   Logging functions   --- */
/* ----------------------------- */

namespace fcf {
  namespace NTest {

    #ifdef FCF_TEST_IMPLEMENTATION
      FCF_TEST_API Logger& logger() {
        static Logger* logger = nullptr;
        static std::once_flag flag;

        std::call_once(flag, []() {
          logger = new Logger();
        });

        return *logger;
      }
    #endif

    inline Logger::Writer ftl(unsigned int a_category) {
      return logger().ftl(a_category);
    }

    inline Logger::Writer err(unsigned int a_category) {
      return logger().err(a_category);
    }

    inline Logger::Writer wrn(unsigned int a_category) {
      return logger().wrn(a_category);
    }

    inline Logger::Writer att(unsigned int a_category) {
      return logger().att(a_category);
    }

    inline Logger::Writer log(unsigned int a_category) {
      return logger().log(a_category);
    }

    inline Logger::Writer inf(unsigned int a_category) {
      return logger().inf(a_category);
    }

    inline Logger::Writer dbg(unsigned int a_category) {
      return logger().dbg(a_category);
    }

    inline Logger::Writer trc(unsigned int a_category) {
      return logger().trc(a_category);
    }

  } // NTest namespace
} // fcf namespace

#endif // #ifndef Z___FCF_TEST__TEST_HPP___
