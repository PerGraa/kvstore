#include <mutex>
#include <vector>
#include <chrono>
#include <thread>

// Silence Clang compiler warnings
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wpessimizing-move"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wcast-align"

#include "embeddedRest/UrlRequest.hpp"

#pragma clang diagnostic pop

/** Thread safe cout class
  * Exemple of use:
  *    PrintThread{} << "Hello world!" << std::endl;
  */
class PrintThread : public std::ostringstream {
 public:
  PrintThread() = default;

  ~PrintThread() {
    std::lock_guard<std::mutex> guard(_mutexPrint);
    std::cout << this->str();
  }

 private:
  static std::mutex _mutexPrint;
};

std::mutex PrintThread::_mutexPrint{};

void send_request(int thread_id, int number_of_messages,
                  std::chrono::milliseconds millisecs) {
  UrlRequest request;
  request.host("0.0.0.0");
  request.port(8888);

  // 1 second timeout
  request.timeout = {1, 0};

  std::random_device random_device;
  std::mt19937 engine{random_device()};
  std::uniform_int_distribution<int> dist(1, 20);
  std::uniform_real_distribution<> dist_real(0.75, 1.25);

  for (int i = 0; i < number_of_messages; ++i) {
    switch (dist(engine) % 4) {
      case 0: {
        request.method("PUT");
        request.uri("/put/key/" + std::to_string(dist(engine) % 10) + "/value/" +
                    std::to_string(i) + "_thread" + std::to_string(thread_id));
      } break;
      case 1: {
        request.method("DELETE");
        request.uri("/delete/key/" + std::to_string(dist(engine) % 10));
      } break;
      case 2: {
        request.method("GET");
        request.uri("/get/key/" + std::to_string(dist(engine) % 10));
      } break;
      case 3: {
        request.method("GET");
        request.uri("/size");
      } break;

      default: { } break; }

    auto response = request.perform();

    // TODO print if debug set
    if (true or response.statusCode() == 408) {
      PrintThread{} << "Thread[" << thread_id << "] MsgID[" << i << thread_id
                    << "] Status[" << response.statusCode()
                    << "] Body: " << response.body();
    }

    std::this_thread::sleep_for(millisecs * dist_real(engine));
  }
}

int main() {
  std::cout << "Starting REST client\n";

  std::vector<std::thread> vt;
  constexpr int number_of_messages = 2000;             // TODO >= 1
  constexpr int number_of_threads  = 10;               // TODO >= 1
  constexpr std::chrono::milliseconds millisecs(250);  // TODO >= 1

  for (int i = 0; i < number_of_threads; ++i) {
    vt.emplace_back(std::thread{send_request, i, number_of_messages, millisecs});
  }

  for (auto &t : vt) {
    t.join();
  }

  std::cout << "Threads[" << number_of_threads << "]\nTotal messages["
            << number_of_threads * number_of_messages << "]\nSleep interval[~"
            << millisecs.count() << "ms]\n";
  return EXIT_SUCCESS;
}
