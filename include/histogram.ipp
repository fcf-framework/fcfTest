namespace fcf {
  namespace NTest {

    template <typename TItem, typename TCounter>
    HistogramBasic<TItem, TCounter>::HistogramBasic()
      : _counter(0)
      , _init(true)
      , _min(0)
      , _max(0)
      , _vector(10, 0)
      , _buffer(10, 0){
    }

    template <typename TItem, typename TCounter>
    HistogramBasic<TItem, TCounter>::HistogramBasic(size_t a_capacity)
      : _counter(0)
      , _init(true)
      , _min(0)
      , _max(0)
      , _vector(std::max(a_capacity, (size_t)3), 0)
      , _buffer(std::max(a_capacity, (size_t)3), 0){
    }

    template <typename TItem, typename TCounter>
    void HistogramBasic<TItem, TCounter>::append(TItem a_item) {
      ++_counter;
      if (_init) {
        if (_counter == 1) {
          _min = a_item;
          _max = a_item;
          _vector[0] += 1;
        } else {
          TItem newMin = std::min(a_item, _min);
          TItem newMax = std::max(a_item, _max);
          if (newMin < _min) {
            std::swap(_vector.front(), _vector.back());
          }
          size_t index = _min == a_item ? 0 : _vector.size()-1;
          _vector[index] += 1;
          _min = newMin;
          _max = newMax;
          if (_min != _max) {
            _init = false;
          }
        }
        return;
      }

      if (a_item < _min || a_item > _max) {
        TItem newMin     = std::min(a_item, _min);
        TItem newMax     = std::max(a_item, _max);
        _build(_min, _max, _vector, newMin, newMax, _buffer);
        std::swap(_buffer, _vector);
        _min = newMin;
        _max = newMax;
      }

      TItem scale  = _max - _min;
      TItem weight = a_item - _min;
      size_t index = _calcIndex(weight, scale, _vector.size());
      _vector[index] += 1;
    }

    template <typename TItem, typename TCounter>
    TItem HistogramBasic<TItem, TCounter>::min() const {
      return _min;
    }

    template <typename TItem, typename TCounter>
    TItem HistogramBasic<TItem, TCounter>::max() const {
      return _max;
    }

    template <typename TItem, typename TCounter>
    size_t HistogramBasic<TItem, TCounter>::size() const {
      return _vector.size();
    }

    template <typename TItem, typename TCounter>
    size_t HistogramBasic<TItem, TCounter>::counter() const {
      return _counter;
    }

    template <typename TItem, typename TCounter>
    std::pair<TItem, TItem> HistogramBasic<TItem, TCounter>::rangeByValue(TItem a_value, TItem a_min, TItem a_max, size_t a_size) {
      if (a_min > a_max){
        std::swap(a_min, a_max);
      }
      if (a_value < a_min || a_value > a_max) {
        throw std::out_of_range("Value goes beyond the histogram range");
      }
      a_size              = std::max(a_size, (size_t)1);
      TItem    range      = a_max - a_min + 1;
      size_t   index      = ((double)a_size / range) * (a_value - a_min);
      TCounter leftValue  = a_min + ((double)index / a_size) * range;
      TCounter rightValue = a_min + std::max((TItem)((double)(index+1) / a_size * range), (TItem)1) - 1;
      return { leftValue, rightValue };
    }

    template <typename TItem, typename TCounter>
    std::pair<TItem, TItem> HistogramBasic<TItem, TCounter>::rangeByValue(TItem a_value, TItem a_min, TItem a_max) const {
      return rangeByValue(a_value, a_min, a_max, _vector.size());
    }

    template <typename TItem, typename TCounter>
    std::pair<TItem, TItem> HistogramBasic<TItem, TCounter>::rangeByValue(TItem a_value, size_t a_size) const {
      return rangeByValue(a_value, _min, _max, a_size);
    }

    template <typename TItem, typename TCounter>
    std::pair<TItem, TItem> HistogramBasic<TItem, TCounter>::rangeByValue(TItem a_value) const {
      return rangeByValue(a_value, _min, _max, _vector.size());
    }

    template <typename TItem, typename TCounter>
    std::pair<TItem, TItem> HistogramBasic<TItem, TCounter>::rangeByIndex(size_t a_index, TItem a_min, TItem a_max, size_t a_size) {
      if (a_min > a_max) {
        std::swap(a_min, a_max);
      }
      if (a_index >= a_size) {
        throw std::out_of_range("Index goes beyond the histogram size");
      }
      TItem range = a_max - a_min + 1;
      TItem leftValue  = a_min + (TItem)(((double)a_index / a_size) * range);
      TItem rightValue = a_min + (TItem)(((double)(a_index + 1) / a_size) * range) - 1;

      if (leftValue < a_min) leftValue = a_min;
      if (rightValue > a_max) rightValue = a_max;

      return { leftValue, rightValue };
    }

    template <typename TItem, typename TCounter>
    std::pair<TItem, TItem> HistogramBasic<TItem, TCounter>::rangeByIndex(size_t a_index, TItem a_min, TItem a_max) const {
      return rangeByIndex(a_index, a_min, a_max, _vector.size());
    }

    template <typename TItem, typename TCounter>
    std::pair<TItem, TItem> HistogramBasic<TItem, TCounter>::rangeByIndex(size_t a_index, size_t a_size) const {
      return rangeByIndex(a_index, _min, _max, a_size);
    }

    template <typename TItem, typename TCounter>
    std::pair<TItem, TItem> HistogramBasic<TItem, TCounter>::rangeByIndex(size_t a_index) const {
      return rangeByIndex(a_index, _min, _max, _vector.size());
    }

    template <typename TItem, typename TCounter>
    static TItem HistogramBasic<TItem, TCounter>::median(const std::vector<TCounter>& a_vector, TItem a_min, TItem a_max) {
      if (a_min > a_max){
        std::swap(a_min, a_max);
      }
      TCounter totalSum = std::accumulate(a_vector.begin(), a_vector.end(), 0);
      if (totalSum == 0) {
        return a_min;
      }
      double half      = (double)totalSum / 2.0;
      TCounter sum     = 0;
      size_t n         = a_vector.size();
      double lastRight = 0;
      double medianPosition = 0;
      for (size_t i = 0; i < n; ++i) {
        if (sum + a_vector[i] == (TCounter)half) {
          typename std::vector<TCounter>::const_iterator it        = a_vector.begin()+i+1;
          typename std::vector<TCounter>::const_iterator itEnd     = a_vector.end();
          typename std::vector<TCounter>::const_iterator nonZeroIt = std::find_if(it, itEnd, [](TCounter a_item){ return !!a_item; });
          size_t offset = nonZeroIt - it;
          medianPosition = i + (offset/2) + 1;
          return a_min + ( medianPosition * (a_max-a_min) / a_vector.size() );
        } if (sum + a_vector[i] > (TCounter)half || i + 1 == n) {
          Heights heights = _heights(a_vector, i, lastRight, !!i);
          lastRight = heights.right;

          double area1 = heights.left + heights.center / 2;
          double expected = half - sum;
          if (half <= area1 + sum) {
            //   /|
            //  / |
            // |  |
            // ---
            // expected = (lh + xh) / 2;
            // expected = (lh + lh + (h-lh)*k) / 2;
            //
            // 2*expected - 2*lh
            // ----------------- = k
            //   h - lh
            //
            medianPosition = heights.center != heights.left ? (double)i + std::abs(2*(expected-heights.left) / (heights.center - heights.left))
                                                                : 0.5;
          } else {
            // |\
            // | \
            // |  |
            // ---
            // expected = (h + xh) / 2;
            // expected = (h + h - (h-rh)*(1-k)) / 2;
            //
            //      2*expected - 2*h
            // 1 - ------------------ = k
            //        h - rh
            //
            medianPosition = heights.center != heights.left ? (double)i + 1 - std::abs(2*(expected-heights.center) / (heights.center - heights.right))
                                                                : 0.5;
          }
          return a_min + ( medianPosition * (a_max-a_min) / a_vector.size() );
        }
        sum += a_vector[i];
      }
      return a_min;
    }

    template <typename TItem, typename TCounter>
    TItem HistogramBasic<TItem, TCounter>::median(const std::vector<TCounter>& a_vector) const {
      return median(a_vector, _min, _max);
    }

    template <typename TItem, typename TCounter>
    TItem HistogramBasic<TItem, TCounter>::median(TItem a_min, TItem a_max) const {
      std::vector<TCounter> vector(_vector.size());
      _build(_min, _max, _vector, a_min, a_max, vector);
      return median(vector, a_min, a_max);
    }

    template <typename TItem, typename TCounter>
    TItem HistogramBasic<TItem, TCounter>::median() const {
      return median(_vector, _min, _max);
    }

    template <typename TItem, typename TCounter>
    static std::vector<TCounter> HistogramBasic<TItem, TCounter>::countVector(const std::vector<TCounter>& a_source, TItem a_sourceMin, TItem a_sourceMax, TItem a_min, TItem a_max, size_t a_size) {
      a_size = std::max(a_size, (size_t)3);
      if (a_min == a_sourceMin && a_max == a_sourceMax && a_size == a_source.size()) {
        return a_source;
      }
      std::vector<TCounter> vector(a_size);
      _build(a_sourceMin, a_sourceMax, a_source, a_min, a_max, vector);
      return vector;
    }

    template <typename TItem, typename TCounter>
    std::vector<TCounter> HistogramBasic<TItem, TCounter>::countVector(TItem a_min, TItem a_max, size_t a_size) const {
      return countVector(_vector, _min, _max, a_min, a_max, a_size);
    }

    template <typename TItem, typename TCounter>
    std::vector<TCounter> HistogramBasic<TItem, TCounter>::countVector(TItem a_min, TItem a_max) const {
      return countVector(_vector, _min, _max, a_min, a_max, _vector.size());
    }

    template <typename TItem, typename TCounter>
    std::vector<TCounter> HistogramBasic<TItem, TCounter>::countVector(size_t a_size) const {
      return countVector(_vector, _min, _max, _min, _max, a_size);
    }

    template <typename TItem, typename TCounter>
    std::vector<TCounter> HistogramBasic<TItem, TCounter>::countVector() const {
      return countVector(_vector, _min, _max, _min, _max, _vector.size());
    }

    template <typename TItem, typename TCounter>
    static std::string HistogramBasic<TItem, TCounter>::toTable(const std::vector<TCounter>& a_vector, TItem a_min, TItem a_max) {
      std::stringstream result;
      size_t lineNumberLength = 0;
      size_t valueLength = 0;
      size_t counterLength = 0;
      std::stringstream ss;
      for(size_t i = 0; i < a_vector.size(); ++i) {
        std::pair<TItem, TItem> range = rangeByIndex(i, a_min, a_max, a_vector.size());

        ss << range.first;
        valueLength = std::max(ss.str().length(), valueLength);
        ss.str("");
        ss.clear();

        ss << a_vector[i];
        counterLength = std::max(ss.str().length(), counterLength);
        ss.str("");
        ss.clear();

        ss << i+1;
        lineNumberLength = std::max(ss.str().length(), lineNumberLength);
        ss.str("");
        ss.clear();
      }

      std::string lineNumberHeader = "#";
      lineNumberLength= std::max(lineNumberLength, lineNumberHeader.length());

      std::string valueHeader = "values";
                                    //values
                                    //[12:12]
      valueLength = std::max(valueLength, (size_t)2);

      std::string countHeader = "count";
      counterLength = std::max(counterLength, countHeader.length());


      result << "╔═"<< _drawLine(lineNumberLength) << "═╦═"
             << _drawLine(valueLength*2 + 3)  << "═╦═"
             << _drawLine(counterLength)  << "═╗"
             << std::endl;
      result << "║ "
             << std::setfill(' ') << std::setw(lineNumberLength) << lineNumberHeader << " ║ "
             << std::setfill(' ') << std::setw(valueLength*2 + 3) << valueHeader << " ║ "
             << std::setfill(' ') << std::setw(counterLength) << countHeader << " ║"
             << std::endl;
      result << "╠═"<< _drawLine(lineNumberLength) << "═╬═"
             << _drawLine(valueLength*2 + 3)  << "═╬═"
             << _drawLine(counterLength)  << "═╣"
             << std::endl
            ;



      for(size_t i = 0; i < a_vector.size(); ++i) {
        std::pair<TItem, TItem> range = rangeByIndex(i, a_min, a_max, a_vector.size());
        result  << "║ "
                << std::setfill(' ') << std::setw(lineNumberLength) << (i + 1 ) << " ║ "
                << "["
                  << std::setfill(' ') << std::setw(valueLength) << range.first 
                  << ":"
                  << std::setfill(' ') << std::setw(valueLength) << range.second
                  << "]"
                  << " ║ "
                << std::setfill(' ') << std::setw(counterLength) << a_vector[i] << " ║"
                << std::endl;
      }
      result << "╚═"<< _drawLine(lineNumberLength) << "═╩═"
             << _drawLine(valueLength*2 + 3)  << "═╩═"
             << _drawLine(counterLength)  << "═╝"
             << std::endl;

      return result.str();
    }

    template <typename TItem, typename TCounter>
    std::string HistogramBasic<TItem, TCounter>::toTable(TItem a_min, TItem a_max, size_t a_size) const {
      std::vector<TCounter> vector(a_size);
      _build(_min, _max, _vector, a_min, a_max, vector);
      return toTable(vector, a_min, a_max);
    }

    template <typename TItem, typename TCounter>
    std::string HistogramBasic<TItem, TCounter>::toTable(size_t a_size) const {
      return toTable(_min, _max, a_size);
    }


    template <typename TItem, typename TCounter>
    static std::string HistogramBasic<TItem, TCounter>::toBarChart(const std::vector<TCounter>& a_vector, TItem a_min, TItem a_max, size_t a_width, size_t a_height) {
      std::stringstream result;

      a_width = std::max(a_width, (size_t)1);
      a_height = std::max(a_height, (size_t)1);

      std::vector<TCounter> vector(a_width);
      _build(a_min, a_max, a_vector, a_min, a_max, vector);

      TCounter currentCounterMin = std::numeric_limits<TCounter>::max();
      TCounter currentCounterMax = std::numeric_limits<TCounter>::min();
      for(TCounter v : vector){
        currentCounterMin = std::min(v, currentCounterMin);
        currentCounterMax = std::max(v, currentCounterMax);
      }
      TCounter currentCounterScale = currentCounterMax - currentCounterMin;
      if (currentCounterScale == 0) currentCounterScale = 1;

      size_t maxCount = 0;
      for(size_t r = a_height-1; r < std::numeric_limits<size_t>::max(); --r) {
        for(size_t c = 0; c < a_width; ++c) {
          TCounter currentValue = vector[c] - currentCounterMin;
          double   displayValue = (double)currentValue * (double)a_height / (double)currentCounterScale;
          result << (displayValue > (double)r ? "|" : " ");
          maxCount = std::max(maxCount, vector[c]);
        }
        result << std::endl;
      }
      result << _drawLine((int)a_width) << std::endl;

      double stepx = (double)(a_max - a_min) / a_width;
      double stepy = (double)maxCount / a_height;

      result <<  std::fixed << std::setprecision(2);
      result << "OX (value): [" << a_min << ":" << a_max << "]" << ";  Step: " << stepx << std::endl;
      result << "OY (count): [" << 0 << ":" << maxCount << "]" << ";  Step: " << stepy << std::endl;
      return result.str();
    }

    template <typename TItem, typename TCounter>
    std::string HistogramBasic<TItem, TCounter>::toBarChart(TItem a_min, TItem a_max, size_t a_width, size_t a_height) const {
      std::vector<TCounter> vector(a_width);
      _build(_min, _max, _vector, a_min, a_max, vector);
      return toBarChart(vector, a_min, a_max, a_width, a_height);
    }

    template <typename TItem, typename TCounter>
    std::string HistogramBasic<TItem, TCounter>::toBarChart(size_t a_width, size_t a_height) const {
      return toBarChart(_min, _max, a_width, a_height);
    }

    template <typename TItem, typename TCounter>
    typename HistogramBasic<TItem, TCounter>::Heights HistogramBasic<TItem, TCounter>::_heights(const std::vector<TCounter>& a_source, size_t a_index, double a_lastHeight, bool a_enableLastHeight) {
      Heights result;

      if (a_source.size() == 1) {
        result.left = result.right = a_source[a_index];
      } else if (a_index+1 >= a_source.size()) {
        result.left = result.right = a_enableLastHeight ? a_lastHeight
                                                        : a_source[a_index];
      } else {
        result.left   = a_enableLastHeight            ? a_lastHeight :
                        a_index + 1 < a_source.size() ? a_source[a_index + 1] :
                                                        a_source[a_index];
        result.right = a_index+1 < a_source.size() ? a_source[a_index + 1]
                                                    : result.left;
      }

      // p = p1 + p2;
      // p = lh*lw + (h-lh)*lw/2 + rh*rw + (h-rh)*rw/2
      // p - lh*lw - rh*rw = (h-lh)*lw/2 + (h-rh)*rw/2
      // 2(p - lh*lw - rh*rw) = (h-lh)*lw + (h-rh)*rw
      // 2 * (p - lh*lw - rh*rw) + lh*lw + rw*wh = h*lw + h*rw
      // 2*p - lh*lw - rh*rw
      // ------------------- = h
      //       lw + rw
      result.center = 2.0 * a_source[a_index] - result.left/2.0 - result.right/2.0;
      if (result.center < 0) {
        // 2*p - lh*w*k/2 - rh*w*k/2
        // ------------------------- = 0
        //           w
        //
        // lh*w*k/2 + rh*w*k/2 = 2 * p
        //
        //         4*p
        // k = -------------
        //      lh*w + rh*w
        //
        result.center  = 0;
        double k       = 4.0 * a_source[a_index] / (result.left + result.right);
        result.left   *= k;
        result.right  *= k;
      }

      return result;
    }

    template <typename TItem, typename TCounter>
    void HistogramBasic<TItem, TCounter>::_build(TItem a_sourceMin, TItem a_sourceMax, const std::vector<TCounter>& a_source, TItem a_newMin, TItem a_newMax, std::vector<TCounter>& a_destination) {
      std::fill(a_destination.begin(), a_destination.end(), 0);

      TItem sourceRange = a_sourceMax - a_sourceMin + 1;
      TItem destinationRange = a_newMax - a_newMin + 1;

      double lastRight                    = 0;
      for(size_t i = 0; i < a_source.size(); ++i) {
        bool isLast = i + 1 == a_source.size();

        double indexRatio = (double)i / a_source.size();
        double nextIndexRatio = (double)(i+1) / a_source.size();

        double leftSourceValue  = i == 0 ? (double)a_sourceMin
                                         : (double)a_sourceMin + indexRatio * sourceRange;
        double rightSourceValue = isLast ? (double)a_sourceMax
                                         : (double)a_sourceMin +  nextIndexRatio * sourceRange - 1;

        if (leftSourceValue > a_newMax || rightSourceValue < a_newMin) {
          continue;
        }

        double leftDestinationValue                = std::min(std::max((double)leftSourceValue, (double)a_newMin), (double)a_newMax);
        double rightDestinationValue               = std::min(std::max((double)rightSourceValue, (double)a_newMin), (double)a_newMax);
        double leftDestinationRatio                = (double)(leftDestinationValue - a_newMin) / destinationRange;
        double rightDestinationRatioNI             = (double)(rightDestinationValue - a_newMin) / destinationRange;
        double leftDestinationIndexF               = leftDestinationRatio * a_destination.size();
        double rightDestinationIndexF              = ((double)(rightDestinationValue - a_newMin+1) / destinationRange)* a_destination.size();
        std::ptrdiff_t leftDestinationIndex        = (std::ptrdiff_t)(leftDestinationRatio * a_destination.size());
        std::ptrdiff_t rightDestinationIndex       = (std::ptrdiff_t)(rightDestinationRatioNI * a_destination.size());
        std::ptrdiff_t leftDestinationAccessIndex  = std::max(std::min(leftDestinationIndex, (std::ptrdiff_t)a_destination.size()-1), (std::ptrdiff_t)0);
        std::ptrdiff_t rightDestinationAccessIndex = std::max(std::min(rightDestinationIndex, (std::ptrdiff_t)a_destination.size()-1), (std::ptrdiff_t)0);
        double scalek                              = ((double)destinationRange / a_destination.size()) / ((double)sourceRange / a_source.size());

        Heights heights = _heights(a_source, i, lastRight, !!i);
        lastRight = heights.right;
        heights.left *= scalek;
        heights.center *= scalek;
        heights.right *= scalek;

        TCounter area = a_source[i];
        for(std::ptrdiff_t destinationIndex = leftDestinationAccessIndex; destinationIndex <= rightDestinationAccessIndex; ++destinationIndex) {
          double bitDestinationValueBegin = (double)destinationRange * destinationIndex / a_destination.size();
          bitDestinationValueBegin        = std::max(std::min(bitDestinationValueBegin, rightDestinationValue+1), leftDestinationValue);
          double bitDestinationValueEnd   = (double)destinationRange * (destinationIndex + 1) / a_destination.size();
          bitDestinationValueEnd          = std::max(std::min(bitDestinationValueEnd, rightDestinationValue+1), leftDestinationValue);

          double bitDestinationRatioBegin = (double)(bitDestinationValueBegin - leftDestinationValue) / (rightDestinationValue - leftDestinationValue + 1);
          double bitDestinationRatioEnd   = (double)(bitDestinationValueEnd - leftDestinationValue) / (rightDestinationValue - leftDestinationValue + 1);

          double dArea = 0;
          if (bitDestinationRatioBegin <= 0.5){
            double leftk   = bitDestinationRatioBegin*2.0;
            double rightk  = std::min(bitDestinationRatioEnd, 0.5)*2.0;
            double widthk  = (rightk - leftk) * (rightDestinationIndexF - leftDestinationIndexF) / 2.0;
            double lefth   = heights.left + (heights.center - heights.left) * leftk;
            double righth  = heights.left + (heights.center - heights.left) * rightk;
            dArea         += std::abs(righth + lefth) / 2 * widthk;
          }
          if (bitDestinationRatioEnd >= 0.5){
            double leftk   = std::max(bitDestinationRatioBegin, 0.5)*2.0 - 1;
            double rightk  = bitDestinationRatioEnd * 2.0 - 1;
            double widthk  = (rightk - leftk) * (rightDestinationIndexF - leftDestinationIndexF) / 2.0;
            double lefth   = heights.center + (heights.right - heights.center) * leftk;
            double righth  = heights.center + (heights.right - heights.center) * rightk;
            dArea         += std::abs(righth + lefth) / 2 * widthk;
          }

          TCounter s      = std::round(dArea - 0.1);
          s               = area > s ? s : area;
          area           -= s;

          a_destination[destinationIndex] += s;
        }

        if (!area && leftDestinationAccessIndex == 0 && !a_destination.front()) {
          auto itEnd = a_destination.begin() + rightDestinationAccessIndex + 1;
          auto it = std::find_if(a_destination.begin() + leftDestinationAccessIndex, itEnd, [](TCounter a_item){ return !!a_item; });
          if (it != itEnd) {
            ++a_destination.front();
            --*it;
          }
        }

        while(area) {
          if (leftDestinationAccessIndex == 0 && !a_destination.front()) {
            ++a_destination.front();
            --area;
            if (!area) {
              break;
            }
          }

          if (rightDestinationAccessIndex + 1 ==  (std::ptrdiff_t)a_destination.size() &&  !a_destination.back()) {
            ++a_destination.back();
            --area;
            if (!area) {
              break;
            }
          }

          size_t tsize  = rightDestinationAccessIndex - leftDestinationAccessIndex + 1;
          size_t size   = tsize / 2;
          size_t offset = size;
          if (!tsize || tsize % 2) {
            ++a_destination[leftDestinationAccessIndex + size];
            ++offset;
            --area;
          }
          for(size_t i = 0; area && i < size; ++i) {
            size_t leftIndex = leftDestinationAccessIndex + size - i - 1;
            ++a_destination[leftIndex];
            --area;
            if (!area) {
              break;
            }
            size_t rightIndex = leftDestinationAccessIndex + offset + i;
            ++a_destination[rightIndex];
            --area;
            if (!area) {
              break;
            }
          }
        }

      }
    }

    template <typename TItem, typename TCounter>
    size_t HistogramBasic<TItem, TCounter>::_calcIndex(TItem a_weight, TItem a_scale, size_t a_size) const {
      if (a_scale == 0) return 0;
      size_t result = std::min((size_t)((long double)a_size * (long double)a_weight / (long double)a_scale), a_size-1);
      return result;
    }

    template <typename TItem, typename TCounter>
    static std::string HistogramBasic<TItem, TCounter>::_drawLine(int length) {
      std::string unicodeLine = "";
      for(int i = 0; i < length; ++i) {
        unicodeLine += "═";
      }
      return unicodeLine;
    }

  }
}


