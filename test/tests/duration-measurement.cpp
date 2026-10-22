#include <fcfTest/test.hpp>

FCF_TEST_DEFINE("fcfTest", "duration-measurement", "duration-measurement-release"){
  volatile int dummy = 0;
  {
    size_t size = 1000;

    fcf::NTest::TimeDuration td;
    fcf::NTest::TimeDuration tdCorrection;
    for(size_t i = 0; i < size; ++i){
      fcf::NTest::Duration::Options options;
      options.correction = 0;
      fcf::NTest::Duration duration(options);
      duration.begin();
      duration.end();
      td += duration.duration();
    }

    for(size_t i = 0; i < size; ++i){
      fcf::NTest::Duration duration;
      duration.begin();
      duration.end();
      tdCorrection += duration.duration();
    }

    fcf::NTest::log() << "beg/end emp (without corretion):      " << td << std::endl;
    fcf::NTest::log() << "beg/end emp (with corretion):         " << tdCorrection << std::endl;

    FCF_TEST(tdCorrection/size < 50, tdCorrection, tdCorrection/size);
  }

  {
    size_t size = 10000;

    fcf::NTest::TimeDuration td;
    fcf::NTest::TimeDuration tdCorrection;
    fcf::NTest::TimeDuration tdClear;
    for(size_t i = 0; i < size; ++i){
      fcf::NTest::Duration::Options options;
      options.correction = 0;
      fcf::NTest::Duration duration(options);
      duration.begin();
      for(volatile size_t j = 0; j < 15; ++j){ ++dummy; }
      duration.end();
      td += duration.duration();
    }
    for(size_t i = 0; i < size; ++i){
      fcf::NTest::Duration::Options options;
      options.correction = 1;
      fcf::NTest::Duration duration(options);
      duration.begin();
      for(volatile size_t j = 0; j < 15; ++j){ ++dummy; }
      duration.end();
      tdCorrection += duration.duration();
    }

    for(size_t i = 0; i < size; ++i){
      unsigned long long beg = fcf::NTest::SteadyClock()();
      for(volatile size_t j = 0; j < 15; ++j){ 
        ++dummy; 
      }
      unsigned long long end = fcf::NTest::SteadyClock()();
      unsigned long long dur = end - beg;
      dur = dur > fcf::NTest::NDetails::clockDelay().clock ? dur - fcf::NTest::NDetails::clockDelay().clock : 0;
      tdClear += dur;
    }

    fcf::NTest::log() << "beg/end (without corretion):          " << td << std::endl;
    fcf::NTest::log() << "beg/end (with corretion):             " << tdCorrection << std::endl;
    fcf::NTest::log() << "beg/end (clear):                      " << tdClear << std::endl;


    FCF_TEST(tdClear * 0.75 < tdCorrection && tdClear * 1.25 > tdCorrection, tdClear * 0.75, tdClear * 1.25, tdClear, tdCorrection);
  }

  {
    //(unsigned long long a_iterations, unsigned long long a_measurementStep, unsigned long long a_warmup, bool a_ignoreCorrection)
    size_t size = 10;
    size_t iterations = 10000;
    size_t innerLoopIterations = 150;
    fcf::NTest::TimeDuration td;
    fcf::NTest::TimeDuration tdCorrection;
    fcf::NTest::TimeDuration tdAvgCorrection;
    fcf::NTest::TimeDuration tdClear;

    volatile size_t j = 0;
    volatile size_t k = 0;
    for(size_t i = 0; i < size; ++i){
      fcf::NTest::Duration::Options options;
      options.correction = 0;
      options.iterations = iterations;
      fcf::NTest::Duration duration(options);
      duration([&](){
        for(j = 0; j < innerLoopIterations; ++j){ ++dummy; }
      });
      td += duration.duration();
    }
    for(size_t i = 0; i < size; ++i){
      fcf::NTest::Duration::Options options;
      options.iterations = iterations;
      fcf::NTest::Duration duration(options);
      duration([&](){
        for(j = 0; j < innerLoopIterations; ++j){ ++dummy; }
      });
      tdCorrection += duration.duration();
    }
    for(volatile size_t i = 0; i < size; ++i){
      fcf::NTest::Duration::Options options;
      options.correction = 0;
      options.measurementStep = 1000;
      options.iterations = iterations;
      fcf::NTest::Duration duration(options);
      duration([&](){
        for(j = 0; j < innerLoopIterations; ++j){
          ++dummy;
        }
      });
      tdAvgCorrection += duration.duration();
    }

    for(volatile size_t i = 0; i < size; ++i){
      fcf::NTest::Duration::Options options;
      options.correction = 0;
      fcf::NTest::Duration duration(options);
      duration.begin();
      for(k = 0; k < iterations; ++k){
        for(j = 0; j < innerLoopIterations; ++j){
          ++dummy;
        }
      }
      duration.end();

      tdClear += duration.duration();
    }


    fcf::NTest::log() << "loop (without corretion):             " << td << "; AVG: " << td/iterations << std::endl;
    fcf::NTest::log() << "loop (with corretion):                " << tdCorrection << "; AVG: " << tdCorrection/iterations << std::endl;
    fcf::NTest::log() << "loop (avg):                           " << tdAvgCorrection << "; AVG: " << tdAvgCorrection/iterations << std::endl;
    fcf::NTest::log() << "loop (clear):                         " << tdClear << "; AVG: " << tdClear/iterations << std::endl;

    FCF_TEST(tdClear * 0.75 < tdCorrection && tdClear * 1.25 > tdCorrection, tdClear * 0.75, tdClear * 1.25, tdClear, tdCorrection);
  }


  {
    fcf::NTest::Duration::Options options;
    options.correction = 0;
    fcf::NTest::Duration duration(options);
    duration.begin();
      duration.begin(1);
      duration.end(1);
    duration.end();
    fcf::NTest::log() << "Nested (without correction) [0]:      " << duration.duration(0) << "; [1]: " << duration.duration(1) << std::endl;
  }
  {
    fcf::NTest::Duration duration;
    duration.begin();
      duration.begin(1);
      duration.end(1);
    duration.end();
    fcf::NTest::log() << "Nested (with correction) [0]:         " << duration.duration(0) << "; [1]: " << duration.duration(1) << std::endl;
  }

  {
    fcf::NTest::Duration::Options options;
    options.correction = 1;
    options.iterations = 1000;
    fcf::NTest::Duration duration(options);
    duration.begin();
      duration(1, [&](){
          ++dummy;
      });
    duration.end();
    fcf::NTest::log() << "Nested loop (without correction) [0]: " << duration.duration(0) << "; [1]: " << duration.duration(1) << std::endl;
  }
  {
    fcf::NTest::Duration::Options options;
    options.correction = 0;
    options.iterations = 1000;
    fcf::NTest::Duration duration(options);
    duration.begin();
      duration(1, [&](){
          ++dummy;
      });
    duration.end();
    fcf::NTest::log() << "Nested loop (with correction) [0]:    " << duration.duration(0) << "; [1]: " << duration.duration(1) << std::endl;
  }
}
