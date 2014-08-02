#include "../priorityQueue.h"
#define BOOST_TEST_MODULE tots_utility_test
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test_MinPriorityQueue ) {
  tots::MinPriorityQueue<int, int> queue(42);
  BOOST_CHECK(queue.size() == 0);
}
