#include "eigen_mask.h"
#include <gtest/gtest.h>
TEST(MaskedTest, TestDenseMask) {
  Eigen::Matrix<bool, 1, Eigen::Dynamic> m(1, 3);
  m << true, false, true;
  Eigen::MatrixXd mat(2, 3);
  mat << 1.0, 2.0, 3.0, 5.0, 6, 7;
  Eigen::SparseMatrix<double> res = mask(mat, m);
  ASSERT_EQ(res.nonZeros(), 4);
  Eigen::Matrix<bool, 1, Eigen::Dynamic> m2(1, 2);
  m2 << true, false;
  Eigen::SparseMatrix<double> res2;
  res2 = mask(res.transpose(), m2);
  ASSERT_EQ(res2.nonZeros(), 2);
}
