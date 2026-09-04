namespace fcf {
  namespace NTest {

    template <typename TClock>
    DurationBasic<TClock>::DurationBasic()
      : _timepoint(0)
      , _measurements({Measurement{}})
      , _options(1LL, 10, 1LL, 0LL)
    {}

    template <typename TClock>
    DurationBasic<TClock>::DurationBasic(unsigned long long a_iterationCount, unsigned long long a_measurementStep, unsigned long long a_warmupCount)
      : _timepoint(0)
      , _measurements({Measurement{}})
      , _options(std::max((long long)a_iterationCount, 1LL),
                 10,
                 std::max((long long)a_measurementStep, 1LL),
                 (long long)a_warmupCount
                )
    {}

    template <typename TClock>
    DurationBasic<TClock>::DurationBasic(const Options& a_options)
      : _timepoint(0)
      , _measurements({Measurement{}})
      , _options(std::max(a_options.iterationCount, 1LL),
                 a_options.histogramSize >= 0  ? std::max(a_options.histogramSize, 2)  : 10,
                 std::max(a_options.measurementStep, 1LL),
                 std::max(a_options.warmupCount, 0LL)
                )
    {}

    template <typename TClock>
    typename DurationBasic<TClock>::Options DurationBasic<TClock>::options() const {
      return _options;
    }

    template <typename TClock>
    void DurationBasic<TClock>::options(const Options& a_options) {
      if (a_options.iterationCount >= 0) {
        _options.iterationCount = std::max(a_options.iterationCount, 1LL);
      }
      if (a_options.histogramSize >= 0) {
        _options.histogramSize = std::max(a_options.histogramSize, 2LL);
      }
      if (a_options.measurementStep >= 0) {
        _options.measurementStep = std::max(a_options.measurementStep, 1LL);
      }
      if (a_options.warmupCount >= 0) {
        _options.warmupCount = std::max(a_options.warmupCount, 0LL);
      }
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

          long long iterationCount = _measurements[i].options.iterationCount < 0 ? _options.iterationCount
                                                                                 : _measurements[i].options.iterationCount;
          int histogramSize        = _measurements[i].options.histogramSize < 0  ?  _options.histogramSize 
                                                                                 : std::max(_measurements[i].options.histogramSize, 2);


          _measurements[i].duration += diff;
          _measurements[i].iteration += std::max(iterationCount, 1LL);

          if (iterationCount) {
              TimeDuration avgDiff = diff / iterationCount;
              if (_measurements[i].iteration == iterationCount) {
                _measurements[i].min = avgDiff;
                _measurements[i].max = avgDiff;
              } else {
                _measurements[i].min = std::min(avgDiff, _measurements[i].min);
                _measurements[i].max = std::max(avgDiff, _measurements[i].max);
              }
              _appendHistogram(i, diff, iterationCount, histogramSize, startLevel, endLevel);
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

      long long warmupCount     = a_options.warmupCount < 0 ? _options.warmupCount : a_options.warmupCount;
      long long iterationCount  = a_options.iterationCount < 0 ? _options.iterationCount : a_options.iterationCount;
      long long measurementStep = a_options.measurementStep < 0 ?  _options.measurementStep : std::max(a_options.measurementStep, 1LL);
      int histogramSize         = a_options.histogramSize < 0 ?  _options.histogramSize : std::max(a_options.histogramSize, 2);


      for(unsigned long long i = 0; i < warmupCount; ++i) {
        a_function();
      }

      if (!iterationCount){
        return;
      }

      bool isFirstMeasurement = true;
      TimeDuration min = 0;
      TimeDuration max = 0;
      TimePoint beginTimepoint = _clock();
      TimePoint timepoint = beginTimepoint;

      for(unsigned long long i = 0; i < iterationCount; ++i) {
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
            _appendHistogram(level, diff, measurementStep, histogramSize);
          }
          timepoint = currentTimestamp;
        }
      }

      TimePoint endTimepoint = _clock();

      unsigned long long remainder = iterationCount % measurementStep;
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
          _appendHistogram(level, remainderDiff, remainder, histogramSize);
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
        _measurements[i].iteration += iterationCount;
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
    typename DurationBasic<TClock>::TimeDuration DurationBasic<TClock>::median(int a_level) const {
      return (size_t)a_level < _measurements.size() 
                ? _measurements[(size_t)a_level].histogram.median()
                : 0;
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
    void DurationBasic<TClock>::_appendHistogram(size_t a_level, TimeDuration a_value, size_t a_count, int a_histogramSize, size_t a_ignoreStart, size_t a_ignoreEnd) {
      if (a_level >= _measurements.size()) {
        return;
      }

      if (!_measurements[a_level].histogram.overflow(a_value)) {
        _measurements[a_level].histogram.append(a_value, a_count);
      } else {
        TimePoint t1 = _clock();
        if (a_histogramSize >= 0) {
          _measurements[a_level].histogram.size(a_histogramSize);
        }
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
