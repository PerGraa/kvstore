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
    // Reserve order in resulting JSON output, not that it matters much.
    json["value"]  = value;
    json["key"]    = key;
    json["result"] = result;
    json["type"]   = "put";

    return response{(result ? 200 : 500),  // HTTP OK or HTTP Internal Server Error
                    json};
  }

  response delete_response(const std::string &key, bool result) override {
    Json json;
    json["key"]    = key;
    json["result"] = result;
    json["type"]   = "delete";

    return json;
  }

  response get_response(const std::string &key, const std::string &value,
                        bool result) override {
    Json json;

    if (result) {
      json["value"] = value;
    } else {
      // Results in the JSON type "null"
      // Fair choice since the empty string would still be a string,
      // JSON null is not a string.
      json["value"] = Json{};
    }

    json["key"]    = key;
    json["result"] = result;
    json["type"]   = "get";

    return response{(result ? 200 : 404),  // HTTP OK or HTTP Not Found
                    json};
  }

  response size_response(size_t size) override {
    Json json;
    json["size"]   = size;
    json["result"] = true;
    json["type"]   = "size";

    return json;
  }
};
}  // namespace kvstore

#endif  // JSON_RESPONSE_HPP
