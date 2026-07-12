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
#define FCF_TEST_VERSION_MINOR        1
#define FCF_TEST_VERSION_MAINTENANCE  11

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
 * @brief FCF_TEST_DECLARE. Macro to declare a new test case.
 * Registers the test function with the storage system and assigns it to a group and part.
 *
 * @param am_part The name of the part (logical grouping level).
 * @param am_group The name of the group (sub-grouping level).
 * @param am_test The name or identifier of the test function.
 */
#ifndef FCF_TEST_DECLARE
  #define Z__FCF_TEST_DECLARE_IMPL(am_className, am_part, am_group, am_test)\
    namespace {\
    struct am_className { \
      am_className() {\
        ::fcf::NTest::getStorage().append( ::fcf::NTest::Test{ 1000000, am_test, 1000000, am_group, 1000000, am_part, test } );\
      }\
      static void test();\
    };\
    am_className Z__FCF_TEST__CONCAT3(am_className, _reg_, __COUNTER__);\
    }\
    void am_className::test()

  #define FCF_TEST_DECLARE(am_part, am_group, am_test)\
    Z__FCF_TEST_DECLARE_IMPL(Z__FCF_TEST__CONCAT2(fcf_test_,__COUNTER__), am_part,  am_group, am_test)
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
      ::fcf::NTest::Details::Printer _fcf_test_error_printer(Z__FCF_TEST__STRINGIFY(Z__FCF_TEST__REMOVE_PARENTHESIS(Z__FCF_TEST__REMOVE_PARENTHESIS_ARGUMENT exp)), \
                                         __FILE__, \
                                         Z__FCF_TEST__STRINGIFY(__LINE__)\
                                         Z__FCF_TEST__APPEND_TO_LIST(_fcf_test_names, __VA_ARGS__)\
                                         );\
      std::runtime_error exception(_fcf_test_error_printer(__VA_ARGS__));\
      ::fcf::NTest::state().error(exception.what(), false);\
      throw exception;\
    }

  #define FCF_TEST_CHECK(exp, ...) \
    ::fcf::NTest::Details::Printer(Z__FCF_TEST__STRINGIFY(Z__FCF_TEST__REMOVE_PARENTHESIS(Z__FCF_TEST__REMOVE_PARENTHESIS_ARGUMENT exp)), \
                                       __FILE__, \
                                       Z__FCF_TEST__STRINGIFY(__LINE__)\
                                       Z__FCF_TEST__APPEND_TO_LIST(_fcf_test_names, __VA_ARGS__)\
                                       ).inlineCheck((exp), __VA_ARGS__)
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

    enum ELogMessageCategory {
      LMC_USER                  = 0x0001,
      LMC_START                 = 0x0002,
      LMC_END                   = 0x0004,
      LMC_COMPLETE_NEW_LINE     = 0x0008,
      LMC_COMPLETE              = 0x0010,
      LMC_ERROR_NEW_LINE        = 0x0020,
      LMC_ERROR                 = 0x0040,
      LMC_RESULT                = 0x0080,
      LMC_DURATION              = 0x0100,
      LMC_TEST_START            = 0x0200,
      LMC_TEST_START_MESSAGE    = 0x0400,
      LMC_TEST_COMPLETE         = 0x0800,
      LMC_TEST_ERROR            = 0x1000,
      LMC_TEST_ERROR_MESSAGE    = 0x2000,
      LMC_TEST_END              = 0x4000,
      LMC_RUN_ERROR             = 0x8000,
      LMC_TEST                  = LMC_USER | LMC_TEST_COMPLETE | LMC_TEST_ERROR | LMC_TEST_ERROR_MESSAGE,
      LMC_ALL                   = 0xFFFF,
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
    FCF_TEST_API Storage& getStorage();

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
      int         nameOrder;       ///< Execution order within the test.
      std::string name;            ///< Name of the test function.
      int         groupOrder;      ///< Execution order within the group.
      std::string group;           ///< Name of the test group.
      int         partOrder;       ///< Execution order within the part.
      std::string part;            ///< Name of the test part.
      void (*testFunction)();      ///< Pointer to the test function to execute.

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
               nameOrder < a_test.nameOrder ? true :
               nameOrder > a_test.nameOrder ? false :
               name < a_test.name ? true :
               name > a_test.name ? false :
                                   false;
      }
    };



    /**
     * @brief Configuration options for running tests.
     */
    struct Options {
      struct File {
        std::string  file;
        std::string  format;
      };

      // Not in use, reserved for future use.
      struct Selector {
        std::string  part;
        std::string  group;
        std::string  test;
      };

      std::vector<std::string> parts;           ///< List of part names to run (empty means all).
      std::vector<std::string> groups;          ///< List of group names to run (empty means all).
      std::vector<std::string> tests;           ///< List of specific test names to run (empty means all).
      std::vector<Selector>    selectors;       ///< Not in use, reserved for future use.
      std::vector<std::string> ignoreParts;     ///< List of ignore part names.
      std::vector<std::string> ignoreGroups;    ///< List of ignore group names to run.
      std::vector<std::string> ignoreTests;     ///< List of ignore specific test names to run.
      std::vector<Selector>    ignoreSelectors; ///< Not in use, reserved for future use.
      ELogLevel                logLevel;        ///< Desired logging level.
      std::string              format;
      bool                     noBreak;
      std::vector<File>        files;

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
     * @brief Central storage for registered tests, parts, and groups.
     */
    class FCF_TEST_API Storage {
      public:

        void partOrder(const char* a_name, int a_order);

        void groupOrder(const char* a_name, int a_order);

        void testOrder(const char* a_name, int a_order);

        /**
         * @brief Adds a new test to the storage, organizing it into parts and groups.
         *
         * @param a_test The Test object containing metadata and function pointer.
         */
        void append(const Test& a_test);

        /**
         * @brief Recursively selects tests based on parts.
         *
         * @param a_dst Destination set where selected tests will be inserted.
         * @param a_options Configuration options (filters).
         */
        void select(std::set<Test>& a_dst, const Options& a_options);

      private:
        typedef std::map<std::string, int> OrderMap;

        /**
         * @brief Enumerates permission states for allowing or ignoring tests.
         */
        enum EAllow {
          IGNORE,
          NONE,
          ALLOW,
          FORCE_ALLOW
        };

        /**
         * @brief Internal state used to validate if all requested elements exist.
         */
        struct SearchState {
          public:
            std::map<std::string, bool> tests;
            std::map<std::string, bool> groups;
            std::map<std::string, bool> parts;

            void check();

          private:
            void _check(std::map<std::string, bool>& elements, const char* a_typeName);
        };

        /**
         * @brief Determines the allowance state of a specific name based on allow and ignore lists.
         */
        template <typename TAllowList>
        EAllow checkAllow(EAllow a_allow, const TAllowList& a_allowList, const TAllowList& a_ignoreList, const std::string& a_name);

        /**
         * @brief Populates the search state with requested items and marks them as found if they exist.
         */
        template <typename TMap, typename TAllowList>
        void checkExists(std::map<std::string, bool>& a_state, const TMap& a_map, const TAllowList& a_allowList);

        Parts         _parts;           ///< Map of parts to their groups.
        OrderMap  _partOrders;      ///< Execution order for each part.
        OrderMap  _groupOrders;     ///< Execution order for each group.
        OrderMap  _testOrders;      ///< Execution order for each test.
        std::mutex    _mutex;

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
         * On subsequent calls, it mathematically shifts the global start timestamp forward
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
/* ===                    Testing status                  === */
/* ===                                                    === */
/* ========================================================== */

namespace fcf {
  namespace NTest {

    namespace NDetails {
      FCF_TEST_API void runImpl(const Options& a_options, bool a_enableThrow, bool* a_errorPtr);
    } // NDetails namespace

    class FCF_TEST_API State {
        friend void NDetails::runImpl(const Options& a_options, bool a_enableThrow, bool* a_errorPtr);

      public:
        Test                    test();
        void                    test(const Test& a_test);
        std::set<Test>          tests();
        size_t                  testsCount();
        void                    tests(const std::set<Test>& a_tests);
        Duration                duration();
        void                    duration(const Duration& a_duration);
        void                    error(const char* a_error, bool a_ignoreExists);
        std::list<std::string>  errors();
        void                    errors(const std::list<std::string>& a_errors);

      private:
        void                    _resumeDuration();
        void                    _endDuration();
        Test                   _test;
        std::set<Test>         _tests;
        Duration               _duration;
        std::list<std::string> _errors;
        std::mutex             _mutex;
    };

    FCF_TEST_API State& state();

  } // NTest namespace
} // fcf namespace

/* ========================================================== */
/* ===                                                    === */
/* ===         Helper types for storing user data         === */
/* ===                                                    === */
/* ========================================================== */

namespace fcf {
  namespace NTest {

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
        template <typename Ty, typename... ArgPack>
        static SharedPtrAny make(ArgPack&&... a_args);

        SharedPtrAny() noexcept;
        SharedPtrAny(std::nullptr_t) noexcept;
        SharedPtrAny(const SharedPtrAny& a_source) noexcept;
        SharedPtrAny(SharedPtrAny&& a_source) noexcept;

        ~SharedPtrAny();

        SharedPtrAny& operator=(const SharedPtrAny& a_source) noexcept;
        SharedPtrAny& operator=(SharedPtrAny&& a_source) noexcept;

        template <typename Ty>
        Ty* cast() const noexcept;

        int count() const noexcept;

        explicit operator bool() const noexcept;

        void release();

      private:
        explicit SharedPtrAny(ControlBlockBase* a_block) noexcept;

        ControlBlockBase* _block;
    };

  } // NTest namespace
} // fcf namespace


/* ========================================================== */
/* ===                                                    === */
/* ===               Logging and formatting               === */
/* ===                                                    === */
/* ========================================================== */

namespace fcf {
  namespace NTest {

    class FCF_TEST_API Logger;
    struct LogMessageContext;
    struct LogFormatContext;
    typedef std::function<std::string(Logger&, LogMessageContext&)> LogPrefixFunction;
    typedef std::function<void(Logger&, LogMessageContext&)>        LogFormatFunction;
    struct LogFormatSettings;
    struct LogFormat;
    typedef std::list<LogFormat> LogFormats;
    struct LogPrefixSettings;
    struct LogPrefix;
    typedef std::list<LogPrefix> LogPrefixes;
    struct LogOutputTarget;
    typedef std::list<LogOutputTarget> LogOutputTargets;
    class FCF_TEST_API LogWriter;

    /**
    * @brief Declaration for the global logger instance.
    * @return Reference to the Logger instance.
    */
    FCF_TEST_API Logger& logger();

    /**
     * @brief Returns the output stream for fatal messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline LogWriter ftl();

    /**
     * @brief Returns the output stream for error messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline LogWriter err();

    /**
     * @brief Returns the output stream for warning messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline LogWriter wrn();

    /**
     * @brief Returns the output stream for attention messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline LogWriter att();

    /**
     * @brief Returns the output stream for log messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline LogWriter log();

    /**
     * @brief Returns the output stream for informational messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline LogWriter inf();

    /**
     * @brief Returns the output stream for debug messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline LogWriter dbg();

    /**
     * @brief Returns the output stream for trace messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline LogWriter trc();

    /**
     * @brief The output stream returns for the test inner message. The log recording is always performed.
     * @return Reference to the output stream.
     */
    inline LogWriter sys(ELogMessageCategory a_messageCategory);

    class FCF_TEST_API Logger {
        friend LogWriter;
        friend void NDetails::runImpl(const Options& a_options, bool a_enableThrow, bool* a_errorPtr);

        struct Environment {
          ELogLevel           level;
          std::string         format;
          LogOutputTargets    targets;
        };

      public:

        Logger();

        LogWriter ftl();

        LogWriter err();

        LogWriter wrn();

        LogWriter att();

        LogWriter log();

        LogWriter inf();

        LogWriter dbg();

        LogWriter trc();

        LogWriter sys(ELogMessageCategory a_messageCategory);

        const char* levelStr() const;

        void levelStr(const char* a_level);

        ELogLevel level() const;

        void level(ELogLevel a_level);

        static ELogLevel toLevel(std::string a_level, ELogLevel a_default = LL_LOG);

        static const char* toLevelStr(ELogLevel a_level);

        LogPrefixes prefixes();

        void prefixes(const LogPrefixes& a_prefixes);

        void clearPrefixes(bool a_defaultState = false);

        void appendPrefixStr(const std::string& a_prefix);

        void appendPrefixStr(const std::string& a_prefix, const LogPrefixSettings& a_options);

        void appendPrefixFunc(const LogPrefixFunction& a_prefix);

        void appendPrefixFunc(const LogPrefixFunction& a_prefix, const LogPrefixSettings& a_options);

        void appendPrefix(const LogPrefix& a_prefix);

        LogFormats formats();

        void formats(LogFormats& a_formats);

        void clearFormats(bool a_defaultState = false);

        void appendFormat(const LogFormat& a_format);

        void appendFormatFunc(const LogFormatFunction& a_prefix, const LogFormatSettings& a_options);

        LogOutputTargets targets();

        void targets(const LogOutputTargets& a_targets);

        void clearTargets(bool a_defaultState);

        void appendTarget(const LogOutputTarget& a_stream);

      protected:

        static void _appendTarget(const LogOutputTarget& a_stream, Environment& a_environment );

        Environment _getEnvironment();

        void _setEnvironment(const Environment& a_environment);

        void _write(fcf::NTest::ELogLevel a_level, ELogMessageCategory a_messageCategory, const std::string& a_message);

        LogWriter _log(ELogLevel a_level, ELogMessageCategory a_messageCategory);

        Environment _environment;
        std::list<LogPrefix> _prefixes;
        std::list<LogFormat> _formats;
        std::recursive_mutex    _mutex;
        bool                    _newLine;
    };


    class FCF_TEST_API LogWriter {
      public:
        LogWriter();

        LogWriter(const LogWriter& a_output)  = delete;

        LogWriter(LogWriter&& a_output);

        LogWriter(Logger& a_logger, ELogLevel a_level, ELogMessageCategory a_loggerMessageCategory);

        ~LogWriter();

        template <typename Ty>
        LogWriter& operator<<(const Ty& a_value);

        LogWriter& operator<<(std::ostream& (*a_manipulator)(std::ostream&));

      private:
        Logger*             _logger;
        ELogLevel           _level;
        ELogMessageCategory _loggerMessageCategory;
        std::stringstream   _sstream;
    };


    struct LogMessageContext {
      ELogMessageCategory   category;
      const std::string     origin;
      std::string           message;
      size_t                line;
      ELogLevel             level;
      std::ostream*         stream;
      LogFormatContext*     data;

      LogMessageContext() = delete;
      LogMessageContext(const LogMessageContext&) = delete;
      LogMessageContext& operator=(const LogMessageContext&) = delete;
      LogMessageContext(const std::string& a_message)
        : origin(a_message) {
      }
    };

    struct LogFormatContext {
      std::string         strValue;
      unsigned long long  ullValue;
      void*               ptrValue;
      SharedPtrAny        sptrValue;
    };


    struct LogPrefixSettings {
      std::string         name;
      bool                multiLine;
      unsigned int        messageCategories;
      LogPrefixSettings()
        : name("")
        , multiLine(false)
        , messageCategories(LMC_USER)
      {}
    };

    struct LogFormatSettings {
      std::string         name;
    };

    struct LogFormat {
      LogFormatFunction func;
      LogFormatSettings options;
    };

    struct LogPrefix {
      std::string         str;
      LogPrefixFunction  func;
      LogPrefixSettings options;
    };


    class FCF_TEST_API LogJunitFormatter {
      public:
        static void format(Logger& a_logger, LogMessageContext& a_messageContext);
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

    typedef std::map<std::string, LogFormatContext> LogFormatContextStorage;

    struct LogOutputTarget {
      std::string                     name;
      std::ostream*                   stream;
      std::string                     format;
      LogFormatContextStorage  prefixData;
      LogFormatContextStorage  formatData;
    };



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
          getStorage().append(a_test);
        }
      };

      struct PartOrderRegistrator {
        PartOrderRegistrator(const char* a_name, int a_order) {
          getStorage().partOrder(a_name, a_order);
        }
      };

      struct GroupOrderRegistrator {
        GroupOrderRegistrator(const char* a_name, int a_order) {
          getStorage().groupOrder(a_name, a_order);
        }
      };

      struct TestOrderRegistrator {
        TestOrderRegistrator(const char* a_name, int a_order) {
          getStorage().testOrder(a_name, a_order);
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
        std::cout << "  --test-ignore-part PART_NAME - Exclude tests in the specified part(s). The parameter can be used multiple times" << std::endl;
        std::cout << "  --test-ignore-group GROUP_NAME - Exclude tests in the specified group(s). The parameter can be used multiple times" << std::endl;
        std::cout << "  --test-ignore-test TEST_NAME - Exclude the specified test(s). The parameter can be used multiple times" << std::endl;
        std::cout << "  --test-log-level LEVEL - Logging level (VALUES: def, off, ftl, err, wrn, att, log, inf, dbg, trc, all)" << std::endl;
        std::cout << "  --test-no-break - In case of an error, testing does not stop" << std::endl;
        std::string formats;
        for(auto format : logger().formats()) {
          formats += ", ";
          formats += format.options.name;
        }
        std::cout << "  --test-format FORMAT - Output format (default" + formats + ")." << std::endl;

        std::cout << "  --test-file FILE_PATH - Log file" << std::endl
                  << "                          Use the default format or specify the --test-format parameter" << std::endl;
        std::cout << "  --test-file-default FILE_PATH - Log file (format: default)." << std::endl;
        for(auto format : logger().formats()) {
          std::cout << "  --test-file-" << format.options.name << " FILE_PATH - Log file (format: " << format.options.name << ")." << std::endl;
        }
        std::cout << "  --test-help  - Help message" << std::endl;
        std::cout << std::endl;
        std::cout << "Explanatory details:" << std::endl;
        std::cout << "  1. The --test-part, --test-group, --test-test commands are combined using the OR operation" << std::endl;
        std::cout << "  2. The --test-ignore-part, --test-ignore-group, --test-ignore-test commands are combined using the OR operation" << std::endl;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      FCF_TEST_API void cmdList() {
        Options options;
        std::set<Test> tests;
        getStorage().select(tests, options);
        std::cout << "List of tests:" << std::endl;
        for(const Test& test : tests) {
          std::cout << "  \"" << test.part << "\" -> \"" << test.group << "\" -> \"" << test.name  << "\""<< std::endl;
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
      FCF_TEST_API Storage& getStorage() {
        static Storage* storage = nullptr;
        static std::once_flag flag;

        std::call_once(flag, []() {
          storage = new Storage();
        });

        return *storage;
      }
    #endif


    #ifdef FCF_TEST_IMPLEMENTATION
      Test State::test(){
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
      std::set<Test> State::tests(){
        std::lock_guard<std::mutex> lock(_mutex);
        return _tests;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      size_t State::testsCount(){
        std::lock_guard<std::mutex> lock(_mutex);
        return _tests.size();
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void State::tests(const std::set<Test>& a_tests){
        std::lock_guard<std::mutex> lock(_mutex);
        _tests = a_tests;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      Duration State::duration(){
        std::lock_guard<std::mutex> lock(_mutex);
        return _duration;
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
      std::list<std::string> State::errors(){
        std::lock_guard<std::mutex> lock(_mutex);
        return _errors;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void State::errors(const std::list<std::string>& a_errors){
        std::lock_guard<std::mutex> lock(_mutex);
        _errors = a_errors;
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
      #ifdef FCF_TEST_IMPLEMENTATION

        inline std::vector<std::string> parseArgs(int a_argc, const char* const* a_argv);

        FCF_TEST_API ECmdMode cmdRunImpl(Options& a_dstOptions, int a_argc, const char* const* a_argv, ECmdRunMode a_runMode, bool a_enableThrow, bool* a_errorPtr) {
          ECmdMode mode = CM_NONE;

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
            } else if (args[i] == "--test-log-level" && (i+1) < args.size()) {
              a_dstOptions.logLevel = Logger::toLevel(args[i+1], logger().level());
              ++i;
            } else if (args[i] == "--test-list") {
              mode = CM_LIST;
              if (a_runMode == CRM_EXECUTE || a_runMode == CRM_RUN) {
                cmdList();
                return mode;
              }
            } else if (args[i] == "--test-format" && (i+1) < args.size()) {
              a_dstOptions.format = args[i+1];
              ++i;
            } else if (args[i] == "--test-part" && (i+1) < args.size()) {
              a_dstOptions.parts.push_back(args[i+1]);
              ++i;
            } else if (args[i] == "--test-group" && (i+1) < args.size()) {
              a_dstOptions.groups.push_back(args[i+1]);
              ++i;
            } else if (args[i] == "--test-test" && (i+1) < args.size()) {
              a_dstOptions.tests.push_back(args[i+1]);
              ++i;
            } else if (args[i] == "--test-ignore-part" && (i+1) < args.size()) {
              a_dstOptions.ignoreParts.push_back(args[i+1]);
              ++i;
            } else if (args[i] == "--test-ignore-group" && (i+1) < args.size()) {
              a_dstOptions.ignoreGroups.push_back(args[i+1]);
              ++i;
            } else if (args[i] == "--test-ignore-test" && (i+1) < args.size()) {
              a_dstOptions.ignoreTests.push_back(args[i+1]);
              ++i;
            } else if (args[i] == "--test-no-break") {
              a_dstOptions.noBreak = true;
            } else if (args[i] == "--test-file" && (i+1) < args.size()) {
              a_dstOptions.files.push_back({args[i+1], ""});
              ++i;
            } else if (args[i] == "--test-file-default" && (i+1) < args.size()) {
              a_dstOptions.files.push_back({args[i+1], "default"});
              ++i;
            }
            for(auto format : logger().formats()) {
              std::string param = "--test-file-" + format.options.name;
              if (args[i] == param && (i+1) < args.size()) {
                a_dstOptions.files.push_back({args[i+1], format.options.name});
                ++i;
                break;
              }
            }

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

          std::set<Test>          lastTests = state().tests();
          Test                    lastTest = state().test();
          Duration                lastDuration = state().duration();
          std::list<std::string>  lastErrors = state().errors();

          try {
            logger()._setEnvironment(newEnv);

            std::set<Test> tests;
            getStorage().select(tests, a_options);

            state().duration({});
            state().tests(tests);
            state().test({});
            state().errors({});

            sys(LMC_START);

            unsigned int errorCounter = 0;
            unsigned int passedCounter = 0;
            for(const Test& test : tests) {
              state().test(test);
              state().errors({});
              sys(LMC_TEST_START);
              sys(LMC_TEST_START_MESSAGE) << "Performing the test: \"" + test.part + "\" -> \"" + test.group + "\" -> \"" + test.name + "\" ..." << std::endl;
              state()._resumeDuration();

              try {
                test.testFunction();
                state()._endDuration();
              } catch(std::exception& e) {
                state()._endDuration();
                state().error(e.what(), true);
              }

              std::list<std::string> errors = state().errors();
              if (!errors.size()) {
                ++passedCounter;
                sys(LMC_TEST_COMPLETE) << Z__FCF_TEST_ANSI_SUCCESS << "[SUCCESS]" << Z__FCF_TEST_ANSI_RESET
                                       << " Test completed successfully (" << state().duration().lastTotalDurationStr(true) << " sec)" << std::endl;
                sys(LMC_TEST_END);
              } else {
                totalErrorFlag = true;
                ++errorCounter;
                for(std::string errorMesssage : errors) {
                  errorMesssage.erase(errorMesssage.find_last_not_of(" \t\n\r\f\v") + 1);
                  sys(LMC_TEST_ERROR_MESSAGE) << errorMesssage << std::endl;
                }
                sys(LMC_TEST_ERROR) << Z__FCF_TEST_ANSI_FAILED << "[FAILED]" << Z__FCF_TEST_ANSI_RESET << " Test failed (" << state().duration().lastTotalDurationStr(true) << " sec)" << std::endl;
                sys(LMC_TEST_END);
                if (a_options.noBreak) {
                  continue;
                } else {
                  break;
                }
              }
            }

            unsigned int skippedCounter = tests.size() - passedCounter - errorCounter;

            if (!errorCounter) {
              sys(LMC_COMPLETE_NEW_LINE) << std::endl;
              sys(LMC_COMPLETE) << Z__FCF_TEST_ANSI_SUCCESS << "[SUCCESS]" << Z__FCF_TEST_ANSI_RESET << " All tests were completed." << std::endl;
            } else {
              sys(LMC_ERROR_NEW_LINE) << std::endl;
              sys(LMC_ERROR) << Z__FCF_TEST_ANSI_FAILED << "[FAILED]" << Z__FCF_TEST_ANSI_RESET << " Testing completed with failures." << std::endl;
            }

            sys(LMC_RESULT)   << "Tests: " << passedCounter << " passed, " << errorCounter << " failed, " << skippedCounter << " skipped, " << tests.size() << " total" << std::endl;
            sys(LMC_DURATION) << "Duration: " << state().duration().totalDurationStr(true) << " sec" << std::endl;

            sys(LMC_END);

            state().tests(lastTests);
            state().test(lastTest);
            state().duration(lastDuration);
            state().errors(lastErrors);

            logger()._setEnvironment(lastEnv);

            {
              std::lock_guard<std::recursive_mutex> lock(mutex);
              globalRunState = false;
            }
          } catch(const std::exception& a_error) {
            sys(LMC_RUN_ERROR) << "Error: " << a_error.what() << std::endl;

            state().tests(lastTests);
            state().test(lastTest);
            state().duration(lastDuration);
            state().errors(lastErrors);

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

    namespace Details {

      template <typename... TPack>
      struct PrintPack {
        template <typename TIterator, typename TArg, typename... TPack2>
        std::string operator()(TIterator a_begName, TIterator a_endName, const TArg& a_arg, const TPack2&... a_pack) {
          std::stringstream ss;
          std::string name = a_begName != a_endName ? *a_begName : "arg";
          name = name.length() && (unsigned char)name[0] <= (unsigned char)' ' ? name.substr(1, std::string::npos) : name;
          ss << "    " << name << ": " << a_arg << std::endl;
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
          : expr (a_expr)
          , file(a_file)
          , line(a_line) {
          _appendValue(a_argPack...);
        }

        template <typename... TArgPack>
        std::string operator()(const TArgPack&... a_pack) {
          expr = expr.length() && (unsigned char)expr[0] <= (unsigned char)' ' ? expr.substr(1, std::string::npos) : expr;\
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
            expr = expr.length() && (unsigned char)expr[0] <= (unsigned char)' ' ? expr.substr(1, std::string::npos) : expr;\
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
      };
    } // Details namespace
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
      void Storage::groupOrder(const char* a_name, int a_order) {
        std::lock_guard<std::mutex> lock(_mutex);
        _groupOrders[a_name] = a_order;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Storage::testOrder(const char* a_name, int a_order) {
        std::lock_guard<std::mutex> lock(_mutex);
        _testOrders[a_name] = a_order;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Storage::append(const Test& a_test) {
        std::lock_guard<std::mutex> lock(_mutex);
        Parts::Map::iterator partIterator = _parts.values.insert( Parts::Map::value_type(a_test.part, Groups() )  ).first;
        Groups::Map::iterator groupIterator = partIterator->second.values.insert( Groups::Map::value_type(a_test.group, Tests() )  ).first;
        groupIterator->second.values[a_test.name] = a_test;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Storage::select(std::set<Test>& a_dst, const Options& a_options) {
        std::lock_guard<std::mutex> lock(_mutex);
        SearchState state;
        checkExists(state.parts, _parts.values, a_options.parts);

        for(const auto& partItem : _parts.values) {

          checkExists(state.groups, partItem.second.values, a_options.groups);

          EAllow allowPart = checkAllow(a_options.parts.empty() ? ALLOW : NONE, a_options.parts, a_options.ignoreParts, partItem.first);

          for(const auto& groupItem : partItem.second.values) {

            checkExists(state.tests, groupItem.second.values, a_options.tests);

            EAllow allowGroup = checkAllow(allowPart, a_options.groups, a_options.ignoreGroups, groupItem.first);

            for(const auto& testItem : groupItem.second.values) {
              EAllow allowTest = checkAllow(allowGroup, a_options.tests, a_options.ignoreTests, testItem.first);

              if (allowTest == NONE || allowTest == IGNORE) {
                continue;
              }

              Test test(testItem.second);
              {
                Storage::OrderMap::const_iterator it = _partOrders.find(test.part);
                if (it != _partOrders.end()) {
                  test.partOrder = it->second;
                }
              }
              {
                Storage::OrderMap::const_iterator it = _groupOrders.find(test.group);
                if (it != _groupOrders.end()) {
                  test.groupOrder = it->second;
                }
              }
              {
                Storage::OrderMap::const_iterator it = _testOrders.find(test.name);
                if (it != _testOrders.end()) {
                  test.nameOrder = it->second;
                }
              }
              a_dst.insert(test);
            }
          }
        }

        state.check();
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Storage::SearchState::check() {
        _check(parts, "parts ");
        _check(groups, "groups ");
        _check(tests, "");
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Storage::SearchState::_check(std::map<std::string, bool>& elements, const char* a_typeName) {
        for(const auto& item : elements) {
          if (!item.second) {
            throw std::runtime_error(std::string() + "The test " + a_typeName + "named '" + item.first + "' cannot be found");
          }
        }
      }
    #endif

    template <typename TAllowList>
    typename Storage::EAllow Storage::checkAllow(EAllow a_allow, const TAllowList& a_allowList, const TAllowList& a_ignoreList, const std::string& a_name) {
      if (a_allow == ALLOW) {
        if (!a_allowList.empty()) {
          auto it = std::find(a_allowList.begin(), a_allowList.end(), a_name);
          if (it != a_allowList.end()) {
            a_allow = FORCE_ALLOW;
          } else {
            a_allow = NONE;
          }
        }
      } else if (a_allow == NONE) {
        if (!a_allowList.empty()) {
          auto it = std::find(a_allowList.begin(), a_allowList.end(), a_name);
          if (it != a_allowList.end()) {
            a_allow = FORCE_ALLOW;
          }
        }
      }
      auto it = std::find(a_ignoreList.begin(), a_ignoreList.end(), a_name);
      if (it != a_ignoreList.end()) {
        a_allow = IGNORE;
      }
      return a_allow;
    }

    template <typename TMap, typename TAllowList>
    void Storage::checkExists(std::map<std::string, bool>& a_state, const TMap& a_map, const TAllowList& a_allowList) {
      for (const auto& allowItem : a_allowList) {
        auto stateIt = a_state.insert({allowItem, false}).first;
        if (a_map.find(allowItem) != a_map.end()) {
          stateIt->second = true;
        }
      }
    }

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
    Ty* SharedPtrAny::cast() const noexcept {
      if (_block && _block->type() == typeid(Ty)) {
        return static_cast<Ty*>(_block->ptr());
      }
      return nullptr;
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
      LogWriter Logger::ftl() {
        return _log(LL_FTL, LMC_USER);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      LogWriter Logger::err() {
        return _log(LL_ERR, LMC_USER);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      LogWriter Logger::wrn() {
        return _log(LL_WRN, LMC_USER);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      LogWriter Logger::att() {
        return _log(LL_ATT, LMC_USER);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      LogWriter Logger::log() {
        return _log(LL_LOG, LMC_USER);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      LogWriter Logger::inf() {
        return _log(LL_INF, LMC_USER);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      LogWriter Logger::dbg() {
        return _log(LL_DBG, LMC_USER);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      LogWriter Logger::trc() {
        return _log(LL_TRC, LMC_USER);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      LogWriter Logger::sys(ELogMessageCategory a_messageCategory) {
        return _log(LL_LOG, a_messageCategory);
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
      LogPrefixes Logger::prefixes() {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        LogPrefixes prefixes = _prefixes;
        return prefixes;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::prefixes(const LogPrefixes& a_prefixes) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        clearPrefixes(false);
        for(const LogPrefix& prefix : a_prefixes) {
          appendPrefix(prefix);
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::clearPrefixes(bool a_defaultState) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        _prefixes.clear();
        if (a_defaultState) {
          LogPrefixSettings lpo;
          lpo.name               = "test-offset";
          lpo.multiLine          = true;
          lpo.messageCategories  = LMC_TEST & (~LMC_USER);
          appendPrefixStr("    ", lpo);
          lpo.name               = "user-offset";
          lpo.multiLine          = true;
          lpo.messageCategories  = LMC_USER;
          appendPrefixStr("  > ", lpo);
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::appendPrefixStr(const std::string& a_prefix) {
        LogPrefix prefix;
        prefix.str = a_prefix;
        appendPrefix(prefix);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::appendPrefixStr(const std::string& a_prefix, const LogPrefixSettings& a_options) {
        LogPrefix prefix;
        prefix.str = a_prefix;
        prefix.options = a_options;
        appendPrefix(prefix);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::appendPrefixFunc(const LogPrefixFunction& a_prefix) {
        LogPrefix prefix;
        prefix.func = a_prefix;
        appendPrefix(prefix);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::appendPrefixFunc(const LogPrefixFunction& a_prefix, const LogPrefixSettings& a_options) {
        LogPrefix prefix;
        prefix.func = a_prefix;
        prefix.options = a_options;
        appendPrefix(prefix);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::appendPrefix(const LogPrefix& a_prefix) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        auto existIt = std::find_if(_prefixes.begin(), _prefixes.end(), [&a_prefix](const LogPrefix& a_item) { return a_prefix.options.name == a_item.options.name; });
        if (existIt != _prefixes.end()) {
          *existIt = a_prefix;
        } else {
          _prefixes.push_back(a_prefix);
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      LogFormats Logger::formats() {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        LogFormats formats = _formats;
        return formats;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::formats(LogFormats& a_formats) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        _formats.clear();
        for(const LogFormat& format : a_formats){
          appendFormat(format);
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::clearFormats(bool a_defaultState) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        _formats.clear();
        if (a_defaultState){
          LogFormatSettings lfo;
          lfo.name          = "junit";
          appendFormatFunc(LogJunitFormatter::format, lfo);
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::appendFormat(const LogFormat& a_format) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        auto existIt = std::find_if(_formats.begin(), _formats.end(), [&a_format](const LogFormat& a_item) { return a_format.options.name == a_item.options.name; });
        if (existIt != _formats.end()) {
          *existIt = a_format;
        } else {
          _formats.push_back(a_format);
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::appendFormatFunc(const LogFormatFunction& a_prefix, const LogFormatSettings& a_options) {
        LogFormat format;
        format.func = a_prefix;
        format.options = a_options;
        appendFormat(format);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      LogOutputTargets Logger::targets() {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        LogOutputTargets result(_environment.targets);
        return result;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::targets(const LogOutputTargets& a_targets) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        clearTargets(false);
        for(const LogOutputTarget& stream : a_targets) {
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
      void Logger::appendTarget(const LogOutputTarget& a_stream) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        _appendTarget(a_stream, _environment);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      void Logger::_appendTarget(const LogOutputTarget& a_stream, Environment& a_environment ) {
        LogOutputTargets::iterator it = std::find_if(
                                           a_environment.targets.begin(),
                                           a_environment.targets.end(),
                                           [&a_stream](LogOutputTarget& a_item){
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
      void Logger::_write(fcf::NTest::ELogLevel a_level, ELogMessageCategory a_messageCategory, const std::string& a_message) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);

        for(LogOutputTarget& stream : _environment.targets) {

          LogMessageContext lms(a_message);
          lms.category      = a_messageCategory;
          lms.message       = lms.origin;
          lms.line          = 0;
          lms.level         = a_level;
          lms.stream        = stream.stream ? stream.stream : &std::cout;
          lms.data          = nullptr;

          const std::string& currentFormatName = stream.format.length()
                                                  ? stream.format
                                                  : _environment.format;

          if (lms.origin.length()) {
            for(LogPrefix prefix : _prefixes) {
              if (!(a_messageCategory & prefix.options.messageCategories)) {
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
                size_t pos = prefix.options.multiLine ? currentMessage.find("\n", lastPos)
                                                      : currentMessage.length()-1;
                if (pos == std::string::npos) {
                  pos = currentMessage.length();
                } else {
                  ++pos;
                }
                lms.message = currentMessage.substr(lastPos, pos - lastPos);

                if (prefix.func) {
                  const char* prefixName = prefix.options.name.empty() ? "default" : prefix.options.name.c_str();
                  LogFormatContextStorage::iterator dataIt = stream.prefixData.find(prefixName);
                  if (dataIt == stream.prefixData.end()) {
                    dataIt = stream.prefixData.insert({prefixName, LogFormatContext()}).first;
                  }
                  lms.data = &dataIt->second;

                  std::string prefixPart = prefix.func(*this, lms);
                  if (prefixPart.length()) {
                    lms.message = prefixPart + lms.message;
                  }
                } else {
                  lms.message = prefix.str + lms.message;
                }
                resultMessage += lms.message;
                lastPos = pos;
                ++lms.line;
              }
              std::swap(lms.message, resultMessage);
            }
          }

          lms.line = 0;

          for(LogFormat format : _formats) {
            const char* formatName = format.options.name.empty() ? "default" : format.options.name.c_str();
            if (formatName == currentFormatName) {
              LogFormatContextStorage::iterator dataIt = stream.formatData.find(formatName);
              if (dataIt == stream.formatData.end()) {
                dataIt = stream.formatData.insert({formatName, LogFormatContext()}).first;
              }
              lms.data = &dataIt->second;
              format.func(*this, lms);
            }
          }

          if (lms.message.length()) {
            (*lms.stream) << lms.message;
          }

          if (a_message.length()) {
            _newLine = a_message[a_message.length()-1] == '\n';
          }
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      LogWriter Logger::_log(ELogLevel a_level, ELogMessageCategory a_messageCategory) {
        if (_environment.level >= a_level || a_messageCategory != LMC_USER) {
          return LogWriter(*this, a_level, a_messageCategory);
        } else {
          return LogWriter();
        }
      }
    #endif

  } // NTest namespace
} // fcf namespace



/* -------------------------------- */
/* --       Implementation      --- */
/* ---   fcf::NTest::LogWriter  --- */
/* -------------------------------- */

namespace fcf {
  namespace NTest {

    #ifdef FCF_TEST_IMPLEMENTATION
      LogWriter::LogWriter()
        : _logger(0) {
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      LogWriter::LogWriter(LogWriter&& a_output)
        : _logger((Logger*)a_output._logger)
        , _level(a_output._level)
        , _loggerMessageCategory(a_output._loggerMessageCategory)
        , _sstream(std::move(a_output._sstream)) {
            a_output._logger = nullptr;
        }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      LogWriter::LogWriter(Logger& a_logger, ELogLevel a_level, ELogMessageCategory a_loggerMessageCategory)
        : _logger(&a_logger), _level(a_level), _loggerMessageCategory(a_loggerMessageCategory) {
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      LogWriter::~LogWriter() {
        if (_logger) {
          _logger->_write(_level, _loggerMessageCategory, _sstream.str());
        }
      }
    #endif

    template <typename Ty>
    LogWriter& LogWriter::operator<<(const Ty& a_value) {
      _sstream << a_value;
      return *this;
    }

    #ifdef FCF_TEST_IMPLEMENTATION
      LogWriter& LogWriter::operator<<(std::ostream& (*a_manipulator)(std::ostream&)) {
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
      void LogJunitFormatter::format(Logger& a_logger, LogMessageContext& a_messageContext) {
        std::ostringstream output;

        switch (a_messageContext.category) {
          case LMC_START:
            {
              a_messageContext.data->sptrValue = SharedPtrAny::make<LogJunitFormatter>();
            }
            break;
          case LMC_TEST_COMPLETE:
          case LMC_TEST_ERROR_MESSAGE:
            {
              LogJunitFormatter* formatHandler = a_messageContext.data->sptrValue.cast<LogJunitFormatter>();
              if (formatHandler) {
                ProcessedInfo pi;
                pi.error = a_messageContext.category == LMC_TEST_ERROR_MESSAGE;
                pi.message = a_messageContext.origin;
                pi.duration = state().duration().lastTotalDuration().count();
                formatHandler->_processed.insert({state().test(), pi});
              }
            }
            break;
          case LMC_END:
            {
              LogJunitFormatter* formatHandler = a_messageContext.data->sptrValue.cast<LogJunitFormatter>();
              if (formatHandler) {

                size_t totalTestCount   = state().testsCount();
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
                for(const std::pair< std::string, std::set<Test> >& currentSuite : suites ) {
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
                             << "name=\"" << xmlAttribute(currentTest.name) << "\" "
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
                             << "name=\"" << xmlAttribute(currentTest.name) << "\" "
                             << "time=\"" << Duration::nsToStr(processedIt->second.duration, false) << "\""
                             << ">\n";
                      output << "      <failure message=\"" << xmlAttribute(shortMessage) << "\" type=\"AssertionError\">\n";
                      output << xmlText(message) << "\n";
                      output << "      </failure>\n";
                      output << "    </testcase>\n";
                    } else {
                      output << "    <testcase "
                             << "classname=\"" << xmlAttribute(currentSuiteName) << "\" "
                             << "name=\"" << xmlAttribute(currentTest.name) << "\" "
                             << "time=\"" << Duration::nsToStr(processedIt->second.duration, false) << "\""
                             << "/>\n";
                    }
                  }
                  output << "  </testsuite>\n";
                }
                output << "</testsuites>\n";

                a_messageContext.data->sptrValue.release();
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

    inline LogWriter ftl() {
      return logger().ftl();
    }

    inline LogWriter err() {
      return logger().err();
    }

    inline LogWriter wrn() {
      return logger().wrn();
    }

    inline LogWriter att() {
      return logger().att();
    }

    inline LogWriter log() {
      return logger().log();
    }

    inline LogWriter inf() {
      return logger().inf();
    }

    inline LogWriter dbg() {
      return logger().dbg();
    }

    inline LogWriter trc() {
      return logger().trc();
    }

    LogWriter sys(ELogMessageCategory a_messageCategory) {
      return logger().sys(a_messageCategory);
    }
  } // NTest namespace
} // fcf namespace

#endif // #ifndef Z___FCF_TEST__TEST_HPP___
