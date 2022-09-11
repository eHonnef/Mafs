#ifndef MAFS_MATRIX_CUDA_OPERATIONS_H
#define MAFS_MATRIX_CUDA_OPERATIONS_H

#include <Mafs/Matrix/Operations/BaseOperations.hpp>
#define UNUSED(x) (void)(x) //@todo: remove

namespace Mafs::Internal {
class CudaMatrixOperations : BaseMatrixOperations {
public:
  CudaMatrixOperations() = default;

  template <typename Derived, typename OtherDerived>
  auto Sum(const MatrixBase<Derived> &lMatrix, const MatrixBase<OtherDerived> &rMatrix) -> Derived {
    //@todo: this function
    UNUSED(rMatrix);
    Derived MatrixRtn(lMatrix);
    return MatrixRtn;
  }
};
}; // namespace Mafs::Internal

#endif // MAFS_MATRIX_CUDA_OPERATIONS_H
