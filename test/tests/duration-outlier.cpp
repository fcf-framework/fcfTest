#include <fcfTest/test.hpp>
#include <thread>

FCF_TEST_DEFINE("fcfTest", "duration-measurement", "duration-outlier"){
  for(size_t i = 0; i < 10; ++i){
    fcf::NTest::Duration::Options options;
    options.iterations = 1000;

    options.outlier = 0.2;

    fcf::NTest::Duration bench(options);

    int count = 0;

    bench([&]() {
        count++;
        if (count % 10 == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } else {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    });

    if (!(bench.max() < 1000*1000)) {
      fcf::NTest::log() << "Benchmark finished!" << std::endl;
      fcf::NTest::log() << "Average duration: " << bench.average() << std::endl;
      fcf::NTest::log() << "Median duration: " << bench.median() << std::endl;
      fcf::NTest::log() << bench.histogram().toTable() << std::endl;
    }
    FCF_TEST(bench.max() < 1000*1000, bench.max());

  }

  {
    fcf::NTest::Duration::Options options;
    options.iterations = 10000;
    options.histogramBins = 30;
    options.outlier = 0.01;
    fcf::NTest::Duration duration(options);

    duration([](){});

    //fcf::NTest::log() << duration.histogram().toBarChart() << std::endl;
    //fcf::NTest::log() << duration.histogram().toTable() << std::endl;
  }

}

