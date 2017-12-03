// Silence GCC compiler warnings from embeddedRest
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include "embeddedRest/UrlRequest.hpp"
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop

int main() {
  UrlRequest request;
  request.host("0.0.0.0");
  request.port(18080);
  //  request.host("google.com");
  request.timeout = {3, 0};  // 1 second timeout
                             //  request.uri("/size");

  request.addHeader("Accept: */*");
  //  request.addHeader("Connection: keep-alive");
  auto response = std::move(request.perform());
  cout << "status code = " << response.statusCode() << ", body = *" << response.body()
       << "*" << endl;

  return 0;

  UrlRequest request1;
  request1.host("http://google.com");  //.port(8888);
                                       //  request.uri("/size");

  request1.method("GET");

  //  request.perform();
  auto response1 = std::move(request1.perform());
  /*  auto response = std::move(request.perform());

    std::cout << "status code[" << response.statusCode()
              << "], body[" << response.body()
              << "], description = " << response.statusDescription() << "]\n";
  */
  return EXIT_SUCCESS;
}
