#include "eigen_nth.h"
#include <gtest/gtest.h>
TEST(NthTest, TestDenseNth) {
  Eigen::Matrix<int, 1, Eigen::Dynamic> m(1, 5);
  m << 10, 2, 5, 3, 40;
}
TEST(NthTest, TestSparseNth) {
  Eigen::SparseMatrix<double, Eigen::RowMajor> m(1, 15);
  m.insert(0, 1) = 1.0;
  m.insert(0, 2) = 2.0;
  m.insert(0, 4) = -1.0;
  m.insert(0, 9) = 10.0;
  m.insert(0, 12) = 5.0;
}
