#ifndef STORE_HPP
#define STORE_HPP

#include "cache.hpp"
#include <mutex>
#include <string>

// TODO(graa):
// namespace caches {

template <typename Cache>
class Store {
 public:
  // Add/update a key-value pair in the store.
  // Key must not be empty string.
  // Value may be empty string.
  // Return: True if a key/value was inserted
  bool put(const std::string &key, const std::string &value) {
    std::lock_guard<std::mutex> guard(m_mutex);

    if (key.empty()) {
      return false;
    }

    return m_cache.insert(key, value);
  }

  // Delete a pair from the store by key
  // Return: True if key/value was present before deletion.
  //         False if key was not found.
  bool del(const std::string &key) {
    std::lock_guard<std::mutex> guard(m_mutex);

    if (key.empty()) {
      return false;
    }

    return m_cache.erase(key);
  }

  // Get a pair from the store by key
  // Return:
  //   bool:   Key found or not.
  //   string: Value if key was found.
  //           Undefined value if key was not found.
  const std::pair<bool, std::string> get(const std::string &key) {
    std::lock_guard<std::mutex> guard(m_mutex);

    if (key.empty()) {
      return {false, ""};
    }

    return m_cache.retrieve(key);
  }

  // Current number of key/value pairs in store
  size_t size() {
    std::lock_guard<std::mutex> guard(m_mutex);

    return m_cache.size();
  }

 private:
  Cache m_cache;
  std::mutex m_mutex;

  // TODO(graa):
  // using range = std::pair<float, float>;
  //  typedef std::mutex Lock;
  //  typedef std::lock_guard<Lock> Guard;
  //  Lock metricsLock;
};

#endif  // STORE_HPP
