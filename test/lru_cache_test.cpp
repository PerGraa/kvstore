#include "picotest/picotest.h"

#include "server/lru_cache.hpp"

using namespace kvstore;
using std::to_string;

TEST(lru_cache, simple) {
  LRUCache<20> c;

  EXPECT_EQ(c.size(), 0u);

  auto r = c.get("foo");
  EXPECT_FALSE(r.first);

  EXPECT_EQ(c.size(), 0u);

  EXPECT_TRUE(c.put("key1", "val0"));
  EXPECT_EQ(c.size(), 1u);
  EXPECT_TRUE(c.put("key1", "val1"));
  EXPECT_EQ(c.size(), 1u);

  r = c.get("key1");
  EXPECT_TRUE(r.first);
  EXPECT_EQ(r.second, "val1");
  EXPECT_EQ(c.size(), 1u);

  r = c.get("key2");
  EXPECT_FALSE(r.first);
  EXPECT_EQ(c.size(), 1u);

  EXPECT_TRUE(c.put("key2", "val2"));
  r = c.get("key2");
  EXPECT_TRUE(r.first);
  EXPECT_EQ(r.second, "val2");
  EXPECT_EQ(c.size(), 2u);

  EXPECT_TRUE(c.del("key1"));
  r = c.get("key1");
  EXPECT_FALSE(r.first);
  EXPECT_EQ(c.size(), 1u);
}

TEST(lru_cache, many) {
  LRUCache<55000> c;
  constexpr int MAX = 4000;

  for (int i = 0; i < MAX; ++i) {
    EXPECT_TRUE(c.put("key" + to_string(i), "val" + to_string(i)));
  }

  for (int i = 0; i < MAX; ++i) {
    auto r = c.get("key" + to_string(i));
    EXPECT_TRUE(r.first);

    EXPECT_EQ(r.second, "val" + to_string(i));

    EXPECT_TRUE(c.del("key" + to_string(i)));
    r = c.get("key" + to_string(i));
    EXPECT_FALSE(r.first);
    EXPECT_FALSE(c.del("key" + to_string(i)));
  }
}

TEST(lru_cache, too_many) {
  LRUCache<20> c;

  EXPECT_TRUE(c.put("key1", "val1"));
  EXPECT_TRUE(c.put("key2", "val2"));
  EXPECT_TRUE(c.put("key3", "val3"));
  EXPECT_TRUE(c.put("key4", "val4"));
  EXPECT_TRUE(c.put("key5", "val5"));
  EXPECT_EQ(c.size(), 2u);

  EXPECT_FALSE(c.get("key1").first);
  EXPECT_FALSE(c.get("key2").first);
  EXPECT_FALSE(c.get("key3").first);
  // Only room for 2 * 2 * 4 bytes in 20 byte cache
  EXPECT_TRUE(c.get("key4").first);
  EXPECT_TRUE(c.get("key5").first);
  EXPECT_EQ(c.size(), 2u);
}

TEST(lru_cache, too_many_lru) {
  LRUCache<40> c;

  EXPECT_TRUE(c.put("key1", "val1"));
  EXPECT_TRUE(c.put("key2", "val2"));
  EXPECT_TRUE(c.put("key3", "val3"));
  EXPECT_TRUE(c.put("key4", "val4"));
  EXPECT_TRUE(c.put("key5", "val5"));
  EXPECT_EQ(c.size(), 5u);

  EXPECT_TRUE(c.get("key1").first);
  EXPECT_TRUE(c.get("key2").first);
  EXPECT_TRUE(c.get("key3").first);
  EXPECT_TRUE(c.get("key4").first);
  EXPECT_TRUE(c.get("key5").first);

  EXPECT_EQ(c.get("key1").second, "val1");
  EXPECT_EQ(c.get("key2").second, "val2");
  EXPECT_EQ(c.get("key3").second, "val3");
  EXPECT_EQ(c.get("key4").second, "val4");
  EXPECT_EQ(c.get("key5").second, "val5");
  EXPECT_EQ(c.size(), 5u);

  EXPECT_TRUE(c.get("key1").first);
  EXPECT_TRUE(c.get("key3").first);
  EXPECT_TRUE(c.get("key5").first);
  EXPECT_EQ(c.size(), 5u);

  EXPECT_TRUE(c.put("key6", "val6"));
  EXPECT_EQ(c.size(), 5u);
  EXPECT_TRUE(c.put("key7", "val7"));
  EXPECT_EQ(c.size(), 5u);

  EXPECT_TRUE(c.get("key1").first);
  EXPECT_TRUE(c.get("key3").first);
  EXPECT_TRUE(c.get("key5").first);
  EXPECT_TRUE(c.get("key6").first);
  EXPECT_TRUE(c.get("key7").first);
  EXPECT_EQ(c.size(), 5u);

  EXPECT_FALSE(c.get("key2").first);
  EXPECT_FALSE(c.get("key4").first);
  EXPECT_EQ(c.size(), 5u);
}

TEST(lru_cache, way_too_many) {
  LRUCache<12> c;
  constexpr int MAX = 999;

  EXPECT_FALSE(c.put("0123456789", "0123456789"));

  for (int i = 0; i < MAX; ++i) {
    EXPECT_TRUE(c.put("key" + to_string(i), "val" + to_string(i)));
  }

  for (int i = 0; i < MAX - 1; ++i) {
    EXPECT_FALSE(c.get("key" + to_string(i)).first);
  }

  EXPECT_TRUE(c.get("key" + to_string(MAX - 1)).first);
}
