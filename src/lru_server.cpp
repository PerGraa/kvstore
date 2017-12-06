#include <algorithm>
#include <iostream>
#include <thread>

#include "server/simple_response.hpp"
#include "server/lru_cache.hpp"
#include "server/rest_server.hpp"

int main() {
  using namespace kvstore;  // NOLINT

  // Select cache type, swap type and response type at compile time
  // with template arguments.

  // The call below will not return in the normal case.
  // Kill server with Ctrl+C in terminal.

  // Simulate there is room for 100 bytes in the LRU cache.
  // Write simple output to clients.
  constexpr int max = 100;
  std::cout << "Server: LRU cache store with " << max << " bytes\n"
            << "JSON REST server output\n";
  start_rest_server<LRUCache<max>, SimpleResponse>();

  return EXIT_SUCCESS;
}
