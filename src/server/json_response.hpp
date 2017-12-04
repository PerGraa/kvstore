#ifndef JSON_RESPONSE_HPP
#define JSON_RESPONSE_HPP

#include "store_response.hpp"
#include <string>

namespace kvstore {

// Write JSON HTTP body responses and try to select fitting HTTP
// response codes.
class JSONResponse : public StoreResponse {
  using Json = crow::json::wvalue;

 public:
  response put_response(const std::string &key, const std::string &value,
                        bool result) override {
    Json json;
    json["type"]   = "put";
    json["result"] = result;
    json["key"]    = key;
    json["value"]  = value;

    return response{(result ? 200 : 200),  // HTTP OK or HTTP Internal Server Error
                    json};
  }

  response delete_response(const std::string &key, bool result) override {
    Json json;
    json["type"]   = "delete";
    json["result"] = result;
    json["key"]    = key;

    return json;
  }

  response get_response(const std::string &key, const std::string &value,
                        bool result) override {
    Json json;
    json["type"]   = "get";
    json["result"] = result;
    json["key"]    = key;

    if (result) {
      json["value"] = value;
    } else {
      // Results in the JSON type "null"
      // Fair choice since the empty string would still be a string,
      // JSON null is not a string.
      json["value"] = Json{};
    }

    return response{(result ? 200 : 404),  // HTTP OK or HTTP Not Found
                    json};
  }

  response size_response(size_t size) override {
    Json json;
    json["type"]   = "size";
    json["result"] = true;
    json["size"]   = size;

    return json;
  }
};
}  // namespace kvstore

#endif  // JSON_RESPONSE_HPP
