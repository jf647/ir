#ifndef PSLV2_H
#define PSLV2_H
#include "flatb/persistent_sl_generated.h"
#include "flatb/sl_descriptor_generated.h"
#include "sl.h"

namespace nvr {
template <typename ds> struct Store {
  std::string path;
  Store(std::string path);
  nvrdb::IndexDescriptor *descriptor();
  void append(const uint8_t *data, size_t size);
};
template <typename NodeType> struct PersistentNode {};
template <typename NodeType> class PersistentSkipList {
private:
public:
  PersistentSkipList(std::string name);
  virtual ~PersistentSkipList();
};

} // namespace nvr

#endif /* PSLV2_H */
