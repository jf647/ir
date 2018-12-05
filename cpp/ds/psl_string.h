#ifndef PSL_STRING_H
#define PSL_STRING_H
#include <string>
#include <vector>

class Token {
public:
  typedef std::string key_type;
  typedef std::vector<std::string> value_type;
  Token(key_type key, value_type value);
  static Token Zero();
  key_type key() const;
  value_type value() const;
  Token &operator+=(const Token &rhs);

private:
  key_type _key;
  value_type _value;
};

#endif /* PSL_STRING_H */
