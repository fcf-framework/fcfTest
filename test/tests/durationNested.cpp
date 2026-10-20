//#include <fcfTest/include/duration.hpp>
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
    TestSingleStepClock::clock = 0;
    fcf::NTest::DurationBasic<TestSingleStepClock>::Options options;
    options.iterations = 10;
    options.correction = 0;
    fcf::NTest::DurationBasic<TestSingleStepClock> duration(options);

    int calls=0;
    duration([&](){
      ++calls;
      duration.begin(1); // 2
      duration.end(1);   // 1
      duration.begin(1);
      duration.end(1);
      duration.begin(2);
      duration.end(2);
    });

    FCF_TEST(calls == 10, calls);

    // loop 10
    //   nested: 9*10
    //
    FCF_TEST(duration.duration(0) == 100, duration.duration(0));
    FCF_TEST(duration.duration(1) == 20, duration.duration(1));
    FCF_TEST(duration.duration(2) == 10, duration.duration(2));

  }
}
