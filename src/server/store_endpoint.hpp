#ifndef STORE_ENDPOINT_HPP
#define STORE_ENDPOINT_HPP

// Silence GCC compiler warnings from Pistache library
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop

template <typename Store>
class StoreEndpoint {
  using Request        = Pistache::Rest::Request;
  using ResponseWriter = Pistache::Http::ResponseWriter;

 public:
  explicit StoreEndpoint(Pistache::Address addr)
      : httpEndpoint(std::make_shared<Pistache::Http::Endpoint>(addr)) {}

  void init(size_t thr = 2) {
    auto opts = Pistache::Http::Endpoint::options().threads(thr);
    httpEndpoint->init(opts);
    setupRoutes();
  }

  void start() {
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serve();
  }

  void shutdown() { httpEndpoint->shutdown(); }

 private:
  void setupRoutes() {
    using namespace Pistache::Rest::Routes;  // NOLINT

    Put(router, "/key/:key/value/:value", bind(&StoreEndpoint::doPut, this));
    Delete(router, "/key/:key", bind(&StoreEndpoint::doDelete, this));
    Get(router, "/key/:key", bind(&StoreEndpoint::doGet, this));
    Get(router, "/size", bind(&StoreEndpoint::doSize, this));
  }

  void doPut(const Request& request, ResponseWriter response) {
    auto key   = request.param(":key").as<std::string>();
    auto value = request.param(":value").as<std::string>();

    if (store.put(key, value)) {
      response.send(Pistache::Http::Code::Ok,
                    "PUT: Saved key[" + key + "] with value[" + value + "]\n");
    } else {
      response.send(Pistache::Http::Code::Internal_Server_Error,
                    "PUT: Could not save key[" + key + "] with value[" + value + "]\n");
    }
  }

  void doDelete(const Request& request, ResponseWriter response) {
    auto key = request.param(":key").as<std::string>();

    if (store.del(key)) {
      response.send(Pistache::Http::Code::Ok,
                    "DEL: Key[" + key + "] found and deleted\n");
    } else {
      response.send(Pistache::Http::Code::Ok, "DEL: Key[" + key + "] not found\n");
    }
  }

  void doGet(const Request& request, ResponseWriter response) {
    auto key = request.param(":key").as<std::string>();

    bool result;
    std::string value;
    std::tie(result, value) = store.get(key);

    if (result) {
      response.send(Pistache::Http::Code::Ok,
                    "GET: Found key[" + key + "] with value[" + value + "]\n");
    } else {
      response.send(Pistache::Http::Code::Not_Found,
                    "GET: Did not find key[" + key + "]\n");
    }
  }

  void doSize(const Request& /*unused*/, ResponseWriter response) {
    response.send(Pistache::Http::Code::Ok,
                  "GET: Current store size[" + std::to_string(store.size()) + "]\n");
  }

  std::shared_ptr<Pistache::Http::Endpoint> httpEndpoint;
  Pistache::Rest::Router router;
  Store store;
};

#endif  // STORE_ENDPOINT_HPP
