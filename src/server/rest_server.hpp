#ifndef REST_SERVER_HPP
#define REST_SERVER_HPP

#include "crow/crow.hpp"

namespace kvstore {

// A REST HTTP server as provided by the Crow library.
// This function will not return in the normal case.
// Kill server with Ctrl+C in terminal.
template <typename StoreType, typename StoreResponseType>
void start_rest_server() {
  crow::SimpleApp app;
  constexpr uint port = 8888;

  StoreType m_store;
  StoreResponseType m_writer;

  // Put
  CROW_ROUTE(app, "/put/key/<string>/value/<string>")
      .methods("PUT"_method)([&m_store, &m_writer](std::string key, std::string value) {
        return m_writer.put_response(key, value, m_store.put(key, value));
      });

  // Delete
  CROW_ROUTE(app, "/delete/key/<string>")
      .methods("DELETE"_method)([&m_store, &m_writer](std::string key) {
        return m_writer.delete_response(key, m_store.del(key));
      });

  // Get
  CROW_ROUTE(app, "/get/key/<string>")
      .methods("GET"_method)([&m_store, &m_writer](std::string key) {
        bool result;
        std::string value;
        std::tie(result, value) = m_store.get(key);

        return m_writer.get_response(key, value, result);
      });

  // Size
  CROW_ROUTE(app, "/size").methods("GET"_method)([&m_store, &m_writer]() {
    return m_writer.size_response(m_store.size());
  });

  app.loglevel(crow::LogLevel::Debug);  // TODO
  app.debug_print();                    // TODO
                                        // app.loglevel(crow::LogLevel::Warning); //TODO

  // According to standard: hardware_concurrency() may return 0
  std::cout << "REST server starting at 0.0.0.0:" << port << '\n'
            << "Server is using " << std::max(1u, std::thread::hardware_concurrency())
            << " threads\n"
            << "Kill server with Ctrl+C\n";

  app.port(port).multithreaded().run();
  std::cout << "REST server shutting down\n";
}

}  // namespace kvstore

#endif  // REST_SERVER_HPP
