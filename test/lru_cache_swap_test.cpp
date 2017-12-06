#include "picotest/picotest.h"

#include "server/protobuf_swap.hpp"
#include "server/lru_cache.hpp"

using namespace kvstore;
using std::to_string;

TEST(lru_cache_swap, simple) {
  LRUCache<10, ProtobufSwap> s;

  auto r = s.get("foo");
  EXPECT_FALSE(r.first);
  EXPECT_EQ(s.size(), 0u);

  EXPECT_TRUE(s.put("key1", "val0"));
  EXPECT_TRUE(s.put("key1", "val1"));
  EXPECT_EQ(s.size(), 1u);

  r = s.get("key1");
  EXPECT_TRUE(r.first);
  EXPECT_EQ(r.second, "val1");
  EXPECT_EQ(s.size(), 1u);

  r = s.get("key2");
  EXPECT_FALSE(r.first);
  EXPECT_EQ(s.size(), 1u);

  EXPECT_TRUE(s.put("key2", "val2"));
  r = s.get("key2");
  EXPECT_TRUE(r.first);
  EXPECT_EQ(r.second, "val2");
  EXPECT_EQ(s.size(), 2u);

  EXPECT_TRUE(s.del("key1"));
  r = s.get("key1");
  EXPECT_FALSE(r.first);
  EXPECT_EQ(s.size(), 1u);
}

TEST(lru_cache_swap, many) {
  LRUCache<100, ProtobufSwap> s;
  constexpr int MAX = 1000;

  for (int i = 0; i < MAX; ++i) {
    EXPECT_EQ(s.size(), i);
    EXPECT_TRUE(s.put("key" + to_string(i), "val" + to_string(i)));
    EXPECT_EQ(s.size(), i + 1);
  }

  for (int i = 0; i < MAX; ++i) {
    EXPECT_EQ(s.size(), MAX - i);
    auto r = s.get("key" + to_string(i));
    EXPECT_TRUE(r.first);

    EXPECT_EQ(r.second, "val" + to_string(i));

    EXPECT_TRUE(s.del("key" + to_string(i)));
    r = s.get("key" + to_string(i));
    EXPECT_FALSE(r.first);
    EXPECT_FALSE(s.del("key" + to_string(i)));
    EXPECT_EQ(s.size(), (MAX - i) - 1);
  }
}
