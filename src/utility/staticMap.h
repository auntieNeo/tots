#ifndef TOTS_UTILITY_STATIC_MAP_H_
#define TOTS_UTILITY_STATIC_MAP_H_

namespace tots { namespace utility {
  /**
   * This is a template class container for a map datastructure in which no
   * data is allocated dynamically, but is instead allocated statically with the
   * class definition.
   *
   * The 
   */
  template<typename K, typename T, size_t MaxSize>
  class StaticMap {
    public:
      StaticMap();
      ~StaticMap();

      void insert(const K& key, const T& value);
      const T value(const K& key);

    private:
      struct KeyValuePair {
        K m_key;
        T m_value;
      };
      KeyValuePair m_items[MaxSize];
      size_t m_size;

      void m_insertionSort();
  };

  template<typename K, typename T, size_t MaxSize>
    StaticMap<K, T, MaxSize>::StaticMap() {
    }

  template<typename K, typename T, size_t MaxSize>
    StaticMap<K, T, MaxSize>::~StaticMap() {
    }

  template<typename K, typename T, size_t MaxSize>
    void StaticMap<K, T, MaxSize>::insert(const K& key, const T& value) {
      assert(m_size < MaxSize);
      m_items[m_size].m_key = key;
      m_items[m_size].m_value = value;
      m_size += 1;

      // perform insertion sort; we assume the MaxSize is relatively small
      m_insertionSort();
    }

  template<typename K, typename T, size_t MaxSize>
    void StaticMap<K, T, MaxSize>::m_insertionSort() {
      // TODO
    }
} }

#endif
