#include "gtest_include.hpp"

#include "server/full_cache.hpp"
#include "server/store.hpp"

using std::to_string;

TEST(full_cache, simple) {
  Store<FullCache> s;

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

TEST(full_cache, many) {
  Store<FullCache> s;
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
