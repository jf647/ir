#ifndef PERSISTENT_MATRIX_H
#define PERSISTENT_MATRIX_H
#include <Eigen/Dense>
#include <string>

struct PersistentMatrixBase {
  struct Header {
    std::string name;
    enum order { ROW, COL } mOrder;
    int primarySize, secondarySize, buffered;
  };
  std::string path;
  Header header;

  PersistentMatrixBase(std::string path);
  int rows();
  int cols();
};
template <typename MatrixType>
struct PersistentMatrix : public PersistentMatrixBase {
  const Eigen::Map<MatrixType> &operator()();
  void operator+=(const MatrixType &rhs);
};

#endif /* PERSISTENT_MATRIX_H */
