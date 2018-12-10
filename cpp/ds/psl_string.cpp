#include "psl_string.h"
#include <fstream>
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

PersistentNode<Token> convert_token(const IndexNode *in, int offset) {
  return PersistentNode<Token>(in->key_as_StringKey()->token()->str(),
                               {offset});
}
void append_token_to_buffer(std::string filename, const Token &token,
                            int &offset, int &size) {
  flatbuffers::FlatBufferBuilder builder(1024);
  auto k = builder.CreateString(token.key());
  auto sk = CreateStringKey(builder, k);
  auto docs = builder.CreateVectorOfStrings(token.value());
  IndexNodeBuilder inb(builder);
  inb.add_key_type(Key_StringKey);
  inb.add_key(sk.Union());
  inb.add_docs(docs);
  auto inbOffset = inb.Finish();
  builder.FinishSizePrefixed(inbOffset);
  size = builder.GetSize();
  std::error_code error;
  std::ofstream fp;
  fp.open(filename, std::ios::app | std::ios::binary);
  offset = fp.tellp();
  fp.write((char *)builder.GetBufferPointer(), size);
  fp.flush();
  fp.close();
}
