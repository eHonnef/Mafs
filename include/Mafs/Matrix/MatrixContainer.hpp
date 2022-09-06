#ifndef MAFS_MATRIXCONTAINER_H
#define MAFS_MATRIXCONTAINER_H

#include <stddef.h>

namespace Mafs {

enum MtxType {
  MtxDynamic = 0 // Use this on Row_/Col_ template parameter to make the matrix dynamic
};

namespace Internal {

template <typename T, size_t Rows_, size_t Cols_> class Container;

/**
 * @brief Fixed size container.
 *
 * The size is defined in the template parameter.
 * There is no bound check, this class is suppose to be used by the MatrixBase and Matrix classes.
 *
 * @tparam T
 * @tparam Rows_
 * @tparam Cols_
 */
template <typename T, size_t Rows_, size_t Cols_> class Container {
protected:
  // Enum containing the container static data.
  enum {
    m_nRows = Rows_,        // Number of rows.
    m_nCols = Cols_,        // Number of cols.
    m_nSize = Rows_ * Cols_ // Container size (m_nRows * m_nCols).
  };
  T m_Array[m_nRows * m_nCols]; // Array containing the Data.
  // Swap array, used to temporary store data when swapping row/col.
  T m_SwapArray[m_nRows > m_nCols ? m_nRows : m_nCols];

public:
  Container() = default;

  T &operator[](size_t nIndex) { return m_Array[nIndex]; }
  const T &operator[](size_t nIndex) const { return m_Array[nIndex]; }

  inline size_t Size() const { return m_nSize; }
  inline size_t RowCount() const { return m_nRows; }
  inline size_t ColCount() const { return m_nCols; }
  inline T *Data() { return m_Array; }
  inline const T *Data() const { return m_Array; }
  inline T *Swap() { return m_SwapArray; }
};

/**
 * @brief Dynamic size container.
 *
 * In the Rows_/Cols_ parameter pass zero or Mtx::Dynamic.
 * There is no bound check, this class is suppose to be used by the MatrixBase and Matrix classes.
 *
 * @tparam T
 */
template <typename T> class Container<T, MtxDynamic, MtxDynamic> {
protected:
  T *m_Array = nullptr; // Array containing the Data.
  // Swap array, used to temporary store data when swapping row/col.
  T *m_SwapArray = nullptr;

  size_t m_nRows = 0; // Number of rows.
  size_t m_nCols = 0; // Number of cols.
  size_t m_nSize = 0; // Container size (m_nRows * m_nCols).

  /**
   * @brief Delete and set to nullptr the Array and SwapArray.
   */
  void Dealloc() {
    if (m_Array != nullptr) {
      delete[] m_Array;
      delete[] m_SwapArray;

      m_Array = nullptr;
      m_SwapArray = nullptr;
    }
  }

  /**
   * @brief Allocate the Array and SwapArray.
   * The array size is m_nSize (m_nRow * m_nCol).
   * The swap array size is the biggest value between "m_nRow and m_nCol".
   *
   * If the Array is allocated, the function first deallocate it by calling Dealloc.
   * @see Dealloc
   * @see m_nRow, m_nCol
   */
  void Alloc() {
    if (m_Array != nullptr)
      Dealloc();
    m_Array = new T[m_nSize];
    m_SwapArray = new T[m_nRows > m_nCols ? m_nRows : m_nCols];
  }

public:
  Container() = default;
  Container(size_t nRows, size_t nCols) { Resize(nRows, nCols); }

  ~Container() { Dealloc(); }

  T &operator[](size_t nIndex) { return m_Array[nIndex]; }
  T &operator[](size_t nIndex) const { return m_Array[nIndex]; }

  inline size_t Size() const { return m_nSize; }
  inline size_t RowCount() const { return m_nRows; }
  inline size_t ColCount() const { return m_nCols; }
  inline T *Data() { return m_Array; }
  inline const T *Data() const { return m_Array; }
  inline T *Swap() { return m_SwapArray; }

  /**
   * @brief Resizes the container by nRows * nCols.
   * It deallocates the array by calling Dealloc, reassign the variables m_nRows, m_nCols, m_nSize
   * and then reallocates it by calling Alloc.
   *
   * @see Dealloc
   * @see Alloc
   * @param nRows
   * @param nCols
   */
  void Resize(size_t nRows, size_t nCols) {
    if (nRows == m_nRows && nCols == m_nCols)
      return; // Array set to same size.
    else if (nRows <= 0 || nCols <= 0)
      return Dealloc(); // Array set to zero, only dealloc.
    else {
      Dealloc();
      m_nRows = nRows;
      m_nCols = nCols;
      m_nSize = nRows * nCols;
      Alloc();
    }
  }
};
}; // namespace Internal
}; // namespace Mafs

#endif // MAFS_MATRIXCONTAINER_H