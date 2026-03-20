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

#ifdef FCF_IMPLEMENTATION
  #ifndef FCF_TEST_IMPLEMENTATION
    #define FCF_TEST_IMPLEMENTATION
    #endif // #ifndef FCF_TEST_IMPLEMENTATION
#endif // #ifdef FCF_IMPLEMENTATION

#ifdef FCF_EXPORT
  #ifndef FCF_TEST_EXPORT
    #define FCF_TEST_EXPORT
  #endif // #ifndef FCF_TEST_EXPORT
#endif // #ifdef FCF_EXPORT

#ifdef FCF_IMPORT
  #ifndef FCF_TEST_IMPORT
    #define FCF_TEST_IMPORT
  #endif // #ifndef FCF_TEST_IMPORT
#endif // #ifdef FCF_IMPORT

#ifndef FCF_TEST_DELC_EXTERN
  #ifdef FCF_TEST_IMPLEMENTATION
    #define FCF_TEST_DELC_EXTERN
  #else
    #define FCF_TEST_DELC_EXTERN extern
  #endif // #ifdef FCF_TEST_IMPLEMENTATION
#endif // #ifndef FCF_TEST_DELC_EXTERN

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

#define _FCF_TEST_DECLARE_CCC2(am_x, am_y, am_z)\
            am_x##am_y##am_z
#define _FCF_TEST_DECLARE_CCC(am_x, am_y, am_z)\
            _FCF_TEST_DECLARE_CCC2(am_x, am_y, am_z)

#define _FCF_TEST_DECLARE_CC2(am_x, am_y)\
            am_x##am_y
#define _FCF_TEST_DECLARE_CC(am_x, am_y)\
            _FCF_TEST_DECLARE_CC2(am_x, am_y)\

#define _FCF_TEST_DECLARE_IMPL(am_className, am_part, am_group, am_test)\
  namespace {\
  struct am_className { \
    am_className(){\
      ::fcf::NTest::getStorage().add(  ::fcf::NTest::Test{ 0, am_test, 0, am_group, 0, am_part, test } );\
    }\
    static void test();\
  };\
  am_className _FCF_TEST_DECLARE_CCC(am_className, _reg_, __COUNTER__);\
  }\
  void am_className::test()


#define FCF_TEST_DECLARE(am_part, am_group, am_test)\
  _FCF_TEST_DECLARE_IMPL(_FCF_TEST_DECLARE_CC(fcf_test_,__COUNTER__), am_part,  am_group, am_test)

#define FCF_TEST_PART_ORDER(am_part, am_order)\
  namespace {\
    ::fcf::NTest::PartOrderRegisrator _FCF_TEST_DECLARE_CC(fcf_test_order_registrator_, __COUNTER__)(am_part, am_order);\
  }

#define FCF_TEST_GROUP_ORDER(am_group, am_order)\
  namespace {\
    ::fcf::NTest::GroupOrderRegisrator _FCF_TEST_DECLARE_CC(fcf_test_order_registrator_, __COUNTER__)(am_group, am_order);\
  }

#define FCF_TEST_TEST_ORDER(am_test, am_order)\
  namespace {\
    ::fcf::NTest::TestOrderRegisrator _FCF_TEST_DECLARE_CC(fcf_test_order_registrator_, __COUNTER__)(am_test, am_order);\
  }

namespace fcf {
  namespace NTest {

    namespace NDetails {

      class EmptyStreamBuffer: public std::streambuf {
        protected:
          int_type overflow(int_type a_char) override{
            return a_char;
          }
      };

      class EmptyStream : public std::ostream {
        public:
          EmptyStream() : std::ostream(&_buffer) {
          }
        private:
          EmptyStreamBuffer _buffer;
      };
    }

    enum LogLevel{
      LL_OFF,
      LL_FTL,
      LL_ERR,
      LL_WRN,
      LL_ATT,
      LL_LOG,
      LL_INF,
      LL_DBG,
      LL_TRC,
      LL_ALL,
    };

    struct Logger {
      public:
        Logger()
          : _level(LL_LOG){
        }

        std::ostream& ftl(){
          return _level >= LL_FTL ? (std::ostream&)std::cout : (std::ostream&)_empty;
        }

        std::ostream& err(){
          return _level >= LL_ERR ? (std::ostream&)std::cout : (std::ostream&)_empty;
        }

        std::ostream& wrn(){
          return _level >= LL_WRN ? (std::ostream&)std::cout : (std::ostream&)_empty;
        }

        std::ostream& att(){
          return _level >= LL_ATT ? (std::ostream&)std::cout : (std::ostream&)_empty;
        }

        std::ostream& log(){
          return _level >= LL_LOG ? (std::ostream&)std::cout : (std::ostream&)_empty;
        }

        std::ostream& inf(){
          return _level >= LL_INF ? (std::ostream&)std::cout : (std::ostream&)_empty;
        }

        std::ostream& dbg(){
          return _level >= LL_DBG ? (std::ostream&)std::cout : (std::ostream&)_empty;
        }

        std::ostream& trc(){
          return _level >= LL_TRC ? (std::ostream&)std::cout : (std::ostream&)_empty;
        }

        const char* getStrLevel(){
          const char* levels[] = {"off", "ftl", "err", "wrn", "att", "log", "inf", "dbg", "trc"};
          int size = sizeof(levels) / sizeof(levels[0]);
          int level = _level < 0     ? 0 :
                      _level >= size ? size - 1 :
                                      _level;
          return levels[level];
        }

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

        int getLogger(){
          return _level;
        }

      protected:
        int                   _level;
        NDetails::EmptyStream _empty;
    };

    #ifdef FCF_TEST_IMPLEMENTATION
      FCF_TEST_DECL_EXPORT Logger& logger(){
        static Logger* logger = 0;
        if (!logger){
          logger = new Logger();
        }
        return *logger;
      }
    #else
      FCF_TEST_DECL_EXPORT Logger& logger();
    #endif

    inline std::ostream& ftl(){
      return logger().ftl();
    }

    inline std::ostream& err(){
      return logger().err();
    }

    inline std::ostream& wrn(){
      return logger().wrn();
    }

    inline std::ostream& att(){
      return logger().att();
    }

    inline std::ostream& log(){
      return logger().log();
    }

    inline std::ostream& inf(){
      return logger().inf();
    }

    inline std::ostream& dbg(){
      return logger().dbg();
    }

    inline std::ostream& trc(){
      return logger().trc();
    }

    struct Test {
      int         nameOrder;
      std::string name;
      int         groupOrder;
      std::string group;
      int         partOrder;
      std::string part;
      void (*testFunction)();

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

    struct Tests{
      typedef std::map<std::string, Test> MapType;
      MapType values;
    };

    struct Groups{
      typedef std::map<std::string, Tests> MapType;
      MapType values;
    };

    struct Parts{
      typedef std::map<std::string, Groups> MapType;
      MapType values;
    };

    struct Options{
      std::vector<std::string> parts;
      std::vector<std::string> groups;
      std::vector<std::string> tests;
      std::string              logLevel;
    };


    struct Storage {
      typedef std::map<std::string, int> OrderMapType;

      Parts        parts;
      OrderMapType partOrders;
      OrderMapType groupOrders;
      OrderMapType testOrders;

      void add(const Test& a_test) {
        Parts::MapType::iterator partIterator = parts.values.insert( Parts::MapType::value_type(a_test.part, Groups() )  ).first;
        Groups::MapType::iterator groupIterator = partIterator->second.values.insert( Groups::MapType::value_type(a_test.group, Tests() )  ).first;
        Test test(a_test);

        {
          OrderMapType::const_iterator it = partOrders.find(a_test.part);
          if (it != partOrders.end()){
            test.partOrder = it->second;
          }
        }
        {
          OrderMapType::const_iterator it = groupOrders.find(a_test.group);
          if (it != groupOrders.end()){
            test.groupOrder = it->second;
          }
        }
        {
          OrderMapType::const_iterator it = testOrders.find(a_test.name);
          if (it != testOrders.end()){
            test.nameOrder = it->second;
          }
        }

        groupIterator->second.values[a_test.name] = test;
      }
    };

    #ifdef FCF_TEST_IMPLEMENTATION
      FCF_TEST_DECL_EXPORT Storage& getStorage(){
        static Storage* storage = 0;
        if (!storage){
          storage = new Storage();
        }
        return *storage;
      }
    #else
      FCF_TEST_DECL_EXPORT Storage& getStorage();
    #endif

    struct Regisrator {
      Regisrator(const Test& a_test){
        getStorage().add(a_test);
      }
    };

    struct PartOrderRegisrator {
      PartOrderRegisrator(const char* a_name, int a_order){
        getStorage().partOrders[a_name] = a_order;
      }
    };

    struct GroupOrderRegisrator {
      GroupOrderRegisrator(const char* a_name, int a_order){
        getStorage().groupOrders[a_name] = a_order;
      }
    };

    struct TestOrderRegisrator {
      TestOrderRegisrator(const char* a_name, int a_order){
        getStorage().testOrders[a_name] = a_order;
      }
    };

    namespace NDetails {
      inline void selectTests(std::set<Test>& a_dst, const Tests& a_tests, const Options& a_options){
        if (a_options.tests.empty()){
          for(const Tests::MapType::value_type& item : a_tests.values){
            a_dst.insert(item.second);
          }
        } else {
          for(const std::string& testName : a_options.groups){
            Tests::MapType::const_iterator it = a_tests.values.find(testName);
            Tests::MapType::const_iterator itEnd = a_tests.values.end();
            if (it != itEnd){
              a_dst.insert(it->second);
            } else {
              throw std::runtime_error(std::string() + "The test named '" + testName + "' cannot be found");
            }
          }
        }
      }

      inline void selectGroups(std::set<Test>& a_dst, const Groups& a_groups, const Options& a_options){
        if (a_options.groups.empty()){
          for(const Groups::MapType::value_type& item : a_groups.values){
            selectTests(a_dst, item.second, a_options);
          }
        } else {
          for(const std::string& groupName : a_options.groups){
            Groups::MapType::const_iterator it = a_groups.values.find(groupName);
            Groups::MapType::const_iterator itEnd = a_groups.values.end();
            if (it != itEnd){
              selectTests(a_dst, it->second, a_options);
            } else {
              throw std::runtime_error(std::string() + "The test group named '" + groupName + "' cannot be found");
            }
          }
        }
      }

      inline void selectParts(std::set<Test>& a_dst, const Options& a_options){
        if (a_options.parts.empty()){
          for(const Parts::MapType::value_type& item : ::fcf::NTest::getStorage().parts.values){
            selectGroups(a_dst, item.second, a_options);
          }
        } else {
          for(const std::string& partName : a_options.parts){
            Parts::MapType::const_iterator it = ::fcf::NTest::getStorage().parts.values.find(partName);
            Parts::MapType::const_iterator itEnd = ::fcf::NTest::getStorage().parts.values.end();
            if (it != itEnd){
              selectGroups(a_dst, it->second, a_options);
            } else {
              throw std::runtime_error(std::string() + "The test part named '" + partName + "' cannot be found");
            }
          }
        }
      }
    }

    #ifdef FCF_TEST_IMPLEMENTATION
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
      FCF_TEST_DECL_EXPORT void cmdHelp();
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
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
      FCF_TEST_DECL_EXPORT void cmdList();
    #endif


    #ifdef FCF_TEST_IMPLEMENTATION
      FCF_TEST_DECL_EXPORT void run(const Options& a_options){
        const char* lastLevel = logger().getStrLevel();
        logger().setStrLevel(a_options.logLevel.c_str());

        try {
          std::set<Test> tests;
          NDetails::selectParts(tests, a_options);

          for(const Test& test : tests) {
            log() << "Performing the test: \"" + test.part + "\" -> \"" + test.group + "\" -> \"" + test.name + "\" ..." << std::endl;
            test.testFunction();
          }

          log() << std::endl;
          log() << "All tests were completed. Number of tests: " << tests.size() << std::endl;
        } catch(...){
          logger().setStrLevel(lastLevel);
          throw;
        }
        logger().setStrLevel(lastLevel);
      }

    #else
      FCF_TEST_DECL_EXPORT void run(const Options& a_options);
    #endif

    enum CmdMode{
      CM_NONE,
      CM_RUN,
      CM_LIST,
      CM_HELP,
    };

    enum CmdRunMode {
      CRM_PARSE,
      CRM_EXECUTE,
      CRM_RUN,
    };

    #ifdef FCF_TEST_IMPLEMENTATION
      FCF_TEST_DECL_EXPORT CmdMode cmdRun(Options& a_dstOptions, int a_argc, const char** a_argv, CmdRunMode a_runMode){
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
          } else if (strcmp(a_argv[i], "--test-log-level") == 0) {
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
          run(a_dstOptions);
        }

        return mode;
      }
    #else
      FCF_TEST_DECL_EXPORT bool cmdRun(Options& a_dstOptions, int a_argc, const char** a_argv, CmdRunMode a_runMode);
    #endif

    class Duration {
      public:
        Duration(unsigned long long a_iterations)
          : _iterations(a_iterations){
        }

        Duration()
          : _iterations(1){
        }

        unsigned long long iterations(){
          return _iterations;
        }

        void begin(){
          _start = std::chrono::high_resolution_clock::now();
        }

        void end(){
          _end = std::chrono::high_resolution_clock::now();
        }

        template <typename TFunctor>
        void operator()(TFunctor&& a_functor){
          begin();
          for(unsigned long long i = 0; i < _iterations; ++i) {
            a_functor();
          }
          end();
        }

        std::chrono::nanoseconds totalDuration(){
          return std::chrono::duration_cast<std::chrono::nanoseconds>(_end - _start);
        }

        std::chrono::nanoseconds duration(){
          return std::chrono::duration_cast<std::chrono::nanoseconds>(_end - _start) / _iterations;
        }

      private:
        unsigned long long                              _iterations;
        std::chrono::high_resolution_clock::time_point _start;
        std::chrono::high_resolution_clock::time_point _end;
    };

  } // NTest namespace
} // fcf namespace


namespace fcf {
  namespace NTest {
    namespace Details {

      template <typename... TPack>
      struct PrintPack {
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

      template <>
      struct PrintPack<> {
        template <typename TIterator>
        std::string operator()(TIterator /*a_begName*/, TIterator /*a_endName*/){
          return std::string();
        }
      };

      template <typename TIterator>
      struct PrintArgs{
        TIterator begin;
        TIterator end;
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
    fcf::NTest::Details::PrintArgs<std::list<std::string>::iterator> p;\
    p.begin = names.begin();\
    p.end = names.end();\
    std::string messge = std::string() + \
                         "Test error: " + #exp + "  [FILE: " + __FILE__ + ":" + FCF_TEST__STRINGIFY(__LINE__) + "]\n" + \
                         p(__VA_ARGS__);\
    throw std::runtime_error(messge);\
  }

#endif // #ifndef ___FCF_TEST__TEST_HPP___
