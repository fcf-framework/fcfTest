#include <fcfTest/test.hpp>

FCF_TEST_DEFINE("fcfTest", "duration", "duration-outlier"){
  {
    fcf::NTest::Duration::Options options;
    options.iterations = 10000;
    options.histogramBins = 30;
    options.outlierThreshold = 0.01;
    fcf::NTest::Duration duration(options);

    duration([](){});

    //fcf::NTest::log() << duration.histogram().toBarChart() << std::endl;
    //fcf::NTest::log() << duration.histogram().toTable() << std::endl;
  }
  
}

