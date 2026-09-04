#ifndef DURATION_HPP
#define DURATION_HPP

#include <vector>
#include <chrono>
#include <algorithm>
#include <functional>
#include "timeduration.hpp"
#include "histogram.hpp"

namespace fcf {
  namespace NTest {

    /**
     * @brief A template class for measuring and analyzing time durations.
     *
     * This class provides a framework for performing micro-benchmarking, allowing
     * for multiple measurement levels, warmup iterations, and statistical analysis
     * through histograms.
     *
     * @tparam TClock A clock type that provides a time point in nanoseconds.
     */
    template <typename TClock>
    class DurationBasic {
      public:
        /** @brief Type for time points, represented as nanoseconds. */
        typedef unsigned long long TimePoint;
        /** @brief Type for time durations. */
        typedef ::fcf::NTest::TimeDuration TimeDuration;
        /** @brief Type for the histogram used in measurements. */
        typedef HistogramBasic<TimeDuration, unsigned long long> HistogramType;

        /**
         * @brief Options for starting a measurement session.
         */
        struct BeginOptions {
          /** @brief Total number of iterations to perform. */
          long long iterationCount;
          /** @brief The number of bins for the histogram. If negative, the histogram size is determined automatically. */
          int       histogramSize;

          /** @brief Default constructor. Initializes iterationCount to 1. */
          BeginOptions()
            : iterationCount(-1)
            , histogramSize(-1)
          {}
          /** @brief Constructor with specified iteration count. @param a_iterationCount Number of iterations. */
          BeginOptions(long long a_iterationCount, int a_histogramSize = -1)
            : iterationCount(a_iterationCount)
            , histogramSize(a_histogramSize)
          {}
        };

        /**
         * @brief Extended options for measurement sessions.
         */
        struct Options : public BeginOptions {
          /** @brief Number of iterations per measurement step. */
          long long measurementStep;
          /** @brief Number of warmup iterations before actual measurement. */
          long long warmupCount;

          /** @brief Default constructor. */
          Options()
            : measurementStep(-1)
            , warmupCount(-1)
          {}
          /**
           * @brief Constructor with specified parameters.
           * @param a_iterationCount Total iterations.
           * @param a_histogramSize The number of bins for the histogram
           * @param a_measurementStep Step size for measurements.
           * @param a_warmupCount Number of warmup iterations.
           */
          Options(unsigned long long a_iterationCount, int a_histogramSize = -1, unsigned long long a_measurementStep = -1, unsigned long long a_warmupCount = -1)
            : BeginOptions(a_iterationCount, a_histogramSize)
            , measurementStep(a_measurementStep)
            , warmupCount(a_warmupCount)
          {}
        };

      private:
        struct Measurement {
          TimeDuration        duration;
          TimePoint           timepoint;
          unsigned long long  iteration;
          TimeDuration        min;
          TimeDuration        max;
          bool                pause;
          BeginOptions        options;
          TimeDuration        excludedTime;
          HistogramType       histogram;

          Measurement()
            : duration(0)
            , timepoint(0)
            , iteration(0)
            , min(0)
            , max(0)
            , pause(true)
            , excludedTime(0)
          {}
        };

      public:
        /** @brief Default constructor. */
        DurationBasic();
        /**
         * @brief Constructor with specified iteration parameters.
         * @param a_iterationCount Total iterations.
         * @param a_measurementStep Step size.
         * @param a_warmupCount Warmup iterations.
         */
        DurationBasic(unsigned long long a_iterationCount, unsigned long long a_measurementStep = 1, unsigned long long a_warmupCount = 0);
        /** @brief Constructor with specified Options. @param a_options Measurement options. */
        DurationBasic(const Options& a_options);

        /** @brief Gets the current measurement options. @return The current options. */
        Options options() const;
        /** @brief Sets the current measurement options. @param a_options The new options. */
        void options(const Options& a_options);

        /**
         * @brief Starts a measurement at a specific level.
         * @param a_options Options for this measurement.
         * @param a_beginLevel Starting level.
         * @param a_endLevel Ending level.
         */
        void begin(const BeginOptions& a_options, int a_beginLevel, int a_endLevel);
        /** @brief Starts a measurement at a specific level. @param a_options Options. @param a_beginLevel Level. */
        void begin(const BeginOptions& a_options, int a_beginLevel = 0);
        /** @brief Starts a measurement with default options. @param a_beginLevel Level. */
        void begin(int a_beginLevel = 0);
        /** @brief Starts a measurement with default options. @param a_beginLevel Starting level. @param a_endLevel Ending level. */
        void begin(int a_beginLevel, int a_endLevel);

        /**
         * @brief Ends a measurement at a specific level.
         * @param a_beginLevel Starting level.
         * @param a_endLevel Ending level.
         */
        void end(int a_beginLevel, int a_endLevel);
        /** @brief Ends a measurement at a specific level. @param a_beginLevel Level. */
        void end(int a_beginLevel=0);

        /**
         * @brief Resets measurements for a specific range of levels.
         * @param a_beginLevel Starting level.
         * @param a_endLevel Ending level (-1 for all).
         */
        void reset(int a_beginLevel = 0, int a_endLevel = -1);

        /** @brief Gets the histogram for a specific level. @param a_level Level index. @return Reference to the histogram. */
        const HistogramType& histogram(size_t a_level = 0) const;
        /** @brief Gets the histogram for a specific level. @param a_level Level index. @return Reference to the histogram. */
        HistogramType& histogram(size_t a_level = 0);

        /**
         * @brief Executes a function and measures its duration.
         * @tparam TFunction The type of the function.
         * @param a_options Options for measurement.
         * @param a_beginLevel Starting level.
         * @param a_endLevel Ending level.
         * @param a_function The function to execute.
         */
        template <typename TFunction>
        void operator()(Options a_options, int a_beginLevel, int a_endLevel, TFunction a_function);

        /**
         * @brief Executes a function and measures its duration.
         * @tparam TFunction The type of the function.
         * @param a_beginLevel Starting level.
         * @param a_endLevel Ending level.
         * @param a_function The function to execute.
         */
        template <typename TFunction>
        void operator()(int a_beginLevel, int a_endLevel, TFunction a_function);

        /**
         * @brief Executes a function and measures its duration.
         * @tparam TFunction The type of the function.
         * @param a_options Options for measurement.
         * @param a_beginLevel Starting level.
         * @param a_function The function to execute.
         */
        template <typename TFunction>
        void operator()(const Options& a_options, int a_beginLevel, TFunction a_function);

        /**
         * @brief Executes a function and measures its duration.
         * @tparam TFunction The type of the function.
         * @param a_beginLevel Starting level.
         * @param a_function The function to execute.
         */
        template <typename TFunction>
        void operator()(int a_beginLevel, TFunction a_function);

        /**
         * @brief Executes a function and measures its duration.
         * @tparam TFunction The type of the function.
         * @param a_options Options for measurement.
         * @param a_function The function to execute.
         */
        template <typename TFunction>
        void operator()(const Options& a_options, TFunction a_function);

        /**
         * @brief Executes a function and measures its duration.
         * @tparam TFunction The type of the function.
         * @param a_function The function to execute.
         */
        template <typename TFunction>
        void operator()(TFunction a_function);

        /** @brief Gets the total duration for a specific level. @param a_level Level index. @return Total duration. */
        TimeDuration duration(int a_level = 0) const;
        /** @brief Gets the average duration for a specific level. @param a_level Level index. @return Average duration. */
        TimeDuration average(int a_level = 0) const;
        /** @brief Gets the median duration for a specific level. @param a_level Level index. @return Median duration. */
        TimeDuration median(int a_median = 0) const;
        /** @brief Gets the minimum duration for a specific level. @param a_level Level index. @return Minimum duration. */
        TimeDuration min(int a_level = 0) const;
        /** @brief Gets the maximum duration for a specific level. @param a_level Level index. @return Maximum duration. */
        TimeDuration max(int a_level = 0) const;

      private:
        inline void _prepare(size_t a_step);
        void _appendHistogram(size_t a_level, TimeDuration a_value, size_t a_count, int a_histogramSize, size_t a_ignoreStart = 0, size_t a_ignoreEnd = 0);

        TimePoint                 _timepoint;
        std::vector<Measurement>  _measurements;
        mutable TClock            _clock;
        Options                   _options;
    };

    /**
     * @brief A clock implementation using std::chrono::steady_clock.
     */
    class SteadyClock {
      public:
        /**
         * @brief Returns the current time in nanoseconds.
         * @return Current time point as unsigned long long.
         */
        unsigned long long operator()() {
          std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
          return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
        }
    };

  }
}

#include "duration.ipp"

#endif // DURATION_HPP
