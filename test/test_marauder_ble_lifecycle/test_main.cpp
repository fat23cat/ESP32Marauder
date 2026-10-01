#include <unity.h>

#include "marauder_ble_lifecycle.h"

struct MockDevice {
  static bool initialized;
  static int deinits;
  static bool getInitialized() { return initialized; }
  static void deinit() { ++deinits; initialized = false; }
};

bool MockDevice::initialized = false;
int MockDevice::deinits = 0;

struct MockAdvertising {
  int stops = 0;
  void stop() { ++stops; }
};

struct MockScan {
  int stops = 0;
  int clears = 0;
  void stop() { ++stops; }
  void clearResults() { ++clears; }
};

void test_shutdown_skips_stale_objects_after_deinit() {
  MockAdvertising advertising;
  MockScan scan;
  MockAdvertising* ad = &advertising;
  MockScan* scanner = &scan;
  int pauses = 0;

  TEST_ASSERT_FALSE(crubShutdownBle<MockDevice>(ad, scanner,
                                                [&]() { ++pauses; }));
  TEST_ASSERT_NULL(ad);
  TEST_ASSERT_NULL(scanner);
  TEST_ASSERT_EQUAL_INT(0, advertising.stops);
  TEST_ASSERT_EQUAL_INT(0, scan.stops);
  TEST_ASSERT_EQUAL_INT(0, MockDevice::deinits);
  TEST_ASSERT_EQUAL_INT(0, pauses);
}

void test_shutdown_stops_live_objects_once() {
  MockDevice::initialized = true;
  MockDevice::deinits = 0;
  MockAdvertising advertising;
  MockScan scan;
  MockAdvertising* ad = &advertising;
  MockScan* scanner = &scan;
  int pauses = 0;

  TEST_ASSERT_TRUE(crubShutdownBle<MockDevice>(ad, scanner,
                                               [&]() { ++pauses; }));
  TEST_ASSERT_NULL(ad);
  TEST_ASSERT_NULL(scanner);
  TEST_ASSERT_EQUAL_INT(1, advertising.stops);
  TEST_ASSERT_EQUAL_INT(1, scan.stops);
  TEST_ASSERT_EQUAL_INT(1, scan.clears);
  TEST_ASSERT_EQUAL_INT(1, MockDevice::deinits);
  TEST_ASSERT_EQUAL_INT(1, pauses);
}

void setUp() {}
void tearDown() {}

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_shutdown_skips_stale_objects_after_deinit);
  RUN_TEST(test_shutdown_stops_live_objects_once);
  return UNITY_END();
}
