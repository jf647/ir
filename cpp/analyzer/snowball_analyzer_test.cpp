#include "snowball_analyzer.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using std::cout;
using testing::ElementsAre;

TEST(TestAnalyzer, SnowballAnalyzerEnglish) {
  SnowballAnalyzer a;
  auto tokens = a.tokens("hello snowball");
  ASSERT_EQ(2, tokens.size());
  ASSERT_THAT(tokens, ElementsAre("hello", "snowbal"));
}
