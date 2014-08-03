#include "../priorityQueue.h"
#define BOOST_TEST_MODULE tots_utility_test
#include <boost/test/unit_test.hpp>

#include <cstdlib>

BOOST_AUTO_TEST_CASE( test_MinPriorityQueue ) {
  { // test MinPriorityQueue starting from size 1
    tots::MinPriorityQueue<int, int> queue(1);
    BOOST_CHECK(queue.size() == 0);
    for(size_t i = 0; i < 100; ++i)
      queue.insert(0xDEADBEEF, 0xDEADBEEF);
    BOOST_CHECK(queue.size() == 100);
    // TODO: iterate over entire queue to make sure it's in the correct order
  }

  { // test MinPriorityQueue starting from size 0
    tots::MinPriorityQueue<int, int> queue(0);
    BOOST_CHECK(queue.size() == 0);
    for(size_t i = 0; i < 100; ++i)
      queue.insert(0xDEADBEEF, 0xDEADBEEF);
    BOOST_CHECK(queue.size() == 100);
  }

  { // test MinPriorityQueue with (deterministic) pseudo-random values
    tots::MinPriorityQueue<int, int> queue(42);
    BOOST_CHECK(queue.size() == 0);
    for(size_t i = 0; i < 1000; ++i)
      queue.insert(rand(), rand());
    BOOST_CHECK(queue.size() == 1000);

    // remove entries at random indexes
    for(size_t i = 0; i < 500; i++)
      queue.remove(rand() % queue.size());
    BOOST_CHECK(queue.size() == 500);
  }
}
