#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iomanip>
#include <stdexcept>
#include <limits>

namespace fcf {
  namespace NTest {

    /**
     * @brief A basic histogram class for frequency distribution analysis.
     * 
     * This class provides tools to collect data points, calculate statistical measures 
     * like median, and visualize the distribution through tables or bar charts.
     * 
     * @tparam TItem The type of the data items (e.g., int, double).
     * @tparam TCounter The type used for counting occurrences (default: size_t).
     */
    template <typename TItem, typename TCounter = size_t>
    class HistogramBasic {
      public:
        /**
         * @brief Default constructor. Initializes an empty histogram with default capacity.
         */
        HistogramBasic();

        /**
         * @brief Constructor with specified capacity.
         * @param a_capacity The initial number of bins in the histogram.
         */
        HistogramBasic(size_t a_capacity);

        HistogramBasic(size_t a_capacity, TItem a_min, TItem a_max);

        /**
         * @brief Appends a new item to the histogram.
         * 
         * If the item falls outside the current range, the histogram will be rebuilt 
         * to accommodate the new min/max values.
         * @param a_item The value to be added.
         */
        void append(TItem a_item, size_t a_count = 1);

        /** @brief Returns the minimum value recorded in the histogram. */
        TItem min() const;

        /** @brief Returns the maximum value recorded in the histogram. */
        TItem max() const;

        /** @brief Returns the number of bins (size of the internal vector). */
        size_t size() const;

        /** @brief Returns the total number of items appended to the histogram. */
        size_t counter() const;

        /**
         * @brief Calculates the range (min, max) for a specific value within a given range and size.
         * @param a_value The value to find the range for.
         * @param a_min The minimum boundary.
         * @param a_max The maximum boundary.
         * @param a_size The number of bins.
         * @return A pair containing the lower and upper bounds of the bin.
         * @throws std::out_of_range if a_value is outside [a_min, a_max].
         */
        static std::pair<TItem, TItem> rangeByValue(TItem a_value, TItem a_min, TItem a_max, size_t a_size);

        /** @brief Calculates the range for a value using current histogram boundaries. */
        std::pair<TItem, TItem> rangeByValue(TItem a_value, TItem a_min, TItem a_max) const;

        /** @brief Calculates the range for a value using current min/max and specified size. */
        std::pair<TItem, TItem> rangeByValue(TItem a_value, size_t a_size) const;

        /** @brief Calculates the range for a value using current histogram boundaries and size. */
        std::pair<TItem, TItem> rangeByValue(TItem a_value) const;

        /**
         * @brief Calculates the range for a specific bin index.
         * @param a_index The index of the bin.
         * @param a_min The minimum boundary.
         * @param a_max The maximum boundary.
         * @param a_size The number of bins.
         * @return A pair containing the lower and upper bounds of the bin.
         * @throws std::out_of_range if a_index is invalid.
         */
        static std::pair<TItem, TItem> rangeByIndex(size_t a_index, TItem a_min, TItem a_max, size_t a_size);

        /** @brief Calculates the range for a bin index using current histogram boundaries. */
        std::pair<TItem, TItem> rangeByIndex(size_t a_index, TItem a_min, TItem a_max) const;

        /** @brief Calculates the range for a bin index using current min/max and specified size. */
        std::pair<TItem, TItem> rangeByIndex(size_t a_index, size_t a_size) const;

        /** @brief Calculates the range for a bin index using current histogram boundaries and size. */
        std::pair<TItem, TItem> rangeByIndex(size_t a_index) const;

        /**
         * @brief Calculates the median value of the distribution.
         * @param a_vector The vector of counts.
         * @param a_min The minimum value of the range.
         * @param a_max The maximum value of the range.
         * @return The estimated median value.
         */
        static TItem median(const std::vector<TCounter>& a_vector, TItem a_min, TItem a_max);

        /** @brief Calculates the median using current histogram data. */
        TItem median(const std::vector<TCounter>& a_vector) const;

        /** @brief Calculates the median for a custom range by rebuilding a temporary vector. */
        TItem median(TItem a_min, TItem a_max) const;

        /** @brief Calculates the median using current histogram data. */
        TItem median() const;

        /**
         * @brief Creates a new count vector by redistributing values from a source vector into a new range.
         * @param a_source The source vector of counts.
         * @param a_sourceMin Min value of the source range.
         * @param a_sourceMax Max value of the source range.
         * @param a_min Min value of the target range.
         * @param a_max Max value of the target range.
         * @param a_size Number of bins in the target vector.
         * @return A new vector of counts.
         */
        static std::vector<TCounter> countVector(const std::vector<TCounter>& a_source, TItem a_sourceMin, TItem a_sourceMax, TItem a_min, TItem a_max, size_t a_size);

        /** @brief Creates a new count vector using current histogram data and a custom range. */
        std::vector<TCounter> countVector(TItem a_min, TItem a_max, size_t a_size) const;

        /** @brief Creates a new count vector using current histogram data and a custom range. */
        std::vector<TCounter> countVector(TItem a_min, TItem a_max) const;

        /** @brief Creates a new count vector using current histogram data and a specified size. */
        std::vector<TCounter> countVector(size_t a_size) const;

        /** @brief Creates a new count vector using current histogram data and default size. */
        std::vector<TCounter> countVector() const;

        /**
         * @brief Generates a formatted ASCII table representing the histogram.
         * @param a_vector The vector of counts.
         * @param a_min The minimum value.
         * @param a_max The maximum value.
         * @return A string containing the ASCII table.
         */
        static std::string toTable(const std::vector<TCounter>& a_vector, TItem a_min, TItem a_max);

        /** @brief Generates a formatted ASCII table for a custom range. */
        std::string toTable(TItem a_min, TItem a_max, size_t a_size) const;

        /** @brief Generates a formatted ASCII table for a custom size. */
        std::string toTable(size_t a_size) const;

        /** @brief Generates a formatted ASCII table. */
        std::string toTable() const;


        /**
         * @brief Generates an ASCII bar chart.
         * @param a_vector The vector of counts.
         * @param a_min The minimum value.
         * @param a_max The maximum value.
         * @param a_width The width of the chart in characters.
         * @param a_height The height of the chart in characters.
         * @return A string containing the bar chart.
         */
        static std::string toBarChart(const std::vector<TCounter>& a_vector, TItem a_min, TItem a_max, size_t a_width, size_t a_height);

        /** @brief Generates an ASCII bar chart for a custom range. */
        std::string toBarChart(TItem a_min, TItem a_max, size_t a_width, size_t a_height) const;

        /** @brief Generates an ASCII bar chart for a custom size. */
        std::string toBarChart(size_t a_width, size_t a_height) const;

      private:

        struct Heights {
          double left;
          double center;
          double right;
        };

        static Heights _heights(const std::vector<TCounter>& a_source, size_t a_index, double a_lastHeight, bool a_enableLastHeight);

        static void _build(TItem a_sourceMin, TItem a_sourceMax, const std::vector<TCounter>& a_source, TItem a_newMin, TItem a_newMax, std::vector<TCounter>& a_destination);

        size_t _calcIndex(TItem a_weight, TItem a_scale, size_t a_size) const;

        static std::string _drawLine(int length);

        unsigned long long    _counter;
        bool                  _init;
        bool                  _initMinMax;
        TItem                 _min;
        TItem                 _max;
        std::vector<TCounter> _vector;
        std::vector<TCounter> _buffer;
    };

  }
}

#include "histogram.ipp"

#endif // HISTOGRAM_HPP
