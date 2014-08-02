#ifndef TASK_QUEUE_H_
#define TASK_QUEUE_H_

#include "../utility/priorityQueue.h"
#include "task.h"

namespace tots {
  union TaskQueueKey {
    struct {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
      uint32_t m_time;
      Subsystem::Priority m_priority;
#elif SDL_BYTEORDER == SDL_LIL_ENDIAN
      Subsystem::Priority m_priority;
      uint32_t m_time;
#endif
    } m_data;
    uint64_t m_key;
  };

  inline bool operator<(TaskQueueKey a, TaskQueueKey b) {
    return a.m_key < b.m_key;
  }

  inline bool operator<=(TaskQueueKey a, TaskQueueKey b) {
    return a.m_key <= b.m_key;
  }

  inline bool operator>(TaskQueueKey a, TaskQueueKey b) {
    return a.m_key > b.m_key;
  }

  inline bool operator>=(TaskQueueKey a, TaskQueueKey b) {
    return a.m_key >= b.m_key;
  }

  inline bool operator==(TaskQueueKey a, TaskQueueKey b) {
    return a.m_key == b.m_key;
  }

  class TaskQueue : public MinPriorityQueue< TaskQueueKey, Task > {
    public:
      TaskQueue(size_t size);
      ~TaskQueue();
  };
}

#endif
