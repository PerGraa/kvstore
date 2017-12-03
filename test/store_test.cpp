#include "picotest/picotest.h"

#include "server/store.hpp"

using namespace kvstore;
using std::to_string;

// main test function should be declared precisely once, so why not here.
int main(int argc, char **argv) { RUN_ALL_TESTS(); }

TEST(store, simple) {
  Store s;

  auto r = s.get("foo");
  EXPECT_FALSE(r.first);

  EXPECT_TRUE(s.put("key1", "val0"));
  EXPECT_TRUE(s.put("key1", "val1"));

  r = s.get("key1");
  EXPECT_TRUE(r.first);
  EXPECT_EQ(r.second, "val1");

  r = s.get("key2");
  EXPECT_FALSE(r.first);

  EXPECT_TRUE(s.put("key2", "val2"));
  r = s.get("key2");
  EXPECT_TRUE(r.first);
  EXPECT_EQ(r.second, "val2");

  EXPECT_TRUE(s.del("key1"));
  r = s.get("key1");
  EXPECT_FALSE(r.first);
}

TEST(store, many) {
  Store s;
  constexpr int MAX = 100000;

  for (int i = 0; i < MAX; ++i) {
    EXPECT_TRUE(s.put("key" + to_string(i), "val" + to_string(i)));
  }

  for (int i = 0; i < MAX; ++i) {
    auto r = s.get("key" + to_string(i));
    EXPECT_TRUE(r.first);

    EXPECT_EQ(r.second, "val" + to_string(i));

    EXPECT_TRUE(s.del("key" + to_string(i)));
    r = s.get("key" + to_string(i));
    EXPECT_FALSE(r.first);
    EXPECT_FALSE(s.del("key" + to_string(i)));
  }
}
