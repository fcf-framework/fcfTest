/**
 * Example 2: Logging and Benchmarking
 * Demonstration: Using the built-in logger and the Duration class to measure time.
 */
#define FCF_TEST_IMPLEMENTATION
#include <fcfTest/test.hpp>
#include <vector>
#include <algorithm>

// Test demonstrating logger functionality
FCF_TEST_DECLARE("Logging", "Levels", "LogLevelsDemo") {
    // Demonstration of all logging levels.
    // By default, the logging level is LL_LOG.
    // Messages with a level lower than the current one (e.g., dbg, trc) 
    // will not be displayed unless the log level is changed via command line arguments.

    fcf::NTest::ftl() << "  Fatal message: Critical failure!" << std::endl;
    fcf::NTest::err() << "  Error message: Something went wrong." << std::endl;
    fcf::NTest::wrn() << "  Warning message: Be careful." << std::endl;
    fcf::NTest::att() << "  Attention message: Check this." << std::endl;
    fcf::NTest::log() << "  Log message: Standard operation." << std::endl;
    fcf::NTest::inf() << "  Info message: Informational data." << std::endl;
    fcf::NTest::dbg() << "  Debug message: Debugging info." << std::endl;
    fcf::NTest::trc() << "  Trace message: Detailed trace." << std::endl;

    // Successful execution check
    FCF_TEST(true);
}

// Test demonstrating benchmarking
FCF_TEST_DECLARE("Benchmark", "Sorting", "VectorSortBenchmark") {
    // Create a Duration object for 1000 iterations
    fcf::NTest::Duration bench(1000);

    // Fill data for sorting
    std::vector<int> data = {5, 2, 9, 1, 5, 6, 3, 8, 4, 7};

    // Result of sorting will be stored here
    std::vector<int> sdata;

    // Use the call operator to run the benchmark
    bench([&data, &sdata]() {
        // Create a copy so that sorting does not modify the original data for the next test
        sdata = data;
        std::sort(sdata.begin(), sdata.end());
    });

    // Output benchmark results
    fcf::NTest::inf() << "  Benchmark completed." << std::endl;
    fcf::NTest::inf() << "    Iterations: " << bench.iterations() << std::endl;
    fcf::NTest::inf() << "    Total time: " << bench.totalDuration().count() << " ns" << std::endl;
    fcf::NTest::inf() << "    Avg time:   " << bench.duration().count() << " ns" << std::endl;

    // We perform a check of the sorting
    FCF_TEST(std::is_sorted(sdata.begin(), sdata.end()));
}

int main(int a_argc, char* a_argv[]) {
    bool error = false;
    // Run tests.
    // To see debug and trace messages, run with the flag:
    // ./logger-and-benchmark-example --test-run --test-log-level dbg
    fcf::NTest::cmdRun(a_argc, a_argv, fcf::NTest::CRM_RUN, &error);
    return error ? 1 : 0;
}
