#include "persistent_matrix.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdio.h>
using testing::ElementsAre;

TEST(TestPersistentMatrixBase, EmptyBinding) {
  ASSERT_THROW(PersistentMatrixBase binding("/tmp/psm/c"), StoreException);
}

TEST(TestPersistentMatrixBase, CreateBinding) {
  auto pmb = PersistentMatrixBase::create("test", "/tmp/psm/b", 5, ROW);
  ASSERT_EQ(pmb.primarySize(), 5);
  ASSERT_EQ(pmb.secondarySize(), 0);
  {
    auto v = std::vector<double>{1.0, 2.0, 1.0, 3.14, 4.0};
    auto offset = pmb.write(v);
    ASSERT_EQ(pmb.secondarySize(), 1);
    auto ms = pmb.primarySize() * pmb.secondarySize();
    auto data = pmb.data();
    std::vector<double> res(data, data + ms);
    ASSERT_THAT(res, ElementsAre(1.0, 2.0, 1.0, 3.14, 4.0));
  }
  {
    auto v = std::vector<double>{1.0, 2.0, 1.0, 3.14, 4.0};
    auto offset = pmb.write(v);
    v = std::vector<double>{1.0, 2.0, 1.0, 3.14, 4.0};
    offset = pmb.write(v);
    ASSERT_EQ(pmb.secondarySize(), 3);
    auto ms = pmb.primarySize() * pmb.secondarySize();
    auto data = pmb.data();
    std::vector<double> res(data, data + ms);
    ASSERT_THAT(
        res, testing::ElementsAreArray({1.0, 2.0, 1.0, 3.14, 4.0, 1.0, 2.0, 1.0,
                                        3.14, 4.0, 1.0, 2.0, 1.0, 3.14, 4.0}));
  }
}
TEST(TestPersistentMatrix, CreateBinding) {
  auto pm = create_matrix<Eigen::MatrixXd>("test", "/tmp/psm/m", 5);
  for (int i = 0; i < 3; i++) {
    auto v = std::vector<double>{1.0, 2.0, 1.0, 3.14, 4.0};
    pm += v;
    auto md = pm.get();
    ASSERT_EQ(md.rows(), 5);
    ASSERT_EQ(md.cols(), i + 1);
  }
}
