#include <thread>
#include <vector>

#include "gtest_include.hpp"

#include "server/store.hpp"

using namespace kvstore;
using std::to_string;

TEST(thread, ten) {
  constexpr size_t MAX_THREADS = 10;
  constexpr size_t MAX_ENTRIES = 10000;

  Store s;
  std::vector<std::thread> workers;

  // Sanity check
  EXPECT_EQ(s.size(), 0u);

  // Put
  for (size_t i = 0; i < MAX_THREADS; ++i) {
    workers.push_back(std::thread([i, &s]() {
      std::string postfix;
      for (size_t j = 0; j < MAX_ENTRIES; ++j) {
        postfix = "_" + to_string(i) + "_" + to_string(j);
        EXPECT_TRUE(s.put("key" + postfix, "val" + postfix));
      }
    }));
  }

  for (auto &t : workers) {
    t.join();
  }

  EXPECT_EQ(s.size(), MAX_ENTRIES * MAX_THREADS);
  workers.clear();

  // Successful get
  for (size_t i = 0; i < MAX_THREADS; ++i) {
    workers.push_back(std::thread([i, &s]() {
      std::string postfix;
      for (size_t j = 0; j < MAX_ENTRIES; ++j) {
        postfix = "_" + to_string(i) + "_" + to_string(j);
        auto r  = s.get("key" + postfix);
        EXPECT_TRUE(r.first);
        EXPECT_EQ(r.second, "val" + postfix);
      }
    }));
  }

  for (auto &t : workers) {
    t.join();
  }

  EXPECT_EQ(s.size(), MAX_ENTRIES * MAX_THREADS);
  workers.clear();

  // Successful delete
  for (size_t i = 0; i < MAX_THREADS; ++i) {
    workers.push_back(std::thread([i, &s]() {
      std::string postfix;
      for (size_t j = 0; j < MAX_ENTRIES; ++j) {
        postfix = "_" + to_string(i) + "_" + to_string(j);
        EXPECT_TRUE(s.del("key" + postfix));
      }
    }));
  }

  for (auto &t : workers) {
    t.join();
  }

  EXPECT_EQ(s.size(), 0u);
  workers.clear();

  // Unsuccessful get
  for (size_t i = 0; i < MAX_THREADS; ++i) {
    workers.push_back(std::thread([i, &s]() {
      std::string postfix;
      for (size_t j = 0; j < MAX_ENTRIES; ++j) {
        postfix = "_" + to_string(i) + "_" + to_string(j);
        auto r  = s.get("key" + postfix);
        EXPECT_FALSE(r.first);
      }
    }));
  }

  for (auto &t : workers) {
    t.join();
  }
}
