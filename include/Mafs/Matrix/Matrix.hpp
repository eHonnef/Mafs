#ifndef MAFS_MATRIX_H
#define MAFS_MATRIX_H

#include <Mafs/Matrix/MatrixBase.hpp>
#include <string>

namespace Mafs {
template <typename T, size_t Rows_, size_t Cols_, size_t Options_>
class Matrix : public Internal::MatrixBase<Matrix<T, Rows_, Cols_, Options_>> {
public:
  Matrix() : Internal::MatrixBase<Matrix<T, Rows_, Cols_, Options_>>() {}

  Matrix(size_t nRows, size_t nCols)
      : Internal::MatrixBase<Matrix<T, Rows_, Cols_, Options_>>(nRows, nCols) {}
};

template <typename T, size_t Rows_, size_t Cols_, size_t Options_>
struct Internal::MatrixTraits<Matrix<T, Rows_, Cols_, Options_>> {
public:
  typedef T Type;
  enum { Rows = Rows_, Cols = Cols_, Options = Options_ };
};
}; // namespace Mafs

#endif // MAFS_MATRIX_H
