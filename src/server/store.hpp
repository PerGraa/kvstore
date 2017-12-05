#ifndef STORE_HPP
#define STORE_HPP

#include <mutex>
#include <string>
#include <unordered_map>

namespace kvstore {

// Store ABC with pure virtual functions.
// Handle all synchronized store access in this class.
// Use Curiously Recurring Template Pattern (CRTP) in derived
// classes for providing compile-time polymorphism.
template <typename CacheType>
class StoreBase {
  using Lock  = std::mutex;
  using Guard = std::lock_guard<Lock>;

 public:
  // Add/update a key-value pair in the store.
  // Key must not be empty string.
  // Value may be empty string.
  // Return: True if a key/value was inserted
  bool put(const std::string &key, const std::string &value) {
    Guard guard(m_store_lock);

    // Empty key not allowed.
    if (key.empty()) {
      return false;
    }

    return static_cast<CacheType *>(this)->put_impl(key, value);
  }

  // Delete a pair from the store by key
  // Key must not be empty string.
  // Return: True if key/value was present before deletion.
  //         False if key was not found.
  bool del(const std::string &key) {
    Guard guard(m_store_lock);

    // Empty key not allowed.
    if (key.empty()) {
      return false;
    }

    return static_cast<CacheType *>(this)->delete_impl(key);
  }

  // Get a pair from the store by key
  // Key must not be empty string.
  // Return:
  //   bool:   Key found or not.
  //   string: Value if key was found.
  //           Undefined value if key was not found.
  const std::pair<bool, std::string> get(const std::string &key) {
    Guard guard(m_store_lock);

    // Empty key not allowed.
    if (key.empty()) {
      return {false, ""};
    }

    return static_cast<CacheType *>(this)->get_impl(key);
  }

  // Current number of key/value pairs in store
  size_t size() {
    Guard guard(m_store_lock);

    return static_cast<CacheType *>(this)->size_impl();
  }

 private:
  virtual bool put_impl(const std::string &key, const std::string &value) = 0;
  virtual bool delete_impl(const std::string &key) = 0;
  virtual const std::pair<bool, std::string> get_impl(const std::string &key) = 0;
  virtual size_t size_impl() = 0;

  Lock m_store_lock;
};

// The basic store which just stores everything in memory.
class Store : public StoreBase<Store> {
  // Allow our base class to call our private member functions
  friend class StoreBase<Store>;

 private:
  bool put_impl(const std::string &key, const std::string &value) override {
    auto p = m_map.insert({key, value});
    if (!p.second) {
      // Node exist, overwrite previous value
      p.first->second = value;
    }
    return true;
  }

  bool delete_impl(const std::string &key) override { return (m_map.erase(key) > 0); }

  const std::pair<bool, std::string> get_impl(const std::string &key) override {
    try {
      return {true, m_map.at(key)};
    } catch (const std::out_of_range &) {
      // Not found
      return {false, key};
    }
  }

  size_t size_impl() override { return m_map.size(); }

 private:
  std::unordered_map<std::string, std::string> m_map;
};

}  // namespace kvstore

#endif  // STORE_HPP
