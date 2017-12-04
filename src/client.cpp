#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <sstream>

// Silence Clang compiler warnings
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wpessimizing-move"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wcast-align"

#include "embeddedRest/UrlRequest.hpp"

#pragma clang diagnostic pop

// Thread safe cout class
// Example of use: PrintThread{} << "Hello world!" << std::endl;
class PrintThread : public std::ostringstream {
 public:
  PrintThread() = default;

  ~PrintThread() {
    std::lock_guard<std::mutex> guard(g_mutex_print);
    std::cout << this->str();
  }

 private:
  static std::mutex g_mutex_print;
};

std::mutex PrintThread::g_mutex_print{};

void send_request(int thread_id, int number_of_messages, int number_of_keys, int ms) {
  std::chrono::milliseconds millisecs(ms);

  UrlRequest request;
  request.host("0.0.0.0");
  request.port(8888);

  // 1 second timeout
  request.timeout = {1, 0};

  std::random_device random_device;
  std::mt19937 engine{random_device()};
  std::uniform_int_distribution<int> dist_discrete(1, 4 * number_of_keys);
  std::uniform_real_distribution<> dist_real(0.75, 1.25);

  auto get_random_key = [&]() {
    return std::to_string(dist_discrete(engine) % number_of_keys);
  };

  for (int i = 0; i < number_of_messages; ++i) {
    // Select one of the four rest methods evenly.
    switch (dist_discrete(engine) % 4) {
      case 0: {
        request.method("PUT");
        request.uri("/put/key/" + get_random_key() + "/value/" + std::to_string(i) +
                    "_thread" + std::to_string(thread_id));
      } break;
      case 1: {
        request.method("DELETE");
        request.uri("/delete/key/" + get_random_key());
      } break;
      case 2: {
        request.method("GET");
        request.uri("/get/key/" + get_random_key());
      } break;
      case 3: {
        request.method("GET");
        request.uri("/size");
      } break;

      default: { } break; }

    // Wait for response or timeout
    auto response = request.perform();

    // These prints with mutex adds wall time for running client.
    // Linux on older laptop:       4-5% extra time
    // Linux in VM on newer laptop: TODO
    PrintThread{} << "Thread[" << thread_id << "] MsgID[" << i << thread_id
                  << "] Status[" << response.statusCode()
                  << "] Body: " << response.body()
                  << (response.statusCode() == 408 ? "\n" : "");

    // Sleep a bit
    std::this_thread::sleep_for(millisecs * dist_real(engine));
  }
}

bool check_input(char* input, int& target, std::string name, std::string usage) {
  std::istringstream ss(input);
  target = 0;

  if (!(ss >> target) || target < 1) {
    std::cerr << usage;
    std::cerr << name << " must be > 0\n";
    return false;
  }
  return true;
}

int main(int argc, char** argv) {
  int number_of_threads  = 0;
  int number_of_keys     = 0;
  int number_of_messages = 0;
  int millisecs          = 0;
  std::string usage =
      "client <number_of_threads> <number_of_keys> <messages_per_thread> "
      "<sleep_in_milliseconds>\nExample: client 2 10 20 500\n";

  if (argc != 5) {
    std::cerr << usage;
    std::cerr << "Wrong parameters given\n";
    return EXIT_FAILURE;
  }

  if ((!check_input(argv[1], number_of_threads, "number_of_threads", usage)) ||
      (!check_input(argv[2], number_of_keys, "number_of_keys", usage)) ||
      (!check_input(argv[3], number_of_messages, "messages_per_thread", usage)) ||
      (!check_input(argv[4], millisecs, "sleep_in_milliseconds", usage))) {
    return EXIT_FAILURE;
  }

  std::vector<std::thread> vt;

  std::cout << "Sending messages\n";

  for (int i = 0; i < number_of_threads; ++i) {
    // Start threads
    vt.emplace_back(
        std::thread{send_request, i, number_of_messages, number_of_keys, millisecs});
  }

  // Wait for all threads to finish
  for (auto& t : vt) {
    t.join();
  }

  std::cout << "Done. Client exiting\n";
  return EXIT_SUCCESS;
}
