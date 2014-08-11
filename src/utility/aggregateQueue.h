#ifndef AGGREGATE_QUEUE_H_
#define AGGREGATE_QUEUE_H_

namespace tots { namespace utility {
  class EventQueue;
  /*!
   * The AggregateQueue class combines many Queue objects efficiently. It is
   * essentially a deque container that
   *
   *
   * Since Queue objects are essentially reference-counted pointers to simple,
   * contiguous blocks of memory, these pointers can be aggregated easily and
   * efficiently. This facilitates efficient communication between threads
   * without having to copy large blocks of data every frame. The idea is that
   * once a thread has written to a Queue object, the Queue is passed by
   * reference and is read-only, such that no thread locking needs to occur.
   * Additionally, memory for queues is allocated from shared pools, which
   * all but eliminates memory allocation costs every frame.
   */
  template<typename T>
  class AggregateQueue {
    public:
      AggregateQueue();
      ~AggregateQueue();

      void push(const T &item);

//      void merge(const AggregateQueue<T> &queue);

      T pop();
//      Queue<T> popBatch();

      T peek();
//      Queue<T> peekBatch();

    private:
//      std::vector< Queue<T> > m_queues;
      T *m_items;
  };

  template<typename T, typename K>
  AggregateQueue<T, K>::AggregateQueue() {
  }

  template<typename T, typename K>
  AggregateQueue<T, K>::~AggregateQueue() {
  }

  template<typename T, typename K>
  void AggregateQueue<T, K>::append(const T &item) {
    m_queues[item.type()].append(item);
  }

  /*
  template<typename T, typename K>
  void AggregateQueue<T, K>::appendBatch(const K &key, const Queue<T> &queue) {
    if(m_queues.find(key) == m_queues.end()) {
      m_queues.emplace(queue.type(), queue);
    }
  }
  */

  template<typename T, typename K>
  T AggregateQueue<T, K>::pop(const K &key) {
  }

  /*
  template<typename T, typename K>
  T AggregateQueue<T, K>::popBatch(const K &key) {
  }
  */

  template<typename T, typename K>
  T AggregateQueue<T, K>::peek(const K &key) {
  }

  /*
  template<typename T, typename K>
  T AggregateQueue<T, K>::peekBatch(const K &key) {
  }
  */
} }

#endif
