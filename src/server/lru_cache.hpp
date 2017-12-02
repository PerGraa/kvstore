#ifndef LRU_CACHE_HPP
#define LRU_CACHE_HPP

#include "store.hpp"
#include <algorithm>
#include <vector>

template <size_t MAX_SIZE>
class LRUCache : public Store<LRUCache<MAX_SIZE>> {
  using vectorPair = std::vector<std::pair<std::string, std::string>>;

 public:
  bool put_impl(const std::string &key, const std::string &value) override {
    if (key.length() + value.length() > MAX_SIZE) {
      // Do not bother if there will never be space for given key/value
      return false;
    }

    // Remove from current position, if present
    delete_impl(key);

    // Update estimated memory use
    m_current_size += key.length();
    m_current_size += value.length();

    // Insert/move in back
    m_vector.emplace_back(key, value);

    auto was_inserted = true;

    // Delete pairs from front until below/equal MAX_SIZE again
    while (!m_vector.empty() && (m_current_size > MAX_SIZE)) {
      if (m_vector.begin()->first == key) {
        was_inserted = false;
      }
      delete_impl(m_vector.begin()->first);
    }

    return was_inserted;
  }

  const std::pair<bool, std::string> get_impl(const std::string &key) override {
    auto it = find_by_key(key);

    if (it != m_vector.end()) {
      // Beware of invalidating iterator
      auto value = it->second;
      // Key has been used, move to back.
      m_vector.erase(it);
      m_vector.emplace_back(key, value);
      return {true, value};
    }

    return {false, key};
  }

  bool delete_impl(const std::string &key) override {
    auto it = find_by_key(key);

    if (it != m_vector.end()) {
      // Reduce estimated memory use
      m_current_size -= it->first.length() + it->second.length();
      m_vector.erase(it);
      return true;
    }

    // No key/value in cache
    return false;
  }

  size_t size_impl() override { return m_vector.size(); }

 private:
  vectorPair::iterator find_by_key(const std::string &key) {
    return std::find_if(m_vector.begin(), m_vector.end(),
                        [&key](const std::pair<std::string, std::string> &e) {
                          return e.first == key;
                        });
  }

  vectorPair m_vector;
  size_t m_current_size = 0;
};

#endif  // LRU_CACHE_HPP
