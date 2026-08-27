#include <fcfTest/test.hpp>
#include "helpers.hpp"


namespace fcf {
  namespace NTest {

    template <typename TClock>
    class DurationBasic {
      public:
        typedef unsigned long long TimePoint;
        typedef unsigned long long TimeDuration;

        struct BeginOptions {
          unsigned long long iterationCount;

          BeginOptions()
            : iterationCount(1)
          {}
          BeginOptions(unsigned long long a_iterationCount)
            : iterationCount(std::max(a_iterationCount, (unsigned long long)1))
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
          Measurement()
            : duration(0)
            , timepoint(0)
            , iteration(0)
            , min(0)
            , max(0)
            , pause(true)
          {}
        };

      public:

        DurationBasic()
          : _pause(true)
          , _measurements({Measurement{}})
        {}

        DurationBasic(unsigned long long a_iterationCount, unsigned long long a_measurementStep = 1, unsigned long long a_warmupCount = 0)
          : _pause(true)
          , _measurements({Measurement{}})
          , _options(a_iterationCount, a_measurementStep, a_warmupCount)
        {}

        DurationBasic(const Options& a_options)
          : _pause(true)
          , _measurements({Measurement{}})
          , _options(a_options)
        { }

        Options options() const {
          return _options;
        }

        void options(const Options& a_options) {
          _options = a_options;
        }

        void begin(const BeginOptions& a_options, int a_beginLevel = 0, int a_endLevel = -1) {
          if (a_endLevel > 0) {
            _prepare(a_endLevel-1);
          }
          size_t endLevel   = a_endLevel < 0 ? _measurements.size() : (size_t)a_endLevel;
          size_t startLevel = std::min((size_t)std::max(a_beginLevel, 0), endLevel);
          TimePoint timepoint = _clock();
          for(size_t i = startLevel; i < endLevel; ++i) {
            Measurement& m = _measurements[i];
            if (m.pause) {
              m.pause = false;
              m.timepoint = timepoint;
              m.options = a_options;
            }
          }
        }

        void begin(int a_beginLevel = 0, int a_endLevel = -1) {
          begin(_options, a_beginLevel, a_endLevel);
        }

        void end(int a_beginLevel = 0, int a_endLevel = -1) {
          TimePoint timepoint = _clock();
          if (a_endLevel > 0) {
            _prepare(a_endLevel-1);
          }
          size_t endLevel   = a_endLevel < 0 ? _measurements.size() : (size_t)a_endLevel;
          size_t startLevel = std::min((size_t)std::max(a_beginLevel, 0), endLevel);
          for(size_t i = startLevel; i < endLevel; ++i) {
            if (!_measurements[i].pause) {
              _measurements[i].pause = true;
              TimeDuration diff = timepoint - _measurements[i].timepoint;
              _measurements[i].duration += diff;
              _measurements[i].iteration += _measurements[i].options.iterationCount;
              diff /= _measurements[i].options.iterationCount;
              if (_measurements[i].iteration == 1) {
                _measurements[i].min = diff;
                _measurements[i].max = diff;
              } else {
                _measurements[i].min = std::min(diff, _measurements[i].min);
                _measurements[i].max = std::min(diff, _measurements[i].max);
              }
            }
          }
        }

        void reset(int a_beginLevel = 0, int a_endLevel = -1){
          if (a_endLevel > 0) {
            _prepare(a_endLevel-1);
          }
          size_t endLevel   = a_endLevel < 0 ? _measurements.size() : (size_t)a_endLevel;
          size_t startLevel = std::min((size_t)std::max(a_beginLevel, 0), endLevel);
          for(size_t i = startLevel; i < endLevel; ++i) {
            _measurements[i] = {};
          }
        }

        template <typename TFunction>
        void operator()(Options a_options, int a_beginLevel, int a_endLevel, TFunction a_function){
          if (a_endLevel > 0) {
            _prepare(a_endLevel-1);
          }
          size_t endLevel   = a_endLevel < 0 ? _measurements.size() : (size_t)a_endLevel;
          size_t startLevel = std::min((size_t)std::max(a_beginLevel, 0), endLevel);

          for(unsigned long long i = 0; i < a_options.warmupCount; ++i) {
            a_function();
          }

          bool isFirstMeasurement = true;
          TimeDuration min = 0;
          TimeDuration max = 0;
          TimePoint beginTimepoint = _clock();
          TimePoint timestamp = beginTimepoint;

          for(unsigned long long i = 0; i < a_options.iterationCount; ++i) {
            a_function();
            if ((i + 1) % a_options.measurementStep == 0) {
              TimePoint currentTimestamp = _clock();
              TimeDuration diff = (currentTimestamp - timestamp) / a_options.measurementStep;
              if (isFirstMeasurement) {
                min = diff;
                max = diff;
                isFirstMeasurement = false;
              } else {
                min = std::min(diff, min);
                max = std::max(diff, max);
              }
              timestamp = currentTimestamp;
            }
          }

          TimePoint endTimepoint = _clock();
          TimeDuration diff = endTimepoint - beginTimepoint;
          for(size_t i = startLevel; i < endLevel; ++i) {
            _measurements[i].duration   = diff;
            if (!_measurements[i].iteration) {
              _measurements[i].min        = min;
              _measurements[i].max        = max;
            } else {
              _measurements[i].min        = std::min(_measurements[i].min, min);
              _measurements[i].max        = std::max(_measurements[i].max, max);
            }
            _measurements[i].iteration += a_options.iterationCount;
          }

        }

        template <typename TFunction>
        void operator()(int a_beginLevel, int a_endLevel, TFunction a_function){
          (*this)(_options, a_beginLevel, a_beginLevel+1, a_function);
        }

        template <typename TFunction>
        void operator()(const Options& a_options, int a_beginLevel, TFunction a_function){
          (*this)(a_options, a_beginLevel, a_beginLevel+1, a_function);
        }

        template <typename TFunction>
        void operator()(int a_beginLevel, TFunction a_function){
          (*this)(_options, a_beginLevel, a_beginLevel+1, a_function);
        }

        template <typename TFunction>
        void operator()(const Options& a_options, TFunction a_function){
          (*this)(a_options, 0, 1, a_function);
        }

        template <typename TFunction>
        void operator()(TFunction a_function){
          (*this)(_options, 0, 1, a_function);
        }

        TimeDuration duration(int a_level = 0) const {
          if ((size_t)a_level < _measurements.size()) {
            const Measurement& m = _measurements[(size_t)a_level];
            return m.pause ? m.duration
                           : m.duration + (_clock() - m.timepoint);
          } else {
            return 0;
          }
        }

        TimeDuration average(int a_level = 0) const {
          if ((size_t)a_level < _measurements.size()) {
            const Measurement& m = _measurements[(size_t)a_level];
            TimeDuration d = m.pause ? m.duration
                                     : m.duration + (_clock() - m.timepoint);
            return d / m.iterationCount;
          } else {
            return 0;
          }
        }

        TimeDuration min(int a_level = 0) const {
          if ((size_t)a_level < _measurements.size()) {
            return _measurements[(size_t)a_level].min;
          } else {
            return 0;
          }
        }

        TimeDuration max(int a_level = 0) const {
          if ((size_t)a_level < _measurements.size()) {
            return _measurements[(size_t)a_level].max;
          } else {
            return 0;
          }
        }


      private:
        inline void _prepare(size_t a_step){
          while (a_step >= _measurements.size()){
            _measurements.push_back(Measurement());
          }
        }
        bool                      _pause;
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


struct TestClock{
    TestClock()
      : clock(0)
      , step(1) {
    }
    unsigned long long operator()() {
      clock += step;
      step += 1;
      return clock;
    }
    unsigned long long clock;
    unsigned long long step;
};
namespace {
  struct TestSingleStepClock{
      TestSingleStepClock() {
        clock = 0;
      }
      unsigned long long operator()() {
        clock += 1;
        return clock;
      }
      static unsigned long long clock;
  };
  unsigned long long TestSingleStepClock::clock = 0;
}

FCF_TEST_DEFINE("fcfTest", "duration", "duration single measurement (graph as tests)"){
  {
    fcf::NTest::DurationBasic<TestSingleStepClock> duration;

    duration.begin(0, 3);
    duration.end();
    FCF_TEST(duration.duration() == 1, duration.duration());
    FCF_TEST(duration.duration(1) == 1, duration.duration(1));
    FCF_TEST(duration.duration(2) == 1, duration.duration(2));
    FCF_TEST(TestSingleStepClock::clock == 2, TestSingleStepClock::clock);

    duration.reset(2);
    FCF_TEST(duration.duration() == 1, duration.duration());
    FCF_TEST(duration.duration(1) == 1, duration.duration(1));
    FCF_TEST(duration.duration(2) == 0, duration.duration(2));

    duration.begin();
    duration.end();
    FCF_TEST(duration.duration() == 2, duration.duration());
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(2) == 1, duration.duration(2));

    duration.reset(1);
    FCF_TEST(duration.duration() == 2, duration.duration());
    FCF_TEST(duration.duration(1) == 0, duration.duration(1));
    FCF_TEST(duration.duration(2) == 0, duration.duration(2));

    duration.begin(0, 2);
    duration.end();
    FCF_TEST(duration.duration() == 3, duration.duration());
    FCF_TEST(duration.duration(1) == 1, duration.duration(1));
    FCF_TEST(duration.duration(2) == 0, duration.duration(2));

    duration.begin();
    duration.end();
    FCF_TEST(duration.duration() == 4, duration.duration());
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(2) == 1, duration.duration(2));
  }
}


FCF_TEST_DEFINE("fcfTest", "duration", "duration single measurement (simple)"){
  {
    fcf::NTest::DurationBasic<TestClock> duration;

    FCF_TEST(duration.duration() == 0, duration.duration());
    FCF_TEST(duration.duration(1) == 0, duration.duration());
    duration.begin(0, 2);
    duration.end();
    FCF_TEST(duration.duration() == 2, duration.duration());  // end(1+2) - start(1)
    FCF_TEST(duration.duration() == 2, duration.duration());
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(2) == 0, duration.duration(2));
    FCF_TEST(duration.duration(2) == 0, duration.duration(2));

    duration.begin();
    auto d = duration.duration();
    FCF_TEST(d == 6, d);  // duration(2) + current(1+2+3+4) - begin(1+2+3)
    duration.end();
    FCF_TEST(duration.duration() == 11, duration.duration());  // duration(2) + current(1+2+3+4+5) - begin(1+2+3)
    FCF_TEST(duration.duration(1) == 11, duration.duration(1));
    FCF_TEST(duration.duration(1) == 11, duration.duration(1));
  }

  {
    fcf::NTest::DurationBasic<TestClock> duration;

    FCF_TEST(duration.duration() == 0, duration.duration());
    FCF_TEST(duration.duration(1) == 0, duration.duration());
    duration.begin();
    duration.end();
    FCF_TEST(duration.duration() == 2, duration.duration());  // end(1+2) - start(1)
    FCF_TEST(duration.duration() == 2, duration.duration());
    FCF_TEST(duration.duration(1) == 0, duration.duration(1));
    FCF_TEST(duration.duration(1) == 0, duration.duration(1));
  }

  {
    fcf::NTest::DurationBasic<TestClock> duration;

    FCF_TEST(duration.duration() == 0, duration.duration());
    FCF_TEST(duration.duration(1) == 0, duration.duration());
    duration.begin(0, 2);
    duration.end();
    FCF_TEST(duration.duration() == 2, duration.duration());  // end(1+2) - start(1)
    FCF_TEST(duration.duration() == 2, duration.duration());
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
  }

  {
    fcf::NTest::DurationBasic<TestClock> duration;

    FCF_TEST(duration.duration() == 0, duration.duration());
    FCF_TEST(duration.duration(1) == 0, duration.duration());
    duration.begin(0,2);
    duration.end();
    FCF_TEST(duration.duration() == 2, duration.duration());  // end(1+2) - start(1)
    FCF_TEST(duration.duration() == 2, duration.duration());
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(2) == 0, duration.duration(2));
    FCF_TEST(duration.duration(2) == 0, duration.duration(2));
  }

  {
    fcf::NTest::DurationBasic<TestClock> duration;

    FCF_TEST(duration.duration() == 0, duration.duration());
    FCF_TEST(duration.duration(1) == 0, duration.duration());
    duration.begin(0, 4);
    duration.end();
    FCF_TEST(duration.duration() == 2, duration.duration());  // end(1+2) - start(1)
    FCF_TEST(duration.duration() == 2, duration.duration());
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(2) == 2, duration.duration(2));
    FCF_TEST(duration.duration(2) == 2, duration.duration(2));
    FCF_TEST(duration.duration(3) == 2, duration.duration(3));
    FCF_TEST(duration.duration(3) == 2, duration.duration(3));
  }
  {
    fcf::NTest::DurationBasic<TestClock> duration;

    FCF_TEST(duration.duration() == 0, duration.duration());
    FCF_TEST(duration.duration(1) == 0, duration.duration());
    duration.begin(0, 4);
    duration.end();
    FCF_TEST(duration.duration() == 2, duration.duration());  // end(1+2) - start(1)
    FCF_TEST(duration.duration() == 2, duration.duration());
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(2) == 2, duration.duration(2));
    FCF_TEST(duration.duration(2) == 2, duration.duration(2));
    FCF_TEST(duration.duration(3) == 2, duration.duration(3));
    FCF_TEST(duration.duration(3) == 2, duration.duration(3));
    FCF_TEST(duration.duration(4) == 0, duration.duration(4));
    FCF_TEST(duration.duration(4) == 0, duration.duration(4));
  }


}

