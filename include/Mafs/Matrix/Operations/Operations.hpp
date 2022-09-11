#ifndef MAFS_MATRIXOPERATIONS_H
#define MAFS_MATRIXOPERATIONS_H

#include <Mafs/Matrix/MatrixBase.hpp>
#include <Mafs/Matrix/Operations/BasicOperations.hpp>
#include <Mafs/Matrix/Operations/CudaOperations.hpp>
#include <stdexcept>
#include <type_traits>

namespace Mafs::Internal {

/**
 * @brief Static class that contains the basic operations for the Matrix.
 * @todo: write this comment
 *
 */
static class MatrixOperations {
protected:
  enum { m_OpMode = MAFS_MATRIX_OPERATION_MODE };

  union {
    BasicMatrixOperations BasicOperations{};
    CudaMatrixOperations CudaOperations;
  };

  constexpr auto Operations() {
    if constexpr (AreEnumsEqual<m_OpMode, MtxOpCuda>())
      return CudaOperations;
    else
      return BasicOperations;
  }

public:
  MatrixOperations() = default;

  template <typename Derived, typename OtherDerived>
  auto Sum(const MatrixBase<Derived> &lMatrix, const MatrixBase<OtherDerived> &rMatrix) -> Derived {
    return Operations().Sum(lMatrix, rMatrix);
  }

  template <typename Derived, typename OtherDerived>
  bool Equals(const MatrixBase<Derived> &lMatrix, const MatrixBase<OtherDerived> &rMatrix) {
    return true;
  }
} MtxOperation; // Static instantiation of MtxOperation
};              // namespace Mafs::Internal

#endif // MAFS_MATRIXOPERATIONS_H
