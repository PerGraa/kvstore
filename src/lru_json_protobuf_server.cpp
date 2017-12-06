#include <algorithm>
#include <iostream>
#include <thread>

#include "server/json_response.hpp"
#include "server/lru_cache.hpp"
#include "server/protobuf_swap.hpp"
#include "server/rest_server.hpp"

int main() {
  using namespace kvstore;  // NOLINT

  // Select cache type, swap type and response type at compile time
  // with template arguments.

  // The call below will not return in the normal case.
  // Kill server with Ctrl+C in terminal.

  // Simulate there is room for 1000 bytes in the LRU cache.
  // Use a swap file with protobuf serialization.
  // Write JSON output to clients.
  constexpr int max = 1000;
  std::cout << "Server: LRU cache store with " << max << " bytes\n"
            << "Protobuf swap file used when cache runs full\n"
            << "JSON REST server output\n";
  start_rest_server<LRUCache<max, ProtobufSwap>, JSONResponse>();

  return EXIT_SUCCESS;
}
