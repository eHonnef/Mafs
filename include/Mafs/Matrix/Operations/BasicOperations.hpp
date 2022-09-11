#ifndef MAFS_MATRIX_BASIC_OPERATIONS_H
#define MAFS_MATRIX_BASIC_OPERATIONS_H

#include <Mafs/Matrix/Operations/BaseOperations.hpp>

namespace Mafs::Internal {
class BasicMatrixOperations : BaseMatrixOperations {
public:
  BasicMatrixOperations() = default;

  template <typename Derived, typename OtherDerived>
  auto Sum(const MatrixBase<Derived> &lMatrix, const MatrixBase<OtherDerived> &rMatrix) -> Derived {

    //@todo: make a static condition if both matrices are static
    if (lMatrix.RowCount() != rMatrix.RowCount() || lMatrix.ColCount() != rMatrix.ColCount())
      throw std::domain_error(fmt::format(
          "RowCount and ColCount must be equal. lMatrix[{}][{}] / rMatrix[{}][{}]",
          lMatrix.RowCount(), lMatrix.ColCount(), rMatrix.RowCount(), rMatrix.ColCount()));

    Derived MatrixRtn(lMatrix);
    for (size_t i = 0; i < rMatrix.RowCount(); ++i)
      for (size_t j = 0; j < rMatrix.ColCount(); ++j)
        MatrixRtn(i, j) += rMatrix(i, j);

    return MatrixRtn;
  }
};
}; // namespace Mafs::Internal

#endif // MAFS_MATRIX_BASIC_OPERATIONS_H
