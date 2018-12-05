#include "cpp/utils/logger.hpp"
#include "persistent_sl.h"
#include <fstream>
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
  fp.write((char *)&bufSize, sizeof(bufSize));
  fp.write((char *)builder.GetBufferPointer(), bufSize);
  fp.flush();
  // IndexBuilder indexBuilder(builder);
  // indexBuilder.add_header(header);
  // auto offset = indexBuilder.Finish();
  // builder.Finish(offset);
  // uint8_t *buf = builder.GetBufferPointer();
  // auto bufSize = builder.GetSize();
}

mio::mmap_sink load_header(std::string filename) {
  if (!exist_file(filename)) {
    create_file(filename);
  }
  std::error_code error;
  auto rw_map = mio::make_mmap_sink(filename, 0, mio::map_entire_file, error);
  if (error) {
    BOOST_LOG_TRIVIAL(error) << error.message();
  }
  return rw_map;
}

void add_to_buffer(std::string filename, const Token &token) {
  flatbuffers::FlatBufferBuilder builder(1024);
  auto k = builder.CreateString(token.key());
  auto sk = CreateStringKey(builder, k);
  auto docs = builder.CreateVectorOfStrings(token.value());
  IndexNodeBuilder inb(builder);
  inb.add_key_type(Key_StringKey);
  inb.add_key(sk.Union());
  inb.add_docs(docs);
  auto inbOffset = inb.Finish();
  builder.Finish(inbOffset);
  auto bufSize = builder.GetSize();
  std::error_code error;
  std::ofstream fp;
  fp.open(filename, std::ios::ate | std::ios::binary);
  fp.write((char *)&bufSize, sizeof(bufSize));
  fp.write((char *)builder.GetBufferPointer(), bufSize);
  fp.flush();
}
