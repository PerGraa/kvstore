#include <algorithm>
#include <iostream>
#include <thread>

#include "server/json_response.hpp"
#include "server/fifo_cache.hpp"
#include "server/rest_server.hpp"

int main() {
  using namespace kvstore;  // NOLINT

  // Select cache type and response type at compile with template
  // arguments.

  // The call below will not return in the normal case.
  // Kill server with Ctrl+C in terminal.

  // Simulate there is room for 100 bytes in the FIFO cache.
  // Write JSON output to clients.
  constexpr int max = 100;
  std::cout << "Server: FIFO cache store with " << max <<" bytes\n"
            << "JSON REST server output\n";
  start_rest_server<FIFOCache<max>, JSONResponse>();

  return EXIT_SUCCESS;
}
