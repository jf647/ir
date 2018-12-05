#include "psl_string.h"
Token::Token(Token::key_type key, Token::value_type value)
    : _key(key), _value(value) {}
Token Token::Zero() { return Token("", {""}); }
Token::key_type Token::key() const { return _key; }
Token::value_type Token::value() const { return _value; }
Token &Token::operator+=(const Token &rhs) {
  if (_key == rhs.key()) {
    _value.insert(_value.end(), rhs._value.begin(), rhs._value.end());
    std::sort(_value.begin(), _value.end());
    auto last = std::unique(_value.begin(), _value.end());
    _value.erase(last);
  }
  return *this;
}
