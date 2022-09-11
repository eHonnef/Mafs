#ifndef MAFS_MATRIX_DATA_TYPES_H
#define MAFS_MATRIX_DATA_TYPES_H

namespace Mafs {
/**
 * @brief Enum MtxOpMode
 * Contains the operations mode. Eg.: use cuda, parallel(openmp), basic...
 * Basically, list every class above.
 */
enum MtxOperationMode {
  // Matrix operations mode
  MtxOpBasic = 0,
  MtxOpCuda = 1
};

/**
 * @brief Matrix options bitmask.
 * Use as follow: int Options = MtxColMajor
 *
 * The default value is:
 * MtxDefaultOptions = MtxRowMajor
 */
enum MtxOptions {
  // Matrix storage type, use only one
  MtxRowMajor = 0, // Stores the matrix as row major.
  MtxColMajor = 1, // Stores the matrix as col major.
  // Default options for the Matrix (RowMajor).
  MtxDefaultOptions = (0 | MtxRowMajor),
};

enum MtxType {
  MtxDynamic = 0 // Use this on Row_/Col_ template parameter to make the matrix dynamic
};

#ifndef MAFS_MATRIX_OPERATION_MODE
#define MAFS_MATRIX_OPERATION_MODE MtxOpBasic
#endif

}; // namespace Mafs

#endif // MAFS_MATRIX_DATA_TYPES_H
