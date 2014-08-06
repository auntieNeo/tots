#include "taskQueue.h"

namespace tots { namespace framework {
  TaskQueue::TaskQueue(size_t size) : tots::utility::MinPriorityQueue< TaskQueueKey, Task >(size) {
  }

  TaskQueue::~TaskQueue() {
  }
} }
