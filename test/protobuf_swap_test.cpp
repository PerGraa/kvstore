#include "picotest/picotest.h"

#include "server/protobuf_swap.hpp"

using namespace kvstore;
using std::to_string;

template <typename SwapType = EmptySwap>
class SwapMock : public SwapType {
 public:
  bool swap_save(const std::string& key, const std::string& value) {
    return SwapType::swap_save(key, value);
  }

  const std::pair<bool, std::string> swap_get(const std::string& key) {
    return SwapType::swap_get(key);
  }

  bool swap_has(const std::string& key) { return SwapType::swap_has(key); }

  bool swap_delete(const std::string& key) { return SwapType::swap_delete(key); }

  size_t swap_size() { return SwapType::swap_size(); }
};

TEST(protobuf_swap, simple) {
  SwapMock<ProtobufSwap> s;

  auto r = s.swap_get("foo");
  EXPECT_FALSE(r.first);
  EXPECT_FALSE(s.swap_has("foo"));
  EXPECT_EQ(s.swap_size(), 0u);

  EXPECT_TRUE(s.swap_save("key1", "val0"));
  EXPECT_TRUE(s.swap_save("key1", "val1"));
  EXPECT_EQ(s.swap_size(), 1u);

  r = s.swap_get("key1");
  EXPECT_TRUE(r.first);
  EXPECT_EQ(r.second, "val1");
  EXPECT_TRUE(s.swap_has("key1"));
  EXPECT_EQ(s.swap_size(), 1u);

  r = s.swap_get("key2");
  EXPECT_FALSE(r.first);
  EXPECT_FALSE(s.swap_has("key2"));
  EXPECT_EQ(s.swap_size(), 1u);

  EXPECT_TRUE(s.swap_save("key2", "val2"));
  r = s.swap_get("key2");
  EXPECT_TRUE(r.first);
  EXPECT_EQ(r.second, "val2");
  EXPECT_TRUE(s.swap_has("key2"));
  EXPECT_EQ(s.swap_size(), 2u);

  EXPECT_TRUE(s.swap_delete("key1"));
  r = s.swap_get("key1");
  EXPECT_FALSE(r.first);
  EXPECT_FALSE(s.swap_has("key1"));
  EXPECT_EQ(s.swap_size(), 1u);
}

TEST(protobuf_swap, many) {
  SwapMock<ProtobufSwap> s;
  constexpr int MAX = 1000;

  for (int i = 0; i < MAX; ++i) {
    EXPECT_EQ(s.swap_size(), i);
    EXPECT_TRUE(s.swap_save("key" + to_string(i), "val" + to_string(i)));
    EXPECT_TRUE(s.swap_has("key" + to_string(i)));
    EXPECT_EQ(s.swap_size(), i + 1);
  }

  for (int i = 0; i < MAX; ++i) {
    EXPECT_EQ(s.swap_size(), MAX - i);
    auto r = s.swap_get("key" + to_string(i));
    EXPECT_TRUE(r.first);

    EXPECT_EQ(r.second, "val" + to_string(i));

    EXPECT_TRUE(s.swap_delete("key" + to_string(i)));
    r = s.swap_get("key" + to_string(i));
    EXPECT_FALSE(r.first);
    EXPECT_FALSE(s.swap_delete("key" + to_string(i)));
    EXPECT_EQ(s.swap_size(), (MAX - i) - 1);
  }
}
