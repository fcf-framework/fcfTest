#ifndef TIMEDURATION_HPP
#define TIMEDURATION_HPP

#
#include <string>
#include <sstream>
#include <iomanip>

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

        friend std::ostream& operator<< (std::ostream& a_stream, const TimeDuration& a_td) {
          a_stream << a_td.str();
          return a_stream;
        }

        TimeDuration operator+(const TimeDuration& a_rhs) const {
          return TimeDuration(_duration + a_rhs._duration);
        }

        template <typename Ty>
         TimeDuration operator+(const Ty& a_value) const {
          return TimeDuration(_duration + a_value);
        }

        TimeDuration operator-(const TimeDuration& a_rhs) const {
          return TimeDuration(_duration - a_rhs._duration);
        }

        TimeDuration operator*(const TimeDuration& a_rhs) const {
          return TimeDuration(_duration * a_rhs._duration);
        }

        template <typename Ty>
        TimeDuration operator*(const Ty& a_value) const {
          return TimeDuration(_duration * a_value);
        }


        TimeDuration operator/(const TimeDuration& a_rhs) const {
          return TimeDuration(_duration / a_rhs._duration);
        }

        template <typename Ty>
        TimeDuration operator/(const Ty& a_value) const {
          return TimeDuration(_duration / a_value);
        }

        bool operator==(const TimeDuration& a_rhs) const {
          return _duration == a_rhs._duration;
        }

        template <typename Ty>
        bool operator==(const Ty& a_value) const {
          return _duration == a_value;
        }

        bool operator!=(const TimeDuration& a_rhs) const {
          return _duration != a_rhs._duration;
        }

        bool operator<(const TimeDuration& a_rhs) const {
          return _duration < a_rhs._duration;
        }

        template <typename Ty>
        bool operator<(const Ty& a_value) const {
          return _duration < a_value;
        }

        bool operator>(const TimeDuration& a_rhs) const {
          return _duration > a_rhs._duration;
        }

        bool operator<= (const TimeDuration& a_rhs) const {
          return _duration <= a_rhs._duration;
        }

        bool operator>= (const TimeDuration& a_rhs) const {
          return _duration >= a_rhs._duration;
        }

        bool operator!() const {
          return _duration == 0;
        }

        TimeDuration& operator=(unsigned long long a_seconds) {
          _duration = a_seconds;
          return *this;
        }

        TimeDuration& operator=(const TimeDuration& a_rhs) {
          _duration = a_rhs._duration;
          return *this;
        }

        TimeDuration& operator+=(const TimeDuration& a_rhs) {
          _duration += a_rhs._duration;
          return *this;
        }

        TimeDuration& operator-= (const TimeDuration& a_rhs) {
          _duration -= a_rhs._duration;
          return *this;
        }

        TimeDuration& operator*= (const TimeDuration& a_rhs) {
          _duration *= a_rhs._duration;
          return *this;
        }

        TimeDuration& operator/= (const TimeDuration& a_rhs) {
          _duration /= a_rhs._duration;
          return *this;
        }
    };

  }
}

inline double operator+(double a_left, fcf::NTest::TimeDuration a_right){
  return a_left + (double)a_right;
}

inline double operator-(double a_left, fcf::NTest::TimeDuration a_right){
  return a_left - (double)a_right;
}

inline double operator*(double a_left, fcf::NTest::TimeDuration a_right){
  return a_left * (double)a_right;
}

inline double operator/(double a_left, fcf::NTest::TimeDuration a_right){
  return a_left / (double)a_right;
}

inline bool operator>(double a_left, fcf::NTest::TimeDuration a_right){
  return a_left > (double)a_right;
}

inline bool operator<(double a_left, fcf::NTest::TimeDuration a_right){
  return a_left < (double)a_right;
}



#endif
