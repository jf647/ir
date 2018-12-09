#include "cpp/utils/logger.hpp"
#include "psl_string.h"
#include <gtest/gtest.h>
TEST(TestPersistentSL, TestNewList) {
  std::cout << "hello";
  BOOST_LOG_TRIVIAL(info) << "creating";
  PersistentSkipList<Token> psl("/tmp/psl");
}
