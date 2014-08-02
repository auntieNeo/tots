#include "taskQueue.h"

namespace tots {
  TaskQueue::TaskQueue(size_t size) : MinPriorityQueue< TaskQueueKey, Task >(size) {
  }

  TaskQueue::~TaskQueue() {
  }
}
