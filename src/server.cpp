#include <algorithm>
#include <iostream>
#include <thread>

#include "server/simple_response.hpp"
#include "server/store.hpp"
#include "server/rest_server.hpp"

int main() {
  using namespace kvstore;  // NOLINT

  // This call will not return in the normal case.
  // Kill server with Ctrl+C in terminal.
  start_rest_server<Store, SimpleResponse>();

  return EXIT_SUCCESS;
}
