#ifndef DURATION_HPP
#define DURATION_HPP

#include <vector>
#include <chrono>
#include <algorithm>
#include <functional>
#include "histogram.hpp"

namespace fcf {
  namespace NTest {

    /**
     * @brief Класс для замера времени выполнения операций с поддержкой гистограммы.
     *
     * @tparam TClock Тип объекта, вызываемого для получения текущего времени.
     */
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

        DurationBasic()
          : _pause(true)
          , _timepoint(0)
          , _measurements({Measurement{}})
          , _options()
        {}

        DurationBasic(unsigned long long a_iterationCount, unsigned long long a_measurementStep = 1, unsigned long long a_warmupCount = 0)
          : _pause(true)
          , _timepoint(0)
          , _measurements({Measurement{}})
          , _options(a_iterationCount, a_measurementStep, a_warmupCount)
        {}

        DurationBasic(const Options& a_options)
          : _pause(true)
          , _timepoint(0)
          , _measurements({Measurement{}})
          , _options(a_options)
        {}

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
              m.excludedTime = 0;
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
              TimeDuration rawDiff = timepoint - _measurements[i].timepoint;
              TimeDuration diff = rawDiff > _measurements[i].excludedTime ? rawDiff - _measurements[i].excludedTime : 0;

              _measurements[i].duration += diff;
              _measurements[i].iteration += _measurements[i].options.iterationCount;

              if (_measurements[i].options.iterationCount > 0) {
                  TimeDuration avgDiff = diff / _measurements[i].options.iterationCount;
                  if (_measurements[i].iteration == _measurements[i].options.iterationCount) {
                    _measurements[i].min = avgDiff;
                    _measurements[i].max = avgDiff;
                  } else {
                    _measurements[i].min = std::min(avgDiff, _measurements[i].min);
                    _measurements[i].max = std::max(avgDiff, _measurements[i].max);
                  }
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
            _measurements[i] = Measurement();
          }
        }

        /**
         * @brief Добавляет значение в гистограмму указанного уровня.
         * @param a_level Уровень замера.
         * @param a_value Значение для добавления.
         *
         * Время выполнения этого метода (включая возможное расширение гистограммы)
         * исключается из замера всех активных таймеров.
         */
        void appendHistogram(size_t a_level, TimeDuration a_value) {
          if (a_level >= _measurements.size()) {
            return;
          }

          TimePoint t1 = _clock();
          _measurements[a_level].histogram.append(a_value);
          TimePoint t2 = _clock();

          TimeDuration cost = t2 - t1;

          // Исключаем время из всех активных таймеров
          for(size_t i = 0; i < _measurements.size(); ++i) {
            if (!_measurements[i].pause) {
              _measurements[i].excludedTime += cost;
            }
          }
        }

        const HistogramType& histogram(size_t a_level) const {
          if (a_level < _measurements.size()) {
            return _measurements[a_level].histogram;
          }
          static const HistogramType empty;
          return empty;
        }

        HistogramType& histogram(size_t a_level) {
          if (a_level < _measurements.size()) {
            return _measurements[a_level].histogram;
          }
          // Если уровень не существует, создаем его
          _prepare(a_level);
          return _measurements[a_level].histogram;
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
              appendHistogram(a_beginLevel, diff);
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
          (*this)(_options, a_beginLevel, a_endLevel, a_function);
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
            if (m.pause) {
              return m.duration > m.excludedTime ? m.duration - m.excludedTime : 0;
            } else {
              TimeDuration current = _clock() - m.timepoint;
              TimeDuration total = m.duration + current;
              return total > m.excludedTime ? total - m.excludedTime : 0;
            }
          } else {
            return 0;
          }
        }

        TimeDuration average(int a_level = 0) const {
          if ((size_t)a_level < _measurements.size()) {
            const Measurement& m = _measurements[(size_t)a_level];
            TimeDuration d = duration(a_level);
            if (m.iteration > 0) {
              return d / m.iteration;
            }
            return 0;
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

#include "duration.ipp"

#endif // DURATION_HPP
