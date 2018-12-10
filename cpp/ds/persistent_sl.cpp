#include "cpp/utils/logger.hpp"
#include "persistent_sl.h"
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <system_error> // for std::error_code
bool exist_file(std::string filename) {
  struct stat buffer;
  return stat(filename.c_str(), &buffer) == 0;
}

void create_file(std::string filename, int &headerSize) {
  flatbuffers::FlatBufferBuilder builder(1024);
  builder.ForceDefaults(true);
  auto name = builder.CreateString("persisted");
  auto version = 0;
  auto count = 0;
  auto bufferedCount = 0;
  auto listStart = 0;
  auto listEnd = 0;
  auto bufferStart = 0;
  auto bufferEnd = 0;

  auto header = CreateHeader(builder, name, version, count, bufferedCount,
                             listStart, listEnd, bufferStart, bufferEnd);
  builder.Finish(header);
  headerSize = builder.GetSize();
  std::ofstream fp;
  fp.open(filename, std::ios::out | std::ios::binary);
  fp.write((char *)builder.GetBufferPointer(), headerSize);
  fp.flush();
}

mio::ummap_sink load_header(std::string filename) {
  if (!exist_file(filename)) {
    int headerSize;
    create_file(filename, headerSize);
    auto hmap = mio::ummap_sink(filename);
    auto header = flatbuffers::GetMutableRoot<Header>(hmap.data());
    header->mutate_buffer_start(headerSize);
    header->mutate_list_start(headerSize);
    header->mutate_buffer_end(headerSize);
    header->mutate_list_end(headerSize);
    // std::error_code error_code;
    // hmap.sync(error_code);
    return hmap;
  }
  return mio::ummap_sink(filename);
}
