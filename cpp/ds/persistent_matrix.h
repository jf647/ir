#ifndef PERSISTENT_MATRIX_H
#define PERSISTENT_MATRIX_H
#include <Eigen/Dense>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class StoreException : public std::exception {
  std::string message;

public:
  StoreException(std::string message) : message(message) {
    std::cout << "message" << message;
  }
  ~StoreException() = default;
  virtual const char *what() const throw() override {
    auto s = "Exeption in opening store" + message;
    return s.c_str();
  }
};

enum order { ROW, COL };
class PersistentMatrixBase {
  struct Header;
  struct Store;
  std::string path;
  std::unique_ptr<Header> header;
  std::unique_ptr<Store> store;

public:
  PersistentMatrixBase(std::string path);
  PersistentMatrixBase(PersistentMatrixBase &&other);
  ~PersistentMatrixBase();
  static PersistentMatrixBase create(std::string name, std::string path,
                                     int primarySize, order o);
  double *data();
  void clean();
  int secondarySize();
  int primarySize();
  int write(std::vector<double> vec);
};

template <typename MatrixType>
struct PersistentMatrix : public PersistentMatrixBase {
  PersistentMatrix(std::string path);
  const Eigen::Map<MatrixType> *get();
  void operator+=(std::vector<double> rhs);
};

template <typename MatrixType>
PersistentMatrix<MatrixType> create_matrix(std::string name, std::string path,
                                           int primarySize) {
  PersistentMatrixBase::create(name, path, primarySize,
                               PersistentMatrix<MatrixType>::IsRowMajor ? ROW
                                                                        : COL);
  return PersistentMatrix<MatrixType>(path);
}

template <>
struct PersistentMatrix<Eigen::MatrixXd> : public PersistentMatrixBase {
  enum { IsRowMajor = false };
  Eigen::Map<Eigen::MatrixXd> *md;

  PersistentMatrix(std::string path) : PersistentMatrixBase(path) {
    md =
        new Eigen::Map<Eigen::MatrixXd>(data(), primarySize(), secondarySize());
  }
  const Eigen::Map<Eigen::MatrixXd> *get() {
    new (md)
        Eigen::Map<Eigen::MatrixXd>(data(), primarySize(), secondarySize());
    clean();
    return md;
  }
  void operator+=(std::vector<double> rhs) { write(rhs); }
};

#endif /* PERSISTENT_MATRIX_H */
