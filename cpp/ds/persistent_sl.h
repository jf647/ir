#ifndef PERSISTENT_SL_H
#define PERSISTENT_SL_H
#include "flatb/persistent_sl_generated.h"
#include "sl.h"
#include <mio/mmap.hpp>

using namespace nvrdb;

template <typename NodeType> class PersistentNode {
public:
  typedef typename NodeType::key_type key_type;
  typedef std::vector<int> value_type;

private:
  key_type _key;
  value_type indexOffset;

public:
  PersistentNode(key_type key, value_type index)
      : _key(key), indexOffset(index) {}
  key_type key() const { return _key; }
  value_type value() const { return indexOffset; }
  PersistentNode &operator+=(const PersistentNode &rhs) {
    indexOffset.insert(indexOffset.end(), rhs.indexOffset.begin(),
                       rhs.indexOffset.end());
    return *this;
  }
  static PersistentNode Zero() {
    auto z = NodeType::Zero();
    return PersistentNode(z.key(), {});
  }
};

template <typename NodeType>
class PersistentSkipList : SkipList<PersistentNode<NodeType>> {
private:
  mio::ummap_sink rw_map;
  Header *header;
  std::string _filename;

public:
  typedef typename NodeType::key_type key_type;
  typedef typename NodeType::value_type value_type;
  PersistentSkipList(std::string filename);
  virtual ~PersistentSkipList();
  void operator+=(const NodeType &rhs);
  value_type operator[](const key_type &rhs);
  const static NodeType zero;
};

template <typename NodeType>
const NodeType PersistentSkipList<NodeType>::zero = NodeType::Zero();
mio::ummap_sink load_header(std::string filename);

template <typename NodeType>
PersistentNode<NodeType> convert(const IndexNode *in, int offset);

template <typename NodeType>
PersistentSkipList<NodeType>::PersistentSkipList(std::string filename)
    : SkipList<PersistentNode<NodeType>>(10, 0.5),
      rw_map(load_header(filename)),
      header(flatbuffers::GetMutableRoot<Header>(rw_map.data())),
      _filename(filename) {

  if (header->count() > 0) {
    auto current = header->list_start();
    auto data = rw_map.data() + header->list_start();
    while (current < header->list_end()) {
      auto in = flatbuffers::GetSizePrefixedRoot<IndexNode>(data);
      auto pn = convert<NodeType>(in, current);
      SkipList<PersistentNode<NodeType>>::operator+=(pn);
      auto nodeSize = flatbuffers::GetPrefixedSize(data);
      current += nodeSize;
      data += nodeSize;
    }
  }
  if (header->buffered_count() > 0) {
    auto current = header->list_start();
    auto data = rw_map.data() + header->buffer_start();
    while (current < header->buffer_end()) {
      auto in = flatbuffers::GetSizePrefixedRoot<IndexNode>(data);
      auto pn = convert<NodeType>(in, current);
      SkipList<PersistentNode<NodeType>>::operator+=(pn);
      auto nodeSize = flatbuffers::GetPrefixedSize(data);
      current += nodeSize;
      data += nodeSize;
    }
  }
}
template <typename NodeType>
void add_to_buffer(std::string filename, const NodeType &token, int &offset,
                   int &size);
template <typename NodeType>
void PersistentSkipList<NodeType>::operator+=(const NodeType &rhs) {
  int offset = 0;
  int size = 0;
  add_to_buffer<NodeType>(_filename, rhs, offset, size);
  auto nBufferedCount = header->buffered_count() + 1;
  header->mutate_buffered_count(nBufferedCount);
  header->mutate_buffer_end(header->buffer_end() + size);
  PersistentNode<NodeType> pn(rhs.key(), {offset});
  SkipList<PersistentNode<NodeType>>::operator+=(pn);
  std::error_code error;
  rw_map.sync(error);
  if (error) {
  }
}
template <typename NodeType>
typename PersistentSkipList<NodeType>::value_type PersistentSkipList<NodeType>::
operator[](const PersistentSkipList<NodeType>::key_type &rhs) {
  const static typename NodeType::value_type zero_value =
      NodeType::Zero().value();
  const static typename PersistentNode<NodeType>::value_type pers_zero_value =
      PersistentNode<NodeType>::Zero().value();
  auto in = SkipList<PersistentNode<NodeType>>::operator[](rhs);
  if (in == pers_zero_value) {
    std::cout << "none" << std::endl;
    return zero_value;
  }
  std::vector<std::string> docs;
  for (int offset : in) {
    std::cout << "off" << offset << "mmap" << rw_map.mapped_length()
              << std::endl;
    auto node =
        flatbuffers::GetSizePrefixedRoot<IndexNode>(rw_map.data() + offset);
    auto d = node->docs();
    std::cout << "s" << d->size() << std::endl;
    // std::cout << node->key_as_StringKey()->token()->str() << std::endl;
    for (int i = 0; i < d->size(); i++) {
      auto s = d->GetAsString(i)->str();
      std::cout << "v" << s << std::endl;
      docs.push_back(s);
    }
  }
  return docs;
}

template <typename NodeType>
PersistentSkipList<NodeType>::~PersistentSkipList() {}

#endif /* PERSISTENT_SL_H */
