#include <unity.h>

#include "BleSpamCycle.h"

void test_runs_one_payload_per_ui_iteration() {
  BleSpamCycle cycle;
  int calls = 0;
  int selected = -1;
  for (int expected = 0; expected < 6; ++expected) {
    cycle.runNext([&](int index) {
      ++calls;
      selected = index;
    });
    TEST_ASSERT_EQUAL_INT(expected + 1, calls);
    TEST_ASSERT_EQUAL_INT(expected, selected);
  }
  cycle.runNext([&](int index) { selected = index; });
  TEST_ASSERT_EQUAL_INT(0, selected);
}

void test_reentry_starts_new_cycle() {
  BleSpamCycle cycle;
  cycle.runNext([](int) {});
  cycle.runNext([](int) {});
  cycle.reset();
  int selected = -1;
  cycle.runNext([&](int index) { selected = index; });
  TEST_ASSERT_EQUAL_INT(0, selected);
}

void setUp() {}
void tearDown() {}

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_runs_one_payload_per_ui_iteration);
  RUN_TEST(test_reentry_starts_new_cycle);
  return UNITY_END();
}
