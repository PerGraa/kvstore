#ifndef STORE_RESPONSE_HPP
#define STORE_RESPONSE_HPP

#include "pistache_include.hpp" //TODO
#include "crow/crow.hpp"
#include <string>

namespace kvstore {

// ABC for writing different types of REST HTTP server responses
class StoreResponse {
 public:
  using ResponseWriter = Pistache::Http::ResponseWriter;
  using response = crow::response;

  virtual void put_response(ResponseWriter &response,  // NOLINT
                            const std::string &key, const std::string &value,
                            bool result) = 0;

  virtual void delete_response(ResponseWriter &response,  // NOLINT
                               const std::string &key, bool result) = 0;

  virtual void get_response(ResponseWriter &response,  // NOLINT
                            const std::string &key, const std::string &value,
                            bool result) = 0;

  virtual void size_response(ResponseWriter &response,  // NOLINT
                             size_t size) = 0;
  virtual response size_response(size_t size) = 0;
};
}  // namespace kvstore

#endif  // STORE_RESPONSE_HPP
