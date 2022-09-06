#ifndef MAFS_MATRIXOPERATIONS_H
#define MAFS_MATRIXOPERATIONS_H

#include <Mafs/Matrix/MatrixBase.hpp>
#include <Mafs/Matrix/Operations/BasicOperations.hpp>
#include <Mafs/Matrix/Operations/CudaOperations.hpp>
#include <stdexcept>
#include <type_traits>

namespace Mafs {

/* Enum MtxOpMode
 * Contains the operations mode. Eg.: use cuda, parallel(openmp), basic...
 * Basically, list every class above.
 */
enum MtxOperationMode {
  // Matrix operations mode
  MtxOpBasic = 0,
  MtxOpCuda = 1
};

#ifndef MAFS_MATRIX_OPERATION_MODE
#define MAFS_MATRIX_OPERATION_MODE MtxOpBasic
#endif

namespace Internal {
template <size_t OperationMode_> class MatrixOperations {
protected:
  enum { m_OpMode = OperationMode_ };

  union uOperations {
    BasicMatrixOperations OpBasic;
    CudaMatrixOperations OpCuda;
  };

public:
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

  // template <typename Derived, typename OtherDerived>
  // bool Equals(const MatrixBase<Derived> &lMatrix, const MatrixBase<OtherDerived> &rMatrix) {
  //   return true;
  // }

  // template <class T, class U>
  // auto Sum(const MatrixBase<T> &lMatrix, const MatrixBase<U> &rMatrix)
  //     -> MatrixBase<decltype(std::declval<T &>() + std::declval<U &>())> {
  //   // switch (m_OpMode) {
  //   // case MtxOpBasic:
  //   //   return uOperations::OpBasic.Sum(lMatrix, rMatrix);
  //   // case MtxOpCuda:
  //   //   return uOperations::OpCuda.Sum(lMatrix, rMatrix);
  //   // default:
  //   //   break;
  //   // }
  // }
};

// extern MatrixOperations<MAFS_MATRIX_OPERATION_MODE> operationk;

}; // namespace Internal
}; // namespace Mafs

#endif // MAFS_MATRIXOPERATIONS_H
