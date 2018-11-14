#ifndef VECTOR_INDEX_H
#define VECTOR_INDEX_H

#include "Eigen/Dense"
#include "Eigen/Sparse"
#include "cpp/ceigen/eigen_mask.h"
#include "cpp/field/vector_field.h"
#include "index.h"

#include "cpp/index/ann/mrpt.h"

using Eigen::Map;
using Eigen::MatrixXd;
using Eigen::SparseMatrix;
using Eigen::SparseVector;
using Eigen::VectorXd;

typedef Eigen::Matrix<bool, 1, -1> VectorXb;

using std::string;
using std::vector;
class BaseVectorIndex : public Index {

protected:
  MatrixXd _docs;
  int currDoc;

private:
  void growIf(vector<int> docs) {
    int newCols = _docs.cols();
    int oldRows = _docs.rows();

    auto maxDocId = *(std::max_element(docs.begin(), docs.end()));
    if (maxDocId >= _docs.rows()) {
      int newRows = oldRows * 2;
      _docs.conservativeResize(newRows, newCols);
      _docs.block(oldRows, 0, newRows - oldRows, _docs.cols()).setZero();
    }
    currDoc = maxDocId + 1;
  }

public:
  BaseVectorIndex(string fieldName, int vecLength)
      : Index(fieldName), _docs(10, vecLength), currDoc(0) {
    _docs.setZero();
  }
  virtual ~BaseVectorIndex() {}
  void store(shared_ptr<Field> field) override {
    auto iField = static_pointer_cast<VectorField>(field);
    growIf(iField->docs());
    VectorXd v = VectorXd::Map(iField->vec().data(), _docs.cols());
    for (int docId : iField->docs()) {
      _docs.row(docId) = v;
    }
  }
  virtual void sortBy(DocCollector &collector, vector<double> query, int k) = 0;
};
class ApproximateVectorIndex : public BaseVectorIndex {

private:
  /* data */
  class InnerIndex {
  private:
    MatrixXd innerDocs;
    int numTrees, depth;
    float density;
    shared_ptr<Mrpt> _index;

  public:
    InnerIndex(const ApproximateVectorIndex &enclose, int numTrees, int depth,
               float density)
        : innerDocs(enclose.effective()), numTrees(numTrees), depth(depth),
          density(density) {}
    void build() {
      Map<MatrixXd> *innerDocsMap =
          new Map<MatrixXd>(&innerDocs(0), innerDocs.rows(), innerDocs.cols());
      _index = make_shared<Mrpt>(innerDocsMap, numTrees, depth, density);
      _index->grow();
    }
    vector<int> query(vector<double> q, int k) {
      return _index->query(Map<VectorXd>(q.data(), q.size(), Eigen::RowMajor),
                           k, 1, 2);
    }
  };
  InnerIndex innerIndex;
  int numTrees, depth;
  float density;
  MatrixXd effective() const {
    auto tm = _docs.block(0, 0, currDoc, _docs.cols());
    auto b = tm.transpose();
    return b;
  }

public:
  ApproximateVectorIndex(string fieldName, int vecLength, int numTrees,
                         int depth, float density)
      : BaseVectorIndex(fieldName, vecLength),
        innerIndex(*this, numTrees, depth, density), numTrees(numTrees),
        depth(depth), density(density) {}
  void refresh() override {
    innerIndex = InnerIndex(*this, numTrees, depth, density);
    innerIndex.build();
  }
  void sortBy(DocCollector &collector, vector<double> query, int k) override {
    collector.collect(innerIndex.query(query, k));
  }
  virtual ~ApproximateVectorIndex() {}
};

class VectorIndex : public BaseVectorIndex {
private:
public:
  VectorIndex(string fieldName, int vecLength)
      : BaseVectorIndex(fieldName, vecLength){};
  void sortBy(DocCollector &collector, vector<double> query, int k) override {
    VectorXd v = VectorXd::Map(query.data(), _docs.cols());
    MatrixXd m = v.asDiagonal();
    auto tm = _docs.block(0, 0, currDoc, _docs.cols());

    vector<int> maskDocs = collector.elligible();
    if (maskDocs.size() == 0) {
      VectorXd res = (tm * m).rowwise().sum();
      vector<int> ids(currDoc);
      vector<double> scores(currDoc);
      std::iota(ids.begin(), ids.end(), 0);
      VectorXd::Map(&scores[0], res.size()) = res;
      collector.collect(ids, scores);
      return;
    }
    VectorXb maskVec(tm.rows());
    for (int doc : maskDocs) {
      maskVec.coeffRef(0, doc) = true;
    }
    SparseMatrix<double> masked = mask(tm.transpose(), maskVec).transpose();
    SparseMatrix<double> queryDiagonal = m.sparseView();
    SparseMatrix<double> dotRes = masked * queryDiagonal;
    SparseVector<double> scores(_docs.rows());
    for (int k = 0; k < dotRes.outerSize(); ++k)
      for (Eigen::SparseMatrix<double>::InnerIterator it(dotRes, k); it; ++it) {
        scores.coeffRef(it.row(), 0) += it.value();
      }
    vector<int> docs(scores.nonZeros());
    vector<double> scoresVec(scores.nonZeros());
    int i = 0;
    for (Eigen::SparseVector<double>::InnerIterator it(scores); it; ++it) {
      docs[i] = it.row();
      scoresVec[i] = it.value();
      ++i;
    }
    collector.collect(docs, scoresVec);
  }

  virtual ~VectorIndex() {}
};

#endif /* VECTOR_INDEX_H */
