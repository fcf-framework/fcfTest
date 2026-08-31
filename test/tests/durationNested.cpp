#include <fcfTest/include/duration.hpp>
#include <fcfTest/test.hpp>
#include "helpers.hpp"

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

FCF_TEST_DEFINE("fcfTest", "duration", "duration nested"){
  {
    fcf::NTest::DurationBasic<TestSingleStepClock> duration(10);

    int calls=0;
    duration([&](){
      ++calls;
      duration.begin(1);
      duration.end(1);
      duration.begin(1);
      duration.end(1);
      duration.begin(2);
      duration.end(2);
    });

    FCF_TEST(calls == 10, calls);

    // loop 12 - 1
    //   nested: 6*10
    //  
    FCF_TEST(duration.duration(0) == 71, duration.duration(0));
    FCF_TEST(duration.duration(1) == 20, duration.duration(1));
    FCF_TEST(duration.duration(2) == 10, duration.duration(2));

  }
}
