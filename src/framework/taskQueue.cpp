#include "taskQueue.h"

namespace tots { namespace framework {
  TaskQueue::TaskQueue(size_t size) : MinPriorityQueue< TaskQueueKey, Task >(size) {
  }

  TaskQueue::~TaskQueue() {
  }
} }
