#include "sl.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

using testing::ElementsAre;
class Token {
public:
  typedef std::string key_type;
  typedef std::vector<std::string> value_type;
  Token(key_type key, value_type value) : _key(key), _value(value) {}
  static Token Zero() { return Token("", {""}); }
  key_type key() const { return _key; }
  value_type value() const { return _value; }
  Token &operator+=(const Token &rhs) {
    if (_key == rhs.key()) {
      _value.insert(_value.end(), rhs._value.begin(), rhs._value.end());
      std::sort(_value.begin(), _value.end());
      auto last = std::unique(_value.begin(), _value.end());
      _value.erase(last);
    }
    return *this;
  }

private:
  key_type _key;
  value_type _value;
};

TEST(TestSL, TestHappy) {
  auto sl = SkipList<Token>(5, 0.5);

  for (int i = 0; i < 10; i++) {
    sl += Token(std::to_string(i + 1), {"hello" + std::to_string(i)});
  }
  for (int i = 0; i < 10; i++) {
    auto docs = sl[std::to_string(i)];
    ASSERT_EQ(docs.size(), 1);
    ASSERT_THAT(docs, ElementsAre("hello" + std::to_string(i)));
  }
}
