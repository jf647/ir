#ifndef EIGEN_MASK_H
#define EIGEN_MASK_H
#include <Eigen/SparseCore>
#include <iostream>
template <class MatrixType, class MaskType> class Masked;

namespace Eigen {
namespace internal {
template <class MatrixType, class MaskType>
struct traits<Masked<MatrixType, MaskType>> : traits<MatrixType> {
  typedef Eigen::Sparse StorageKind;
  typedef typename MatrixType::StorageIndex StorageIndex;
  enum { Flags = int(traits<MatrixType>::Flags) & (RowMajorBit) };
};
} // namespace internal
} // namespace Eigen

template <class MatrixType, class MaskType>
class Masked : public Eigen::SparseMatrixBase<Masked<MatrixType, MaskType>> {
  typedef Eigen::SparseMatrixBase<Masked> Base;
  typedef Eigen::Index Index;
  typedef
      typename Eigen::internal::ref_selector<MatrixType>::type MatrixTypeNested;
  typedef typename Eigen::internal::ref_selector<MaskType>::type MaskTypeNested;

public:
  EIGEN_SPARSE_PUBLIC_INTERFACE(Masked);
  typedef
      typename Eigen::internal::remove_all<MatrixType>::type NestedExpression;

  explicit Masked(const MatrixType &arg, const MaskType &mask)
      : m_matrix(arg), m_mask(mask) {
    EIGEN_STATIC_ASSERT_VECTOR_ONLY(MaskType);
    //    EIGEN_STATIC_ASSERT(MaskType::ColsAtCompileTime ==
    //                            MatrixType::ColsAtCompileTime,
    //                        THIS_METHOD_IS_ONLY_FOR_VECTORS_OF_A_SPECIFIC_SIZE);
    EIGEN_STATIC_ASSERT(
        (Eigen::internal::is_same<bool, typename MaskType::Scalar>::value),
        THE_MATRIX_OR_EXPRESSION_THAT_YOU_PASSED_DOES_NOT_HAVE_THE_EXPECTED_TYPE);
  }
  Index rows() const { return m_matrix.rows(); }
  Index cols() const { return m_matrix.cols(); }

  const typename Eigen::internal::remove_all<MatrixTypeNested>::type &
  nestedExpression() const {
    return m_matrix;
  }
  const typename Eigen::internal::remove_all<MaskTypeNested>::type &
  maskedExpression() const {
    return m_mask;
  }

protected:
  MatrixTypeNested m_matrix;
  MaskTypeNested m_mask;
};
namespace Eigen {
namespace internal {

// for dense - sparse op
template <typename MatrixType, typename MaskType>
struct unary_evaluator<Masked<MatrixType, MaskType>, IndexBased>
    : evaluator_base<Masked<MatrixType, MaskType>> {
  typedef Masked<MatrixType, MaskType> XprType;
  typedef typename nested_eval<MatrixType, XprType::ColsAtCompileTime>::type
      MatrixTypeNested;
  typedef typename remove_all<MatrixTypeNested>::type MatrixTypeNestedCleaned;

  typedef typename nested_eval<MaskType, XprType::ColsAtCompileTime>::type
      MaskTypeNested;
  typedef typename remove_all<MaskTypeNested>::type MaskTypeNestedCleaned;

  enum { IsRowMajor = (XprType::Flags & RowMajorBit) == RowMajorBit };
  typedef typename XprType::Scalar Scalar;
  typedef typename XprType::StorageIndex StorageIndex;

  explicit unary_evaluator(const XprType &xpr)
      : m_argImpl(xpr.nestedExpression()), m_maskImpl(xpr.maskedExpression()),
        m_view(xpr) {}
  enum {
    CoeffReadCost = evaluator<MatrixType>::CoeffReadCost,
    Flags = XprType::Flags
  };
  class InnerIterator {
  public:
    EIGEN_STRONG_INLINE InnerIterator(const unary_evaluator &sve, Index outer)
        : m_sve(sve), m_inner(0), m_outer(outer),
          m_end(sve.m_view.innerSize()) {
      incrementToNonZero();
    }

    EIGEN_STRONG_INLINE InnerIterator &operator++() {
      m_inner++;
      incrementToNonZero();
      return *this;
    }
    EIGEN_STRONG_INLINE Scalar value() const {
      return (IsRowMajor) ? m_sve.m_argImpl.coeff(m_outer, m_inner)
                          : m_sve.m_argImpl.coeff(m_inner, m_outer);
    }

    EIGEN_STRONG_INLINE StorageIndex index() const { return m_inner; }
    inline Index row() const { return IsRowMajor ? m_outer : index(); }
    inline Index col() const { return IsRowMajor ? index() : m_outer; }
    EIGEN_STRONG_INLINE operator bool() const {
      return m_inner < m_end && m_inner >= 0;
    }

  protected:
    const unary_evaluator &m_sve;
    Index m_inner;
    const Index m_outer;
    const Index m_end;

  private:
    void incrementToNonZero() {
      while ((bool(*this)) && !m_sve.m_maskImpl.coeff(0, col())) {
        m_inner++;
      }
    }
  };

protected:
  evaluator<MatrixTypeNestedCleaned> m_argImpl;
  evaluator<MaskTypeNestedCleaned> m_maskImpl;
  const XprType &m_view;
};

// For sparse - sparse operation.
template <typename MatrixType, typename MaskType>
struct unary_evaluator<Masked<MatrixType, MaskType>, IteratorBased>
    : evaluator_base<Masked<MatrixType, MaskType>> {

  typedef Masked<MatrixType, MaskType> XprType;
  typedef typename nested_eval<MatrixType, XprType::ColsAtCompileTime>::type
      MatrixTypeNested;
  typedef typename remove_all<MatrixTypeNested>::type MatrixTypeNestedCleaned;

  typedef typename nested_eval<MaskType, XprType::ColsAtCompileTime>::type
      MaskTypeNested;
  typedef typename remove_all<MaskTypeNested>::type MaskTypeNestedCleaned;
  typedef
      typename evaluator<MatrixTypeNestedCleaned>::InnerIterator EvalIterator;

  explicit unary_evaluator(const XprType &xpr)
      : m_argImpl(xpr.nestedExpression()), m_maskImpl(xpr.maskedExpression()) {}

  enum {
    CoeffReadCost = evaluator<MatrixType>::CoeffReadCost,
    Flags = XprType::Flags
  };
  class InnerIterator : public EvalIterator {
    typedef typename XprType::Scalar Scalar;

  public:
    EIGEN_STRONG_INLINE InnerIterator(const unary_evaluator &sve, Index outer)
        : EvalIterator(sve.m_argImpl, outer), m_sve(sve) {
      incrementToNonZero();
    }

    EIGEN_STRONG_INLINE InnerIterator &operator++() {
      EvalIterator::operator++();
      incrementToNonZero();
      return *this;
    }

    using EvalIterator::value;

  protected:
    const unary_evaluator &m_sve;

  private:
    void incrementToNonZero() {
      while ((bool(*this)) && !m_sve.m_maskImpl.coeff(0, EvalIterator::col())) {
        EvalIterator::operator++();
      }
    }
  };

  typedef typename XprType::CoeffReturnType CoeffReturnType;

  // evaluator(const XprType &xpr)
  //    : m_argImpl(xpr.m_arg), m_maskImpl(xpr.m_mask) {}
  // CoeffReturnType coeff(Index row, Index col) const {
  //  if (m_maskImpl.coeff(1, col))
  //    return m_argImpl.coeff(row, col);
  //  return 0;
  //}

protected:
  evaluator<MatrixTypeNestedCleaned> m_argImpl;
  evaluator<MaskTypeNestedCleaned> m_maskImpl;
};
} // namespace internal
} // namespace Eigen
template <class MatrixType, class MaskType>
Masked<MatrixType, MaskType> mask(const Eigen::MatrixBase<MatrixType> &arg,
                                  const Eigen::MatrixBase<MaskType> &mask) {
  return Masked<MatrixType, MaskType>(arg.derived(), mask.derived());
}
template <class MatrixType, class MaskType>
Masked<MatrixType, MaskType>
mask(const Eigen::SparseMatrixBase<MatrixType> &arg,
     const Eigen::MatrixBase<MaskType> &mask) {
  return Masked<MatrixType, MaskType>(arg.derived(), mask.derived());
}
#endif // EIGEN_MASK_H
