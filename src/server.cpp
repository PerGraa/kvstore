#include <algorithm>
#include <iostream>
#include <thread>

#include "server/full_cache.hpp"
#include "server/store.hpp"
#include "server/store_endpoint.hpp"

int main() {
  //  Store<FullCache> full;
  //  Store<FIFOCache<100>> fifo;
  //  Store<LRUCache<100>> lru;

  Pistache::Port server_port(8888);
  Pistache::Address server_address(Pistache::Ipv4::any(), server_port);

  // According to standard: hardware_concurrency() may return 0
  uint number_of_threads = std::max(2u, std::thread::hardware_concurrency());

  StoreEndpoint<Store<FullCache>> store_endpoint(server_address);

  store_endpoint.init(number_of_threads);
  std::cout << "server starting using " << number_of_threads << " threads\n";
  store_endpoint.start();

  store_endpoint.shutdown();

  return EXIT_SUCCESS;
}
