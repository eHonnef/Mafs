#ifndef MAFS_MATRIX_CUDA_OPERATIONS_H
#define MAFS_MATRIX_CUDA_OPERATIONS_H

// #include <Mafs/Matrix/MatrixBase.hpp>
#include <type_traits>

namespace Mafs::Internal {
class CudaMatrixOperations {
public:
  // template <class T, class U>
  // auto Sum(const MatrixBase<T> &lMatrix, const MatrixBase<U> &rMatrix)
  //     -> MatrixBase<decltype(std::declval<T &>() + std::declval<U &>())>;
  // template <class T, class U> void InplaceSum(MatrixBase<T> &lMatrix, const MatrixBase<U> &rMatrix);
};

}; // namespace Mafs::Internal

#endif // MAFS_MATRIX_CUDA_OPERATIONS_H
