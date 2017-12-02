#ifndef FIFO_CACHE_HPP
#define FIFO_CACHE_HPP

#include "store.hpp"
#include <algorithm>
#include <vector>

namespace kvstore {

// A FIFO cache.
// For simulation purposes, pretend that the up to MAX_SIZE number of
// chars/bytes saved in the cache is equal to the actual memory use by
// the data structure.
template <size_t MAX_SIZE>
class FIFOCache : public StoreBase<FIFOCache<MAX_SIZE>> {
 public:
  // Updating an existing value will *not* move position in queue.
  bool put_impl(const std::string &key, const std::string &value) override {
    if (key.length() + value.length() > MAX_SIZE) {
      // Do not bother if there will never be space for given key/value
      return false;
    }

    auto it = find_by_key(key);

    // Update existing pair or..
    if (it != m_vector.end()) {
      // Recalculate estimated memory use
      m_current_size -= it->second.length();
      m_current_size += value.length();
      *it = {key, value};
    } else {  //..add new pair
      // Update estimated memory use
      m_current_size += key.length();
      m_current_size += value.length();
      m_vector.emplace_back(key, value);
    }

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
      return {true, it->second};
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
  using vectorOfPair = std::vector<std::pair<std::string, std::string>>;

  vectorOfPair::iterator find_by_key(const std::string &key) {
    return std::find_if(m_vector.begin(), m_vector.end(),
                        [&key](const auto &e) { return e.first == key; });
  }

  vectorOfPair m_vector;
  size_t m_current_size = 0;
};
}  // namespace kvstore

#endif  // FIFO_CACHE_HPP
