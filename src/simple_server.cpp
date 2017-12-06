#include <algorithm>
#include <iostream>
#include <thread>

#include "server/simple_response.hpp"
#include "server/rest_server.hpp"
#include "server/store.hpp"

int main() {
  using namespace kvstore;  // NOLINT

  // Select cache type, swap type and response type at compile time
  // with template arguments.

  // The call below will not return in the normal case.
  // Kill server with Ctrl+C in terminal.

  // Full cache store and simple responses to client.
  std::cout << "Server: Full store\n"
            << "Simple REST server output\n";
  start_rest_server<Store, SimpleResponse>();

  return EXIT_SUCCESS;
}
