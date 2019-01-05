#include "cpp/ds/ann/mrpt.h"
#include "persistent_matrix.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdio.h>
#include <thread>
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
    ASSERT_EQ(md->rows(), 5);
    ASSERT_EQ(md->cols(), i + 1);
  }
}

void perf(std::string train, std::string test, int dim, int k);
TEST(NonPersistentM, Perf) {
  perf("data/mnist/train.bin", "data/mnist/test.bin", 784, 10);
}

void perf_persistent(std::string train, std::string test, int dim, int k);
TEST(TestPersistentMatrix, Perf) {
  perf_persistent("data/mnist/train.bin", "data/mnist/test.bin", 784, 10);
}

void perf_persistent_load(std::string train, std::string test, int dim, int k);
TEST(TestPersistentMatrix, PerfLoadIndex) {
  perf_persistent_load("data/perf-test", "data/mnist/test.bin", 784, 10);
}

TEST(TestPersistentMatrix, SparseFileConcurrentWrite) {
  auto pm = create_matrix<Eigen::MatrixXd>("test", "/tmp/psm/sparsefile", 5);
  std::thread t[100];
  for (int i = 0; i < 100; i++) {
    t[i] = std::thread([&pm]() { pm += {1, 2, 3, 4, 5}; });
  }
  for (int i = 0; i < 100; i++) {
    t[i].join();
  }
  ASSERT_EQ(pm.secondarySize(), 100);
}

TEST(TestPersistentMatrix, Refresh) {
  auto pm = create_matrix<Eigen::MatrixXd>("test", "/tmp/psm/refresh", 5);
  auto mat = pm.get();
  ASSERT_EQ(mat->cols(), 0);
  pm += {1, 2, 3, 4, 5};
  auto mat2 = pm.get();
  ASSERT_EQ(mat->cols(), 1);
}

double *get_data(const char *file, int dim, int *n) {
  struct stat sb;
  stat(file, &sb);
  int N = sb.st_size / (sizeof(float) * dim);
  *n = N;

  float *data = new float[N * dim];

  FILE *fd;
  fd = fopen(file, "rb");
  fread(data, sizeof(float), N * dim, fd);
  fclose(fd);
  double *ddata = new double[N * dim];
  for (int i = 0; i < N * dim; i++)
    ddata[i] = (double)data[i];

  return ddata;
}

void perf(std::string train, std::string test, int dim, int k) {
  int n, n_test;
  double *train_data = get_data(train.c_str(), dim, &n);
  double *test_data = get_data(test.c_str(), dim, &n_test);
  const Map<Eigen::MatrixXd> *X =
      new Eigen::Map<Eigen::MatrixXd>(train_data, dim, n);
  Mrpt index(X, 0, 0, 1.);

  VectorXi idx(n);
  std::iota(idx.data(), idx.data() + n, 0);

  // omp_set_num_threads(1);
  for (int i = 0; i < n_test; ++i) {
    // double start = omp_get_wtime();
    auto result =
        index.exact_knn(Map<VectorXd>(&test_data[i * dim], dim), k, idx, n);
    // double end = omp_get_wtime();
    // printf("%g\n", end - start);
  }

  delete[] train_data;
  delete[] test_data;
  delete X;
}
std::vector<std::vector<double>> get_data_vectors(const char *file, int dim,
                                                  int *n) {
  struct stat sb;
  stat(file, &sb);
  int N = sb.st_size / (sizeof(float) * dim);
  *n = N;
  float *data = new float[N * dim];
  FILE *fd;
  fd = fopen(file, "rb");
  fread(data, sizeof(float), N * dim, fd);
  fclose(fd);
  std::vector<std::vector<double>> res(N);
  for (int i = 0; i < N; i++) {
    res[i] = std::vector<double>(dim);
    for (int j = 0; j < dim; j++) {
      res[i][j] = (double)data[i * dim + j];
    }
  }
  return res;
}
void perf_persistent(std::string train, std::string test, int dim, int k) {
  int n, n_test;
  std::vector<std::vector<double>> train_data =
      get_data_vectors(train.c_str(), dim, &n);
  std::vector<std::vector<double>> test_data =
      get_data_vectors(test.c_str(), dim, &n_test);

  auto pm = create_matrix<Eigen::MatrixXd>("test", "/tmp/psm/perf", dim);
  for (auto v : train_data)
    pm += v;
  Mrpt index(pm.get(), 0, 0, 1.);

  VectorXi idx(n);
  std::iota(idx.data(), idx.data() + n, 0);

  // omp_set_num_threads(1);
  for (int i = 0; i < n_test; ++i) {
    // double start = omp_get_wtime();

    auto result =
        index.exact_knn(Map<VectorXd>(test_data[i].data(), dim), k, idx, n);
    // double end = omp_get_wtime();
    // printf("%g\n", end - start);
  }
}
void perf_persistent_load(std::string train, std::string test, int dim, int k) {
  int n_test;
  auto pm = create_matrix<Eigen::MatrixXd>("test", train, dim);
  Mrpt index(pm.get(), 0, 0, 1.);

  int n = pm.secondarySize();
  VectorXi idx(n);
  std::iota(idx.data(), idx.data() + n, 0);

  std::vector<std::vector<double>> test_data =
      get_data_vectors(test.c_str(), dim, &n_test);

  // omp_set_num_threads(1);
  for (int i = 0; i < n_test; ++i) {
    // double start = omp_get_wtime();

    auto result =
        index.exact_knn(Map<VectorXd>(test_data[i].data(), dim), k, idx, n);
    // double end = omp_get_wtime();
    // printf("%g\n", end - start);
  }
}
