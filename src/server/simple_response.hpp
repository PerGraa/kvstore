#ifndef SIMPLE_RESPONSE_HPP
#define SIMPLE_RESPONSE_HPP

#include "store_response.hpp"
#include <string>

namespace kvstore {

// Write simple human readable HTTP body responses and try to select
// fitting HTTP response codes.
class SimpleResponse : public StoreResponse {
 public:
  void put_response(ResponseWriter &response, const std::string &key,
                    const std::string &value, bool result) override {
    if (result) {
      response.send(Pistache::Http::Code::Ok,
                    "PUT: Saved key[" + key + "] with value[" + value + "]\n");
    } else {
      response.send(Pistache::Http::Code::Internal_Server_Error,
                    "PUT: Could not save key[" + key + "] with value[" + value + "]\n");
    }
  }

  void delete_response(ResponseWriter &response, const std::string &key,
                       bool result) override {
    if (result) {
      response.send(Pistache::Http::Code::Ok,
                    "DEL: Key[" + key + "] found and deleted\n");
    } else {
      response.send(Pistache::Http::Code::Ok, "DEL: Key[" + key + "] not found\n");
    }
  }

  void get_response(ResponseWriter &response, const std::string &key,
                    const std::string &value, bool result) override {
    if (result) {
      response.send(Pistache::Http::Code::Ok,
                    "GET: Found key[" + key + "] with value[" + value + "]\n");
    } else {
      response.send(Pistache::Http::Code::Not_Found,
                    "GET: Did not find key[" + key + "]\n");
    }
  }

  void size_response(ResponseWriter &response, size_t size) override {
    response.send(Pistache::Http::Code::Ok,
                  "GET: Current store size[" + std::to_string(size) + "]\n");
  }
};
}  // namespace kvstore

#endif  // SIMPLE_RESPONSE_HPP
