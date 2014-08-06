#include "../priorityQueue.h"
#define BOOST_TEST_MODULE tots_utility_test
#include <boost/test/unit_test.hpp>

using namespace tots::utility;

#include <cstdlib>

template<typename K, typename T>
void checkMinPriorityQueue(const MinPriorityQueue<K, T> &queue);
template<typename K, typename T>
void checkMinPriorityQueueDestructive(MinPriorityQueue<K, T> &queue);

template<typename K, typename T>
void checkMinPriorityQueue(const MinPriorityQueue<K, T> &queue) {
  // easiest way to iterate over a priority queue is to just copy it
  MinPriorityQueue<K, T> queueCopy(queue);
  checkMinPriorityQueueDestructive(queueCopy);
}

template<typename K, typename T>
void checkMinPriorityQueueDestructive(MinPriorityQueue<K, T> &queue) {
  // make sure each key is in the correct order
  if(queue.hasNext()) {
    BOOST_CHECK(queue.size() > 0);
    K lastKey = queue.nextKey();
    queue.popNext();
    while(queue.hasNext()) {
      BOOST_CHECK(queue.size() > 0);
      K currentKey = queue.nextKey();
      queue.popNext();
      BOOST_CHECK(lastKey <= currentKey);
    }
  }
  BOOST_CHECK(queue.size() == 0);
  BOOST_CHECK(!queue.hasNext());
}

BOOST_AUTO_TEST_CASE( test_MinPriorityQueue ) {
  { // test MinPriorityQueue starting from size 1
    MinPriorityQueue<int, int> queue(1);
    BOOST_CHECK(queue.size() == 0);
    BOOST_CHECK(!queue.hasNext());
    for(size_t i = 0; i < 100; ++i) {
      queue.insert(0xDEADBEEF, 0xDEADBEEF);
      BOOST_CHECK(queue.hasNext());
    }
    BOOST_CHECK(queue.size() == 100);
    // iterate over entire queue to make sure it's in the correct order
    checkMinPriorityQueue(queue);
  }

  { // test MinPriorityQueue starting from size 0
    MinPriorityQueue<int, int> queue(0);
    BOOST_CHECK(queue.size() == 0);
    BOOST_CHECK(!queue.hasNext());
    for(size_t i = 0; i < 100; ++i) {
      queue.insert(0xDEADBEEF, 0xDEADBEEF);
      BOOST_CHECK(queue.hasNext());
    }
    BOOST_CHECK(queue.size() == 100);
    // iterate over entire queue to make sure it's in the correct order
    checkMinPriorityQueue(queue);
  }

  { // test MinPriorityQueue with (deterministic) pseudo-random values
    MinPriorityQueue<int, int> queue(42);
    BOOST_CHECK(queue.size() == 0);
    BOOST_CHECK(!queue.hasNext());
    for(size_t i = 0; i < 1000; ++i) {
      queue.insert(rand(), rand());
      BOOST_CHECK(queue.hasNext());
    }
    BOOST_CHECK(queue.size() == 1000);
    // iterate over entire queue to make sure it's in the correct order
    checkMinPriorityQueue(queue);

    // remove entries at random indexes
    for(size_t i = 0; i < 500; i++) {
      queue.remove(rand() % queue.size());
      BOOST_CHECK(queue.hasNext());
    }
    BOOST_CHECK(queue.size() == 500);
    // iterate over entire queue to make sure it's in the correct order
    checkMinPriorityQueue(queue);

    // remove the rest of the entries by popping them
    while(queue.hasNext())
      queue.popNext();
    BOOST_CHECK(queue.size() == 0);
    // iterate over entire queue to make sure it's in the correct order
    checkMinPriorityQueue(queue);
  }
}
