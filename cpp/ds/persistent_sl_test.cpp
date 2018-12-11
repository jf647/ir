#include "cpp/utils/logger.hpp"
#include "psl_string.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdio.h>
using testing::ElementsAre;

typedef std::function<void(const Header *)> header_checks;

void check_headers(std::string filename,
                   std::initializer_list<header_checks> checks) {
  auto hmap = load_header(filename);
  auto header = flatbuffers::GetRoot<Header>(hmap.data());
  ASSERT_EQ(header->name()->str(), "persisted");
  ASSERT_TRUE(header->buffer_start() > 0);
  for (auto check : checks) {
    check(header);
  }
}

header_checks check_buffered_count(int count) {
  return [&count](const Header *h) { ASSERT_EQ(h->buffered_count(), count); };
}

TEST(TestPersistentSL, TestNewList) {
  remove("/tmp/psl");
  PersistentSkipList<Token> psl("/tmp/psl");
  check_headers("/tmp/psl", {});
  for (int i = 0; i < 100; i++) {
    psl += Token(std::to_string(i), {"hello" + std::to_string(i)});
  }
  check_headers("/tmp/psl", {check_buffered_count(100)});
  psl.refresh();
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
