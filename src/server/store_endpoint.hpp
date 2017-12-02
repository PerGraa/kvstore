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

namespace kvstore {

template <typename StoreType>
class StoreEndpoint {
  using Request        = Pistache::Rest::Request;
  using ResponseWriter = Pistache::Http::ResponseWriter;

 public:
  explicit StoreEndpoint(Pistache::Address addr)
      : m_http_endpoint(std::make_shared<Pistache::Http::Endpoint>(addr)) {}

  void init(size_t thr = 2) {
    auto opts = Pistache::Http::Endpoint::options().threads(thr);
    m_http_endpoint->init(opts);
    setup_routes();
  }

  void start() {
    m_http_endpoint->setHandler(m_router.handler());
    m_http_endpoint->serve();
  }

  void shutdown() { m_http_endpoint->shutdown(); }

 private:
  void setup_routes() {
    using namespace Pistache::Rest::Routes;  // NOLINT

    Put(m_router, "/key/:key/value/:value", bind(&StoreEndpoint::do_put, this));
    Delete(m_router, "/key/:key", bind(&StoreEndpoint::do_delete, this));
    Get(m_router, "/key/:key", bind(&StoreEndpoint::do_get, this));
    Get(m_router, "/size", bind(&StoreEndpoint::do_size, this));
  }

  void do_put(const Request& request, ResponseWriter response) {
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

  void do_delete(const Request& request, ResponseWriter response) {
    auto key = request.param(":key").as<std::string>();

    if (m_store.del(key)) {
      response.send(Pistache::Http::Code::Ok,
                    "DEL: Key[" + key + "] found and deleted\n");
    } else {
      response.send(Pistache::Http::Code::Ok, "DEL: Key[" + key + "] not found\n");
    }
  }

  void do_get(const Request& request, ResponseWriter response) {
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

  void do_size(const Request& /*unused*/, ResponseWriter response) {
    response.send(Pistache::Http::Code::Ok,
                  "GET: Current store size[" + std::to_string(m_store.size()) + "]\n");
  }

  std::shared_ptr<Pistache::Http::Endpoint> m_http_endpoint;
  Pistache::Rest::Router m_router;
  StoreType m_store;
};

// This function will not return in the normal case.
// Kill server with Ctrl+C in terminal.
template <typename Store>
void start_rest_server() {
  Pistache::Address server_address(Pistache::Ipv4::any(), 8888);

  // According to standard: hardware_concurrency() may return 0
  uint number_of_threads = std::max(2u, std::thread::hardware_concurrency());

  StoreEndpoint<Store> store_endpoint(server_address);
  store_endpoint.init(number_of_threads);

  std::cout << "REST server starting at address:port [" << server_address.host() << ':'
            << server_address.port() << "]\n"
            << "Server is using " << number_of_threads << " threads\n";

  store_endpoint.start();

  // Execution does not normally reach this point
  store_endpoint.shutdown();
}

}  // namespace kvstore

#endif  // STORE_ENDPOINT_HPP
