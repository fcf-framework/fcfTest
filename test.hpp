#ifndef _FCF_TEST__TEST_HPP___
#define _FCF_TEST__TEST_HPP___

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
#include <mutex>
#include "VERSION"

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
#ifndef _FCF_TEST_DELC_EXTERN
  #ifdef FCF_TEST_IMPLEMENTATION
    #define _FCF_TEST_DELC_EXTERN
  #else
    #define _FCF_TEST_DELC_EXTERN extern
  #endif // #ifdef FCF_TEST_IMPLEMENTATION
#endif // #ifndef _FCF_TEST_DELC_EXTERN

/**
 * @brief Defines the export/declaration macro for Windows environments.
 * Uses `__declspec(dllexport)` or `__declspec(dllimport)` based on
 * whether symbols are being exported or imported, otherwise defaults to empty.
 */
#ifndef _FCF_TEST_DECL_EXPORT
  #ifdef WIN32
    #if defined(FCF_TEST_EXPORT)
      #define _FCF_TEST_DECL_EXPORT __declspec(dllexport)
    #elif defined(FCF_TEST_IMPORT)
      #define _FCF_TEST_DECL_EXPORT __declspec(dllimport)
    #else
      #define _FCF_TEST_DECL_EXPORT
    #endif // #if defined(FCF_TEST_EXPORT)
  #else
    #define _FCF_TEST_DECL_EXPORT
  #endif // #ifdef WIN32
#endif // #ifndef _FCF_TEST_DECL_EXPORT

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
    am_className() {\
      ::fcf::NTest::getStorage().append( ::fcf::NTest::Test{ 0, am_test, 0, am_group, 0, am_part, test } );\
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

#include <chrono>
#include <string>
#include <cstdio>

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
        std::string totalDurationStr(bool a_friendly) const {
          return nsToStr( totalDuration().count(), a_friendly);
        }

        /**
         * @brief Returns a string representation of the last active segment duration.
         * @return Formatted string: "SECONDS.MILLIS`MICROS`NANOS".
         */
        std::string lastTotalDurationStr(bool a_friendly) const {
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
        std::string durationStr(bool a_friendly) const {
          return nsToStr(duration().count(), a_friendly);
        }

        /**
         * @brief Returns a string representation of the average iteration duration based on the last segment.
         * @return Formatted string: "SECONDS.MILLIS`MICROS`NANOS".
         */
        std::string lastDurationStr(bool a_friendly) const {
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
          char buf[64];
          if (a_friendly) {
            snprintf(buf, sizeof(buf), "%u.%03u`%03u`%03u",
                     (unsigned int)(a_ns / 1000000000),
                     (unsigned int)((a_ns / 1000000) % 1000),
                     (unsigned int)((a_ns / 1000) % 1000),
                     (unsigned int)(a_ns % 1000));
          } else {
            snprintf(buf, sizeof(buf), "%u.%09u",
                     (unsigned int)(a_ns / 1000000000),
                     (unsigned int)(a_ns % 1000000000));
          }
          return buf;
        }

      private:
        unsigned long long                    _iterations;  ///< Total number of iterations for test execution and average metrics calculation.
        bool                                  _pause;       ///< State flag: true if the timer is paused/stopped, false if actively running.
        std::chrono::steady_clock::time_point _start;       ///< Modifiable starting time point of the global (accumulated) tracker.
        std::chrono::steady_clock::time_point _end;         ///< Ending time point of the global (accumulated) tracker.
        std::chrono::steady_clock::time_point _lastStart;   ///< Immutable starting time point of the last opened interval segment.
        std::chrono::steady_clock::time_point _lastEnd;     ///< Ending time point of the last completed interval segment.
    };

    namespace NDetails {

      /**
       * @brief A custom stream buffer that discards all characters written to it.
       * Used internally for loggers that are disabled.
       */
      class EmptyStreamBuffer: public std::streambuf {
        protected:
          int_type overflow(int_type a_char) override {
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

    struct Test;

    enum ELoggerMessageType {
      LMT_USER                  = 0x0001,
      LMT_START                 = 0x0002,
      LMT_END                   = 0x0004,
      LMT_COMPLETE              = 0x0008,
      LMT_ERROR                 = 0x0010,
      LMT_RESULT                = 0x0020,
      LMT_DURATION              = 0x0040,
      LMT_TEST_START            = 0x0080,
      LMT_TEST_START_MESSAGE    = 0x0100,
      LMT_TEST_COMPLETE         = 0x0200,
      LMT_TEST_ERROR            = 0x0400,
      LMT_TEST_ERROR_MESSAGE    = 0x0800,
      LMT_TEST_END              = 0x1000,
      LMT_TEST                  = LMT_USER | LMT_TEST_COMPLETE | LMT_TEST_ERROR | LMT_TEST_ERROR_MESSAGE,
      LMT_ALL                   = 0xFFFF,
    };

    struct LoggerHandlerDataWrapperBase {
      virtual ~LoggerHandlerDataWrapperBase();
      virtual void* get() = 0;
    };

    #ifdef FCF_TEST_IMPLEMENTATION
      LoggerHandlerDataWrapperBase::~LoggerHandlerDataWrapperBase() {
      }
    #endif

    template <typename Ty>
    struct LoggerHandlerDataWrapper : public LoggerHandlerDataWrapperBase {
      template <typename ...TPack>
      LoggerHandlerDataWrapper(TPack... a_argPack)
        : data(a_argPack...) {
      }
      virtual void* get() {
        return &data;
      };
      Ty data;
    };


    class LoggerHandlerData {
      public:
        struct HistoryValueType {
          std::string                                     strValue;
          unsigned long long                              ullValue;
          void*                                           ptrValue;
          std::shared_ptr<LoggerHandlerDataWrapperBase>   sptrValue;
        };
        typedef std::map<std::string, HistoryValueType>  HistoryItemType;
        typedef std::list< HistoryItemType >             HistoryType;

        HistoryType& history() {
          return _history;
        }

        void append(const HistoryItemType& a_item) {
          _history.push_back(a_item);
        }

      private:
        HistoryType     _history;
    };

    struct LoggerMessageStatus {
      ELoggerMessageType    messageType;
      const std::string*    origin;
      std::string*          message;
      size_t                line;
      ELogLevel             level;
      Duration*             duration;
      const Test*           test;
      const std::set<Test>* tests;
      std::ostream*         stream;
      LoggerHandlerData*    data;
    };

    struct LoggerPrefixOptions {
      std::string         name;
      bool                multiLine;
      unsigned int        messageTypes;
      LoggerPrefixOptions()
        : name("")
        , multiLine(false)
        , messageTypes(LMT_USER)
      {}
    };

    struct LoggerFormatOptions {
      std::string         name;
    };

    struct Logger;

    class LoggerOutput {
      public:
        LoggerOutput()
          : _logger(0) {
        }

        LoggerOutput(const LoggerOutput& a_output)
          : _logger((Logger*)a_output._logger)
          , _level(a_output._level)
          , _loggerMessageType(a_output._loggerMessageType)
          , _sstream(a_output._sstream.str()) {
        }

        LoggerOutput(Logger& a_logger, ELogLevel a_level, ELoggerMessageType a_loggerMessageType)
          : _logger(&a_logger), _level(a_level), _loggerMessageType(a_loggerMessageType) {
        }

        ~LoggerOutput();

        template <typename Ty>
        LoggerOutput& operator<<(const Ty& a_value) {
          _sstream << a_value;
          return *this;
        }

        LoggerOutput& operator<<(std::ostream& (*a_manipulator)(std::ostream&)) {
          a_manipulator(_sstream);
          return *this;
        }

      private:
        Logger*             _logger;
        ELogLevel           _level;
        ELoggerMessageType  _loggerMessageType;
        std::stringstream   _sstream;
    };

    class LoggerJunitFormat {
      public:
        static const LoggerHandlerData::HistoryValueType& getHistoryValue(const LoggerHandlerData::HistoryItemType& a_item, const char* a_key);
        static const LoggerHandlerData::HistoryValueType& getHistoryValue(const LoggerHandlerData::HistoryType& a_history, const char* a_key);
        static std::string suiteName(const Test& a_test);
        static void format(Logger& a_logger, LoggerMessageStatus& a_messageStatus);
        static std::string xmlAttribute(const std::string& a_string);
        static std::string xmlText(const std::string& a_string);

      private:
        struct TestStatusType {
          bool                error;
          std::string         message;
          unsigned long long  duration;
        };
        std::map<Test, TestStatusType> _processed;
    };

    /**
     * @brief Configuration options for running tests.
     */
    struct Options {
      std::vector<std::string> parts;         ///< List of part names to run (empty means all).
      std::vector<std::string> groups;        ///< List of group names to run (empty means all).
      std::vector<std::string> tests;         ///< List of specific test names to run (empty means all).
      std::vector<std::string> ignoreParts;   ///< List of ignore part names.
      std::vector<std::string> ignoreGroups;  ///< List of ignore group names to run.
      std::vector<std::string> ignoreTests;   ///< List of ignore specific test names to run.
      ELogLevel                logLevel;      ///< Desired logging level.
      std::string              format;
      bool                     noBreak;
      std::ostream*            stream;

      Options()
        : logLevel(LL_DEF)
        , format("default")
        , noBreak(false)
        , stream(0) {
      }
    };

    namespace NDetails {
      _FCF_TEST_DECL_EXPORT void runImpl(const Options& a_options, bool a_enableThrow, bool* a_errorPtr);
    }

    /**
     * @brief Represents a logging instance with configurable output streams based on level.
     */
    struct Logger {
        friend LoggerOutput;
        friend void cmdHelp();
        friend void NDetails::runImpl(const Options& a_options, bool a_enableThrow, bool* a_errorPtr);
        typedef std::map<std::string, LoggerHandlerData> HandlerDataStorageType;

      public:
        typedef std::function<std::string(Logger&, LoggerMessageStatus&)> PrefixFunctionType;
        typedef std::function<void(Logger&, LoggerMessageStatus&)>        FormatFunctionType;

      protected:
        struct PrefixType {
          std::string         str;
          PrefixFunctionType  func;
          LoggerPrefixOptions options;
        };

        struct FormatType {
          FormatFunctionType  func;
          LoggerFormatOptions options;
        };

        struct EnvironmentType {
          ELogLevel               level;
          const Test*             test;
          const std::set<Test>*   tests;
          Duration*               bench;
          std::string             format;
          HandlerDataStorageType  prefixData;
          HandlerDataStorageType  formatData;
          std::ostream*           stream;
        };

      public:

        /**
         * @brief Constructs a logger with the default log level (LL_LOG).
         */
        Logger()
          : _environment{LL_LOG, nullptr, nullptr, nullptr, "default", {}, {}, nullptr}
          , _newLine(true)
        {
          LoggerPrefixOptions lpo;
          lpo.name          = "offset";
          lpo.multiLine     = true;
          lpo.messageTypes  = LMT_TEST;
          appendPrefixStr("  ", lpo);

          LoggerFormatOptions lfo;
          lfo.name          = "junit";
          appendFormatFunc(LoggerJunitFormat::format, lfo);
        }

        /**
         * @brief Returns the output stream for fatal messages.
         * @return Reference to the output stream.
         */
        LoggerOutput ftl() {
          return _log(LL_FTL, LMT_USER);
        }

        /**
         * @brief Returns the output stream for error messages.
         * @return Reference to the output stream.
         */
        LoggerOutput err() {
          return _log(LL_ERR, LMT_USER);
        }

        /**
         * @brief Returns the output stream for warning messages.
         * @return Reference to the output stream.
         */
        LoggerOutput wrn() {
          return _log(LL_WRN, LMT_USER);
        }

        /**
         * @brief Returns the output stream for attention messages.
         * @return Reference to the output stream.
         */
        LoggerOutput att() {
          return _log(LL_ATT, LMT_USER);
        }

        /**
         * @brief Returns the output stream for log messages.
         * @return Reference to the output stream.
         */
        LoggerOutput log() {
          return _log(LL_LOG, LMT_USER);
        }

        /**
         * @brief Returns the output stream for informational messages.
         * @return Reference to the output stream.
         */
        LoggerOutput inf() {
          return _log(LL_INF, LMT_USER);
        }

        /**
         * @brief Returns the output stream for debug messages.
         * @return Reference to the output stream.
         */
        LoggerOutput dbg() {
          return _log(LL_DBG, LMT_USER);
        }

        /**
         * @brief Returns the output stream for trace messages.
         * @return Reference to the output stream.
         */
        LoggerOutput trc() {
          return _log(LL_TRC, LMT_USER);
        }

        /**
         * @brief The output stream returns for the test inner message. The log recording is always performed.
         * @return Reference to the output stream.
         */
        LoggerOutput sys(ELoggerMessageType a_messageType) {
          return _log(LL_LOG, a_messageType);
        }

        /**
         * @brief Returns the current string representation of the log level.
         * @return Pointer to a static string representing the level name.
         */
        const char* levelStr() const {
          return toLevelStr(_environment.level);
        }

        /**
         * @brief Sets the log level by name.
         * @param a_level Pointer to a string representing the desired log level (e.g., "dbg", "err").
         */
        void levelStr(const char* a_level) {
          level(toLevel(a_level));
        }

        /**
         * @brief Returns the current ELogLevel value of the log level.
         * @return ELogLevel representation of the log level.
         */
        ELogLevel level() const {
          return _environment.level;
        }

        /**
         * @brief Sets the log level.
         * @param a_level An ELogLevel value representing the desired logging level.
         * @throw std::runtime_error throws if the value passed is LL__DEF
         */
        void level(ELogLevel a_level) {
          if (a_level == LL_DEF) {
            throw std::runtime_error("LL_DEF value cannot be set as primary value");
          }
          _environment.level = a_level;
        }

        /**
         * @brief Sets the current test instance for the logger.
         *
         * This method allows the logger to associate a specific test case with its
         * output, enabling context-aware logging during test execution.
         *
         * @param a_test Pointer to the Test object to be used as the current context.
         */
        void test(const Test* a_test) {
          _environment.test = a_test;
        }

        /**
         * @brief Retrieves the current test instance from the logger.
         *
         * @return A constant pointer to the currently active Test object.
         */
        const Test* test() {
          return _environment.test;
        }

        void tests(const std::set<Test>* a_tests) {
          _environment.tests = a_tests;
        }

        const std::set<Test>* tests() {
          return _environment.tests;
        }

        void bench(Duration* a_bench) {
          _environment.bench = a_bench;
        }

        Duration* bench() {
          return _environment.bench;
        }

        void format(const std::string& a_format) {
          _environment.format = a_format.length() ? a_format : std::string("default");
        }

        const std::string& format() const {
          return _environment.format;
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
          for(int i = 0; i < size; ++i) {
            int l = i - 1;
            if (a_level == levels[i]) {
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
        static const char* toLevelStr(ELogLevel a_level) {
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
        void appendPrefixStr(const std::string& a_prefix, const LoggerPrefixOptions& a_options) {
          PrefixType prefix;
          prefix.str = a_prefix;
          prefix.options = a_options;
          _prefixes.push_back(prefix);
        }

        void appendPrefixStr(const std::string& a_prefix) {
          PrefixType prefix;
          prefix.str = a_prefix;
          _prefixes.push_back(prefix);
        }

        /**
         * @brief Adds a functional prefix to all log messages.
         * @param a_prefix A function that returns a string to be used as a prefix.
         */
        void appendPrefixFunc(const PrefixFunctionType& a_prefix, const LoggerPrefixOptions& a_options) {
          PrefixType prefix;
          prefix.func = a_prefix;
          prefix.options = a_options;
          _prefixes.push_back(prefix);
        }

        void appendPrefixFunc(const PrefixFunctionType& a_prefix) {
          PrefixType prefix;
          prefix.func = a_prefix;
          _prefixes.push_back(prefix);
        }

        void appendFormatFunc(const FormatFunctionType& a_prefix, const LoggerFormatOptions& a_options) {
          FormatType format;
          format.func = a_prefix;
          format.options = a_options;

          auto existIt = std::find_if(_formats.begin(), _formats.end(), [&a_options](const FormatType& a_item) { return a_options.name == a_item.options.name; });
          if (existIt != _formats.end()) {
            _formats.erase(existIt);
          }
          _formats.push_back(format);
        }

      protected:

        const EnvironmentType& _getEnvironment() {
          return _environment;
        }

        void _setEnvironment(const EnvironmentType& a_environment) {
          _environment = a_environment;
        }

        void _write(fcf::NTest::ELogLevel a_level, ELoggerMessageType a_messageType, const std::string& a_message) {
          std::lock_guard<std::recursive_mutex> lock(_mutex);

          std::string messageStr(a_message);

          LoggerMessageStatus lms;
          lms.messageType   = a_messageType;
          lms.origin        = &a_message;
          lms.message       = &messageStr;
          lms.line          = 0;
          lms.level         = a_level;
          lms.duration      = _environment.bench;
          lms.test          = _environment.test;
          lms.tests         = _environment.tests;
          lms.stream        = _environment.stream ? _environment.stream : &std::cout;
          lms.data          = nullptr;

          if (messageStr.length()) {
            for(PrefixType prefix : _prefixes) {
              if (!(a_messageType & prefix.options.messageTypes)) {
                continue;
              }
              if (!_newLine) {
                continue;
              }
              size_t lastPos = 0;
              std::string resultMessage;
              while(lastPos < messageStr.length()) {
                size_t pos = prefix.options.multiLine ? messageStr.find("\n", lastPos)
                                                      : messageStr.length()-1;
                if (pos == std::string::npos) {
                  pos = messageStr.length();
                } else {
                  ++pos;
                }
                std::string line = messageStr.substr(lastPos, pos - lastPos);
                lms.message = &line;

                if (prefix.func) {
                  const char* prefixName = prefix.options.name.empty() ? "default" : prefix.options.name.c_str();
                  HandlerDataStorageType::iterator dataIt = _environment.prefixData.find(prefixName);
                  if (dataIt == _environment.prefixData.end()) {
                    dataIt = _environment.prefixData.insert({prefixName, LoggerHandlerData()}).first;
                  }
                  lms.data = &dataIt->second;

                  std::string prefixPart = prefix.func(*this, lms);
                  if (prefixPart.length()) {
                    *lms.message = prefixPart + *lms.message;
                  }
                } else {
                  *lms.message = prefix.str + *lms.message;
                }
                resultMessage += *lms.message;
                lastPos = pos;
              }
              std::swap(messageStr,resultMessage);
              lms.message = &messageStr;
            }
          }

          for(FormatType format : _formats) {
            const char* formatName = format.options.name.empty() ? "default" : format.options.name.c_str();
            if (formatName == _environment.format) {
              HandlerDataStorageType::iterator dataIt = _environment.formatData.find(formatName);
              if (dataIt == _environment.formatData.end()) {
                dataIt = _environment.formatData.insert({formatName, LoggerHandlerData()}).first;
              }
              lms.data = &dataIt->second;
              format.func(*this, lms);
            }
          }

          if (lms.message->length()) {
            (*lms.stream) << *lms.message;
          }

          if (a_message.length()) {
            _newLine = a_message[a_message.length()-1] == '\n';
          }
        }

        LoggerOutput _log(ELogLevel a_level, ELoggerMessageType a_messageType) {
          if (_environment.level >= a_level || a_messageType != LMT_USER) {
            return LoggerOutput(*this, a_level, a_messageType);
          } else {
            return LoggerOutput();
          }
        }

        EnvironmentType         _environment;
        std::list<PrefixType>   _prefixes;
        std::list<FormatType>   _formats;
        std::recursive_mutex    _mutex;
        bool                    _newLine;
    };

    #ifdef FCF_TEST_IMPLEMENTATION
      LoggerOutput::~LoggerOutput() {
        if (_logger) {
          _logger->_write(_level, _loggerMessageType, _sstream.str());
        }
      }
    #endif


    #ifdef FCF_TEST_IMPLEMENTATION
      /**
       * @brief Returns a reference to the global logger instance.
       * Initializes the static logger on first call if not present.
       * @return Reference to the Logger instance.
       */
      _FCF_TEST_DECL_EXPORT Logger& logger() {
        static Logger* logger = 0;
        if (!logger) {
          logger = new Logger();
        }
        return *logger;
      }
    #else
      /**
       * @brief Declaration for the global logger instance.
       * @return Reference to the Logger instance.
       */
      _FCF_TEST_DECL_EXPORT Logger& logger();
    #endif

    /**
     * @brief Returns the output stream for fatal messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline LoggerOutput ftl() {
      return logger().ftl();
    }

    /**
     * @brief Returns the output stream for error messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline LoggerOutput err() {
      return logger().err();
    }

    /**
     * @brief Returns the output stream for warning messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline LoggerOutput wrn() {
      return logger().wrn();
    }

    /**
     * @brief Returns the output stream for attention messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline LoggerOutput att() {
      return logger().att();
    }

    /**
     * @brief Returns the output stream for log messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline LoggerOutput log() {
      return logger().log();
    }

    /**
     * @brief Returns the output stream for informational messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline LoggerOutput inf() {
      return logger().inf();
    }

    /**
     * @brief Returns the output stream for debug messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline LoggerOutput dbg() {
      return logger().dbg();
    }

    /**
     * @brief Returns the output stream for trace messages (global shortcut).
     * @return Reference to the output stream.
     */
    inline LoggerOutput trc() {
      return logger().trc();
    }

    /**
     * @brief The output stream returns for the test inner message. The log recording is always performed.
     * @return Reference to the output stream.
     */
    inline LoggerOutput sys(ELoggerMessageType a_messageType) {
      return logger().sys(a_messageType);
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
     * @brief Container for a collection of tests within a group.
     */
    struct Tests {
      typedef std::map<std::string, Test> MapType;
      MapType values; ///< Map of test names to Test objects.
    };

    /**
     * @brief Container for a collection of groups within a part.
     */
    struct Groups {
      typedef std::map<std::string, Tests> MapType;
      MapType values; ///< Map of group names to Tests containers.
    };

    /**
     * @brief Container for a collection of groups across all parts.
     */
    struct Parts {
      typedef std::map<std::string, Groups> MapType;
      MapType values; ///< Map of part names to Groups containers.
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
      void append(const Test& a_test) {
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
      _FCF_TEST_DECL_EXPORT Storage& getStorage() {
        static Storage* storage = 0;
        if (!storage) {
          storage = new Storage();
        }
        return *storage;
      }
    #else
      /**
       * @brief Declaration for the global storage instance.
       * @return Reference to the Storage instance.
       */
      _FCF_TEST_DECL_EXPORT Storage& getStorage();
    #endif

    /**
     * @brief Base class for order registration objects.
     *
     * @param a_test The test object associated with the registration (used in constructor).
     */
    struct Regisrator {
      Regisrator(const Test& a_test) {
        getStorage().append(a_test);
      }
    };

    /**
     * @brief Registers an execution order for a specific part.
     *
     * @param a_name The name of the part.
     * @param a_order The desired execution order (integer).
     */
    struct PartOrderRegisrator {
      PartOrderRegisrator(const char* a_name, int a_order) {
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
      GroupOrderRegisrator(const char* a_name, int a_order) {
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
      TestOrderRegisrator(const char* a_name, int a_order) {
        getStorage().testOrders[a_name] = a_order;
      }
    };

    #ifdef FCF_TEST_IMPLEMENTATION
      /**
       * @brief Displays help information for the test runner.
       */
      _FCF_TEST_DECL_EXPORT void cmdHelp() {
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
        for(auto format : logger()._formats) {
          formats += ", ";
          formats += format.options.name;
        }
        std::cout << "  --test-format FORMAT - Output format (default" + formats + ")." << std::endl;
        std::cout << "  --test-help  - Help message" << std::endl;
        std::cout << std::endl;
        std::cout << "Explanatory details:" << std::endl;
        std::cout << "  1. The --test-part, --test-group, --test-test commands are combined using the OR operation" << std::endl;
        std::cout << "  2. The --test-ignore-part, --test-ignore-group, --test-ignore-test commands are combined using the OR operation" << std::endl;
      }
    #else
      /**
       * @brief Declaration for displaying a list of all registered tests.
       */
      _FCF_TEST_DECL_EXPORT void cmdHelp();
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      /**
       * @brief  Displays a list of all registered tests.
       */
      _FCF_TEST_DECL_EXPORT void cmdList() {
        Options options;
        std::set<Test> tests;
        NDetails::select(tests, options);
        std::cout << "List of tests:" << std::endl;
        for(const Test& test : tests) {
          std::cout << "  \"" << test.part << "\" -> \"" << test.group << "\" -> \"" << test.name  << "\""<< std::endl;
        }
      }
    #else
      /**
       * @brief Declaration for displaying a list of all registered tests.
       */
      _FCF_TEST_DECL_EXPORT void cmdList();
    #endif

    namespace NDetails {
      #ifdef FCF_TEST_IMPLEMENTATION
        _FCF_TEST_DECL_EXPORT void runImpl(const Options& a_options, bool a_enableThrow, bool* a_errorPtr) {
          static std::recursive_mutex mutex;
          static bool globalRunState = false;
          {
            std::lock_guard<std::recursive_mutex> lock(mutex);
            #ifndef _FCF_TEST_RECURCIVE_RUN_DISABLE
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

          Logger::EnvironmentType lastEnv = logger()._getEnvironment();
          Logger::EnvironmentType newEnv {
                                a_options.logLevel != LL_DEF ? a_options.logLevel : lastEnv.level,
                                0,
                                0,
                                0,
                                a_options.format.length() ? a_options.format : lastEnv.format,
                                {},
                                {},
                                a_options.stream
                              };

          try {
            logger()._setEnvironment(newEnv);

            Duration bench;
            std::set<Test> tests;

            sys(LMT_START);

            NDetails::select(tests, a_options);
            logger().tests(&tests);

            unsigned int errorCounter = 0;
            unsigned int passedCounter = 0;
            logger().bench(&bench);
            for(const Test& test : tests) {
              sys(LMT_TEST_START);
              sys(LMT_TEST_START_MESSAGE) << "Performing the test: \"" + test.part + "\" -> \"" + test.group + "\" -> \"" + test.name + "\" ..." << std::endl;
              logger().test(&test);
              bench.resume();
              ++passedCounter;
              try {
                test.testFunction();
              } catch(std::exception& e) {
                bench.end();
                totalErrorFlag = true;
                ++errorCounter;
                std::string errorMesssage = e.what();
                errorMesssage = errorMesssage.erase(errorMesssage.find_last_not_of(" \t\n\r\f\v") + 1);
                sys(LMT_TEST_ERROR_MESSAGE) << errorMesssage << std::endl;
                sys(LMT_TEST_ERROR) << "Test failed (" << bench.lastTotalDurationStr(true) << " sec)" << std::endl;
                sys(LMT_TEST_END);
                if (a_options.noBreak) {
                  continue;
                } else {
                  break;
                }
              }
              bench.end();
              sys(LMT_TEST_COMPLETE) << "Test completed successfully (" << bench.lastTotalDurationStr(true) << " sec)" << std::endl;
              sys(LMT_TEST_END);
            }

            unsigned int skipedCounter = tests.size() - passedCounter;

            if (!errorCounter) {
              sys(LMT_COMPLETE) << std::endl
                                << "All tests were completed." << std::endl;
            } else {
              sys(LMT_ERROR) << std::endl
                             << "Testing completed with failures." << std::endl;
            }

            sys(LMT_RESULT)   << "Tests: " << passedCounter << " passed, " << errorCounter << " failed, " << skipedCounter << " skiped, " << tests.size() << " total" << std::endl;
            sys(LMT_DURATION) << "Duration: " << bench.totalDurationStr(true) << " sec" << std::endl;

            sys(LMT_END);

            logger()._setEnvironment(lastEnv);
            {
              std::lock_guard<std::recursive_mutex> lock(mutex);
              globalRunState = false;
            }
          } catch(const std::exception&) {
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

    /**
     * @brief Executes the selected tests based on provided options.
     *
     * @param a_options Configuration options specifying which tests to run and logging level.
     * @param a_errorPtr (default = (bool*)0) A pointer to a variable receiving error information.
     *                                        If an error occurs, the value is set to true.
     *                                        If a null pointer is passed, the function throws an exception.
     */
    inline void run(const Options& a_options, bool* a_errorPtr) {
      NDetails::runImpl(a_options, false, a_errorPtr);
    }

    inline void run(const Options& a_options) {
      NDetails::runImpl(a_options, true, 0);
    }

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

    namespace NDetails {

      #ifdef FCF_TEST_IMPLEMENTATION

        inline std::vector<std::string> parseArgs(int a_argc, const char* const* a_argv);

        _FCF_TEST_DECL_EXPORT ECmdMode cmdRunImpl(Options& a_dstOptions, int a_argc, const char* const* a_argv, ECmdRunMode a_runMode, bool a_enableThrow, bool* a_errorPtr) {
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
            }
          }
          if ((mode == CM_RUN && a_runMode == CRM_EXECUTE) || a_runMode == CRM_RUN) {
            runImpl(a_dstOptions, a_enableThrow, a_errorPtr);
          }

          return mode;
        }

      #else

        _FCF_TEST_DECL_EXPORT ECmdMode cmdRunImpl(Options& a_dstOptions, int a_argc, const char* const* a_argv, ECmdRunMode a_runMode, bool a_enableThrow, bool* a_errorPtr);

      #endif

    } // NDetails namespace

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
    ECmdMode cmdRun(Options& a_dstOptions, int a_argc, Ty a_argv, ECmdRunMode a_runMode, bool* a_errorPtr) {
      return cmdRun(a_dstOptions, a_argc, (const char* const*)a_argv, a_runMode, a_errorPtr);
    }

    template <typename Ty>
    ECmdMode cmdRun(Options& a_dstOptions, int a_argc, Ty a_argv, ECmdRunMode a_runMode) {
      return cmdRun(a_dstOptions, a_argc, (const char* const*)a_argv, a_runMode);
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
    inline ECmdMode cmdRun(int a_argc, Ty a_argv, ECmdRunMode a_runMode, bool* a_errorPtr) {
      Options options;
      return cmdRun(options, a_argc, (const char* const*)a_argv, a_runMode, a_errorPtr);
    }

    template <typename Ty>
    inline ECmdMode cmdRun(int a_argc, Ty a_argv, ECmdRunMode a_runMode) {
      Options options;
      return cmdRun(options, a_argc, (const char* const*)a_argv, a_runMode);
    }

    #ifdef FCF_TEST_IMPLEMENTATION
      const LoggerHandlerData::HistoryValueType& LoggerJunitFormat::getHistoryValue(const LoggerHandlerData::HistoryItemType& a_item, const char* a_key) {
        const static LoggerHandlerData::HistoryValueType empty = {"", 0, nullptr, nullptr};
        LoggerHandlerData::HistoryItemType::const_iterator it = a_item.find(a_key);
        if (it != a_item.end()) {
          return it->second;
        } else  {
          return empty;
        }
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      const LoggerHandlerData::HistoryValueType& LoggerJunitFormat::getHistoryValue(const LoggerHandlerData::HistoryType& a_history, const char* a_key) {
        const static LoggerHandlerData::HistoryValueType empty = {"", 0, nullptr, nullptr};
        if (!a_history.size()) {
          return empty;
        }
        return getHistoryValue(a_history.front(), a_key);
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      std::string LoggerJunitFormat::suiteName(const Test& a_test) {
        return a_test.part + "/" + a_test.group;
      }
    #endif

    #ifdef FCF_TEST_IMPLEMENTATION
      std::string LoggerJunitFormat::xmlAttribute(const std::string& a_string) {
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
      std::string LoggerJunitFormat::xmlText(const std::string& a_string) {
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
      void LoggerJunitFormat::format(Logger& a_logger, LoggerMessageStatus& a_messageStatus) {
        std::ostringstream output;

        switch (a_messageStatus.messageType) {
          case LMT_START:
            {
              std::shared_ptr<LoggerHandlerDataWrapperBase> formatHandler(new LoggerHandlerDataWrapper<LoggerJunitFormat>());
              a_messageStatus.data->append({ { "handler", {"", 0, 0, formatHandler} } });
            }
            break;
          case LMT_TEST_COMPLETE:
          case LMT_TEST_ERROR_MESSAGE:
            {
              LoggerHandlerDataWrapperBase* formatHandlerWrapper = (LoggerHandlerDataWrapperBase*)getHistoryValue(a_messageStatus.data->history(), "handler").sptrValue.get();
              LoggerJunitFormat* formatHandler = formatHandlerWrapper ? (LoggerJunitFormat*)formatHandlerWrapper->get() : (LoggerJunitFormat*)0;
              if (formatHandler) {
                TestStatusType ts;
                ts.error = a_messageStatus.messageType == LMT_TEST_ERROR_MESSAGE;
                ts.message = *a_messageStatus.origin;
                ts.duration = a_messageStatus.duration->lastTotalDuration().count();
                formatHandler->_processed.insert({*a_messageStatus.test, ts});
              }
            }
            break;
          case LMT_END:
            {
              LoggerHandlerDataWrapperBase* formatHandlerWrapper = (LoggerHandlerDataWrapperBase*)getHistoryValue(a_messageStatus.data->history(), "handler").sptrValue.get();
              LoggerJunitFormat* formatHandler = formatHandlerWrapper ? (LoggerJunitFormat*)formatHandlerWrapper->get() : (LoggerJunitFormat*)0;
              if (formatHandler) {

                size_t totalTestCount   = a_messageStatus.tests->size();
                size_t totalTestFailure = std::count_if(formatHandler->_processed.begin(),
                                                        formatHandler->_processed.end(),
                                                        [](const std::pair<Test, TestStatusType>& a_item) {
                                                          return a_item.second.error;
                                                        });
                size_t totalTestSkipped  = a_messageStatus.tests->size() - formatHandler->_processed.size();

                std::map<std::string, std::set<Test> > suites;
                for(const Test& test : *a_messageStatus.tests) {
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
                       << "time=\"" << a_messageStatus.duration->totalDurationStr(false) << "\""
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
                      std::string message = processedIt->second.message.erase(processedIt->second.message.find_last_not_of(" \t\n\r\f\v") + 1);
                      std::string shortMessage = message.substr(0, message.find("\n"));
                      shortMessage = shortMessage.substr(0, shortMessage.find("[FILE:"));
                      shortMessage = shortMessage.erase(shortMessage.find_last_not_of(" \t\n\r\f\v") + 1);
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

                a_messageStatus.data->history().clear();
              }
            }
            break;
        }

        *a_messageStatus.message = output.str();
      }
    #endif

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
        std::string operator()(TIterator a_begName, TIterator a_endName, const TArg& a_arg, const TPack2&... a_pack) {
          std::stringstream ss;
          std::string name = a_begName != a_endName ? *a_begName : "arg";
          ss << "    " << name << ": " << a_arg << std::endl;
          if (a_begName != a_endName) {
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
        std::string operator()(TIterator /*a_begName*/, TIterator /*a_endName*/) {
          return std::string();
        }
      };

      /**
       * @brief A helper structure to manage printing of arguments with names.
       *
       * @tparam TIterator Iterator type for the name list.
       */
      template <typename TIterator>
      struct PrintArgs {
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
        std::string operator()(const TArgPack&... a_pack) {
          std::string result;
          if (sizeof...(TArgPack) && begin != end) {
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
  if (!(exp)) { \
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
            for(const auto& item : elements) {
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
      EAllow checkAllow(EAllow a_allow, const TAllowList& a_allowList, const TAllowList& a_ignoreList, const std::string& a_name) {
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
      void checkExists(std::map<std::string, bool>& a_state, const TMap& a_map, const TAllowList& a_allowList) {
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
      inline void select(std::set<Test>& a_dst, const Options& a_options) {
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

              if (allowTest == NONE || allowTest == IGNORE) {
                continue;
              }

              Test test(testItem.second);
              {
                Storage::OrderMapType::const_iterator it = getStorage().partOrders.find(test.part);
                if (it != getStorage().partOrders.end()) {
                  test.partOrder = it->second;
                }
              }
              {
                Storage::OrderMapType::const_iterator it = getStorage().groupOrders.find(test.group);
                if (it != getStorage().groupOrders.end()) {
                  test.groupOrder = it->second;
                }
              }
              {
                Storage::OrderMapType::const_iterator it = getStorage().testOrders.find(test.name);
                if (it != getStorage().testOrders.end()) {
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
#endif // #ifndef _FCF_TEST__TEST_HPP___
