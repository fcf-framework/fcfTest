#include <fcfTest/include/duration.hpp>
#include <fcfTest/test.hpp>
#include "helpers.hpp"



struct TestClock{
    TestClock()
      : clock(0)
      , step(1) {
    }
    unsigned long long operator()() {
      clock += step;
      step += 1;
      return clock;
    }
    unsigned long long clock;
    unsigned long long step;
};
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

FCF_TEST_DEFINE("fcfTest", "duration", "duration single measurement (graph as tests)"){
  {
    fcf::NTest::DurationBasic<TestSingleStepClock> duration;

    duration.begin(0, 3);
    duration.end(0, -1);
    FCF_TEST(duration.duration() == 1, duration.duration());
    FCF_TEST(duration.duration(1) == 1, duration.duration(1));
    FCF_TEST(duration.duration(2) == 1, duration.duration(2));
    FCF_TEST(TestSingleStepClock::clock == 2, TestSingleStepClock::clock);

    duration.reset(2);
    FCF_TEST(duration.duration() == 1, duration.duration());
    FCF_TEST(duration.duration(1) == 1, duration.duration(1));
    FCF_TEST(duration.duration(2) == 0, duration.duration(2));

    duration.begin(0, -1);
    duration.end(0, -1);
    FCF_TEST(duration.duration() == 2, duration.duration());
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(2) == 1, duration.duration(2));

    duration.reset(1);
    FCF_TEST(duration.duration() == 2, duration.duration());
    FCF_TEST(duration.duration(1) == 0, duration.duration(1));
    FCF_TEST(duration.duration(2) == 0, duration.duration(2));

    duration.begin(0, 2);
    duration.end(0, 2);
    FCF_TEST(duration.duration() == 3, duration.duration());
    FCF_TEST(duration.duration(1) == 1, duration.duration(1));
    FCF_TEST(duration.duration(2) == 0, duration.duration(2));

    duration.begin(0, -1);
    duration.end(0, -1);
    FCF_TEST(duration.duration() == 4, duration.duration());
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(2) == 1, duration.duration(2));
  }
}


FCF_TEST_DEFINE("fcfTest", "duration", "duration single measurement (simple)"){
  {
    fcf::NTest::DurationBasic<TestClock> duration;

    FCF_TEST(duration.duration() == 0, duration.duration());
    FCF_TEST(duration.duration(1) == 0, duration.duration());
    duration.begin(0, 2);
    duration.end(0, -1);
    FCF_TEST(duration.duration() == 2, duration.duration());  // end(1+2) - start(1)
    FCF_TEST(duration.duration() == 2, duration.duration());
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(2) == 0, duration.duration(2));
    FCF_TEST(duration.duration(2) == 0, duration.duration(2));

    duration.begin(0, -1);
    auto d = duration.duration();
    FCF_TEST(d == 6, d);  // duration(2) + current(1+2+3+4) - begin(1+2+3)
    duration.end(0, -1);
    FCF_TEST(duration.duration() == 11, duration.duration());  // duration(2) + current(1+2+3+4+5) - begin(1+2+3)
    FCF_TEST(duration.duration(1) == 11, duration.duration(1));
    FCF_TEST(duration.duration(1) == 11, duration.duration(1));
  }

  {
    fcf::NTest::DurationBasic<TestClock> duration;

    FCF_TEST(duration.duration() == 0, duration.duration());
    FCF_TEST(duration.duration(1) == 0, duration.duration());
    duration.begin(0, -1);
    duration.end(0, -1);
    FCF_TEST(duration.duration() == 2, duration.duration());  // end(1+2) - start(1)
    FCF_TEST(duration.duration() == 2, duration.duration());
    FCF_TEST(duration.duration(1) == 0, duration.duration(1));
    FCF_TEST(duration.duration(1) == 0, duration.duration(1));
  }

  {
    fcf::NTest::DurationBasic<TestClock> duration;

    FCF_TEST(duration.duration() == 0, duration.duration());
    FCF_TEST(duration.duration(1) == 0, duration.duration());
    duration.begin(0, 2);
    duration.end(0, -1);
    FCF_TEST(duration.duration() == 2, duration.duration());  // end(1+2) - start(1)
    FCF_TEST(duration.duration() == 2, duration.duration());
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
  }

  {
    fcf::NTest::DurationBasic<TestClock> duration;

    FCF_TEST(duration.duration() == 0, duration.duration());
    FCF_TEST(duration.duration(1) == 0, duration.duration());
    duration.begin(0,2);
    duration.end(0, -1);
    FCF_TEST(duration.duration() == 2, duration.duration());  // end(1+2) - start(1)
    FCF_TEST(duration.duration() == 2, duration.duration());
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(2) == 0, duration.duration(2));
    FCF_TEST(duration.duration(2) == 0, duration.duration(2));
  }

  {
    fcf::NTest::DurationBasic<TestClock> duration;

    FCF_TEST(duration.duration() == 0, duration.duration());
    FCF_TEST(duration.duration(1) == 0, duration.duration());
    duration.begin(0, 4);
    duration.end(0, -1);
    FCF_TEST(duration.duration() == 2, duration.duration());  // end(1+2) - start(1)
    FCF_TEST(duration.duration() == 2, duration.duration());
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(2) == 2, duration.duration(2));
    FCF_TEST(duration.duration(2) == 2, duration.duration(2));
    FCF_TEST(duration.duration(3) == 2, duration.duration(3));
    FCF_TEST(duration.duration(3) == 2, duration.duration(3));
  }
  {
    fcf::NTest::DurationBasic<TestClock> duration;

    FCF_TEST(duration.duration() == 0, duration.duration());
    FCF_TEST(duration.duration(1) == 0, duration.duration());
    duration.begin(0, 4);
    duration.end(0, -1);
    FCF_TEST(duration.duration() == 2, duration.duration());  // end(1+2) - start(1)
    FCF_TEST(duration.duration() == 2, duration.duration());
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(1) == 2, duration.duration(1));
    FCF_TEST(duration.duration(2) == 2, duration.duration(2));
    FCF_TEST(duration.duration(2) == 2, duration.duration(2));
    FCF_TEST(duration.duration(3) == 2, duration.duration(3));
    FCF_TEST(duration.duration(3) == 2, duration.duration(3));
    FCF_TEST(duration.duration(4) == 0, duration.duration(4));
    FCF_TEST(duration.duration(4) == 0, duration.duration(4));
  }


}


namespace {
  uint64_t simulate_work(uint64_t iterations) {
      volatile uint64_t result = 0;
      for (volatile uint64_t i = 0; i < iterations; ++i) {
          result += (i * i) % 12345;
      }
      return result;
  }
}

FCF_TEST_DEFINE("fcfTest", "duration", "duration operator()"){
  {
    fcf::NTest::DurationBasic<fcf::NTest::SteadyClock> duration(10000, 10, 100);
    duration([](){
        simulate_work(10000);
        });
    fcf::NTest::log() << duration.histogram(0).toBarChart(30, 10)<<std::endl;
    fcf::NTest::log() << duration.histogram(0).toTable()<<std::endl;
  }
  {
    fcf::NTest::DurationBasic<TestClock> duration;
    int calls = 0;

    duration([&](){ ++calls; });

    FCF_TEST(calls == 1, calls);
    /*
     * begin 1
     *   iteration measurement 1 + 2
     * end measurement 1 + 2 + 3
     * duration  = 6 - 1
     */
    FCF_TEST(duration.duration() == 5, duration.duration());
    FCF_TEST(duration.average() == 5, duration.average());
    FCF_TEST(duration.min() == 2, duration.min());
    FCF_TEST(duration.max() == 2, duration.max());
    FCF_TEST(duration.histogram(0).counter() == 1, duration.histogram(0).counter());
    FCF_TEST(duration.histogram(0).min() == 2, duration.histogram(0).min());
    FCF_TEST(duration.histogram(0).max() == 2, duration.histogram(0).max());
  }

  {
    fcf::NTest::DurationBasic<TestClock> duration;
    fcf::NTest::DurationBasic<TestClock>::Options options(1, 1, 0);
    int calls = 0;

    duration(options, 0, 3, [&](){ ++calls; });

    FCF_TEST(calls == 1, calls);
    FCF_TEST(duration.duration(0) == 5, duration.duration(0));
    FCF_TEST(duration.duration(1) == 5, duration.duration(1));
    FCF_TEST(duration.duration(2) == 5, duration.duration(2));
    FCF_TEST(duration.duration(3) == 0, duration.duration(3));
    FCF_TEST(duration.min(0) == 2, duration.min(0));
    FCF_TEST(duration.max(0) == 2, duration.max(0));
    FCF_TEST(duration.min(1) == 2, duration.min(1));
    FCF_TEST(duration.max(1) == 2, duration.max(1));
    FCF_TEST(duration.histogram(0).counter() == 1, duration.histogram(0).counter());
    FCF_TEST(duration.histogram(1).counter() == 1, duration.histogram(1).counter());
    FCF_TEST(duration.histogram(2).counter() == 1, duration.histogram(2).counter());
    FCF_TEST(duration.histogram(3).counter() == 0, duration.histogram(3).counter());
  }

  {
    fcf::NTest::DurationBasic<TestClock> duration;
    fcf::NTest::DurationBasic<TestClock>::Options options(1, 1, 2);
    int calls = 0;

    duration(options, [&](){ ++calls; });

    FCF_TEST(calls == 3, calls);
    FCF_TEST(duration.duration() == 5, duration.duration());
    FCF_TEST(duration.min() == 2, duration.min());
    FCF_TEST(duration.max() == 2, duration.max());
    FCF_TEST(duration.histogram(0).counter() == 1, duration.histogram(0).counter());
  }

  {
    fcf::NTest::DurationBasic<TestClock> duration;
    fcf::NTest::DurationBasic<TestClock>::Options options(4, 2, 0);
    int calls = 0;

    duration(options, 0, 1, [&](){ ++calls; });

    // 0: 1
    // 1: 3(1+2)
    // 2:
    // 3: 6 (1+2+3) (max 3/2)
    // end 10
    FCF_TEST(calls == 4, calls);
    FCF_TEST(duration.duration() == 9, duration.duration());
    FCF_TEST(duration.average() == 9/4, duration.average());
    FCF_TEST(duration.min() == 1, duration.min());
    FCF_TEST(duration.max() == 1, duration.max());
    FCF_TEST(duration.histogram(0).counter() == 4, duration.histogram(0).counter());
    FCF_TEST(duration.histogram(0).min() == 1, duration.histogram(0).min());
    FCF_TEST(duration.histogram(0).max() == 1, duration.histogram(0).max());
  }
  {
    fcf::NTest::DurationBasic<TestClock> duration;
    int calls = 0;

    duration([&](){ ++calls; });
    duration([&](){ ++calls; });

    FCF_TEST(calls == 2, calls);
    // duration 1: 1+2+3 - 1 = 5
    // duration 2: 1+2+3+4+5+6 - (1+2+3+4) = 11
    FCF_TEST(duration.duration() == 11+5, duration.duration());
    FCF_TEST(duration.average() == 16/2, duration.average());
    // min: 1+2 - 1 = 2
    // max: 1+2+3+4+5 - (1+2+3+4) = 5
    FCF_TEST(duration.min() == 2, duration.min());
    FCF_TEST(duration.max() == 5, duration.max());
    FCF_TEST(duration.histogram(0).counter() == 2, duration.histogram(0).counter());
    FCF_TEST(duration.histogram(0).min() == 2, duration.histogram(0).min());
    FCF_TEST(duration.histogram(0).max() == 5, duration.histogram(0).max());
  }

  {
    fcf::NTest::DurationBasic<TestClock> duration;
    int calls = 0;

    duration(1, [&](){ ++calls; });

    FCF_TEST(calls == 1, calls);
    FCF_TEST(duration.duration(0) == 0, duration.duration(0));
    FCF_TEST(duration.duration(1) == 6-1, duration.duration(1));
    FCF_TEST(duration.duration(2) == 0, duration.duration(2));
    FCF_TEST(duration.min(1) == 2, duration.min(1));
    FCF_TEST(duration.max(1) == 2, duration.max(1));
    FCF_TEST(duration.histogram(0).counter() == 0, duration.histogram(0).counter());
    FCF_TEST(duration.histogram(1).counter() == 1, duration.histogram(1).counter());
  }

  {
    fcf::NTest::DurationBasic<TestClock> duration;
    fcf::NTest::DurationBasic<TestClock>::Options options(1, 1, 0);
    int calls = 0;

    duration(options, 1, [&](){ ++calls; });

    FCF_TEST(calls == 1, calls);
    FCF_TEST(duration.duration(0) == 0, duration.duration(0));
    FCF_TEST(duration.duration(1) == 6-1, duration.duration(1));
    FCF_TEST(duration.min(1) == 2, duration.min(1));
    FCF_TEST(duration.max(1) == 2, duration.max(1));
    FCF_TEST(duration.histogram(0).counter() == 0, duration.histogram(0).counter());
    FCF_TEST(duration.histogram(1).counter() == 1, duration.histogram(1).counter());
  }

  {
    fcf::NTest::DurationBasic<TestClock> duration;
    int calls = 0;

    duration(0, 2, [&](){ ++calls; });

    FCF_TEST(calls == 1, calls);
    FCF_TEST(duration.duration(0) == 5, duration.duration(0));
    FCF_TEST(duration.duration(1) == 5, duration.duration(1));
    FCF_TEST(duration.histogram(0).counter() == 1, duration.histogram(0).counter());
    FCF_TEST(duration.histogram(1).counter() == 1, duration.histogram(1).counter());
  }

  {
    fcf::NTest::DurationBasic<TestClock> duration;
    fcf::NTest::DurationBasic<TestClock>::Options options(1, 2, 0);
    int calls = 0;

    duration(options, [&](){ ++calls; });

    FCF_TEST(calls == 1, calls);
    FCF_TEST(duration.duration() == 3-1, duration.duration());
    FCF_TEST(duration.average() == 3-1, duration.average());
    FCF_TEST(duration.min() == 2, duration.min());
    FCF_TEST(duration.max() == 2, duration.max());
    FCF_TEST(duration.histogram(0).counter() == 1, duration.histogram(0).counter());
  }

  {
    fcf::NTest::DurationBasic<TestClock> duration;
    int calls = 0;

    duration([&](){ ++calls; });
    duration.reset();

    FCF_TEST(calls == 1, calls);
    FCF_TEST(duration.duration() == 0, duration.duration());
    FCF_TEST(duration.average() == 0, duration.average());
    FCF_TEST(duration.min() == 0, duration.min());
    FCF_TEST(duration.max() == 0, duration.max());
    FCF_TEST(duration.histogram(0).counter() == 0, duration.histogram(0).counter());
  }
}

