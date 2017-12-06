#ifndef LRU_CACHE_HPP
#define LRU_CACHE_HPP

#include "store.hpp"
#include "swap.hpp"
#include <algorithm>
#include <vector>

namespace kvstore {

// A LRU cache.
// For simulation purposes, pretend that number of chars/bytes saved
// in the cache is equal to the actual memory use by the data
// structure.
// Can be extended with a swap, defaults to the EmptySwap which puts
// all input into the void.
template <size_t MAX_SIZE, typename SwapType = EmptySwap>
class LRUCache : public StoreBase<LRUCache<MAX_SIZE, SwapType>>, public SwapType {
  // Allow our base class to call our private member functions
  friend class StoreBase<LRUCache<MAX_SIZE, SwapType>>;

 private:
  bool put_impl(const std::string &key, const std::string &value) override {
    // Remove from current position in primary storage, if present
    delete_impl(key);

    // Update estimated memory use
    m_current_size += key.length();
    m_current_size += value.length();

    // Remove from secondary storage, if present
    SwapType::swap_delete(key);

    // Insert/move in back
    m_vector.emplace_back(key, value);

    // Delete pairs from front until below/equal MAX_SIZE again
    // Note that a key/value inserted in the code above may be swapped
    // into secondary storage, which may be into the void.
    while (!m_vector.empty() && (m_current_size > MAX_SIZE)) {
      auto front = m_vector.begin();

      // Copy to secondary storage
      SwapType::swap_save(front->first, front->second);
      // Reduce estimated memory use
      m_current_size -= front->first.length() + front->second.length();
      // Delete from primary storage
      m_vector.erase(front);
    }

    // Did the put succeed in the end?
    // This can be probably be optimized be keeping better track in
    // the current function.
    return (find_by_key(key) != m_vector.end()) || SwapType::swap_has(key);
  }

  const std::pair<bool, std::string> get_impl(const std::string &key) override {
    // Try primary storage
    auto it = find_by_key(key);

    if (it != m_vector.end()) {
      // Beware of invalidating iterator
      auto value = it->second;
      // Key has been used, move to back.
      m_vector.erase(it);
      m_vector.emplace_back(key, value);
      return {true, value};
    }

    // No key/value in primary, try to find/delete it in swap
    return SwapType::swap_get(key);
  }

  bool delete_impl(const std::string &key) override {
    auto it = find_by_key(key);

    if (it != m_vector.end()) {
      // Reduce estimated memory use
      m_current_size -= it->first.length() + it->second.length();
      m_vector.erase(it);
      return true;
    }

    // No key/value in cache, try to delete it in swap.
    return SwapType::swap_delete(key);
  }

  size_t size_impl() override { return m_vector.size() + SwapType::swap_size(); }

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

#endif  // LRU_CACHE_HPP
