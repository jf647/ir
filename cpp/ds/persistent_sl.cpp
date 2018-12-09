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

void create_file(std::string filename) {
  flatbuffers::FlatBufferBuilder builder(1024);
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
  int bufSize = builder.GetSize();
  std::ofstream fp;
  fp.open(filename, std::ios::out | std::ios::binary);
  fp.write((char *)builder.GetBufferPointer(), bufSize);
  fp.flush();
}

mio::ummap_sink load_header(std::string filename) {
  if (!exist_file(filename)) {
    create_file(filename);
  }
  return mio::ummap_sink(filename, 0, mio::map_entire_file);
}
