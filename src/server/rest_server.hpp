#ifndef REST_SERVER_HPP
#define REST_SERVER_HPP

#include "crow/crow.hpp"

namespace kvstore {

// A REST HTTP server as provided by the Crow library.
template <typename StoreType, typename StoreResponseType>
void start_CROW_server() {
  crow::SimpleApp app;
  constexpr uint port = 8888;

  StoreType m_store;
  StoreResponseType m_writer;

  CROW_ROUTE(app, "/size")
    ([&m_store, &m_writer]() {
      return m_writer.size_response(m_store.size());
    });

  app.loglevel(crow::LogLevel::Debug); //TODO
  app.debug_print(); // TODO
// app.loglevel(crow::LogLevel::Warning); //TODO


  std::cout << "REST server starting at 0.0.0.0:" << port << '\n'
            << "Server is using " << std::max(1u, std::thread::hardware_concurrency()) << " threads\n"
            << "Kill server with Ctrl+C\n";
  
  app.port(port).multithreaded().run();
  std::cout << "REST server shutting down\n";
}

}  // namespace kvstore

#endif  // REST_ENDPOINT_HPP
