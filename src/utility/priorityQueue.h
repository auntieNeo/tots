#ifndef PRIORITY_QUEUE_H_
#define PRIORITY_QUEUE_H_

#include <cstdlib>
#include <cstring>

#include "../common.h"

namespace tots {
  template<typename K, typename T>
  class MinPriorityQueue {
    public:
      MinPriorityQueue(size_t size);
      virtual ~MinPriorityQueue();

      void insert(const K &key, const T &value);
      T next() const;
      K nextKey() const;
      T peekNext() const { return next(); }
      K peekNextKey() const { return nextKey(); }
      T popNext();
      bool hasNext() const { return m_heapSize > 0; }

      size_t size() const { return m_heapSize; };
      T at(size_t index) const;
      K keyAt(size_t index) const;

      void remove(size_t index);

    private:
      struct KeyValuePair {
        K m_key;
        T m_value;
      };
      KeyValuePair *m_heap;
      size_t m_heapSize, m_heapAllocated;

      void m_sinkMinHeapify(size_t i);
      void m_floatMinHeapify(size_t i);
      void m_growHeap();
  };

  template<typename K, typename T>
  MinPriorityQueue<K, T>::MinPriorityQueue(size_t size) {
    m_heapSize = 0;
    m_heap = static_cast<KeyValuePair *>(malloc(size * sizeof(KeyValuePair)));
    m_heapAllocated = size;
  }

  template<typename K, typename T>
  MinPriorityQueue<K, T>::~MinPriorityQueue() {
    free(m_heap);
  }

  template<typename K, typename T>
  void MinPriorityQueue<K, T>::insert(const K &key, const T &value) {
    if(m_heapSize == m_heapAllocated)
      m_growHeap();

    m_heap[m_heapSize].m_key = key;
    m_heap[m_heapSize].m_value = value;
    m_heapSize += 1;
    m_floatMinHeapify(m_heapSize - 1);
  }

  template<typename K, typename T>
  T MinPriorityQueue<K, T>::next() const {
    assert(m_heapSize >= 1);
    return m_heap[0].m_value;
  }

  template<typename K, typename T>
  K MinPriorityQueue<K, T>::nextKey() const {
    assert(m_heapSize >= 1);
    return m_heap[0].m_key;
  }

  template<typename K, typename T>
  T MinPriorityQueue<K, T>::popNext() {
    assert(m_heapSize >= 1);

    T result = m_heap[0].m_value;
    m_heap[0] = m_heap[--m_heapSize];
    if(m_heapSize > 0)
      m_sinkMinHeapify(0);

    return result;
  }

  template<typename K, typename T>
  T MinPriorityQueue<K, T>::at(size_t index) const {
    assert(index < m_heapSize);
    return m_heap[index].m_value;
  }

  template<typename K, typename T>
  K MinPriorityQueue<K, T>::keyAt(size_t index) const {
    assert(index < m_heapSize);
    return m_heap[index].m_key;
  }

  template<typename K, typename T>
  void MinPriorityQueue<K, T>::remove(size_t index) {
    assert(m_heapSize > 0);
    assert(index >= 0);
    assert(index < m_heapSize);

    // shrink the heap
    m_heapSize -= 1;

    if(index == m_heapSize)
      return;  // removing the last element is trivial

    // copy the element that fell off the heap to the given index
    m_heap[index] = m_heap[m_heapSize];

    if(index > 0) {
      size_t parent = (index + 1) / 2 - 1;
      // float heapify if the new key at index is smaller than its parent key
      if(m_heap[index].m_key < m_heap[parent].m_key) {
        m_floatMinHeapify(index);
        return;
      }
    }
    // sink heapify in case the new key at index is larger than either of its children
    m_sinkMinHeapify(index);
  }

  template<typename K, typename T>
  void MinPriorityQueue<K, T>::m_sinkMinHeapify(size_t i) {
    assert(m_heapSize > 0);
    assert(i >= 0);
    assert(i < m_heapSize);

    size_t smallest;
    while(true)
    {
      size_t left = (i + 1) * 2 - 1;
      size_t right = (i + 1) * 2;
      if((left < m_heapSize) && (m_heap[left].m_key < m_heap[i].m_key))
        smallest = left;
      else
        smallest = i;
      if((right < m_heapSize) && (m_heap[right].m_key < m_heap[smallest].m_key))
        smallest = right;
      if(smallest != i) {
        KeyValuePair temp = m_heap[i];
        m_heap[i] = m_heap[smallest];
        m_heap[smallest] = temp;
      }
      else
        break;
    }
  }

  template<typename K, typename T>
  void MinPriorityQueue<K, T>::m_floatMinHeapify(size_t i) {
    size_t parent = (i + 1) / 2 - 1;
    while(i > 0 && m_heap[i].m_key < m_heap[parent].m_key) {
      KeyValuePair temp = m_heap[parent];
      m_heap[parent] = m_heap[i];
      m_heap[i] = temp;
      i = parent;
      parent = (i + 1) / 2 - 1;
    }
  }

  template<typename K, typename T>
  void MinPriorityQueue<K, T>::m_growHeap() {
    if(m_heapAllocated == 0)
      m_heapAllocated = 16;  // arbitrary number
    assert(m_heapSize <= m_heapAllocated);
    size_t newHeapAllocated = m_heapAllocated * 2;
    KeyValuePair *newHeap = static_cast<KeyValuePair *>(malloc(newHeapAllocated * sizeof(KeyValuePair)));
    memcpy(newHeap, m_heap, m_heapSize * sizeof(KeyValuePair));
    free(m_heap);
    m_heap = newHeap;
    m_heapAllocated = newHeapAllocated;
  }
}

#endif
