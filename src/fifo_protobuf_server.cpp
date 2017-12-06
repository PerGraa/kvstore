#include <algorithm>
#include <iostream>
#include <thread>

#include "server/simple_response.hpp"
#include "server/fifo_cache.hpp"
#include "server/protobuf_swap.hpp"
#include "server/rest_server.hpp"

int main() {
  using namespace kvstore;  // NOLINT

  // Select cache type, swap type and response type at compile time
  // with template arguments.

  // The call below will not return in the normal case.
  // Kill server with Ctrl+C in terminal.

  // Simulate there is room for 200 bytes in the FIFO cache.
  // Use a swap file with protobuf serialization.
  // Write simple output to clients.
  constexpr int max = 200;
  std::cout << "Server: FIFO cache store with " << max << " bytes\n"
            << "Simple server output\n";
  start_rest_server<FIFOCache<max, ProtobufSwap>, SimpleResponse>();

  return EXIT_SUCCESS;
}
