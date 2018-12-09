#ifndef PERSISTENT_SL_H
#define PERSISTENT_SL_H
#include "flatb/persistent_sl_generated.h"
#include "sl.h"
#include <iostream>
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
  static PersistentNode Zero() {
    auto z = NodeType::Zero();
    return PersistentNode(z.key(), -1);
  }
};

template <typename NodeType>
class PersistentSkipList : SkipList<PersistentNode<NodeType>> {
private:
  mio::ummap_sink rw_map;
  std::string _filename;
  const Header *header;

public:
  typedef typename NodeType::key_type key_type;
  typedef typename NodeType::value_type value_type;
  PersistentSkipList(std::string filename);
  virtual ~PersistentSkipList();
  void operator+=(const NodeType &rhs);
  const value_type &operator[](const key_type &rhs);
};

mio::ummap_sink load_header(std::string filename);

template <typename NodeType>
PersistentNode<NodeType> convert(const IndexNode *in, int offset);

template <typename NodeType>
PersistentSkipList<NodeType>::PersistentSkipList(std::string filename)
    : SkipList<PersistentNode<NodeType>>(10, 0.5),
      header(flatbuffers::GetRoot<Header>(rw_map.data())),
      rw_map(load_header(filename)), _filename(filename) {
  if (header->count() > 0) {
    std::error_code error;
    auto ro_map = mio::make_mmap_source(_filename, header->list_start(),
                                        header->list_end(), error);
    if (error) {
    }
    while (ro_map.offset() < ro_map.length()) {
      auto in = flatbuffers::GetRoot<IndexNode>(ro_map.data());
      auto pn = convert<NodeType>(in, ro_map.offset());
      SkipList<PersistentNode<NodeType>>::operator+=(pn);
    }
  }
}

template <typename NodeType>
PersistentSkipList<NodeType>::~PersistentSkipList() {}

#endif /* PERSISTENT_SL_H */
