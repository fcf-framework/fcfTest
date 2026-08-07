#define Z__FCF_TEST_RECURCIVE_RUN_DISABLE
#define FCF_TEST_IMPLEMENTATION
#include <fcfTest/test.hpp>

FCF_TEST_GROUP_ORDER("macro", 1);
FCF_TEST_GROUP_ORDER("cmd",   1000001);
FCF_TEST_GROUP_ORDER("cmdRun", 1000002);

int main(int a_argc, char* a_argv[]) {
  bool error = false;
  fcf::NTest::Options options;
  options.ignoreSelectors.push_back(
    fcf::NTest::Options::Selector{
      {
        "subrun",
        "subrun-export",
        "subrun-order 1",
        "subrun-order 2",
        "subrun-order 3",
        "subrun-fixture",
        "subrun-fixture-part-001",
        "subrun-fixture-part-002",
        "subrun-fixture-part-003",
        "subrun-fixture-part_error-all-001",
        "subrun-fixture-part_error-all-002",
        "subrun-fixture-part_error-all-003",
        "subrun-fixture-part_error-nobreak-all-001",
        "subrun-fixture-part_error-nobreak-all-002",
        "subrun-fixture-part_error-nobreak-all-003",
        "subrun-fixture-part_error-nobreak-all-004",
        "subrun-fixture-group-multi-001",
        "subrun-fixture-group-multi-002",
        "subrun-fixture-group-multi-003"
      },
      {},
      {}
    }
  );
  fcf::NTest::cmdRun(options, a_argc, a_argv, fcf::NTest::CRM_RUN, &error);
  return error ? 1 : 0;
}

