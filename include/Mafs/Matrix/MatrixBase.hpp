#ifndef MAFS_MATRIXBASE_H
#define MAFS_MATRIXBASE_H

#include <Mafs/Utils/Utils.hpp>
#include <exception>
// #include <format>
#include <fmt/core.h> //@todo: remove after gcc implements format

namespace Mafs {

enum MtxOptions {
  MtxRowMajor = 0, // Default
  MtxColMajor = 1,
};
enum MtxType { MtxDynamic };

namespace Internal {
template <typename T, size_t Rows_, size_t Cols_> class Container;

/**
 * Fixed size container.
 *
 * The size is defined in the template parameter.
 * There is no bound check, this class is suppose to be used by the MatrixBase and Matrix classes.
 */
template <typename T, size_t Rows_, size_t Cols_> class Container {
protected:
  enum { m_nRows = Rows_, m_nCols = Cols_, m_nSize = Rows_ * Cols_ };
  T m_Array[m_nRows * m_nCols];
  T m_SwapArray[m_nRows > m_nCols ? m_nRows : m_nCols];

public:
  Container() = default;

  T &operator[](size_t nIndex) { return m_Array[nIndex]; }
  T &operator[](size_t nIndex) const { return m_Array[nIndex]; }

  inline size_t Size() const { return m_nSize; }
  inline size_t RowCount() const { return m_nRows; }
  inline size_t ColCount() const { return m_nCols; }
  inline T *Data() { return m_Array; }
  inline T *Swap() { return m_SwapArray; }
};

/**
 * Dynamic size container.
 *
 * In the Rows_/Cols_ parameter pass zero or Mtx::Dynamic.
 * There is no bound check, this class is suppose to be used by the MatrixBase and Matrix classes.
 */
template <typename T> class Container<T, MtxDynamic, MtxDynamic> {
protected:
  T *m_Array = nullptr;
  T *m_SwapArray = nullptr;

  size_t m_nRows = 0;
  size_t m_nCols = 0;
  size_t m_nSize = 0;

  void Dealloc() {
    if (m_Array != nullptr) {
      delete[] m_Array;
      delete[] m_SwapArray;

      m_Array = nullptr;
      m_SwapArray = nullptr;
    }
  }

  void Alloc() {
    if (m_Array != nullptr)
      Dealloc();
    m_Array = new T[m_nSize];
    m_SwapArray = new T[m_nRows > m_nCols ? m_nRows : m_nCols];
  }

public:
  Container() = default;
  Container(size_t nRows, size_t nCols) { Resize(nRows, nCols); }

  T &operator[](size_t nIndex) { return m_Array[nIndex]; }
  T &operator[](size_t nIndex) const { return m_Array[nIndex]; }

  inline size_t Size() const { return m_nSize; }
  inline size_t RowCount() const { return m_nRows; }
  inline size_t ColCount() const { return m_nCols; }
  inline T *Data() { return m_Array; }
  inline T *Swap() { return m_SwapArray; }

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

/**
 * This is the base class for the Matrix class.
 *
 * It is used in the Operations static class.
 * It contains the basic functionality of a matrix.
 */
template <typename T, size_t Rows_, size_t Cols_, size_t Options_> class MatrixBase {
protected:
  enum {
    m_bIsDynamic = (Rows_ == MtxDynamic) || (Cols_ == MtxDynamic),
    m_MtxStorage = Options_ & 0x1
  };

  Container<T, Rows_, Cols_> m_Data;

  /**
   * Converts a matrix indexing (eg.: Matrix[1][2]) to an array index depending if it is Row or Col
   * major.
   */
  inline size_t Index(size_t nRow, size_t nCol) const {
    if constexpr (AreEnumsEqual<m_MtxStorage, MtxColMajor>())
      return nCol * m_Data.RowCount() + nRow;
    else
      return nRow * m_Data.ColCount() + nCol;
  }

  /**
   * Checks if the given index is in the container bounds, throw exception if not.
   */
  inline void BoundCheck(size_t nIndex) {
    if (nIndex >= m_Data.Size())
      throw std::out_of_range(fmt::format("Index {} is out of range", nIndex));
  }

  /**
   * Checks if the given row/col is in the container bounds, throw exception if not.
   */
  inline void BoundCheck(size_t nRow, size_t nCol) {
    if (nRow >= m_Data.RowCount() || nCol >= m_Data.ColCount())
      throw std::out_of_range(fmt::format("Index [{}][{}] is out of range", nRow, nCol));
  }

  /**
   * Swaps row/col using std::mempcy.
   * It is used when the values in memory are contiguous.
   * @see SwapCols
   * @see SwapRows
   */
  void GenericMemSwap(size_t lIndex, size_t rIndex, size_t nOffset) {
    // Save RIndex to swap.
    std::memcpy(m_Data.Swap(), m_Data.Data() + (rIndex * nOffset), sizeof(T) * nOffset);

    // Copy LIndex to RIndex
    std::memcpy(m_Data.Data() + (rIndex * nOffset), m_Data.Data() + (lIndex * nOffset),
                sizeof(T) * nOffset);

    // Copy swap to LIndex location
    std::memcpy(m_Data.Data() + (lIndex * nOffset), m_Data.Swap(), sizeof(T) * nOffset);
  }

  /**
   * Swaps row/col using a loop.
   * It is used when the values in memory are separeted.
   * @see SwapCols
   * @see SwapRows
   */
  void GenericLoopSwap(size_t lIndex, size_t rIndex, size_t nOffset) {
    T TmpValue;
    size_t aIndex, bIndex;
    for (size_t i = 0; i < nOffset; ++i) {
      if constexpr (AreEnumsEqual<m_MtxStorage, MtxRowMajor>()) {
        aIndex = Index(i, lIndex);
        bIndex = Index(i, rIndex);
      } else if constexpr (AreEnumsEqual<m_MtxStorage, MtxColMajor>()) {
        aIndex = Index(lIndex, i);
        bIndex = Index(rIndex, i);
      }
      TmpValue = m_Data[bIndex];
      m_Data[bIndex] = m_Data[aIndex];
      m_Data[aIndex] = TmpValue;
    }
  }

public:
  /**
   * Default constructor.
   * It can build either a static or dynamic matrix, it depends on the template parameter.
   *
   * Static = you passed the _Rows/_Cols template parameter.
   * Dynamic = you passed MtxDynamic (or 0) as _Rows/_Cols template parameter.
   */
  MatrixBase() = default;

  /**
   * Construct a dynamic matrix.
   * You cannot use this constructor if the matrix is static.
   *
   * Static = you passed the _Rows/_Cols template parameter.
   * Dynamic = you passed MtxDynamic (or 0) as _Rows/_Cols template parameter.
   */
  MatrixBase(size_t nRows, size_t nCols) {
    static_assert(m_bIsDynamic, "MatrixBase is static. You are trying to build a dynamic matrix, "
                                "use MtxDynamic on Rows_/Cols_ template params");
    m_Data.Resize(nRows, nCols);
  }

  /**
   * Access an element inside the matrix.
   * Returns a reference to the element at position [nRow][nCol] in the matrix.
   * The function checks whether [nRow][nCol] is within the bounds of valid elements in the
   * matrix, throwing an out_of_range exception if it is not (i.e., if [nRow][nCol] is greater than
   * its size).
   *
   * Usage: Matrix(row, col);
   * @see At
   */
  T &operator()(size_t nRow, size_t nCol) { return At(nRow, nCol); }

  /**
   * Access an element inside the matrix.
   * Returns a const reference to the element at position [nRow][nCol] in the matrix.
   * The function checks whether [nRow][nCol] is within the bounds of valid elements in the
   * matrix, throwing an out_of_range exception if it is not (i.e., if [nRow][nCol] is greater than
   * its size).
   *
   * Usage: Matrix(row, col);
   * @see At
   */
  T &operator()(size_t nRow, size_t nCol) const { return At(nRow, nCol); }

  /**
   * Access an element inside the matrix.
   * Returns a reference to the element at position [nRow][nCol] in the matrix.
   * The function checks whether [nRow][nCol] is within the bounds of valid elements in the
   * matrix, throwing an out_of_range exception if it is not (i.e., if [nRow][nCol] is greater than
   * its size).
   */
  T &At(size_t nRow, size_t nCol) {
    BoundCheck(nRow, nCol);
    return m_Data[Index(nRow, nCol)];
  }

  /**
   * Access an element inside the matrix.
   * Returns a constant reference to the element at position [nRow][nCol] in the matrix.
   * The function checks whether [nRow][nCol] is within the bounds of valid elements in the
   * matrix, throwing an out_of_range exception if it is not (i.e., if [nRow][nCol] is greater than
   * its size).
   */
  T &At(size_t nRow, size_t nCol) const { return At(nRow, nCol); }

  /**
   * Fill the matrix with Value.
   */
  void Fill(const T &Value) {
    for (size_t i = 0; i < m_Data.Size(); ++i)
      m_Data[i] = Value;
  }

  /**
   * Swap aRow with bRow, if it is contiguous it uses MemSwap, otherwise it'll use the loop swap.
   * If you are swapping a row and the matrix is stored as row major, then the row values is stored
   * side by side in memory.
   */
  void SwapRows(size_t aRow, size_t bRow) {
    if constexpr (AreEnumsEqual<m_MtxStorage, MtxRowMajor>())
      GenericMemSwap(aRow, bRow, m_Data.ColCount());
    else
      GenericLoopSwap(aRow, bRow, m_Data.ColCount());
  }

  /**
   * Swap aCol with bCol, if it is contiguous it uses MemSwap, otherwise it'll use the loop swap.
   * If you are swapping a col and the matrix is stored as col major, then the col values is stored
   * side by side in memory.
   */
  void SwapCols(size_t aCol, size_t bCol) {
    if constexpr (AreEnumsEqual<m_MtxStorage, MtxColMajor>())
      GenericMemSwap(aCol, bCol, m_Data.RowCount());
    else
      GenericLoopSwap(aCol, bCol, m_Data.RowCount());
  }

  /**
   * Puts the matrix infos and values to string.
   * It'll print as follow:
   * Matrix<type>[rows][cols] // properties
   * a00 a01 ... a0n
   * a10 a11 ... a1n
   * ... ... ... ...
   * an0 an1 ... ann
   */
  inline auto ToString() -> std::string {
    std::string strOptions = "";

    if constexpr (AreEnumsEqual<m_MtxStorage, MtxRowMajor>())
      strOptions += "RowMajor";
    else
      strOptions += "ColMajor";

    if constexpr (m_bIsDynamic)
      strOptions += ", Dynamic";
    else
      strOptions += ", Static";

    // @todo: improve this (maybe put on a function, use memmove, idk... just improve it)
    std::string strData = "";
    for (size_t i = 0; i < m_Data.RowCount(); ++i) {
      for (size_t j = 0; j < m_Data.ColCount(); ++j)
        strData += fmt::format("{} ", m_Data[Index(i, j)]);
      strData += "\n";
    }

    return fmt::format("Matrix<{}>[{}][{}] // {}\n{}\n", typeid(T).name(), m_Data.RowCount(),
                       m_Data.ColCount(), strOptions, strData);
  }
};

// template <typename T> using MatrixBaseX = MatrixBase<T, MtxDynamic, MtxDynamic>;

};     // namespace Internal
};     // namespace Mafs
#endif // MAFS_MATRIXBASE_H
