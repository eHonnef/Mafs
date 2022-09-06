#ifndef MAFS_MATRIX_BASIC_OPERATIONS_H
#define MAFS_MATRIX_BASIC_OPERATIONS_H

// #include <Mafs/Matrix/MatrixBase.hpp>
#include <type_traits>

namespace Mafs::Internal {
class BasicMatrixOperations {
public:
  // template <class T, class U>
  // auto Sum(const MatrixBase<T> &lMatrix, const MatrixBase<U> &rMatrix)
  //     -> MatrixBase<decltype(std::declval<T &>() + std::declval<U &>())>;
  // {
  //   rtnMatx = lMatrix;
  //   InplaceSum(rtnMatx, rMatrix);
  //   return rtnMatx;
  // }

  // template <class T, class U> void InplaceSum(MatrixBase<T> &lMatrix, const MatrixBase<U> &rMatrix);

  // transpose
  // determinant
  // inverse
  // multiply
  // equals
  // subtract
  // divide
};
}; // namespace Mafs::Internal

#endif // MAFS_MATRIX_BASIC_OPERATIONS_H
