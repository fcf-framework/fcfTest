#ifndef TIMEDURATION_HPP
#define TIMEDURATION_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include <type_traits>

namespace fcf {
  namespace NTest {

    /**
     * @brief A class representing a duration of time, stored in nanoseconds.
     * 
     * This class provides a wrapper around an unsigned long long to represent time 
     * durations and supports various arithmetic and comparison operations.
     */
    class TimeDuration {
      private:
        unsigned long long _duration;

      public:
        /**
         * @brief Constructs a TimeDuration object.
         * @param a_duration The duration in nanoseconds.
         */
        TimeDuration(unsigned long long a_duration = 0) : _duration(a_duration) {}

        /**
         * @brief Gets the raw duration value.
         * @return The duration in nanoseconds.
         */
        unsigned long long count() const {
          return _duration;
        }

        /**
         * @brief Converts the duration to a string representation.
         * @param a_friendly If true, returns a human-readable format with separators.
         * @return A string representation of the duration.
         */
        std::string str(bool a_friendly = false) const {
          std::stringstream ss;
          if (a_friendly) {
            ss << (_duration / 1000000000) << '.'
               << std::setfill('0') << std::setw(3) << ((_duration / 1000000) % 1000) << '`'
               << std::setw(3)                      << ((_duration / 1000) % 1000) << '`'
               << std::setw(3)                      << (_duration % 1000);
          } else {
            ss << (_duration / 1000000000) << '.'
               << std::setfill('0') << std::setw(9) << (_duration % 1000000000);
          }
          return ss.str();
        }

        /** @brief Explicit conversion to double. @return The duration as a double. */
        explicit operator double() const {
          return (double)_duration;
        }

        /** @brief Explicit conversion to long double. @return The duration as a long double. */
        explicit operator long double() const {
          return (long double)_duration;
        }

        /** @brief Explicit conversion to unsigned long long. @return The duration in nanoseconds. */
        explicit operator unsigned long long() const {
          return _duration;
        }

        /** @brief Addition operator for two TimeDuration objects. @param a_rhs The duration to add. @return Result of addition. */
        inline TimeDuration operator+(const TimeDuration& a_rhs) const {
          return TimeDuration(_duration + a_rhs._duration);
        }

        /** @brief Subtraction operator for two TimeDuration objects. @param a_rhs The duration to subtract. @return Result of subtraction. */
        inline TimeDuration operator-(const TimeDuration& a_rhs) const {
          return TimeDuration(_duration - a_rhs._duration);
        }

        /** @brief Multiplication operator for two TimeDuration objects. @param a_rhs The duration to multiply. @return Result of multiplication. */
        inline TimeDuration operator*(const TimeDuration& a_rhs) const {
          return TimeDuration(_duration * a_rhs._duration);
        }

        /** @brief Division operator for two TimeDuration objects. @param a_rhs The duration to divide by. @return Result of division. */
        inline TimeDuration operator/(const TimeDuration& a_rhs) const {
          return TimeDuration(_duration / a_rhs._duration);
        }

        /**
         * @brief Addition operator with an arithmetic type.
         * @tparam Ty Arithmetic type.
         * @param a_value The value to add.
         * @return Result of addition.
         */
        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, TimeDuration>::type
        operator+(const Ty& a_value) const {
          return TimeDuration(_duration + static_cast<unsigned long long>(a_value));
        }

        /**
         * @brief Subtraction operator with an arithmetic type.
         * @tparam Ty Arithmetic type.
         * @param a_value The value to subtract.
         * @return Result of subtraction.
         */
        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, TimeDuration>::type
        operator-(const Ty& a_value) const {
          return TimeDuration(_duration - static_cast<unsigned long long>(a_value));
        }

        /**
         * @brief Multiplication operator with an arithmetic type.
         * @tparam Ty Arithmetic type.
         * @param a_value The value to multiply.
         * @return Result of multiplication.
         */
        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, TimeDuration>::type
        operator*(const Ty& a_value) const {
          return TimeDuration(_duration * static_cast<unsigned long long>(a_value));
        }

        /**
         * @brief Division operator with an arithmetic type.
         * @tparam Ty Arithmetic type.
         * @param a_value The value to divide by.
         * @return Result of division.
         */
        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, TimeDuration>::type
        operator/(const Ty& a_value) const {
          return TimeDuration(_duration / static_cast<unsigned long long>(a_value));
        }

        /** @brief Equality comparison. @param a_rhs The duration to compare. @return True if equal. */
        inline bool operator==(const TimeDuration& a_rhs) const {
          return _duration == a_rhs._duration;
        }

        /** @brief Inequality comparison. @param a_rhs The duration to compare. @return True if not equal. */
        inline bool operator!=(const TimeDuration& a_rhs) const {
          return _duration != a_rhs._duration;
        }

        /** @brief Less than comparison. @param a_rhs The duration to compare. @return True if less. */
        inline bool operator<(const TimeDuration& a_rhs) const {
          return _duration < a_rhs._duration;
        }

        /** @brief Greater than comparison. @param a_rhs The duration to compare. @return True if greater. */
        inline bool operator>(const TimeDuration& a_rhs) const {
          return _duration > a_rhs._duration;
        }

        /** @brief Less than or equal comparison. @param a_rhs The duration to compare. @return True if less or equal. */
        inline bool operator<=(const TimeDuration& a_rhs) const {
          return _duration <= a_rhs._duration;
        }

        /** @brief Greater than or equal comparison. @param a_rhs The duration to compare. @return True if greater or equal. */
        inline bool operator>=(const TimeDuration& a_rhs) const {
          return _duration >= a_rhs._duration;
        }

        /**
         * @brief Equality comparison with an arithmetic type.
         * @tparam Ty Arithmetic type.
         * @param a_value The value to compare.
         * @return True if equal.
         */
        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
        operator==(const Ty& a_value) const {
          return _duration == static_cast<unsigned long long>(a_value);
        }

        /**
         * @brief Inequality comparison with an arithmetic type.
         * @tparam Ty Arithmetic type.
         * @param a_value The value to compare.
         * @return True if not equal.
         */
        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
        operator!=(const Ty& a_value) const {
          return _duration != static_cast<unsigned long long>(a_value);
        }

        /**
         * @brief Less than comparison with an arithmetic type.
         * @tparam Ty Arithmetic type.
         * @param a_value The value to compare.
         * @return True if less.
         */
        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
        operator<(const Ty& a_value) const {
          return _duration < static_cast<unsigned long long>(a_value);
        }

        /**
         * @brief Greater than comparison with an arithmetic type.
         * @tparam Ty Arithmetic type.
         * @param a_value The value to compare.
         * @return True if greater.
         */
        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
        operator>(const Ty& a_value) const {
          return _duration > static_cast<unsigned long long>(a_value);
        }

        /**
         * @brief Less than or equal comparison with an arithmetic type.
         * @tparam Ty Arithmetic type.
         * @param a_value The value to compare.
         * @return True if less or equal.
         */
        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
        operator<=(const Ty& a_value) const {
          return _duration <= static_cast<unsigned long long>(a_value);
        }

        /**
         * @brief Greater than or equal comparison with an arithmetic type.
         * @tparam Ty Arithmetic type.
         * @param a_value The value to compare.
         * @return True if greater or equal.
         */
        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
        operator>=(const Ty& a_value) const {
          return _duration >= static_cast<unsigned long long>(a_value);
        }

        /** @brief Logical NOT operator. @return True if duration is zero. */
        inline bool operator!() const {
          return _duration == 0;
        }

        /** @brief Assignment operator with unsigned long long. @param a_seconds The new duration. @return Reference to this object. */
        inline TimeDuration& operator=(unsigned long long a_seconds) {
          _duration = a_seconds;
          return *this;
        }

        /** @brief Assignment operator with TimeDuration. @param a_rhs The duration to assign. @return Reference to this object. */
        inline TimeDuration& operator=(const TimeDuration& a_rhs) {
          _duration = a_rhs._duration;
          return *this;
        }

        /** @brief Addition assignment operator. @param a_rhs The duration to add. @return Reference to this object. */
        inline TimeDuration& operator+=(const TimeDuration& a_rhs) {
          _duration += a_rhs._duration;
          return *this;
        }

        /** @brief Subtraction assignment operator. @param a_rhs The duration to subtract. @return Reference to this object. */
        inline TimeDuration& operator-=(const TimeDuration& a_rhs) {
          _duration -= a_rhs._duration;
          return *this;
        }

        /** @brief Multiplication assignment operator. @param a_rhs The duration to multiply. @return Reference to this object. */
        inline TimeDuration& operator*=(const TimeDuration& a_rhs) {
          _duration *= a_rhs._duration;
          return *this;
        }

        /** @brief Division assignment operator. @param a_rhs The duration to divide by. @return Reference to this object. */
        inline TimeDuration& operator/=(const TimeDuration& a_rhs) {
          _duration /= a_rhs._duration;
          return *this;
        }

        /**
         * @brief Addition assignment operator with an arithmetic type.
         * @tparam Ty Arithmetic type.
         * @param a_value The value to add.
         * @return Reference to this object.
         */
        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, TimeDuration&>::type
        operator+=(const Ty& a_value) {
          _duration += static_cast<unsigned long long>(a_value);
          return *this;
        }

        /**
         * @brief Subtraction assignment operator with an arithmetic type.
         * @tparam Ty Arithmetic type.
         * @param a_value The value to subtract.
         * @return Reference to this object.
         */
        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, TimeDuration&>::type
        operator-=(const Ty& a_value) {
          _duration -= static_cast<unsigned long long>(a_value);
          return *this;
        }

        /**
         * @brief Multiplication assignment operator with an arithmetic type.
         * @tparam Ty Arithmetic type.
         * @param a_value The value to multiply.
         * @return Reference to this object.
         */
        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, TimeDuration&>::type
        operator*=(const Ty& a_value) {
          _duration *= static_cast<unsigned long long>(a_value);
          return *this;
        }

        /**
         * @brief Division assignment operator with an arithmetic type.
         * @tparam Ty Arithmetic type.
         * @param a_value The value to divide by.
         * @return Reference to this object.
         */
        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, TimeDuration&>::type
        operator/=(const Ty& a_value) {
          _duration /= static_cast<unsigned long long>(a_value);
          return *this;
        }

        /** @brief Stream insertion operator. @param a_stream Output stream. @param a_td Duration object. @return The stream. */
        friend std::ostream& operator<< (std::ostream& a_stream, const TimeDuration& a_td) {
          a_stream << a_td.str(true);
          return a_stream;
        }
    };

    /**
     * @brief Addition operator for arithmetic type and TimeDuration.
     * @tparam Ty Arithmetic type.
     * @param a_left The arithmetic value.
     * @param a_right The duration.
     * @return Result of addition.
     */
    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, Ty>::type
    operator+(const Ty& a_left, const TimeDuration& a_right) {
      return a_left + (Ty)a_right;
    }

    /**
     * @brief Subtraction operator for arithmetic type and TimeDuration.
     * @tparam Ty Arithmetic type.
     * @param a_left The arithmetic value.
     * @param a_right The duration.
     * @return Result of subtraction.
     */
    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, Ty>::type
    operator-(const Ty& a_left, const TimeDuration& a_right) {
      return a_left - (Ty)a_right;
    }

    /**
     * @brief Multiplication operator for arithmetic type and TimeDuration.
     * @tparam Ty Arithmetic type.
     * @param a_left The arithmetic value.
     * @param a_right The duration.
     * @return Result of multiplication.
     */
    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, Ty>::type
    operator*(const Ty& a_left, const TimeDuration& a_right) {
      return a_left * (Ty)a_right;
    }

    /**
     * @brief Division operator for arithmetic type and TimeDuration.
     * @tparam Ty Arithmetic type.
     * @param a_left The arithmetic value.
     * @param a_right The duration.
     * @return Result of division.
     */
    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, Ty>::type
    operator/(const Ty& a_left, const TimeDuration& a_right) {
      return a_left / (Ty)a_right;
    }

    /**
     * @brief Equality comparison for arithmetic type and TimeDuration.
     * @tparam Ty Arithmetic type.
     * @param a_left The arithmetic value.
     * @param a_right The duration.
     * @return True if equal.
     */
    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
    operator==(const Ty& a_left, const TimeDuration& a_right) {
      return static_cast<unsigned long long>(a_left) == a_right.count();
    }

    /**
     * @brief Inequality comparison for arithmetic type and TimeDuration.
     * @tparam Ty Arithmetic type.
     * @param a_left The arithmetic value.
     * @param a_right The duration.
     * @return True if not equal.
     */
    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
    operator!=(const Ty& a_left, const TimeDuration& a_right) {
      return static_cast<unsigned long long>(a_left) != a_right.count();
    }

    /**
     * @brief Less than comparison for arithmetic type and TimeDuration.
     * @tparam Ty Arithmetic type.
     * @param a_left The arithmetic value.
     * @param a_right The duration.
     * @return True if less.
     */
    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
    operator<(const Ty& a_left, const TimeDuration& a_right) {
      return static_cast<unsigned long long>(a_left) < a_right.count();
    }

    /**
     * @brief Greater than comparison for arithmetic type and TimeDuration.
     * @tparam Ty Arithmetic type.
     * @param a_left The arithmetic value.
     * @param a_right The duration.
     * @return True if greater.
     */
    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
    operator>(const Ty& a_left, const TimeDuration& a_right) {
      return static_cast<unsigned long long>(a_left) > a_right.count();
    }

    /**
     * @brief Less than or equal comparison for arithmetic type and TimeDuration.
     * @tparam Ty Arithmetic type.
     * @param a_left The arithmetic value.
     * @param a_right The duration.
     * @return True if less or equal.
     */
    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
    operator<=(const Ty& a_left, const TimeDuration& a_right) {
      return static_cast<unsigned long long>(a_left) <= a_right.count();
    }

    /**
     * @brief Greater than or equal comparison for arithmetic type and TimeDuration.
     * @tparam Ty Arithmetic type.
     * @param a_left The arithmetic value.
     * @param a_right The duration.
     * @return True if greater or equal.
     */
    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
    operator>=(const Ty& a_left, const TimeDuration& a_right) {
      return static_cast<unsigned long long>(a_left) >= a_right.count();
    }

  }
}

#endif
