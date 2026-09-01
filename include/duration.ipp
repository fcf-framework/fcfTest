namespace fcf {
  namespace NTest {

    template <typename TClock>
    DurationBasic<TClock>::DurationBasic()
      : _timepoint(0)
      , _measurements({Measurement{}})
      , _options()
    {}

    template <typename TClock>
    DurationBasic<TClock>::DurationBasic(unsigned long long a_iterationCount, unsigned long long a_measurementStep, unsigned long long a_warmupCount)
      : _timepoint(0)
      , _measurements({Measurement{}})
      , _options(a_iterationCount, a_measurementStep, a_warmupCount)
    {}

    template <typename TClock>
    DurationBasic<TClock>::DurationBasic(const Options& a_options)
      : _timepoint(0)
      , _measurements({Measurement{}})
      , _options(a_options)
    {}

    template <typename TClock>
    typename DurationBasic<TClock>::Options DurationBasic<TClock>::options() const {
      return _options;
    }

    template <typename TClock>
    void DurationBasic<TClock>::options(const Options& a_options) {
      _options = a_options;
    }

    template <typename TClock>
    void DurationBasic<TClock>::begin(const BeginOptions& a_options, int a_beginLevel, int a_endLevel) {
      _prepare(std::max(a_beginLevel, a_endLevel-1));

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

    template <typename TClock>
    void DurationBasic<TClock>::begin(const BeginOptions& a_options, int a_beginLevel) {
      begin(a_options, a_beginLevel, a_beginLevel+1);
    }

    template <typename TClock>
    void DurationBasic<TClock>::begin(int a_beginLevel) {
      begin(_options, a_beginLevel, a_beginLevel+1);
    }

    template <typename TClock>
    void DurationBasic<TClock>::begin(int a_beginLevel, int a_endLevel) {
      begin(_options, a_beginLevel, a_endLevel);
    }

    template <typename TClock>
    void DurationBasic<TClock>::end(int a_beginLevel, int a_endLevel) {
      TimePoint timepoint = _clock();

      _prepare(std::max(a_beginLevel, a_endLevel-1));

      size_t endLevel   = a_endLevel < 0 ? _measurements.size() : (size_t)a_endLevel;
      size_t startLevel = std::min((size_t)std::max(a_beginLevel, 0), endLevel);
      for(size_t i = startLevel; i < endLevel; ++i) {
        if (!_measurements[i].pause) {
          _measurements[i].pause = true;
          TimeDuration rawDiff = timepoint - _measurements[i].timepoint;
          TimeDuration diff = rawDiff > _measurements[i].excludedTime ? rawDiff - _measurements[i].excludedTime : TimeDuration();

          _measurements[i].duration += diff;
          _measurements[i].iteration += std::max(_measurements[i].options.iterationCount, 1ULL);

          if (_measurements[i].options.iterationCount) {
              TimeDuration avgDiff = diff / _measurements[i].options.iterationCount;
              if (_measurements[i].iteration == _measurements[i].options.iterationCount) {
                _measurements[i].min = avgDiff;
                _measurements[i].max = avgDiff;
              } else {
                _measurements[i].min = std::min(avgDiff, _measurements[i].min);
                _measurements[i].max = std::max(avgDiff, _measurements[i].max);
              }
              _appendHistogram(i, diff, _measurements[i].options.iterationCount, startLevel, endLevel);
          }
        }
      }
    }

    template <typename TClock>
    void DurationBasic<TClock>::end(int a_beginLevel) {
      end(a_beginLevel, a_beginLevel+1);
    }

    template <typename TClock>
    void DurationBasic<TClock>::reset(int a_beginLevel, int a_endLevel){
      if (a_endLevel > 0) {
        _prepare(a_endLevel-1);
      }
      size_t endLevel   = a_endLevel < 0 ? _measurements.size() : (size_t)a_endLevel;
      size_t startLevel = std::min((size_t)std::max(a_beginLevel, 0), endLevel);
      for(size_t i = startLevel; i < endLevel; ++i) {
        _measurements[i] = Measurement();
      }
    }

    template <typename TClock>
    const typename DurationBasic<TClock>::HistogramType& DurationBasic<TClock>::histogram(size_t a_level) const {
      if (a_level < _measurements.size()) {
        return _measurements[a_level].histogram;
      }
      static const HistogramType empty;
      return empty;
    }

    template <typename TClock>
    typename DurationBasic<TClock>::HistogramType& DurationBasic<TClock>::histogram(size_t a_level) {
      if (a_level < _measurements.size()) {
        return _measurements[a_level].histogram;
      }
      _prepare(a_level);
      return _measurements[a_level].histogram;
    }

    template <typename TClock>
    template <typename TFunction>
    void DurationBasic<TClock>::operator()(Options a_options, int a_beginLevel, int a_endLevel, TFunction a_function){
      _prepare(std::max(a_beginLevel, a_endLevel-1));

      size_t endLevel   = a_endLevel < 0 ? _measurements.size() : (size_t)a_endLevel;
      size_t startLevel = std::min((size_t)std::max(a_beginLevel, 0), endLevel);

      for(unsigned long long i = 0; i < a_options.warmupCount; ++i) {
        a_function();
      }

      if (!a_options.iterationCount){
        return;
      }

      bool isFirstMeasurement = true;
      TimeDuration min = 0;
      TimeDuration max = 0;
      TimePoint beginTimepoint = _clock();
      TimePoint timepoint = beginTimepoint;

      unsigned long long measurementStep = std::max(a_options.measurementStep, 1ULL);

      for(unsigned long long i = 0; i < a_options.iterationCount; ++i) {
        a_function();
        if ((i + 1) % measurementStep == 0) {
          TimePoint currentTimestamp = _clock();
          TimeDuration diff = (currentTimestamp - timepoint) / measurementStep;
          if (isFirstMeasurement) {
            min = diff;
            max = diff;
            isFirstMeasurement = false;
          } else {
            min = std::min(diff, min);
            max = std::max(diff, max);
          }
          for(int level = a_beginLevel; level < a_endLevel; ++level) {
            _appendHistogram(level, diff, measurementStep);
          }
          timepoint = currentTimestamp;
        }
      }

      TimePoint endTimepoint = _clock();

      unsigned long long remainder = a_options.iterationCount % measurementStep;
      if (remainder){
        TimeDuration remainderDiff = (endTimepoint - timepoint) / remainder;
        if (isFirstMeasurement) {
          min = remainderDiff;
          max = remainderDiff;
        } else {
          min = std::min(remainderDiff, min);
          max = std::max(remainderDiff, max);
        }
        for(int level = a_beginLevel; level < a_endLevel; ++level) {
          _appendHistogram(level, remainderDiff, remainder);
        }
      }


      TimeDuration diff = endTimepoint - beginTimepoint;
      for(size_t i = startLevel; i < endLevel; ++i) {
        _measurements[i].duration   += diff;
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

    template <typename TClock>
    template <typename TFunction>
    void DurationBasic<TClock>::operator()(int a_beginLevel, int a_endLevel, TFunction a_function){
      (*this)(_options, a_beginLevel, a_endLevel, a_function);
    }

    template <typename TClock>
    template <typename TFunction>
    void DurationBasic<TClock>::operator()(const Options& a_options, int a_beginLevel, TFunction a_function){
      (*this)(a_options, a_beginLevel, a_beginLevel+1, a_function);
    }

    template <typename TClock>
    template <typename TFunction>
    void DurationBasic<TClock>::operator()(int a_beginLevel, TFunction a_function){
      (*this)(_options, a_beginLevel, a_beginLevel+1, a_function);
    }

    template <typename TClock>
    template <typename TFunction>
    void DurationBasic<TClock>::operator()(const Options& a_options, TFunction a_function){
      (*this)(a_options, 0, 1, a_function);
    }

    template <typename TClock>
    template <typename TFunction>
    void DurationBasic<TClock>::operator()(TFunction a_function){
      (*this)(_options, 0, 1, a_function);
    }

    template <typename TClock>
    typename DurationBasic<TClock>::TimeDuration DurationBasic<TClock>::duration(int a_level) const {
      if ((size_t)a_level < _measurements.size()) {
        const Measurement& m = _measurements[(size_t)a_level];
        if (m.pause) {
          return m.duration > m.excludedTime ? m.duration - m.excludedTime : TimeDuration();
        } else {
          TimeDuration current = _clock() - m.timepoint;
          TimeDuration total = m.duration + current;
          return total > m.excludedTime ? total - m.excludedTime : TimeDuration();
        }
      } else {
        return 0;
      }
    }

    template <typename TClock>
    typename DurationBasic<TClock>::TimeDuration DurationBasic<TClock>::average(int a_level) const {
      if ((size_t)a_level < _measurements.size()) {
        const Measurement& m = _measurements[(size_t)a_level];
        TimeDuration d = duration(a_level);
        if (m.iteration > 0) {
          return d / m.iteration;
        }
        return d;
      } else {
        return 0;
      }
    }

    template <typename TClock>
    typename DurationBasic<TClock>::TimeDuration DurationBasic<TClock>::min(int a_level) const {
      if ((size_t)a_level < _measurements.size()) {
        return _measurements[(size_t)a_level].min;
      } else {
        return 0;
      }
    }

    template <typename TClock>
    typename DurationBasic<TClock>::TimeDuration DurationBasic<TClock>::max(int a_level) const {
      if ((size_t)a_level < _measurements.size()) {
        return _measurements[(size_t)a_level].max;
      } else {
        return 0;
      }
    }

    template <typename TClock>
    inline void DurationBasic<TClock>::_prepare(size_t a_step){
      while (a_step >= _measurements.size()){
        _measurements.push_back(Measurement());
      }
    }

    template <typename TClock>
    void DurationBasic<TClock>::_appendHistogram(size_t a_level, TimeDuration a_value, size_t a_count, size_t a_ignoreStart, size_t a_ignoreEnd) {
      if (a_level >= _measurements.size()) {
        return;
      }

      if (!_measurements[a_level].histogram.overflow(a_value)) {
        _measurements[a_level].histogram.append(a_value, a_count);
      } else {
        TimePoint t1 = _clock();
        _measurements[a_level].histogram.append(a_value, a_count);
        TimePoint t2 = _clock();

        TimeDuration diff = t2 - t1;

        for(size_t i = 0; i < _measurements.size(); ++i) {
          if (i >= a_ignoreStart && i < a_ignoreEnd) {
            continue;
          }
          if (!_measurements[i].pause) {
            _measurements[i].excludedTime += diff;
          }
        }
      }
    }

  }
}
