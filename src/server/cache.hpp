#ifndef CACHE_HPP
#define CACHE_HPP

#include <string>

class Cache {
 public:
  // Insert a key-value pair to the cache
  // Return: True if a key/value was inserted
  virtual bool insert(const std::string &key, const std::string &value) = 0;

  // Erase a pair from the store by key
  // Return: True if a key/value was deleted
  virtual bool erase(const std::string &key) = 0;

  // Retrieve a pair from the store by key
  // Return: Pair with success/failure boolean and the value in case
  // of success
  virtual const std::pair<bool, std::string> retrieve(const std::string &key) = 0;

  // Current number of pairs in store
  virtual size_t size() = 0;
};

#endif  // CACHE_HPP
