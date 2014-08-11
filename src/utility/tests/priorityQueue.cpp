#include <gtest/gtest.h>
//#include <gmock/gmock.h>

#include "../priorityQueue.h"

using namespace testing;

namespace tots { namespace utility { namespace tests {

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
        ASSERT_GT(queue.size(), 0);
        K lastKey = queue.nextKey();
        queue.popNext();
        while(queue.hasNext()) {
          ASSERT_GT(queue.size(), 0);
          K currentKey = queue.nextKey();
          queue.popNext();
          ASSERT_LE(lastKey, currentKey);
        }
      }
      ASSERT_EQ(0, queue.size());
      ASSERT_FALSE(queue.hasNext());
    }

  template<typename K, typename T>
    void populateMinPriorityQueue(MinPriorityQueue<K, T> &queue, size_t count, K(*generateKey)(size_t), T(*generateValue)(size_t)) {
      for(size_t i = 0; i < end; ++i) {
        queue.insert(generateKey(i), generateValue(i));
        ASSERT_TRUE(queue.hasNext());
      }
    }

  /**
   * Tests the dynamic growth of MinPriorityQueue from a small size as elements
   * are added.
   */
  TEST(MinPriorityQueue, DynamicGrowth) {
    // test MinPriorityQueue starting from size 1
    MinPriorityQueue<int, int> queue(1);
    ASSERT_EQ(0, queue.size());
    ASSERT_FALSE(queue.hasNext());
    // add 1000 key/value pairs to the queue
    populateMinPriorityQueue(queue, 1000,
        [](size_t i) -> int { return 0xDEADBEEF; },  // fill the queue
        [](size_t i) -> int { return 0xDEADBEEF; });  // with dead beef
    ASSERT_EQ(1000, queue.size());
    // iterate over entire queue to make sure it's in the correct order
    checkMinPriorityQueueDestructive(queue);
    ASSERT_EQ(0, queue.size());
    ASSERT_FALSE(queue.hasNext());
  }

  /**
   * Tests the dynamic growth of MinPriorityQueue from zero size (an edge case)
   * as elements are added.
   */
  /*
  TEST(MinPriorityQueue, DynamicGrowthFromZero) {
    // test MinPriorityQueue starting from size 0
    MinPriorityQueue<int, int> queue(0);
    BOOST_CHECK(queue.size() == 0);
    BOOST_CHECK(!queue.hasNext());
    for(size_t i = 0; i < 100; ++i) {
      queue.insert(0xDEADBEEF, 0xDEADBEEF);
      BOOST_CHECK(queue.hasNext());
    }
    BOOST_CHECK(queue.size() == 100);
    // iterate over entire queue to make sure it's in the correct order
    checkMinPriorityQueueDestructive(queue);
  }
  */



} } }
