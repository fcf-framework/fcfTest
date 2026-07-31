#include <fcfTest/test.hpp>
#include "helpers.hpp"

FCF_TEST_FIXTURE_DECLARE_START("fcfTest-fixture", "*", "*", fcf::NTest::FL_GLOBAL) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("global-fixture-state");
  if (p) {
    ++*p.cast<int>();
  } else {
    fcf::NTest::state().data("global-fixture-state", fcf::NTest::SharedPtrAny::make<int>(0) );
  }
}

FCF_TEST_FIXTURE_DECLARE_END("fcfTest-fixture", "*", "*", fcf::NTest::FL_GLOBAL) {
}


FCF_TEST_DECLARE("fcfTest-fixture", "fixture", "fixture simple") {
  int* statePtr = fcf::NTest::state().data("global-fixture-state").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 0, *statePtr);
}



FCF_TEST_FIXTURE_DECLARE_START("fcfTest-fixture-002", "*", "*", fcf::NTest::FL_PART) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("part-fixture-state");
  if (p) {
    ++*p.cast<int>();
  } else {
    fcf::NTest::state().data("part-fixture-state", fcf::NTest::SharedPtrAny::make<int>(1) );
  }
}

FCF_TEST_FIXTURE_DECLARE_END("fcfTest-fixture-002", "*", "*", fcf::NTest::FL_PART) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("part-fixture-state");
  *p.cast<int>() += 1000;
}


FCF_TEST_DECLARE("fcfTest-fixture-001", "fixture", "fixture") {
  int* statePtr = fcf::NTest::state().data("part-fixture-state").cast<int>();
  FCF_TEST(!statePtr);
}

FCF_TEST_DECLARE("fcfTest-fixture-002", "fixture", "fixture") {
  int* statePtr = fcf::NTest::state().data("part-fixture-state").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1, *statePtr);
}

FCF_TEST_DECLARE("fcfTest-fixture-003", "fixture", "fixture") {
  int* statePtr = fcf::NTest::state().data("part-fixture-state").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1001, *statePtr);
}




FCF_TEST_FIXTURE_DECLARE_START("fcfTest-fixture", "fixture-group-002", "*", fcf::NTest::FL_GROUP) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("group-fixture-state");
  if (p) {
    ++*p.cast<int>();
  } else {
    fcf::NTest::state().data("group-fixture-state", fcf::NTest::SharedPtrAny::make<int>(1) );
  }
}

FCF_TEST_FIXTURE_DECLARE_END("fcfTest-fixture", "fixture-group-002", "*", fcf::NTest::FL_GROUP) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("group-fixture-state");
  *p.cast<int>() += 1000;
}

FCF_TEST_DECLARE("fcfTest-fixture", "fixture-group-001", "fixture") {
  int* statePtr = fcf::NTest::state().data("group-fixture-state").cast<int>();
  FCF_TEST(!statePtr);
}

FCF_TEST_DECLARE("fcfTest-fixture", "fixture-group-002", "fixture") {
  int* statePtr = fcf::NTest::state().data("group-fixture-state").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1, *statePtr);
}

FCF_TEST_DECLARE("fcfTest-fixture", "fixture-group-003", "fixture") {
  int* statePtr = fcf::NTest::state().data("group-fixture-state").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1001, *statePtr);
}




FCF_TEST_FIXTURE_DECLARE_START("fcfTest-fixture", "fixture", "fixture-test-002", fcf::NTest::FL_TEST) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("test-fixture-state");
  if (p) {
    ++*p.cast<int>();
  } else {
    fcf::NTest::state().data("test-fixture-state", fcf::NTest::SharedPtrAny::make<int>(1) );
  }
}

FCF_TEST_FIXTURE_DECLARE_END("fcfTest-fixture", "fixture", "fixture-test-002", fcf::NTest::FL_TEST) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("test-fixture-state");
  *p.cast<int>() += 1000;
}


FCF_TEST_DECLARE("fcfTest-fixture", "fixture", "fixture-test-001") {
  int* statePtr = fcf::NTest::state().data("test-fixture-state").cast<int>();
  FCF_TEST(!statePtr);
}

FCF_TEST_DECLARE("fcfTest-fixture", "fixture", "fixture-test-002") {
  int* statePtr = fcf::NTest::state().data("test-fixture-state").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1);
}

FCF_TEST_DECLARE("fcfTest-fixture", "fixture", "fixture-test-003") {
  int* statePtr = fcf::NTest::state().data("test-fixture-state").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1001, *statePtr);
}



FCF_TEST_FIXTURE_DECLARE_START("fcfTest-fixture", "fixture-run-on-group", "fixture-test-003|fixture-test-001", fcf::NTest::FL_GROUP) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("run-on-group-fixture-state");
  if (p) {
    ++*p.cast<int>();
  } else {
    fcf::NTest::state().data("run-on-group-fixture-state", fcf::NTest::SharedPtrAny::make<int>(1) );
  }
}

FCF_TEST_FIXTURE_DECLARE_END("fcfTest-fixture", "fixture-run-on-group", "fixture-test-003|fixture-test-001", fcf::NTest::FL_GROUP) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("run-on-group-fixture-state");
  *p.cast<int>() = 1000;
}


FCF_TEST_DECLARE("fcfTest-fixture", "fixture-run-on-group", "fixture-test-000") {
  int* statePtr = fcf::NTest::state().data("run-on-group-fixture-state").cast<int>();
  FCF_TEST(!statePtr);
}

FCF_TEST_DECLARE("fcfTest-fixture", "fixture-run-on-group", "fixture-test-001") {
  int* statePtr = fcf::NTest::state().data("run-on-group-fixture-state").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1);
}

FCF_TEST_DECLARE("fcfTest-fixture", "fixture-run-on-group", "fixture-test-002") {
  int* statePtr = fcf::NTest::state().data("run-on-group-fixture-state").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1);
}

FCF_TEST_DECLARE("fcfTest-fixture", "fixture-run-on-group", "fixture-test-003") {
  int* statePtr = fcf::NTest::state().data("run-on-group-fixture-state").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1);
}

FCF_TEST_DECLARE("fcfTest-fixture", "fixture-run-on-group-002", "fixture-test") {
  int* statePtr = fcf::NTest::state().data("run-on-group-fixture-state").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1000);
}




FCF_TEST_FIXTURE_DECLARE_START("fcfTest-fixture", "fixture-run-on-multi-test-002", "*", fcf::NTest::FL_TEST) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("run-on-multi-test-002");
  if (p) {
    ++*p.cast<int>();
  } else {
    fcf::NTest::state().data("run-on-multi-test-002", fcf::NTest::SharedPtrAny::make<int>(0) );
  }
}

FCF_TEST_FIXTURE_DECLARE_END("fcfTest-fixture", "fixture-run-on-multi-test-002", "*", fcf::NTest::FL_TEST) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("run-on-multi-test-002");
  *p.cast<int>() += 1000;
}

FCF_TEST_DECLARE("fcfTest-fixture", "fixture-run-on-multi-test-001", "fixture-test") {
  int* statePtr = fcf::NTest::state().data("run-on-multi-test-002").cast<int>();
  FCF_TEST(!statePtr);
}

FCF_TEST_DECLARE("fcfTest-fixture", "fixture-run-on-multi-test-002", "fixture-test-001") {
  int* statePtr = fcf::NTest::state().data("run-on-multi-test-002").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 0);
}

FCF_TEST_DECLARE("fcfTest-fixture", "fixture-run-on-multi-test-002", "fixture-test-002") {
  int* statePtr = fcf::NTest::state().data("run-on-multi-test-002").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1001, *statePtr);
}

FCF_TEST_DECLARE("fcfTest-fixture", "fixture-run-on-multi-test-002", "fixture-test-003") {
  int* statePtr = fcf::NTest::state().data("run-on-multi-test-002").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 2002, *statePtr);
}

FCF_TEST_DECLARE("fcfTest-fixture", "fixture-run-on-multi-test-003", "fixture-test-003") {
  int* statePtr = fcf::NTest::state().data("run-on-multi-test-002").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 3002, *statePtr);
}



FCF_TEST_FIXTURE_DECLARE_START("fcfTest-fixture-multi-group-002", "*", "*", fcf::NTest::FL_GROUP) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("run-on-multi-group-002");
  if (p) {
    ++*p.cast<int>();
  } else {
    fcf::NTest::state().data("run-on-multi-group-002", fcf::NTest::SharedPtrAny::make<int>(0) );
  }
}

FCF_TEST_FIXTURE_DECLARE_END("fcfTest-fixture-multi-group-002", "*", "*", fcf::NTest::FL_GROUP) {
  fcf::NTest::SharedPtrAny p = fcf::NTest::state().data("run-on-multi-group-002");
  *p.cast<int>() += 1000;
}

FCF_TEST_DECLARE("fcfTest-fixture-multi-group-001", "fixture-run-on-multi-test-001", "fixture-test") {
  int* statePtr = fcf::NTest::state().data("run-on-multi-group-002").cast<int>();
  FCF_TEST(!statePtr);
}

FCF_TEST_DECLARE("fcfTest-fixture-multi-group-002", "fixture-run-on-multi-group-002", "fixture-test-001") {
  int* statePtr = fcf::NTest::state().data("run-on-multi-group-002").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 0);
}

FCF_TEST_DECLARE("fcfTest-fixture-multi-group-002", "fixture-run-on-multi-group-003", "fixture-test-002") {
  int* statePtr = fcf::NTest::state().data("run-on-multi-group-002").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 1001, *statePtr);
}

FCF_TEST_DECLARE("fcfTest-fixture-multi-group-002", "fixture-run-on-multi-group-004", "fixture-test-003") {
  int* statePtr = fcf::NTest::state().data("run-on-multi-group-002").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 2002, *statePtr);
}

FCF_TEST_DECLARE("fcfTest-fixture-multi-group-003", "fixture-run-on-multi-group-003", "fixture-test-003") {
  int* statePtr = fcf::NTest::state().data("run-on-multi-group-002").cast<int>();
  FCF_TEST(statePtr);
  FCF_TEST(*statePtr == 3002, *statePtr);
}

