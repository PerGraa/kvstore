// Silence Clang compiler warnings
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wpessimizing-move"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wcast-align"

#include "embeddedRest/UrlRequest.hpp"

#pragma clang diagnostic pop

int main() {
  UrlRequest request;
  request.host("0.0.0.0");
  request.port(8888);
  //  request.host("google.com");
  request.timeout = {1, 0};  // 1 second timeout
  request.uri("/size");

  request.addHeader("Accept: */*");
  //  request.addHeader("Connection: keep-alive");
  auto response = request.perform();
  cout << "status code = " << response.statusCode() << ", body = *" << response.body()
       << "*" << endl;

  return 0;

  UrlRequest request1;
  request1.host("http://google.com");  //.port(8888);
                                       //  request.uri("/size");

  request1.method("GET");

  //  request.perform();
  auto response1 = request1.perform();
  /*  auto response = std::move(request.perform());

    std::cout << "status code[" << response.statusCode()
              << "], body[" << response.body()
              << "], description = " << response.statusDescription() << "]\n";
  */
  return EXIT_SUCCESS;
}
