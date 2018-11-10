#ifndef EIGEN_NTH_H
#define EIGEN_NTH_H
#include <Eigen/SparseCore>
#include <algorithm>
#include <iostream>
#include <numeric>

template <class MatrixType>
typename Eigen::MatrixBase<MatrixType>::Scalar
nth_element(const Eigen::MatrixBase<MatrixType> &arg, int n) {
  std::vector<int> idx(arg.cols());
  std::iota(idx.begin(), idx.end(), 0);
  std::nth_element(idx.begin(), idx.begin() + n, idx.end(),
                   [&arg](typename Eigen::MatrixBase<MatrixType>::Scalar i,
                          typename Eigen::MatrixBase<MatrixType>::Scalar j) {
                     return arg.coeff(0, i) < arg.coeff(0, j);
                   });
  return arg.coeff(0, idx[n]);
}

double nth_element(const Eigen::SparseMatrix<double, Eigen::RowMajor> &arg,
                   int n) {
  // typedef typename MatrixType::Scalar Scalar;

  std::vector<double> idx;
  for (int k = 0; k < arg.outerSize(); ++k) {
    // for (typename MatrixType::InnerIterator it(arg, k); it; ++it) {
    for (Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator it(arg, k);
         it; ++it) {
      idx.push_back(it.value());
    }
    break;
  }

  std::nth_element(idx.begin(), idx.begin() + n, idx.end());
  return idx[n];
}
#endif // EIGEN_NTH_H
