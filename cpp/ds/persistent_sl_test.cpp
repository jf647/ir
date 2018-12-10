#include "cpp/utils/logger.hpp"
#include "psl_string.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdio.h>
using testing::ElementsAre;

void check_headers(std::string filename) {
  auto hmap = load_header(filename);
  auto header = flatbuffers::GetRoot<Header>(hmap.data());
  ASSERT_EQ(header->name()->str(), "persisted");
  ASSERT_TRUE(header->buffer_start() > 0);
}
TEST(TestPersistentSL, TestNewList) {
  remove("/tmp/psl");
  PersistentSkipList<Token> psl("/tmp/psl");
  check_headers("/tmp/psl");
  for (int i = 0; i < 100; i++) {
    psl += Token(std::to_string(i), {"hello" + std::to_string(i)});
  }
  for (int i = 0; i < 100; i++) {
    auto docs = psl[std::to_string(i)];
    ASSERT_EQ(docs.size(), 1);
    ASSERT_THAT(docs, ElementsAre("hello" + std::to_string(i)));
  }
}

TEST(TestPersistentSL, TestAppendToken) {
  remove("/tmp/psl");
  int offset = 0;
  int size = 0;
  for (int i = 0; i < 100; i++) {
    int currOffset, currSize;
    append_token_to_buffer("/tmp/psl",
                           Token(std::to_string(i), {std::to_string(i)}),
                           currOffset, currSize);
    ASSERT_EQ(offset + size, currOffset);
    auto ro_map = mio::mmap_source("/tmp/psl");
    auto in =
        flatbuffers::GetSizePrefixedRoot<IndexNode>(ro_map.data() + currOffset);
    ASSERT_EQ(in->docs()->GetAsString(0)->str(), std::to_string(i));
    offset = currOffset;
    size = currSize;
  }
}
