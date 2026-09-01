#ifndef TIMEDURATION_HPP
#define TIMEDURATION_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include <type_traits>

namespace fcf {
  namespace NTest {

    class TimeDuration {
      private:
        unsigned long long _duration;

      public:
        TimeDuration(unsigned long long a_duration = 0) : _duration(a_duration) {}

        unsigned long long count() const {
          return _duration;
        }

        std::string str() const {
          unsigned long long seconds = _duration / 1000000000ULL;
          unsigned long long nanoperc = _duration % 1000000000ULL;

          std::ostringstream oss;
          oss << seconds << "." << std::setfill('0') << std::setw(9) << nanoperc;
          return oss.str();
        }

        explicit operator double() const {
          return (double)_duration;
        }

        explicit operator long double() const {
          return (long double)_duration;
        }

        explicit operator unsigned long long() const {
          return _duration;
        }

        inline TimeDuration operator+(const TimeDuration& a_rhs) const {
          return TimeDuration(_duration + a_rhs._duration);
        }

        inline TimeDuration operator-(const TimeDuration& a_rhs) const {
          return TimeDuration(_duration - a_rhs._duration);
        }

        inline TimeDuration operator*(const TimeDuration& a_rhs) const {
          return TimeDuration(_duration * a_rhs._duration);
        }

        inline TimeDuration operator/(const TimeDuration& a_rhs) const {
          return TimeDuration(_duration / a_rhs._duration);
        }

        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, TimeDuration>::type
        operator+(const Ty& a_value) const {
          return TimeDuration(_duration + static_cast<unsigned long long>(a_value));
        }

        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, TimeDuration>::type
        operator-(const Ty& a_value) const {
          return TimeDuration(_duration - static_cast<unsigned long long>(a_value));
        }

        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, TimeDuration>::type
        operator*(const Ty& a_value) const {
          return TimeDuration(_duration * static_cast<unsigned long long>(a_value));
        }

        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, TimeDuration>::type
        operator/(const Ty& a_value) const {
          return TimeDuration(_duration / static_cast<unsigned long long>(a_value));
        }

        inline bool operator==(const TimeDuration& a_rhs) const {
          return _duration == a_rhs._duration;
        }

        inline bool operator!=(const TimeDuration& a_rhs) const {
          return _duration != a_rhs._duration;
        }

        inline bool operator<(const TimeDuration& a_rhs) const {
          return _duration < a_rhs._duration;
        }

        inline bool operator>(const TimeDuration& a_rhs) const {
          return _duration > a_rhs._duration;
        }

        inline bool operator<=(const TimeDuration& a_rhs) const {
          return _duration <= a_rhs._duration;
        }

        inline bool operator>=(const TimeDuration& a_rhs) const {
          return _duration >= a_rhs._duration;
        }

        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
        operator==(const Ty& a_value) const {
          return _duration == static_cast<unsigned long long>(a_value);
        }

        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
        operator!=(const Ty& a_value) const {
          return _duration != static_cast<unsigned long long>(a_value);
        }

        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
        operator<(const Ty& a_value) const {
          return _duration < static_cast<unsigned long long>(a_value);
        }

        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
        operator>(const Ty& a_value) const {
          return _duration > static_cast<unsigned long long>(a_value);
        }

        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
        operator<=(const Ty& a_value) const {
          return _duration <= static_cast<unsigned long long>(a_value);
        }

        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
        operator>=(const Ty& a_value) const {
          return _duration >= static_cast<unsigned long long>(a_value);
        }

        inline bool operator!() const {
          return _duration == 0;
        }

        inline TimeDuration& operator=(unsigned long long a_seconds) {
          _duration = a_seconds;
          return *this;
        }

        inline TimeDuration& operator=(const TimeDuration& a_rhs) {
          _duration = a_rhs._duration;
          return *this;
        }

        inline TimeDuration& operator+=(const TimeDuration& a_rhs) {
          _duration += a_rhs._duration;
          return *this;
        }

        inline TimeDuration& operator-=(const TimeDuration& a_rhs) {
          _duration -= a_rhs._duration;
          return *this;
        }

        inline TimeDuration& operator*=(const TimeDuration& a_rhs) {
          _duration *= a_rhs._duration;
          return *this;
        }

        inline TimeDuration& operator/=(const TimeDuration& a_rhs) {
          _duration /= a_rhs._duration;
          return *this;
        }

        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, TimeDuration&>::type
        operator+=(const Ty& a_value) {
          _duration += static_cast<unsigned long long>(a_value);
          return *this;
        }

        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, TimeDuration&>::type
        operator-=(const Ty& a_value) {
          _duration -= static_cast<unsigned long long>(a_value);
          return *this;
        }

        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, TimeDuration&>::type
        operator*=(const Ty& a_value) {
          _duration *= static_cast<unsigned long long>(a_value);
          return *this;
        }

        template <typename Ty>
        inline typename std::enable_if<std::is_arithmetic<Ty>::value, TimeDuration&>::type
        operator/=(const Ty& a_value) {
          _duration /= static_cast<unsigned long long>(a_value);
          return *this;
        }

        friend std::ostream& operator<< (std::ostream& a_stream, const TimeDuration& a_td) {
          a_stream << a_td.str();
          return a_stream;
        }
    };

    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, Ty>::type
    operator+(const Ty& a_left, const TimeDuration& a_right) {
      return a_left + (Ty)a_right;
    }

    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, Ty>::type
    operator-(const Ty& a_left, const TimeDuration& a_right) {
      return a_left - (Ty)a_right;
    }

    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, Ty>::type
    operator*(const Ty& a_left, const TimeDuration& a_right) {
      return a_left * (Ty)a_right;
    }

    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, Ty>::type
    operator/(const Ty& a_left, const TimeDuration& a_right) {
      return a_left / (Ty)a_right;
    }

    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
    operator==(const Ty& a_left, const TimeDuration& a_right) {
      return static_cast<unsigned long long>(a_left) == a_right.count();
    }

    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
    operator!=(const Ty& a_left, const TimeDuration& a_right) {
      return static_cast<unsigned long long>(a_left) != a_right.count();
    }

    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
    operator<(const Ty& a_left, const TimeDuration& a_right) {
      return static_cast<unsigned long long>(a_left) < a_right.count();
    }

    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
    operator>(const Ty& a_left, const TimeDuration& a_right) {
      return static_cast<unsigned long long>(a_left) > a_right.count();
    }

    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
    operator<=(const Ty& a_left, const TimeDuration& a_right) {
      return static_cast<unsigned long long>(a_left) <= a_right.count();
    }

    template <typename Ty>
    inline typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type
    operator>=(const Ty& a_left, const TimeDuration& a_right) {
      return static_cast<unsigned long long>(a_left) >= a_right.count();
    }

  }
}

#endif
