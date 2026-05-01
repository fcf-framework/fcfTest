/**
 * Example 1: FCF_TEST macro magic
 * Demonstration: Automatic output of variable values when a test fails.
 */
#define FCF_TEST_IMPLEMENTATION
#include <fcfTest/test.hpp>
#include <vector>
#include <string>

// Declare a test: Part -> Group -> Test Name
FCF_TEST_DECLARE("Core", "Containers", "VectorPushBackCheck") {
    std::vector<std::string> items;
    items.push_back("apple");
    items.push_back("banana");

    // Successful test. Nothing will be printed except the execution log.
    FCF_TEST(items.size() == 2, items.size());
}

FCF_TEST_DECLARE("Core", "Math", "SimpleAddition") {
    int a = 10;
    int b = 20;
    int sum = a + b;

    // Intentionally break the condition to demonstrate variable output
    // The macro will automatically substitute names and values of all passed
    // arguments into the error message.
    FCF_TEST(sum == 31, a, b, sum);
}



int main(int argc, char* argv[]) {
    bool error = false;
    // Parses arguments and immediately runs tests,
    // for this we specify the CRM_RUN mode.
    fcf::NTest::cmdRun(argc, argv, fcf::NTest::CRM_RUN, &error);
    return error ? 1 : 0;
}