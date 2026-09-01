#ifndef DURATION_HPP
#define DURATION_HPP

#include <vector>
#include <chrono>
#include <algorithm>
#include <functional>
#include "histogram.hpp"

namespace fcf {
  namespace NTest {

    template <typename TClock>
    class DurationBasic {
      public:
        typedef unsigned long long TimePoint;
        typedef unsigned long long TimeDuration;
        typedef HistogramBasic<TimeDuration, TimeDuration> HistogramType;

        struct BeginOptions {
          unsigned long long iterationCount;

          BeginOptions()
            : iterationCount(1)
          {}
          BeginOptions(unsigned long long a_iterationCount)
            : iterationCount(a_iterationCount)
          {}
        };

        struct Options : public BeginOptions {
          unsigned long long measurementStep;
          unsigned long long warmupCount;
          Options()
            : measurementStep(1)
            , warmupCount(0)
          {}
          Options(unsigned long long a_iterationCount, unsigned long long a_measurementStep = 1, unsigned long long a_warmupCount = 0)
            : BeginOptions(a_iterationCount)
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

        DurationBasic();
        DurationBasic(unsigned long long a_iterationCount, unsigned long long a_measurementStep = 1, unsigned long long a_warmupCount = 0);
        DurationBasic(const Options& a_options);

        Options options() const;
        void options(const Options& a_options);

        void begin(const BeginOptions& a_options, int a_beginLevel, int a_endLevel);
        void begin(const BeginOptions& a_options, int a_beginLevel = 0);
        void begin(int a_beginLevel = 0);
        void begin(int a_beginLevel, int a_endLevel);

        void end(int a_beginLevel, int a_endLevel);
        void end(int a_beginLevel=0);

        void reset(int a_beginLevel = 0, int a_endLevel = -1);

        const HistogramType& histogram(size_t a_level) const;
        HistogramType& histogram(size_t a_level);

        template <typename TFunction>
        void operator()(Options a_options, int a_beginLevel, int a_endLevel, TFunction a_function);

        template <typename TFunction>
        void operator()(int a_beginLevel, int a_endLevel, TFunction a_function);

        template <typename TFunction>
        void operator()(const Options& a_options, int a_beginLevel, TFunction a_function);

        template <typename TFunction>
        void operator()(int a_beginLevel, TFunction a_function);

        template <typename TFunction>
        void operator()(const Options& a_options, TFunction a_function);

        template <typename TFunction>
        void operator()(TFunction a_function);

        TimeDuration duration(int a_level = 0) const;
        TimeDuration average(int a_level = 0) const;
        TimeDuration min(int a_level = 0) const;
        TimeDuration max(int a_level = 0) const;

      private:
        inline void _prepare(size_t a_step);
        void _appendHistogram(size_t a_level, TimeDuration a_value, size_t a_count, size_t a_ignoreStart = 0, size_t a_ignoreEnd = 0);

        TimePoint                 _timepoint;
        std::vector<Measurement>  _measurements;
        mutable TClock            _clock;
        Options                   _options;
    };

    class SteadyClock {
      public:
        unsigned long long operator()() {
          std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
          return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
        }
    };

  }
}

#include "duration.ipp"

#endif // DURATION_HPP