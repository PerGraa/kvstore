#include "gtest_include.hpp"

#include "server/fifo_cache.hpp"
#include "server/store.hpp"

using std::to_string;

TEST(fifo_cache, simple) {
  Store<FIFOCache<20>> s;

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

TEST(fifo_cache, many) {
  Store<FIFOCache<55000>> s;
  constexpr int MAX = 4000;

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

TEST(fifo_cache, too_many) {
  Store<FIFOCache<20>> s;

  EXPECT_TRUE(s.put("key1", "val1"));
  EXPECT_TRUE(s.put("key2", "val2"));
  EXPECT_TRUE(s.put("key3", "val3"));
  EXPECT_TRUE(s.put("key4", "val4"));
  EXPECT_TRUE(s.put("key5", "val5"));

  EXPECT_FALSE(s.get("key1").first);
  EXPECT_FALSE(s.get("key2").first);
  EXPECT_FALSE(s.get("key3").first);
  // Only room for 2 * 2 * 4 bytes in 20 byte cache
  EXPECT_TRUE(s.get("key4").first);
  EXPECT_TRUE(s.get("key5").first);
}

TEST(fifo_cache, too_many_fifo) {
  Store<FIFOCache<40>> s;

  EXPECT_TRUE(s.put("key1", "val1"));
  EXPECT_TRUE(s.put("key2", "val2"));
  EXPECT_TRUE(s.put("key3", "val3"));
  EXPECT_TRUE(s.put("key4", "val4"));
  EXPECT_TRUE(s.put("key5", "val5"));
  EXPECT_EQ(s.size(), 5u);

  EXPECT_TRUE(s.get("key1").first);
  EXPECT_TRUE(s.get("key2").first);
  EXPECT_TRUE(s.get("key3").first);
  EXPECT_TRUE(s.get("key4").first);
  EXPECT_TRUE(s.get("key5").first);
  EXPECT_EQ(s.size(), 5u);

  EXPECT_TRUE(s.get("key1").first);
  EXPECT_TRUE(s.get("key3").first);
  EXPECT_TRUE(s.get("key5").first);
  EXPECT_EQ(s.size(), 5u);

  EXPECT_TRUE(s.put("key6", "val6"));
  EXPECT_EQ(s.size(), 5u);
  EXPECT_TRUE(s.put("key7", "val7"));
  EXPECT_EQ(s.size(), 5u);

  EXPECT_FALSE(s.get("key1").first);
  EXPECT_FALSE(s.get("key2").first);
  EXPECT_TRUE(s.get("key3").first);
  EXPECT_TRUE(s.get("key4").first);
  EXPECT_TRUE(s.get("key5").first);
  EXPECT_TRUE(s.get("key6").first);
  EXPECT_TRUE(s.get("key7").first);

  EXPECT_EQ(s.size(), 5u);
}

TEST(fifo_cache, way_too_many) {
  Store<FIFOCache<12>> s;
  constexpr int MAX = 999;

  EXPECT_FALSE(s.put("0123456789", "0123456789"));

  for (int i = 0; i < MAX; ++i) {
    EXPECT_TRUE(s.put("key" + to_string(i), "val" + to_string(i)));
  }

  for (int i = 0; i < MAX - 1; ++i) {
    EXPECT_FALSE(s.get("key" + to_string(i)).first);
  }

  EXPECT_TRUE(s.get("key" + to_string(MAX - 1)).first);
}
