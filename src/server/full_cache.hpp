#ifndef FULL_CACHE_HPP
#define FULL_CACHE_HPP

#include "cache.hpp"
#include <unordered_map>

class FullCache : public Cache {
 public:
  bool insert(const std::string &key, const std::string &value) override {
    auto p = m_map.insert({key, value});
    if (!p.second) {
      // Node exist, overwrite previous value
      p.first->second = value;
    }
    return true;
  }

  const std::pair<bool, std::string> retrieve(const std::string &key) override {
    try {
      return {true, m_map.at(key)};
    } catch (const std::out_of_range &) {
      return {false, key};
    }
  }

  bool erase(const std::string &key) override { return (m_map.erase(key) > 0); }

  size_t size() override { return m_map.size(); }

 private:
  std::unordered_map<std::string, std::string> m_map;
};

#endif  // FULL_CACHE_HPP
