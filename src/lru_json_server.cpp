#include <algorithm>
#include <iostream>
#include <thread>

#include "server/json_response.hpp"
#include "server/lru_cache.hpp"
#include "server/rest_server.hpp"

int main() {
  using namespace kvstore;  // NOLINT

  // Select cache type and response type at compile with template
  // arguments.

  // The call below will not return in the normal case.
  // Kill server with Ctrl+C in terminal.

  // Simulate there is room for 1000 bytes in the LRU cache.
  // Write JSON output to clients.
  constexpr int max = 1000;
  std::cout << "Server: LRU cache store with " << max << " bytes\n"
            << "JSON REST server output\n";
  start_rest_server<LRUCache<1000>, JSONResponse>();

  return EXIT_SUCCESS;
}
