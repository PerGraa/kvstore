#include <algorithm>
#include <iostream>
#include <thread>

#include "server/full_cache.hpp"
#include "server/store_endpoint.hpp"

int main() {
  // This call will not return in the normal case.
  // Kill server with Ctrl+C in terminal.
  kvstore::start_rest_server<kvstore::FullCache>();

  return EXIT_SUCCESS;
}
