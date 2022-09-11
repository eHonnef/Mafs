#ifndef MAFS_MATRIX_BASE_OPERATIONS_H
#define MAFS_MATRIX_BASE_OPERATIONS_H

#include <Mafs/Matrix/MatrixBase.hpp>

namespace Mafs::Internal {

/**
 * @brief "Base" class for the operations.
 * Its purpose is to document the functions and to provide a base on what is suppose to be
 * implemented.
 *
 */
class BaseMatrixOperations {
public:
  template <typename Derived, typename OtherDerived>
  auto Sum(const MatrixBase<Derived> &lMatrix, const MatrixBase<OtherDerived> &rMatrix) -> Derived;

  template <typename Derived, typename OtherDerived>
  auto InplaceSum(MatrixBase<Derived> &lMatrix, const MatrixBase<OtherDerived> &rMatrix) -> void;

  template <typename Derived, typename OtherDerived>
  auto Subtraction(const MatrixBase<Derived> &lMatrix, const MatrixBase<OtherDerived> &rMatrix)
      -> Derived;

  template <typename Derived, typename OtherDerived>
  auto InplaceSubtraction(MatrixBase<Derived> &lMatrix, const MatrixBase<OtherDerived> &rMatrix)
      -> void;

  template <typename Derived, typename OtherDerived>
  auto Multiplication(const MatrixBase<Derived> &lMatrix, const MatrixBase<OtherDerived> &rMatrix)
      -> Derived;

  template <typename Derived, typename OtherDerived>
  auto InplaceMultiplication(MatrixBase<Derived> &lMatrix, const MatrixBase<OtherDerived> &rMatrix)
      -> void;

  template <typename Derived, typename ScalarType>
  auto ScalarMultiplication(const MatrixBase<Derived> &Matrix, const ScalarType &Scalar) -> Derived;

  template <typename Derived, typename ScalarType>
  auto InplaceScalarMultiplication(MatrixBase<Derived> &Matrix, const ScalarType &Scalar) -> void;

  template <typename Derived> auto Transpose(const MatrixBase<Derived> &Matrix) -> Derived;

  template <typename Derived> auto InplaceTranspose(MatrixBase<Derived> &Matrix) -> void;

  template <typename Derived, typename OtherDerived>
  auto Equals(const MatrixBase<Derived> &lMatrix, const MatrixBase<OtherDerived> &rMatrix) -> bool;

  template <typename Derived, typename ScalarType>
  auto ScalarDivision(const MatrixBase<Derived> &Matrix, const ScalarType &Scalar) -> Derived;

  template <typename Derived, typename ScalarType>
  auto InplaceScalarDivision(MatrixBase<Derived> &Matrix, const ScalarType &Scalar) -> void;
};
}; // namespace Mafs::Internal

#endif // MAFS_MATRIX_BASE_OPERATIONS_H
