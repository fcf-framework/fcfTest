#ifndef ___FCF_TEST__TEST_HPP___
#define ___FCF_TEST__TEST_HPP___

#include <stdexcept>
#include <algorithm>
#include <string>
#include <functional>
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
#include <regex>

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
    enum ELogLevel{
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
     * @brief Represents a logging instance with configurable output streams based on level.
     */
    struct Logger {
      public:

        typedef std::function<std::string(Logger&, ELogLevel)> PrefixFunctionType;

      protected:

        struct Prefix{
          std::string        str;
          PrefixFunctionType func;
        };

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
          return _write(LL_FTL);
        }

        /**
         * @brief Returns the output stream for error messages.
         * @return Reference to the output stream.
         */
        std::ostream& err(){
          return _write(LL_ERR);
        }

        /**
         * @brief Returns the output stream for warning messages.
         * @return Reference to the output stream.
         */
        std::ostream& wrn(){
          return _write(LL_WRN);
        }

        /**
         * @brief Returns the output stream for attention messages.
         * @return Reference to the output stream.
         */
        std::ostream& att(){
          return _write(LL_ATT);
        }

        /**
         * @brief Returns the output stream for log messages.
         * @return Reference to the output stream.
         */
        std::ostream& log(){
          return _write(LL_LOG);
        }

        /**
         * @brief Returns the output stream for informational messages.
         * @return Reference to the output stream.
         */
        std::ostream& inf(){
          return _write(LL_INF);
        }

        /**
         * @brief Returns the output stream for debug messages.
         * @return Reference to the output stream.
         */
        std::ostream& dbg(){
          return _write(LL_DBG);
        }

        /**
         * @brief Returns the output stream for trace messages.
         * @return Reference to the output stream.
         */
        std::ostream& trc(){
          return _write(LL_TRC);
        }

        /**
         * @brief The output stream returns for the test inner message. The log recording is always performed.
         * @return Reference to the output stream.
         */
        std::ostream& tst(){
          return (std::ostream&)std::cout;
        }

        /**
         * @brief Returns the current string representation of the log level.
         * @return Pointer to a static string representing the level name.
         */
        const char* getLevelStr() const{
          return toLevelStr(_level);
        }

        /**
         * @brief Sets the log level by name.
         * @param a_level Pointer to a string representing the desired log level (e.g., "dbg", "err").
         */
        void setLevelStr(const char* a_level){
          setLevel(toLevel(a_level));
        }

        /**
         * @brief Returns the current ELogLevel value of the log level.
         * @return ELogLevel representation of the log level.
         */
        ELogLevel getLevel() const{
          return _level;
        }

        /**
         * @brief Sets the log level.
         * @param a_level An ELogLevel value representing the desired logging level.
         * @throw std::runtime_error throws if the value passed is LL__DEF
         */
        void setLevel(ELogLevel a_level){
          if (a_level == LL_DEF) {
            throw std::runtime_error("LL_DEF value cannot be set as primary value");
          }
          _level = a_level;
        }

        /**
         * @brief Converts a string representation of a log level to its enum value.
         * @param a_level   The string to convert.
         * @param a_default This value is set if the unacceptable value or the value of "def" is introduced.
         * @return The corresponding ELogLevel enum.
         */
        static ELogLevel toLevel(std::string a_level, ELogLevel a_default = LL_LOG) {
          const char* levels[] = {"def", "off", "ftl", "err", "wrn", "att", "log", "inf", "dbg", "trc", "all"};
          a_default = a_default == LL_DEF ? LL_LOG : a_default;
          int size = sizeof(levels) / sizeof(levels[0]);
          for(int i = 0; i < size; ++i){
            int l = i - 1;
            if (a_level == levels[i]){
              return l == LL_DEF ? a_default : (ELogLevel)l;
            }
          }
          return a_default;
        }

        /**
         * @brief Converts a ELogLevel enum value to its string representation.
         * @param a_level The log level to convert.
         * @return A pointer to a static string representing the level name.
         */
        static const char* toLevelStr(ELogLevel a_level){
          const char* levels[] = {"def", "off", "ftl", "err", "wrn", "att", "log", "inf", "dbg", "trc", "all"};
          int size  = sizeof(levels) / sizeof(levels[0]);
          int level = (int)a_level + 1;
          level = a_level < 0     ? 0 :
                  a_level >= size ? size - 1 :
                                    level;
          return levels[level];
        }

        /**
         * @brief Adds a static string prefix to all log messages.
         * @param a_prefix The string to append as a prefix.
         */
        void addedPrefixStr(const std::string& a_prefix){
          Prefix prefix;
          prefix.str = a_prefix;
          _prefixes.push_back(prefix);
        }

        /**
         * @brief Adds a functional prefix to all log messages.
         * @param a_prefix A function that returns a string to be used as a prefix.
         */
        void addedPrefixFunc(const PrefixFunctionType& a_prefix){
          Prefix prefix;
          prefix.func = a_prefix;
          _prefixes.push_back(prefix);
        }

      protected:
        /**
         * @brief Internal method to handle writing to the appropriate stream based on the log level.
         * @param a_forLevel The level of the message being written.
         * @return Reference to the output stream (either std::cout or an empty stream).
         */
        std::ostream& _write(int a_forLevel){
          if (_level >= a_forLevel) {
            for(Prefix prefix : _prefixes){
              if (prefix.func){
                std::cout << prefix.func(*this, (ELogLevel)a_forLevel);
              } else {
                std::cout << prefix.str;
              }
            }
            return (std::ostream&)std::cout;
          } else {
            return (std::ostream&)_empty;
          }
        }
        ELogLevel              _level; ///< Current log level.
        NDetails::EmptyStream _empty; ///< Empty stream buffer for disabled levels.
        std::list<Prefix>     _prefixes;
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
     * @brief The output stream returns for the test inner message. The log recording is always performed.
     * @return Reference to the output stream.
     */
    inline std::ostream& tst(){
      return logger().tst();
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
      std::vector<std::string> parts;         ///< List of part names to run (empty means all).
      std::vector<std::string> groups;        ///< List of group names to run (empty means all).
      std::vector<std::string> tests;         ///< List of specific test names to run (empty means all).
      std::vector<std::string> ignoreParts;   ///< List of ignore part names.
      std::vector<std::string> ignoreGroups;  ///< List of ignore group names to run.
      std::vector<std::string> ignoreTests;   ///< List of ignore specific test names to run.
      ELogLevel                logLevel;      ///< Desired logging level.

      Options() 
        : logLevel(LL_DEF){
      }
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

    namespace NDetails {
      /**
       * @brief Selects tests based on the provided options.
       * @param a_dst Destination set to store selected tests.
       * @param a_options Configuration options for filtering.
       */
      inline void select(std::set<Test>& a_dst, const Options& a_options);
    }

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
        std::cout << "  --test-ignore-part PART_NAME - Exclude tests in the specified part(s). The parameter can be used multiple times" << std::endl;
        std::cout << "  --test-ignore-group GROUP_NAME - Exclude tests in the specified group(s). The parameter can be used multiple times" << std::endl;
        std::cout << "  --test-ignore-test TEST_NAME - Exclude the specified test(s). The parameter can be used multiple times" << std::endl;
        std::cout << "  --test-log-level LEVEL - Logging level (VALUES: def, off, ftl, err, wrn, att, log, inf, dbg, trc, all)" << std::endl;
        std::cout << "  --test-help  - Help message" << std::endl;
      }
    #else
      /**
       * @brief Declaration for displaying a list of all registered tests.
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
        NDetails::select(tests, options);
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
        const char* lastLevel = logger().getLevelStr();
        if (a_options.logLevel != LL_DEF) {
          logger().setLevel(a_options.logLevel);
        }

        if (a_errorPtr){
          *a_errorPtr = false;
        }

        try {
          std::set<Test> tests;
          NDetails::select(tests, a_options);

          for(const Test& test : tests) {
            tst() << "Performing the test: \"" + test.part + "\" -> \"" + test.group + "\" -> \"" + test.name + "\" ..." << std::endl;
            test.testFunction();
          }

          tst() << std::endl;
          tst() << "All tests were completed. Number of tests: " << tests.size() << std::endl;
        } catch(const std::exception& e){
          tst() << e.what() << std::endl;
          logger().setLevelStr(lastLevel);
          if (a_errorPtr){
            *a_errorPtr = true;
          } else {
            throw;
          }
        }

        logger().setLevelStr(lastLevel);
      }

    #else
      /**
       * @brief Declaration for executing the selected tests.
       *
       * @param a_options Configuration options specifying which tests to run and logging level.
       * @param a_errorPtr (default = (bool*)0) A pointer to a variable receiving error information.
       *                                       If an error occurs, the value is set to true.
       *                                       If a null pointer is passed, the function throws an exception.
       */
      FCF_TEST_DECL_EXPORT void run(const Options& a_options, bool* a_errorPtr = 0);
    #endif

    /**
     * @brief Enumerates command modes for the test runner.
     */
    enum ECmdMode{
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

    #ifdef FCF_TEST_IMPLEMENTATION

      namespace NDetails {
        /**
         * @brief Parses command line arguments into a vector of strings.
         * @param a_argc Number of arguments.
         * @param a_argv Array of argument strings.
         * @return A vector containing parsed arguments.
         */
        inline std::vector<std::string> parseArgs(int a_argc, const char* const* a_argv);
      } // NDetails namespace


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
      FCF_TEST_DECL_EXPORT ECmdMode cmdRun(Options& a_dstOptions, int a_argc, const char* const* a_argv, ECmdRunMode a_runMode, bool* a_errorPtr = 0){
        ECmdMode mode = CM_NONE;

        std::vector<std::string> args = NDetails::parseArgs(a_argc, (const char* const*)a_argv);

        for(int i = 0; i < args.size(); ++i){
          if (args[i] == "--test-run"){
            mode = CM_RUN;
          } else if (args[i] == "--test-help"){
            mode = CM_HELP;
            if (a_runMode == CRM_EXECUTE || a_runMode == CRM_RUN){
              cmdHelp();
              return mode;
            }
          } else if (args[i] == "--test-log-level" && (i+1) < args.size()) {
            a_dstOptions.logLevel = Logger::toLevel(args[i+1], logger().getLevel());
            ++i;
          } else if (args[i] == "--test-list"){
            mode = CM_LIST;
            if (a_runMode == CRM_EXECUTE || a_runMode == CRM_RUN){
              cmdList();
              return mode;
            }
          } else if (args[i] == "--test-part" && (i+1) < args.size()){
            a_dstOptions.parts.push_back(args[i+1]);
            ++i;
          } else if (args[i] == "--test-group" && (i+1) < args.size()){
            a_dstOptions.groups.push_back(args[i+1]);
            ++i;
          } else if (args[i] == "--test-test" && (i+1) < args.size()){
            a_dstOptions.tests.push_back(args[i+1]);
            ++i;
          } else if (args[i] == "--test-ignore-part" && (i+1) < args.size()){
            a_dstOptions.ignoreParts.push_back(args[i+1]);
            ++i;
          } else if (args[i] == "--test-ignore-group" && (i+1) < args.size()){
            a_dstOptions.ignoreGroups.push_back(args[i+1]);
            ++i;
          } else if (args[i] == "--test-ignore-test" && (i+1) < args.size()){
            a_dstOptions.ignoreTests.push_back(args[i+1]);
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
      FCF_TEST_DECL_EXPORT ECmdMode cmdRun(Options& a_dstOptions, int a_argc, const char* const* a_argv, ECmdRunMode a_runMode, bool* a_errorPtr = 0);
    #endif

    template <typename Ty>
    ECmdMode cmdRun(Options& a_dstOptions, int a_argc, Ty a_argv, ECmdRunMode a_runMode, bool* a_errorPtr = 0){
      return cmdRun(a_dstOptions, a_argc, (const char* const*)a_argv, a_runMode, a_errorPtr);
    }

    /**
     * @brief Parses command line arguments and executes the appropriate action.
     *
     * @param a_argc Number of command line arguments.
     * @param a_argv Array of command line arguments.
     * @param a_runMode Current mode of execution (parse, execute, or run).
     * @param a_errorPtr (default = (bool*)0) A pointer to a variable that receives information about a test error.
     *                                        If an error occurs, the value is set to true.
     *                                        If a null pointer is passed, the function throws an exception.
     *                                       If an error occurs, the value is set to true.
     * @return The determined command mode after processing.
     */
    template <typename Ty>
    inline ECmdMode cmdRun(int a_argc, Ty a_argv, ECmdRunMode a_runMode, bool* a_errorPtr = 0){
      Options options;
      return cmdRun(options, a_argc, (const char* const*)a_argv, a_runMode, a_errorPtr);
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
       *
       * @tparam TIterator Unused iterator type.
       */
      template <>
      struct PrintPack<> {
        /**
         * @brief Base case operator that returns an empty string.
         *
         * @tparam TIterator Unused iterator type.
         * @param a_begName Unused iterator.
         * @param a_endName Unused iterator.
         * @return An empty string.
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
#define _FCF_TEST__STRINGIFY_2(a_arg) #a_arg
/**
 * @brief Helper macro for stringification (1 step).
 */
#define _FCF_TEST__STRINGIFY_1(a_arg) _FCF_TEST__STRINGIFY_2(a_arg)
/**
 * @brief Macro to convert a macro argument into a string literal.
 */
#define _FCF_TEST__STRINGIFY(a_arg)  _FCF_TEST__STRINGIFY_1(a_arg)

/**
 * @brief Helper macro to append an item to a list.
 */
#define _FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_value) a_list.push_back(a_value);
/**
 * @brief Macro to generate string representations of arguments for error messages.
 */
#define _FCF_TEST__APPEND_TO_LIST__EXPAND1(expand1, ...) #expand1
/**
 * @brief Macro to generate string representations of arguments for error messages (2 args).
 */
#define _FCF_TEST__APPEND_TO_LIST__EXPAND2(expand1, expand2, ...) #expand2
/**
 * @brief Macro to generate string representations of arguments for error messages (3 args).
 */
#define _FCF_TEST__APPEND_TO_LIST__EXPAND3(expand1, expand2, expand3, ...) #expand3
/**
 * @brief Macro to generate string representations of arguments for error messages (4 args).
 */
#define _FCF_TEST__APPEND_TO_LIST__EXPAND4(expand1, expand2, expand3, expand4, ...) #expand4
/**
 * @brief Macro to generate string representations of arguments for error messages (5 args).
 */
#define _FCF_TEST__APPEND_TO_LIST__EXPAND5(expand1, expand2, expand3, expand4, expand5, ...) #expand5
/**
 * @brief Macro to generate string representations of arguments for error messages (6 args).
 */
#define _FCF_TEST__APPEND_TO_LIST__EXPAND6(expand1, expand2, expand3, expand4, expand5, expand6, ...) #expand6
/**
 * @brief Macro to generate string representations of arguments for error messages (7 args).
 */
#define _FCF_TEST__APPEND_TO_LIST__EXPAND7(expand1, expand2, expand3, expand4, expand5, expand6, expand7, ...) #expand7
/**
 * @brief Macro to generate string representations of arguments for error messages (8 args).
 */
#define _FCF_TEST__APPEND_TO_LIST__EXPAND8(expand1, expand2, expand3, expand4, expand5, expand6, expand7, expand8, ...) #expand8
/**
 * @brief Macro to generate string representations of arguments for error messages (9 args).
 */
#define _FCF_TEST__APPEND_TO_LIST__EXPAND9(expand1, expand2, expand3, expand4, expand5, expand6, expand7, expand8, expand9, ...) #expand9
/**
 * @brief Macro to generate string representations of arguments for error messages (10 args).
 */
#define _FCF_TEST__APPEND_TO_LIST__EXPAND10(expand1, expand2, expand3, expand4, expand5, expand6, expand7, expand8, expand9, expand10, ...) #expand10

/**
 * @brief Macro to execute the expansion of argument strings for error messages.
 */
#define _FCF_TEST__APPEND_TO_LIST__EXECUTOR(a_list, a_a1, a_a2, a_a3, a_a4, a_a5, a_a6, a_a7, a_a8, a_a9, a_a10, ...) \
                      _FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a1) \
                      _FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a2) \
                      _FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a3) \
                      _FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a4) \
                      _FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a5) \
                      _FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a6) \
                      _FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a7) \
                      _FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a8) \
                      _FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a9) \
                      _FCF_TEST__APPEND_TO_LIST__APPEND_ITEM(a_list, a_a10)

/**
 * @brief Macro to append variable arguments to a list (variadic).
 */
#define _FCF_TEST__APPEND_TO_LIST(a_list, ...)  _FCF_TEST__APPEND_TO_LIST__EXECUTOR(a_list, \
                                                            _FCF_TEST__APPEND_TO_LIST__EXPAND1(__VA_ARGS__, , , , , , , , , , , ),\
                                                            _FCF_TEST__APPEND_TO_LIST__EXPAND2(__VA_ARGS__, , , , , , , , , , , ),\
                                                            _FCF_TEST__APPEND_TO_LIST__EXPAND3(__VA_ARGS__, , , , , , , , , , , ),\
                                                            _FCF_TEST__APPEND_TO_LIST__EXPAND4(__VA_ARGS__, , , , , , , , , , , ),\
                                                            _FCF_TEST__APPEND_TO_LIST__EXPAND5(__VA_ARGS__, , , , , , , , , , , ),\
                                                            _FCF_TEST__APPEND_TO_LIST__EXPAND6(__VA_ARGS__, , , , , , , , , , , ),\
                                                            _FCF_TEST__APPEND_TO_LIST__EXPAND7(__VA_ARGS__, , , , , , , , , , , ),\
                                                            _FCF_TEST__APPEND_TO_LIST__EXPAND8(__VA_ARGS__, , , , , , , , , , , ),\
                                                            _FCF_TEST__APPEND_TO_LIST__EXPAND9(__VA_ARGS__, , , , , , , , , , , ),\
                                                            _FCF_TEST__APPEND_TO_LIST__EXPAND10(__VA_ARGS__, , , , , , , , , , , )\
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
    _FCF_TEST__APPEND_TO_LIST(names, __VA_ARGS__)\
    fcf::NTest::Details::PrintArgs<std::list<std::string>::iterator> p;\
    p.begin = names.begin();\
    p.end = names.end();\
    std::string messge = std::string() + \
                         "Test error: " + #exp + "  [FILE: " + __FILE__ + ":" + _FCF_TEST__STRINGIFY(__LINE__) + "]\n" + \
                         p(__VA_ARGS__);\
    throw std::runtime_error(messge);\
  }



namespace fcf {
  namespace NTest {
    namespace NDetails {

      /**
       * @brief Parses a single string into a vector of arguments (flags and values).
       * @param a_dstVector The vector to populate with parsed arguments.
       * @param a_input The input string to parse.
       */
      inline void parseArgs(std::vector<std::string>& a_dstVector, std::string a_input) {
        if (a_input == "="){
          return;
        }
        if (!a_input.empty()){
          size_t pos = a_input.find("=");
          if (pos != std::string::npos){
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

      /**
       * @brief Enumerates permission states for allowing/ignoring tests.
       */
      enum EAllow {
        IGNORE,
        NONE,
        ALLOW,
        FORCE_ALLOW
      };

      /**
       * @brief Internal state used to validate if all requested tests/groups/parts exist.
       */
      struct SearchState {
        public:
          std::map<std::string, bool> tests;
          std::map<std::string, bool> groups;
          std::map<std::string, bool> parts;

          /**
           * @brief Validates that all elements in the state maps were actually found.
           * @throws std::runtime_error if any requested element is missing.
           */
          void check() {
            _check(parts, "parts ");
            _check(groups, "groups ");
            _check(tests, "");
          }

        private:
          /**
           * @brief Helper to iterate through a map and check for missing elements.
           * @param a_elements The map to check.
           * @param a_typeName Name of the type for error reporting.
           */
          void _check(std::map<std::string, bool>& elements, const char* a_typeName) {
            for(const auto& item : elements){
              if (!item.second) {
                throw std::runtime_error(std::string() + "The test " + a_typeName + "named '" + item.first + "' cannot be found");
              }
            }
          }
        };

      /**
       * @brief Determines the allowance state of a specific name based on allow and ignore lists.
       * @param a_allow Current allowance state.
       * @param a_allowList List of names that are explicitly allowed.
       * @param a_ignoreList List of names that are explicitly ignored.
       * @param a_name The name to check.
       * @return The updated EAllow state.
       */
      template <typename TAllowList>
      EAllow checkAllow(EAllow a_allow, const TAllowList& a_allowList, const TAllowList& a_ignoreList, const std::string& a_name){
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

      /**
       * @brief Populates the search state with requested items and marks them as found if they exist in the storage.
       * @param a_state The search state to update.
       * @param a_map The actual storage map to check against.
       * @param a_allowList The list of items to look for.
       */
      template <typename TMap, typename TAllowList>
      void checkExists(std::map<std::string, bool>& a_state, const TMap& a_map, const TAllowList& a_allowList){
        for (const auto& allowItem : a_allowList) {
          auto stateIt = a_state.insert({allowItem, false}).first;
          if (a_map.find(allowItem) != a_map.end()) {
            stateIt->second = true;
          }
        }
      }

      /**
       * @brief Recursively selects tests based on parts.
       *
       * @param a_dst Destination set where selected tests will be inserted.
       * @param a_options Configuration options (filters).
       */
      inline void select(std::set<Test>& a_dst, const Options& a_options){
        SearchState state;
        checkExists(state.parts, getStorage().parts.values, a_options.parts);

        for(const auto& partItem : getStorage().parts.values) {

          checkExists(state.groups, partItem.second.values, a_options.groups);

          EAllow allowPart = checkAllow(a_options.parts.empty() ? ALLOW : NONE, a_options.parts, a_options.ignoreParts, partItem.first);

          for(const auto& groupItem : partItem.second.values) {

            checkExists(state.tests, groupItem.second.values, a_options.tests);

            EAllow allowGroup = checkAllow(allowPart, a_options.groups, a_options.ignoreGroups, groupItem.first);

            for(const auto& testItem : groupItem.second.values) {
              EAllow allowTest = checkAllow(allowGroup, a_options.tests, a_options.ignoreTests, testItem.first);

              if (allowTest == NONE || allowTest == IGNORE){
                continue;
              }

              Test test(testItem.second);
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
          }
        }

        state.check();
      }
    } // NDetails namespace
  } // NTest namespace
} // fcf namespace
#endif // #ifndef ___FCF_TEST__TEST_HPP___
