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
          oss << seconds << "." << std::setw(9) << std::setz << nanoperc;
          return oss.str();
        }

        std::ostream& operator<< (std::ostream& a_stream) const {
          a_stream << str();
          return a_stream;
        }

        TimeDuration operator+(const TimeDuration& a_rhs) const {
          return TimeDuration(_duration + a_rhs._duration);
        }

        TimeDuration operator-(const TimeDuration& a_rhs) const {
          return TimeDuration(_duration - a_rhs._duration);
        }

        TimeDuration operator*(const TimeDuration& a_rhs) const {
          return TimeDuration(_duration * a_rhs._duration);
        }

        TimeDuration operator/(const TimeDuration& a_rhs) const {
          return TimeDuration(_duration / a_rhs._duration);
        }

        bool operator==(const TimeDuration& a_rhs) const {
          return _duration == a_rhs._duration;
        }

        bool operator!=(const TimeDuration& a_rhs) const {
          return _duration != a_rhs._duration;
        }

        bool operator<(const TimeDuration& a_rhs) const {
          return _duration < a_rhs._duration;
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
          *this = a_rhs;
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
#endif
