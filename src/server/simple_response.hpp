#ifndef SIMPLE_RESPONSE_HPP
#define SIMPLE_RESPONSE_HPP

#include "store_response.hpp"
#include <string>

namespace kvstore {

// Write simple human readable HTTP body responses and try to select
// fitting HTTP response codes.
class SimpleResponse : public StoreResponse {
 public:
  response put_response(const std::string &key, const std::string &value,
                        bool result) override {
    if (result) {
      return response{200,  // HTTP OK
                      "PUT: Saved key[" + key + "] with value[" + value + ']'};
    }
    return response{500,  // HTTP Internal Server Error
                    "PUT: Could not save key[" + key + "] with value[" + value + ']'};
  }

  response delete_response(const std::string &key, bool result) override {
    if (result) {
      return response{200,  // HTTP OK
                      "DEL: Key[" + key + "] found and deleted"};
    }
    return response{200,  // HTTP OK
                    "DEL: Key[" + key + "] not found"};
  }

  response get_response(const std::string &key, const std::string &value,
                        bool result) override {
    if (result) {
      return response{200,  // HTTP OK
                      "GET: Found key[" + key + "] with value[" + value + "]"};
    }
    return response{404,  // HTTP Not Found
                    "GET: Key[" + key + "] not found"};
  }

  response size_response(size_t size) override {
    return {"GET: Current store size[" + std::to_string(size) + "]"};
  }
};
}  // namespace kvstore

#endif  // SIMPLE_RESPONSE_HPP
