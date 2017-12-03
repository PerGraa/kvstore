#ifndef STORE_RESPONSE_HPP
#define STORE_RESPONSE_HPP

#include "crow/crow.hpp"
#include <string>

namespace kvstore {

// ABC for writing different types of REST HTTP server responses
class StoreResponse {
 public:
  using response = crow::response;

  virtual response put_response(const std::string &key, const std::string &value,
                                bool result) = 0;

  virtual response delete_response(const std::string &key, bool result) = 0;

  virtual response get_response(const std::string &key, const std::string &value,
                                bool result) = 0;

  virtual response size_response(size_t size) = 0;
};
}  // namespace kvstore

#endif  // STORE_RESPONSE_HPP
