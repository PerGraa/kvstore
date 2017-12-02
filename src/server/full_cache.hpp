#ifndef FULL_CACHE_HPP
#define FULL_CACHE_HPP

#include "store.hpp"
#include <unordered_map>

namespace kvstore {

// The trivial cache which just stores everything.
class FullCache : public Store<FullCache> {
 public:
  bool put_impl(const std::string &key, const std::string &value) override {
    auto p = m_map.insert({key, value});
    if (!p.second) {
      // Node exist, overwrite previous value
      p.first->second = value;
    }
    return true;
  }

  const std::pair<bool, std::string> get_impl(const std::string &key) override {
    try {
      return {true, m_map.at(key)};
    } catch (const std::out_of_range &) {
      return {false, key};
    }
  }

  bool delete_impl(const std::string &key) override { return (m_map.erase(key) > 0); }

  size_t size_impl() override { return m_map.size(); }

 private:
  std::unordered_map<std::string, std::string> m_map;
};
}  // namespace kvstore

#endif  // FULL_CACHE_HPP
