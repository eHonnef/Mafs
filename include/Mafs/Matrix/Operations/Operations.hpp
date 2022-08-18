#ifndef MATRIXOPERATIONS_H
#define MATRIXOPERATIONS_H

// #include <Mafs/Matrix/MatrixBase.hpp>
// #include <Mafs/Matrix/Operations/BasicOperations.hpp>
// #include <Mafs/Matrix/Operations/CudaOperations.hpp>
// #include <type_traits>

// /* Enum MtxOpMode
//  * Contains the operations mode. Eg.: use cuda, parallel(openmp), basic...
//  * Basically, list every class above.
//  */
// namespace Mafs {
// enum MtxOpMode { MtxOpBasic, MtxOpCuda };

// namespace Internal {
// template <MtxOpMode _MtxOpMode> class MatrixOperations {
// protected:
//   enum { m_OpMode = _MtxOpMode };

//   union uOperations {
//     BasiMatrixOperations OpBasic;
//     CudaMatrixOperations OpCuda;
//   };

// public:
//   template <class T, class U>
//   auto Sum(const MatrixBase<T> &lMatrix, const MatrixBase<U> &rMatrix)
//       -> MatrixBase<decltype(std::declval<T &>() + std::declval<U &>())> {
//     switch (m_OpMode) {
//     case MtxOpBasic:
//       return uOperations::OpBasic.Sum(lMatrix, rMatrix);
//     case MtxOpCuda:
//       return uOperations::OpCuda.Sum(lMatrix, rMatrix);
//     default:
//       break;
//     }
//   }
// };
// }; // namespace Internal
// }; // namespace Mafs

#endif // MATRIXOPERATIONS_H
