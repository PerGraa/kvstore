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

template <typename StoreType>
class StoreEndpoint {
  using Request        = Pistache::Rest::Request;
  using ResponseWriter = Pistache::Http::ResponseWriter;

 public:
  explicit StoreEndpoint(Pistache::Address addr)
      : m_httpEndpoint(std::make_shared<Pistache::Http::Endpoint>(addr)) {}

  void init(size_t thr = 2) {
    auto opts = Pistache::Http::Endpoint::options().threads(thr);
    m_httpEndpoint->init(opts);
    setupRoutes();
  }

  void start() {
    m_httpEndpoint->setHandler(m_router.handler());
    m_httpEndpoint->serve();
  }

  void shutdown() { m_httpEndpoint->shutdown(); }

 private:
  void setupRoutes() {
    using namespace Pistache::Rest::Routes;  // NOLINT

    Put(m_router, "/key/:key/value/:value", bind(&StoreEndpoint::doPut, this));
    Delete(m_router, "/key/:key", bind(&StoreEndpoint::doDelete, this));
    Get(m_router, "/key/:key", bind(&StoreEndpoint::doGet, this));
    Get(m_router, "/size", bind(&StoreEndpoint::doSize, this));
  }

  void doPut(const Request& request, ResponseWriter response) {
    auto key   = request.param(":key").as<std::string>();
    auto value = request.param(":value").as<std::string>();

    if (m_store.put(key, value)) {
      response.send(Pistache::Http::Code::Ok,
                    "PUT: Saved key[" + key + "] with value[" + value + "]\n");
    } else {
      response.send(Pistache::Http::Code::Internal_Server_Error,
                    "PUT: Could not save key[" + key + "] with value[" + value + "]\n");
    }
  }

  void doDelete(const Request& request, ResponseWriter response) {
    auto key = request.param(":key").as<std::string>();

    if (m_store.del(key)) {
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
    std::tie(result, value) = m_store.get(key);

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
                  "GET: Current store size[" + std::to_string(m_store.size()) + "]\n");
  }

  std::shared_ptr<Pistache::Http::Endpoint> m_httpEndpoint;
  Pistache::Rest::Router m_router;
  StoreType m_store;
};

#endif  // STORE_ENDPOINT_HPP
