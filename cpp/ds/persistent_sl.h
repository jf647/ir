#ifndef PERSISTENT_SL_H
#define PERSISTENT_SL_H
#include "flatb/persistent_sl_generated.h"
#include "psl_string.h"
#include "sl.h"
#include <mio/mmap.hpp>

using namespace nvrdb;

template <typename NodeType> class PersistentNode {
public:
  typedef typename NodeType::key_type key_type;
  typedef int value_type;

private:
  key_type _key;
  int indexOffset;

public:
  PersistentNode(key_type key, int index) : _key(key), indexOffset(index) {}
  key_type key() const { return _key; }
  value_type value() const { return indexOffset; }
  PersistentNode &operator+=(const PersistentNode &rhs) { return *this; }
  static PersistentNode Zero() { return PersistentNode(); }
};

template <typename NodeType>
class PersistentSkipList : SkipList<PersistentNode<NodeType>> {
private:
  mio::mmap_sink rw_map;
  Header *header;
  std::string filename;
  virtual PersistentNode<NodeType> add(const IndexNode &node, int pos) = 0;

public:
  typedef typename NodeType::key_type key_type;
  typedef typename NodeType::value_type value_type;
  PersistentSkipList(std::string filename);
  virtual ~PersistentSkipList();
  void operator+=(const NodeType &rhs);
  const value_type &operator[](const key_type &rhs);
};

mio::mmap_sink load_header(std::string filename);

template <typename NodeType>
PersistentNode<NodeType> convert(IndexNode in, int offset);

template <typename NodeType>
PersistentSkipList<NodeType>::PersistentSkipList(std::string filename)
    : SkipList<NodeType>(10, 0.5), rw_map(load_header(filename)),
      header(flatbuffers::GetRoot<Header>(rw_map.data())), filename(filename) {
  if (header->count() > 0) {
    std::error_code error;
    auto ro_map = mio::make_mmap_source(filename, header->list_start(),
                                        header->list_end(), error);
    if (error) {
    }
    while (ro_map.offset() < ro_map.length()) {
      auto in = flatbuffers::GetRoot<IndexNode>(ro_map.data());
      auto pn = convert<NodeType>(in, ro_map.offset());
      *this += pn;
    }
  }
}

void add_to_buffer(std::string filename, const Token &token);
template <> void PersistentSkipList<Token>::operator+=(const Token &rhs) {
  add_to_buffer(filename, rhs);
}

template <typename NodeType>
PersistentSkipList<NodeType>::~PersistentSkipList() {}

#endif /* PERSISTENT_SL_H */
