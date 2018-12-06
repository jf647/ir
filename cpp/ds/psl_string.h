#ifndef PSL_STRING_H
#define PSL_STRING_H
#include "persistent_sl.h"
#include <algorithm>
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
PersistentNode<Token> convert_token(const IndexNode *in, int offset);
template <> PersistentNode<Token> convert(const IndexNode *in, int offset) {
  return convert_token(in, offset);
}

void add_to_buffer(std::string filename, const Token &token);
template <> void PersistentSkipList<Token>::operator+=(const Token &rhs) {
  add_to_buffer(_filename, rhs);
}
#endif /* PSL_STRING_H */
