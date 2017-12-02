#include "gtest_include.hpp"

#include "server/full_cache.hpp"

using std::to_string;

TEST(full_cache, simple) {
  FullCache c;

  auto r = c.get("foo");
  EXPECT_FALSE(r.first);

  EXPECT_TRUE(c.put("key1", "val0"));
  EXPECT_TRUE(c.put("key1", "val1"));

  r = c.get("key1");
  EXPECT_TRUE(r.first);
  EXPECT_EQ(r.second, "val1");

  r = c.get("key2");
  EXPECT_FALSE(r.first);

  EXPECT_TRUE(c.put("key2", "val2"));
  r = c.get("key2");
  EXPECT_TRUE(r.first);
  EXPECT_EQ(r.second, "val2");

  EXPECT_TRUE(c.del("key1"));
  r = c.get("key1");
  EXPECT_FALSE(r.first);
}

TEST(full_cache, many) {
  FullCache c;
  constexpr int MAX = 100000;

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
