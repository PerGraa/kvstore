#ifndef FIFO_CACHE_HPP
#define FIFO_CACHE_HPP

#include "store.hpp"
#include "swap.hpp"
#include <algorithm>
#include <vector>

namespace kvstore {

// A FIFO cache.
// For simulation purposes, pretend that the up to MAX_SIZE number of
// chars/bytes saved in the cache is equal to the actual memory use by
// the data structure.
// Can be extended with a swap, defaults to the EmptySwap which puts
// all input into the void.
template <size_t MAX_SIZE, class SwapType = EmptySwap>
class FIFOCache : public StoreBase<FIFOCache<MAX_SIZE, SwapType>>,
                  public SwapType {
  // Allow our base class to call our private member functions
  friend class StoreBase<FIFOCache<MAX_SIZE, SwapType>>;

private:
  // Updating an existing value will *not* move position in queue.
  bool put_impl(const std::string &key, const std::string &value) override {

    // Try to update existing pair in primary storage
    auto it = find_by_key(key);

    if (it != m_vector.end()) {
      // Recalculate estimated memory use
      m_current_size -= it->second.length();
      m_current_size += value.length();
      // Update primary with new value
      *it = {key, value};
    } else if(SwapType::swap_has(key)) { // Not in primary storage, try secondary storage
      // Update secondary with new value
      SwapType::swap_save(key, value);
    }
    else { // Not found anywhere, add new pair in primary storage
      // Update estimated memory use
      m_current_size += key.length();
      m_current_size += value.length();
      m_vector.emplace_back(key, value);
    }

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
      return {true, it->second};
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

#endif  // FIFO_CACHE_HPP
