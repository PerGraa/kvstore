#include <algorithm>
#include <iostream>
#include <thread>

#include "server/simple_response.hpp"
#include "server/json_response.hpp"
#include "server/rest_server.hpp"
#include "server/store.hpp"

int main() {
  using namespace kvstore;  // NOLINT

  // This call will not return in the normal case.
  // Kill server with Ctrl+C in terminal.
  start_rest_server<Store, JSONResponse>();

  return EXIT_SUCCESS;
}
