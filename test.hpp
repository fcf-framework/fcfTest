#ifndef ___FCF_TEST__TEST_HPP___
#define ___FCF_TEST__TEST_HPP___

#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#include <streambuf>
#include <cctype>
#include <chrono>
#include <list>
#include <map>
#include <vector>
#include <set>

/**
 * @brief Defines the implementation macro for FCF test functionality.
 * This block ensures that the implementation is only included when the main
 * FCF library is implemented, preventing multiple definition errors.
 */
#ifdef FCF_IMPLEMENTATION
  #ifndef FCF_TEST_IMPLEMENTATION
    #define FCF_TEST_IMPLEMENTATION
  #endif // #ifndef FCF_TEST_IMPLEMENTATION
#endif // #ifdef FCF_IMPLEMENTATION

/**
 * @brief Defines the export macro for FCF test symbols.
 * Inherits the behavior of the main library's `FCF_EXPORT` macro to ensure
 * consistent symbol visibility across different build configurations.
 */
#ifdef FCF_EXPORT
  #ifndef FCF_TEST_EXPORT
    #define FCF_TEST_EXPORT
  #endif // #ifndef FCF_TEST_EXPORT
#endif // #ifdef FCF_EXPORT

/**
 * @brief Defines the import macro for FCF test symbols.
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
#ifndef FCF_TEST_DELC_EXTERN
  #ifdef FCF_TEST_IMPLEMENTATION
    #define FCF_TEST_DELC_EXTERN
  #else
    #define FCF_TEST_DELC_EXTERN extern
  #endif // #ifdef FCF_TEST_IMPLEMENTATION
#endif // #ifndef FCF_TEST_DELC_EXTERN

/**
 * @brief Defines the export/declaration macro for Windows environments.
 * Uses `__declspec(dllexport)` or `__declspec(dllimport)` based on 
 * whether symbols are being exported or imported, otherwise defaults to empty.
 */
#ifndef FCF_TEST_DECL_EXPORT
  #ifdef WIN32
    #if defined(FCF_TEST_EXPORT)
      #define FCF_TEST_DECL_EXPORT __declspec(dllexport)
    #elif defined(FCF_TEST_IMPORT)
      #define FCF_TEST_DECL_EXPORT __declspec(dllimport)
    #else
      #define FCF_TEST_DECL_EXPORT
    #endif // #if defined(FCF_TEST_EXPORT)
  #else
    #define FCF_TEST_DECL_EXPORT
  #endif // #ifdef WIN32
#endif // #ifndef FCF_TEST_DECL_EXPORT

/**
 * @brief Helper macro to generate unique names for internal structures using the COUNTER macro.
 */
#define _FCF_TEST_DECLARE_CCC2(am_x, am_y, am_z)\
            am_x##am_y##am_z
/**
 * @brief Helper macro to generate unique names for internal structures using the COUNTER macro (3 arguments).
 */
#define _FCF_TEST_DECLARE_CCC(am_x, am_y, am_z)\
            _FCF_TEST_DECLARE_CCC2(am_x, am_y, am_z)

/**
 * @brief Helper macro to generate unique names for internal structures using the COUNTER macro (2 arguments).
 */
#define _FCF_TEST_DECLARE_CC2(am_x, am_y)\
            am_x##am_y
/**
 * @brief Helper macro to generate unique names for internal structures using the COUNTER macro (2 arguments).
 */
#define _FCF_TEST_DECLARE_CC(am_x, am_y)\
            _FCF_TEST_DECLARE_CC2(am_x, am_y)

/**
 * @brief Internal implementation helper macro for test declarations.
 * Generates a namespace with a static anonymous class that registers the test upon instantiation.
 * 
 * @param am_className Unique identifier for the generated class.
 * @param am_part The part name of the test group.
 * @param am_group The group name of the test.
 * @param am_test The test function name or identifier.
 */
#define _FCF_TEST_DECLARE_IMPL(am_className, am_part, am_group, am_test)\
  namespace {\
  struct am_className { \
    am_className(){\
      ::fcf::NTest::getStorage().add( ::fcf::NTest::Test{ 0, am_test, 0, am_group, 0, am_part, test } );\
    }\
    static void test();\
  };\
  am_className _FCF_TEST_DECLARE_CCC(am_className, _reg_, __COUNTER__);\
  }\
  void am_className::test()

/**
 * @brief Macro to declare a new test case.
 * Registers the test function with the storage system and assigns it to a group and part.
 * 
 * @param am_part The name of the part (logical grouping level).
 * @param am_group The name of the group (sub-grouping level).
 * @param am_test The name or identifier of the test function.
 */
#define FCF_TEST_DECLARE(am_part, am_group, am_test)\
  _FCF_TEST_DECLARE_IMPL(_FCF_TEST_DECLARE_CC(fcf_test_,__COUNTER__), am_part,  am_group, am_test)

/**
 * @brief Macro to register the order of a test part.
 * Ensures deterministic execution order for parts.
 * 
 * @param am_part The name of the part.
 * @param am_order The desired execution order (integer).
 */
#define FCF_TEST_PART_ORDER(am_part, am_order)\
  namespace {\
    ::fcf::NTest::PartOrderRegisrator _FCF_TEST_DECLARE_CC(fcf_test_order_registrator_, __COUNTER__)(am_part, am_order);\
  }

/**
 * @brief Macro to register the order of a test group.
 * Ensures deterministic execution order for groups within a part.
 * 
 * @param am_group The name of the group.
 * @param am_order The desired execution order (integer).
 */
#define FCF_TEST_GROUP_ORDER(am_group, am_order)\
  namespace {\
    ::fcf::NTest::GroupOrderRegisrator _FCF_TEST_DECLARE_CC(fcf_test_order_registrator_, __COUNTER__)(am_group, am_order);\
  }

/**
 * @brief Macro to register the order of a specific test.
 * Ensures deterministic execution order for tests within a group.
 * 
 * @param am_test The name of the test.
 * @param am_order The desired execution order (integer).
 */
#define FCF_TEST_TEST_ORDER(am_test, am_order)\
  namespace {\
    ::fcf::NTest::TestOrderRegisrator _FCF_TEST_DECLARE_CC(fcf_test_order_registrator_, __COUNTER__)(am_test, am_order);\
  }

namespace fcf {
  namespace NTest {

    namespace NDetails {

      /**
       * @brief A custom stream buffer that discards all characters written to it.
       * Used internally for loggers that are disabled.
       */
      class EmptyStreamBuffer: public std::streambuf {
        protected:
          int_type overflow(int_type a_char) override{
            return a_char;
          }
      };

      /**
       * @brief A custom output stream that writes to an empty buffer (no-op).
       * Used internally for loggers that are disabled.
       */
      class EmptyStream : public std::ostream {
        public:
          EmptyStream() : std::ostream(&_buffer) {
          }
        private:
          EmptyStreamBuffer _buffer;
      };
    }

    /**
     * @brief Enumerates the available log levels.
     */
    enum LogLevel{
      LL_OFF,   ///< No logging.
      LL_FTL,   ///< Fatal level.
      LL_ERR,   ///< Error level.
      LL_WRN,   ///< Warning level.
      LL_ATT,   ///< Attention level.
      LL_LOG,   ///< Log level.
      LL_INF,   ///< Information level.
      LL_DBG,   ///< Debug level.
      LL_TRC,   ///< Trace level.
      LL_ALL,   ///< All levels.
    };

    /**
     * @brief Represents a logging instance with configurable output streams based on level.
     */
    struct Logger {
      public:
        /**
         * @brief Constructs a logger with the default log level (LL_LOG).
         */
        Logger()
          : _level(LL_LOG){
        }

        /**
         * @brief Returns the output stream for fatal messages.
         * @return Reference to the output stream.
         */
        std::ostream& ftl(){
          return _level >= LL_FTL ? (std::ostream&)std::cout : (std::ostream&)_empty;
        }

        /**
         * @brief Returns the output stream for error messages.
         * @return Reference to the output stream.
         */
        std::ostream& err(){
          return _level >= LL_ERR ? (std::ostream&)std::cout : (std::ostream&)_empty;
        }

        /**
         * @brief Returns the output stream for warning messages.
         * @return Reference to the output stream.
         */
        std::ostream& wrn(){
          return _level >= LL_WRN ? (std::ostream&)std::cout : (std::ostream&)_empty;
        }

        /**
         * @brief Returns the output stream for attention messages.
         * @return Reference to the output stream.
         */
        std::ostream& att(){
          return _level >= LL_ATT ? (std::ostream&)std::cout : (std::ostream&)_empty;
        }

        /**
         * @brief Returns the output stream for log messages.
         * @return Reference to the output stream.
         */
        std::ostream& log(){
          return _level >= LL_LOG ? (std::ostream&)std::cout : (std::ostream&)_empty;
        }

        /**
         * @brief Returns the output stream for informational messages.
         * @return Reference to the output stream.
         */
        std::ostream& inf(){
          return _level >= LL_INF ? (std::ostream&)std::cout : (std::ostream&)_empty;
        }

        /**
         * @brief Returns the output stream for debug messages.
         * @return Reference to the output stream.
         */
        std::ostream& dbg(){
          return _level >= LL_DBG ? (std::ostream&)std::cout : (std::ostream&)_empty;
        }

        /**
         * @brief Returns the output stream for trace messages.
         * @return Reference to the output stream.
         */
        std::ostream& trc(){
          return _level >= LL_TRC ? (std::ostream&)std::cout : (std::ostream&)_empty;
        }

        /**
         * @brief Returns the current string representation of the log level.
         * @return Pointer to a static string representing the level name.
         */
        const char* getStrLevel(){
          const char* levels[] = {"off", "ftl", "err", "wrn", "att", "log", "inf", "dbg", "trc"};
          int size = sizeof(levels) / sizeof(levels[0]);
          int level = _level < 0     ? 0 :
                      _level >= size ? size - 1 :
                                      _level;
          return levels[level];
        }

        /**
         * @brief Sets the log level by name.
         * @param a_level Pointer to a string representing the desired log level (e.g., "dbg", "err").
         */
        void setStrLevel(const char* a_level){
          const char* levels[] = {"off", "ftl", "err", "wrn", "att", "log", "inf", "dbg", "trc"};
          int size = sizeof(levels) / sizeof(levels[0]);
          _level = LL_LOG;
          for(int i = 0; i < size; ++i){
            if (std::strcmp(levels[i],a_level) == 0){
              _level = i;
              break;
            }
          }
        }

        /**
         * @brief Returns the current integer value of the log level.
         * @return Integer representation of the log level.
         */
        int getLevel(){
          return _level;
        }

      protected:
        int                   _level; ///< Current log level.
        NDetails::EmptyStream _empty; ///< Empty stream buffer for disabled levels.
    };

    #ifdef FCF_TEST_IMPLEMENTATION
      /**
       * @brief Returns a reference to the global logger instance.
       * Initializes the static logger on first call if not present.
       * @return Reference to the Logger instance.
       */
      FCF_TEST_DECL_EXPORT Logger& logger(){
        static Logger* logger = 0;
        if (!logger){
          logger = new Logger();
        }
        return *logger;
      }
    #else
      /**
       * @brief Declaration for the global logger instance.
       * @return Reference to the Logger instance.
       */
      FCF_TEST_DECL_EXPORT Logger& logger();
    #endif

    /**
     * @brief Returns the output stream for fatal messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline std::ostream& ftl(){
      return logger().ftl();
    }

    /**
     * @brief Returns the output stream for error messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline std::ostream& err(){
      return logger().err();
    }

    /**
     * @brief Returns the output stream for warning messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline std::ostream& wrn(){
      return logger().wrn();
    }

    /**
     * @brief Returns the output stream for attention messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline std::ostream& att(){
      return logger().att();
    }

    /**
     * @brief Returns the output stream for log messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline std::ostream& log(){
      return logger().log();
    }

    /**
     * @brief Returns the output stream for informational messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline std::ostream& inf(){
      return logger().inf();
    }

    /**
     * @brief Returns the output stream for debug messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline std::ostream& dbg(){
      return logger().dbg();
    }

    /**
     * @brief Returns the output stream for trace messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline std::ostream& trc(){
      return logger().trc();
    }

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
      bool operator<(const Test& a_test) const{
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
     * @brief Container for a collection of tests within a group.
     */
    struct Tests{
      typedef std::map<std::string, Test> MapType;
      MapType values; ///< Map of test names to Test objects.
    };

    /**
     * @brief Container for a collection of groups within a part.
     */
    struct Groups{
      typedef std::map<std::string, Tests> MapType;
      MapType values; ///< Map of group names to Tests containers.
    };

    /**
     * @brief Container for a collection of groups across all parts.
     */
    struct Parts{
      typedef std::map<std::string, Groups> MapType;
      MapType values; ///< Map of part names to Groups containers.
    };

    /**
     * @brief Configuration options for running tests.
     */
    struct Options{
      std::vector<std::string> parts;   ///< List of part names to run (empty means all).
      std::vector<std::string> groups;  ///< List of group names to run (empty means all).
      std::vector<std::string> tests;   ///< List of specific test names to run (empty means all).
      std::string              logLevel; ///< Desired logging level.
    };


    /**
     * @brief Central storage for registered tests, parts, and groups.
     */
    struct Storage {
      typedef std::map<std::string, int> OrderMapType;

      Parts        parts;           ///< Map of parts to their groups.
      OrderMapType partOrders;      ///< Execution order for each part.
      OrderMapType groupOrders;     ///< Execution order for each group.
      OrderMapType testOrders;      ///< Execution order for each test.

      /**
       * @brief Adds a new test to the storage, organizing it into parts and groups.
       * 
       * @param a_test The Test object containing metadata and function pointer.
       */
      void add(const Test& a_test) {
        Parts::MapType::iterator partIterator = parts.values.insert( Parts::MapType::value_type(a_test.part, Groups() )  ).first;
        Groups::MapType::iterator groupIterator = partIterator->second.values.insert( Groups::MapType::value_type(a_test.group, Tests() )  ).first;
        groupIterator->second.values[a_test.name] = a_test;
      }
    };

    #ifdef FCF_TEST_IMPLEMENTATION
      /**
       * @brief Returns a reference to the global storage instance.
       * Initializes the static storage on first call if not present.
       * @return Reference to the Storage instance.
       */
      FCF_TEST_DECL_EXPORT Storage& getStorage(){
        static Storage* storage = 0;
        if (!storage){
          storage = new Storage();
        }
        return *storage;
      }
    #else
      /**
       * @brief Declaration for the global storage instance.
       * @return Reference to the Storage instance.
       */
      FCF_TEST_DECL_EXPORT Storage& getStorage();
    #endif

    /**
     * @brief Base class for order registration objects.
     * 
     * @param a_test The test object associated with the registration (used in constructor).
     */
    struct Regisrator {
      Regisrator(const Test& a_test){
        getStorage().add(a_test);
      }
    };

    /**
     * @brief Registers an execution order for a specific part.
     * 
     * @param a_name The name of the part.
     * @param a_order The desired execution order (integer).
     */
    struct PartOrderRegisrator {
      PartOrderRegisrator(const char* a_name, int a_order){
        getStorage().partOrders[a_name] = a_order;
      }
    };

    /**
     * @brief Registers an execution order for a specific group.
     * 
     * @param a_name The name of the group.
     * @param a_order The desired execution order (integer).
     */
    struct GroupOrderRegisrator {
      GroupOrderRegisrator(const char* a_name, int a_order){
        getStorage().groupOrders[a_name] = a_order;
      }
    };

    /**
     * @brief Registers an execution order for a specific test.
     * 
     * @param a_name The name of the test.
     * @param a_order The desired execution order (integer).
     */
    struct TestOrderRegisrator {
      TestOrderRegisrator(const char* a_name, int a_order){
        getStorage().testOrders[a_name] = a_order;
      }
    };

    namespace NDetails {
      
      struct State {
        std::map<std::string, bool> tests;
        std::map<std::string, bool> groups;
        std::map<std::string, bool> parts;
      };
      
      /**
       * @brief Selects tests based on specific criteria and options.
       * 
       * @param a_dst Destination set where selected tests will be inserted.
       * @param a_tests Source map of tests to filter from.
       * @param a_options Configuration options (filters).
       */
      inline void selectTests(std::set<Test>& a_dst, const Tests& a_tests, const Options& a_options, State& a_state){
        if (a_options.tests.empty()){
          for(const Tests::MapType::value_type& item : a_tests.values) {
            Test test(item.second);
            {
              Storage::OrderMapType::const_iterator it = getStorage().partOrders.find(test.part);
              if (it != getStorage().partOrders.end()){
                test.partOrder = it->second;
              }
            }
            {
              Storage::OrderMapType::const_iterator it = getStorage().groupOrders.find(test.group);
              if (it != getStorage().groupOrders.end()){
                test.groupOrder = it->second;
              }
            }
            {
              Storage::OrderMapType::const_iterator it = getStorage().testOrders.find(test.name);
              if (it != getStorage().testOrders.end()){
                test.nameOrder = it->second;
              }
            }
            a_dst.insert(test);
          }
        } else {
          for(const std::string& testName : a_options.tests){
            Tests::MapType::const_iterator it = a_tests.values.find(testName);
            Tests::MapType::const_iterator itEnd = a_tests.values.end();
            if (it != itEnd){
              a_state.tests[testName] = true;
              a_dst.insert(it->second);
            } 
          }
        }
      }

      /**
       * @brief Recursively selects tests based on groups.
       * 
       * @param a_dst Destination set where selected tests will be inserted.
       * @param a_groups Source map of groups to filter from.
       * @param a_options Configuration options (filters).
       */
      inline void selectGroups(std::set<Test>& a_dst, const Groups& a_groups, const Options& a_options, State& a_state){
        if (a_options.groups.empty()){
          for(const Groups::MapType::value_type& item : a_groups.values){
            selectTests(a_dst, item.second, a_options, a_state);
          }
        } else {
          for(const std::string& groupName : a_options.groups){
            Groups::MapType::const_iterator it = a_groups.values.find(groupName);
            Groups::MapType::const_iterator itEnd = a_groups.values.end();
            if (it != itEnd){
              a_state.groups[groupName] = true;
              selectTests(a_dst, it->second, a_options, a_state);
            }
          }
        }
      }

      /**
       * @brief Recursively selects tests based on parts.
       * 
       * @param a_dst Destination set where selected tests will be inserted.
       * @param a_options Configuration options (filters).
       */
      inline void selectParts(std::set<Test>& a_dst, const Options& a_options){
        State state;
        for(const std::string& partName : a_options.parts){
          state.parts[partName] = false;
        }
        for(const std::string& groupName : a_options.groups){
          state.groups[groupName] = false;
        }
        for(const std::string& testName : a_options.tests){
          state.tests[testName] = false;
        }

        if (a_options.parts.empty()){
          for(const Parts::MapType::value_type& item : ::fcf::NTest::getStorage().parts.values){
            selectGroups(a_dst, item.second, a_options, state);
          }
        } else {
          for(const std::string& partName : a_options.parts){
            Parts::MapType::const_iterator it = ::fcf::NTest::getStorage().parts.values.find(partName);
            Parts::MapType::const_iterator itEnd = ::fcf::NTest::getStorage().parts.values.end();
            if (it != itEnd){
              state.parts[partName] = true;
              selectGroups(a_dst, it->second, a_options, state);
            }
          }
        }

        for(const std::string& partName : a_options.parts){
          if (!state.parts[partName]) {
            throw std::runtime_error(std::string() + "The test part named '" + partName + "' cannot be found");
          }
        }
        for(const std::string& groupName : a_options.groups){
          if (!state.groups[groupName]){
            throw std::runtime_error(std::string() + "The test group named '" + groupName + "' cannot be found");
          }
        }
        for(const std::string& testName : a_options.tests){
          if (!state.tests[testName]){
            throw std::runtime_error(std::string() + "The test named '" + testName + "' cannot be found");
          }
        }
      }
    }

    #ifdef FCF_TEST_IMPLEMENTATION
      /**
       * @brief Displays help information for the test runner.
       */
      FCF_TEST_DECL_EXPORT void cmdHelp(){
        std::cout << "Test options:" << std::endl;
        std::cout << "  --test-run  - Run tests" << std::endl;
        std::cout << "  --test-list - Displays a list of all tests" << std::endl;
        std::cout << "  --test-part  PART_NAME - Run only tests from the part. The parameter can be used multiple times" << std::endl;
        std::cout << "  --test-group GROUP_NAME - Run only tests from the group. The parameter can be used multiple times" << std::endl;
        std::cout << "  --test-test  TEST_NAME - Run only this test. The parameter can be used multiple times" << std::endl;
        std::cout << "  --test-log-level LEVEL - Logging level (VALUES: off, ftl, err, wrn, att, log, inf, dbg, trc)"<< std::endl;
        std::cout << "  --test-help  - Help message" << std::endl;
      }
    #else
      /**
       * @brief Declaration for displaying help information.
       */
      FCF_TEST_DECL_EXPORT void cmdHelp();
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      /**
       * @brief  Displays a list of all registered tests.
       */
      FCF_TEST_DECL_EXPORT void cmdList(){
        Options options;
        std::set<Test> tests;
        NDetails::selectParts(tests, options);
        std::cout << "List of tests:" << std::endl;
        for(const Test& test : tests){
          std::cout << "  \"" << test.part << "\" -> \"" << test.group << "\" -> \"" << test.name  << "\""<< std::endl;
        }
      }
    #else
      /**
       * @brief Declaration for displaying a list of all registered tests.
       */
      FCF_TEST_DECL_EXPORT void cmdList();
    #endif


    #ifdef FCF_TEST_IMPLEMENTATION
      /**
       * @brief Executes the selected tests based on provided options.
       * 
       * @param a_options Configuration options specifying which tests to run and logging level.
       * @param a_errorPtr (default = (bool*)0) A pointer to a variable receiving error information. 
       *                                        If an error occurs, the value is set to true.
       *                                        If a null pointer is passed, the function throws an exception.
       */
      FCF_TEST_DECL_EXPORT void run(const Options& a_options, bool* a_errorPtr = 0){
        const char* lastLevel = logger().getStrLevel();
        logger().setStrLevel(a_options.logLevel.c_str());

        if (a_errorPtr){
          *a_errorPtr = false;
        }

        try {
          std::set<Test> tests;
          NDetails::selectParts(tests, a_options);

          for(const Test& test : tests) {
            log() << "Performing the test: \"" + test.part + "\" -> \"" + test.group + "\" -> \"" + test.name + "\" ..." << std::endl;
            test.testFunction();
          }

          log() << std::endl;
          log() << "All tests were completed. Number of tests: " << tests.size() << std::endl;
        } catch(const std::exception& e){
          err() << e.what() << std::endl;
          logger().setStrLevel(lastLevel);
          if (a_errorPtr){
            *a_errorPtr = true;
          } else {
            throw;
          }
        }

        logger().setStrLevel(lastLevel);
      }

    #else
      /**
       * @brief Declaration for executing the selected tests.
       * 
       * @param a_options Configuration options specifying which tests to run and logging level.
       * @param a_errorPtr (default = (bool*)0) A pointer to a variable receiving error information. 
       *                                        If an error occurs, the value is set to true.
       *                                        If a null pointer is passed, the function throws an exception.
       */
      FCF_TEST_DECL_EXPORT void run(const Options& a_options, bool* a_errorPtr = 0);
    #endif

    /**
     * @brief Enumerates command modes for the test runner.
     */
    enum CmdMode{
      CM_NONE,   ///< No specific mode set.
      CM_RUN,    ///< Run tests mode.
      CM_LIST,   ///< List tests mode.
      CM_HELP,   ///< Help mode.
    };

    /**
     * @brief Enumerates modes for command line argument parsing and execution.
     */
    enum CmdRunMode {
      CRM_PARSE,    ///< cmdRun() only parses the command line.
      CRM_EXECUTE,  ///< cmdRun() parses the command line and runs tests if the --test-run flag was passed.
      CRM_RUN,      ///< cmdRun() parses the command line and runs the tests unless the --test-help or --test-list run flags were specified on the command line.
    };

    #ifdef FCF_TEST_IMPLEMENTATION
      /**
       * @brief Parses command line arguments and executes the appropriate action.
       * 
       * @param a_dstOptions Reference to the options structure to populate with parsed arguments.
       * @param a_argc Number of command line arguments.
       * @param a_argv Array of command line arguments.
       * @param a_runMode Current mode of execution (parse, execute, or run).
       * @param a_errorPtr (default = (bool*)0) A pointer to a variable that receives information about a test error.
       *                                        If an error occurs, the value is set to true.
       *                                        If a null pointer is passed, the function throws an exception.
       * @return The determined command mode after processing.
       */
      FCF_TEST_DECL_EXPORT CmdMode cmdRun(Options& a_dstOptions, int a_argc, const char** a_argv, CmdRunMode a_runMode, bool* a_errorPtr = 0){
        CmdMode mode = CM_NONE;

        for(int i = 0; i < a_argc; ++i){
          if (strcmp(a_argv[i], "--test-run") == 0){
            mode = CM_RUN;
          } else if (strcmp(a_argv[i], "--test-help") == 0){
            mode = CM_HELP;
            if (a_runMode == CRM_EXECUTE || a_runMode == CRM_RUN){
              cmdHelp();
              return mode;
            }
          } else if (strcmp(a_argv[i], "--test-log-level") == 0 && (i+1) < a_argc) {
            a_dstOptions.logLevel = a_argv[i+1];
            ++i;
          } else if (strcmp(a_argv[i], "--test-list") == 0){
            mode = CM_LIST;
            if (a_runMode == CRM_EXECUTE || a_runMode == CRM_RUN){
              cmdList();
              return mode;
            }
          } else if (strcmp(a_argv[i], "--test-part") == 0 && (i+1) < a_argc){
            a_dstOptions.parts.push_back(a_argv[i+1]);
            ++i;
          } else if (strcmp(a_argv[i], "--test-group") == 0 && (i+1) < a_argc){
            a_dstOptions.groups.push_back(a_argv[i+1]);
            ++i;
          } else if (strcmp(a_argv[i], "--test-test") == 0 && (i+1) < a_argc){
            a_dstOptions.tests.push_back(a_argv[i+1]);
            ++i;
          }
        }
        if ((mode == CM_RUN && a_runMode == CRM_EXECUTE) || a_runMode == CRM_RUN){
          run(a_dstOptions, a_errorPtr);
        }

        return mode;
      }
    #else
      /**
       * @brief Declaration for parsing command line arguments and executing the appropriate action.
       * 
       * @param a_dstOptions Reference to the options structure to populate with parsed arguments.
       * @param a_argc Number of command line arguments.
       * @param a_argv Array of command line arguments.
       * @param a_runMode Current mode of execution (parse, execute, or run).
       * @param a_errorPtr (default = (bool*)0) A pointer to a variable that receives information about a test error.
       *                                        If an error occurs, the value is set to true.
       *                                        If a null pointer is passed, the function throws an exception.
       * @return The determined command mode after processing.
       */
      FCF_TEST_DECL_EXPORT CmdMode cmdRun(Options& a_dstOptions, int a_argc, const char** a_argv, CmdRunMode a_runMode, bool* a_errorPtr = 0);
    #endif

    /**
     * @brief Parses command line arguments and executes the appropriate action.
     *
     * @param a_argc Number of command line arguments.
     * @param a_argv Array of command line arguments.
     * @param a_runMode Current mode of execution (parse, execute, or run).
     * @param a_errorPtr (default = (bool*)0) A pointer to a variable that receives information about a test error.
     *                                        If an error occurs, the value is set to true.
     *                                        If a null pointer is passed, the function throws an exception.
     * @return The determined command mode after processing.
     */
    inline CmdMode cmdRun(int a_argc, const char** a_argv, CmdRunMode a_runMode, bool* a_errorPtr = 0){
      Options options;
      return cmdRun(options, a_argc, a_argv, a_runMode, a_errorPtr);
    }

    /**
     * @brief Represents a timing duration for benchmarking or performance testing.
     */
    class Duration {
      public:
        /**
         * @brief Constructs a duration object with a specified number of iterations.
         * 
         * @param a_iterations The number of times the enclosed functor will be executed.
         */
        Duration(unsigned long long a_iterations)
          : _iterations(a_iterations){
        }

        /**
         * @brief Constructs a duration object with a default of 1 iteration.
         */
        Duration()
          : _iterations(1){
        }

        /**
         * @brief Returns the number of iterations set for this duration.
         * @return The number of iterations.
         */
        unsigned long long iterations(){
          return _iterations;
        }

        /**
         * @brief Records the start time for timing.
         */
        void begin(){
          _start = std::chrono::high_resolution_clock::now();
        }

        /**
         * @brief Records the end time for timing.
         */
        void end(){
          _end = std::chrono::high_resolution_clock::now();
        }

        /**
         * @brief Executes a functor multiple times and measures the total duration.
         * 
         * @tparam TFunctor The type of the callable object.
         * @param a_functor The callable to execute.
         */
        template <typename TFunctor>
        void operator()(TFunctor&& a_functor){
          begin();
          for(unsigned long long i = 0; i < _iterations; ++i) {
            a_functor();
          }
          end();
        }

        /**
         * @brief Returns the total duration of all iterations in nanoseconds.
         * @return Total duration as nanoseconds.
         */
        std::chrono::nanoseconds totalDuration(){
          return std::chrono::duration_cast<std::chrono::nanoseconds>(_end - _start);
        }

        /**
         * @brief Returns the average duration of a single iteration in nanoseconds.
         * @return Average duration as nanoseconds.
         */
        std::chrono::nanoseconds duration(){
          return std::chrono::duration_cast<std::chrono::nanoseconds>(_end - _start) / _iterations;
        }

      private:
        unsigned long long                              _iterations; ///< Number of iterations to perform.
        std::chrono::high_resolution_clock::time_point _start;       ///< Start timestamp.
        std::chrono::high_resolution_clock::time_point _end;         ///< End timestamp.
    };

  } // NTest namespace
} // fcf namespace


namespace fcf {
  namespace NTest {
    namespace Details {

      /**
       * @brief A template structure to recursively print a pack of arguments with their names.
       * 
       * @tparam TPack The parameter pack to print.
       */
      template <typename... TPack>
      struct PrintPack {
        /**
         * @brief Recursive step to print arguments.
         * 
         * @tparam TIterator Iterator type for the name list.
         * @tparam TArg Type of the current argument.
         * @tparam TPack2 The rest of the parameter pack.
         * @param a_begName Iterator pointing to the beginning of the names.
         * @param a_endName Iterator pointing to the end of the names.
         * @param a_arg The current argument value.
         * @param a_pack The remaining arguments.
         * @return Formatted string with names and values.
         */
        template <typename TIterator, typename TArg, typename... TPack2>
        std::string operator()(TIterator a_begName, TIterator a_endName, const TArg& a_arg, const TPack2&... a_pack){
          std::stringstream ss;
          std::string name = a_begName != a_endName ? *a_begName : "arg";
          ss << "    " << name << ": " << a_arg << std::endl;
          if (a_begName != a_endName){
            ++a_begName;
          }
          return ss.str() + PrintPack<TPack2...>()(a_begName, a_endName, a_pack...);
        }
      };

      /**
       * @brief Specialization of PrintPack for an empty pack.
       * Returns an empty string as the base case for recursion.
       */
      template <>
      struct PrintPack<> {
        /**
         * @brief Base case operator that returns an empty string.
         * 
         * @tparam TIterator Iterator type (unused).
         * @param a_begName Unused iterator.
         * @param a_endName Unused iterator.
         * @return Empty string.
         */
        template <typename TIterator>
        std::string operator()(TIterator /*a_begName*/, TIterator /*a_endName*/){
          return std::string();
        }
      };

      /**
       * @brief A helper structure to manage printing of arguments with names.
       * 
       * @tparam TIterator Iterator type for the name list.
       */
      template <typename TIterator>
      struct PrintArgs{
        TIterator begin; ///< Iterator pointing to the beginning of the names.
        TIterator end;   ///< Iterator pointing to the end of the names.

        /**
         * @brief Executes the print operation for a pack of arguments.
         * 
         * @tparam ...TArgPack Types of the arguments to print.
         * @param ...a_pack The arguments to print.
         * @return Formatted string with names and values.
         */
        template <typename... TArgPack>
        std::string operator()(const TArgPack&... a_pack){
          std::string result;
          if (sizeof...(TArgPack) && begin != end){
            result += "  Values:\n";
          }
          result += PrintPack<TArgPack...>()(begin, end, a_pack...);
          return result;
        }
      };
    } // Details namespace
  } // NTest namespace
} // fcf namespace

/**
 * @brief Macro to convert a macro argument into a string literal.
 */
#define FCF_TEST__STRINGIFY_2(a_arg) #a_arg
/**
 * @brief Helper macro for stringification (1 step).
 */
#define FCF_TEST__STRINGIFY_1(a_arg) FCF_TEST__STRINGIFY_2(a_arg)
/**
 * @brief Macro to convert a macro argument into a string literal.
 */
#define FCF_TEST__STRINGIFY(a_arg)  FCF_TEST__STRINGIFY_1(a_arg)

/**
 * @brief Helper macro to append an item to a list.
 */
#define FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_value) a_list.push_back(a_value);
/**
 * @brief Macro to generate string representations of arguments for error messages.
 */
#define FCF_TEST__APPEND_TO_LIST__EXPAND1(expand1, ...) #expand1
/**
 * @brief Macro to generate string representations of arguments for error messages (2 args).
 */
#define FCF_TEST__APPEND_TO_LIST__EXPAND2(expand1, expand2, ...) #expand2
/**
 * @brief Macro to generate string representations of arguments for error messages (3 args).
 */
#define FCF_TEST__APPEND_TO_LIST__EXPAND3(expand1, expand2, expand3, ...) #expand3
/**
 * @brief Macro to generate string representations of arguments for error messages (4 args).
 */
#define FCF_TEST__APPEND_TO_LIST__EXPAND4(expand1, expand2, expand3, expand4, ...) #expand4
/**
 * @brief Macro to generate string representations of arguments for error messages (5 args).
 */
#define FCF_TEST__APPEND_TO_LIST__EXPAND5(expand1, expand2, expand3, expand4, expand5, ...) #expand5
/**
 * @brief Macro to generate string representations of arguments for error messages (6 args).
 */
#define FCF_TEST__APPEND_TO_LIST__EXPAND6(expand1, expand2, expand3, expand4, expand5, expand6, ...) #expand6
/**
 * @brief Macro to generate string representations of arguments for error messages (7 args).
 */
#define FCF_TEST__APPEND_TO_LIST__EXPAND7(expand1, expand2, expand3, expand4, expand5, expand6, expand7, ...) #expand7
/**
 * @brief Macro to generate string representations of arguments for error messages (8 args).
 */
#define FCF_TEST__APPEND_TO_LIST__EXPAND8(expand1, expand2, expand3, expand4, expand5, expand6, expand7, expand8, ...) #expand8
/**
 * @brief Macro to generate string representations of arguments for error messages (9 args).
 */
#define FCF_TEST__APPEND_TO_LIST__EXPAND9(expand1, expand2, expand3, expand4, expand5, expand6, expand7, expand8, expand9, ...) #expand9
/**
 * @brief Macro to generate string representations of arguments for error messages (10 args).
 */
#define FCF_TEST__APPEND_TO_LIST__EXPAND10(expand1, expand2, expand3, expand4, expand5, expand6, expand7, expand8, expand9, expand10, ...) #expand10

/**
 * @brief Macro to execute the expansion of argument strings for error messages.
 */
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

/**
 * @brief Macro to append variable arguments to a list (variadic).
 */
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

/**
 * @brief Macro to assert a condition and throw an error if it fails.
 * Generates a detailed error message including the failing expression and failed arguments.
 * 
 * @param exp The boolean condition to check.
 * @param ... Variable list of arguments whose values will be included in the error message if 'exp' is false.
 */
#define FCF_TEST(exp, ...) \
  if (!(exp)){ \
    std::list<std::string> names;\
    FCF_TEST__APPEND_TO_LIST(names, __VA_ARGS__)\
    fcf::NTest::Details::PrintArgs<std::list<std::string>::iterator> p;\
    p.begin = names.begin();\
    p.end = names.end();\
    std::string messge = std::string() + \
                         "Test error: " + #exp + "  [FILE: " + __FILE__ + ":" + FCF_TEST__STRINGIFY(__LINE__) + "]\n" + \
                         p(__VA_ARGS__);\
    throw std::runtime_error(messge);\
  }

#endif // #ifndef ___FCF_TEST__TEST_HPP___
