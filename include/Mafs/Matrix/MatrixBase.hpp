#ifndef MAFS_MATRIXBASE_H
#define MAFS_MATRIXBASE_H

#include <Mafs/Matrix/MatrixContainer.hpp>
#include <Mafs/Utils/Utils.hpp>
#include <fmt/core.h>

namespace Mafs::Internal {
template <typename T> struct MatrixTraits;

/**
 * @brief This is the base class for the Matrix class.
 *
 * It is used in the Operations static class.
 * It contains the basic functionality of a matrix.
 *
 * @tparam Derived
 */
template <typename Derived> class MatrixBase {
protected:
  typedef typename MatrixTraits<Derived>::Type Type;
  Container<Type, MatrixTraits<Derived>::Rows, MatrixTraits<Derived>::Cols> m_Container;

  /**
   * @brief Matrix configurations
   *
   */
  enum {
    m_bIsDynamic = (AreEnumsEqual<MatrixTraits<Derived>::Rows, MtxDynamic>() ||
                    AreEnumsEqual<MatrixTraits<Derived>::Cols, MtxDynamic>()),
    m_MtxStorage = MatrixTraits<Derived>::Options & 0x1
  };

  /**
   * @brief Converts a matrix indexing (eg.: Matrix[1][2]) to an array index depending if it is Row
   * or Col major.
   *
   * @param nRow
   * @param nCol
   * @return size_t
   */
  inline size_t Index(size_t nRow, size_t nCol) const {
    if constexpr (AreEnumsEqual<m_MtxStorage, MtxColMajor>())
      return nCol * m_Container.RowCount() + nRow;
    else
      return nRow * m_Container.ColCount() + nCol;
  }

  /**
   * @brief Checks if the given index is in the container bounds, throw exception if not.
   *
   * @param nIndex
   */
  inline void BoundCheck(size_t nIndex) {
    if (nIndex >= m_Container.Size())
      throw std::out_of_range(fmt::format("Index {} is out of range", nIndex));
  }

  /**
   * @brief Checks if the given row/col is in the container bounds, throw exception if not.
   *
   * @param nRow
   * @param nCol
   */
  inline void BoundCheck(size_t nRow, size_t nCol) {
    if (nRow >= m_Container.RowCount() || nCol >= m_Container.ColCount())
      throw std::out_of_range(fmt::format("Index [{}][{}] is out of range", nRow, nCol));
  }

  /**
   * @brief Swaps row/col using std::mempcy.
   * It is used when the values in memory are contiguous.
   *
   * @see SwapCols
   * @see SwapRows
   * @param lIndex
   * @param rIndex
   * @param nOffset
   */
  void GenericMemSwap(size_t lIndex, size_t rIndex, size_t nOffset) {
    // Save RIndex to swap.
    std::memcpy(m_Container.Swap(), m_Container.Data() + (rIndex * nOffset),
                sizeof(Type) * nOffset);

    // Copy LIndex to RIndex
    std::memcpy(m_Container.Data() + (rIndex * nOffset), m_Container.Data() + (lIndex * nOffset),
                sizeof(Type) * nOffset);

    // Copy swap to LIndex location
    std::memcpy(m_Container.Data() + (lIndex * nOffset), m_Container.Swap(),
                sizeof(Type) * nOffset);
  }

  /**
   * @brief Swaps row/col using a loop.
   * It is used when the values in memory are separeted.
   *
   * @see SwapCols
   * @see SwapRows
   * @param lIndex
   * @param rIndex
   * @param nOffset
   */
  void GenericLoopSwap(size_t lIndex, size_t rIndex, size_t nOffset) {
    Type TmpValue;
    size_t aIndex, bIndex;
    for (size_t i = 0; i < nOffset; ++i) {
      if constexpr (AreEnumsEqual<m_MtxStorage, MtxRowMajor>()) {
        aIndex = Index(i, lIndex);
        bIndex = Index(i, rIndex);
      } else if constexpr (AreEnumsEqual<m_MtxStorage, MtxColMajor>()) {
        aIndex = Index(lIndex, i);
        bIndex = Index(rIndex, i);
      }
      TmpValue = m_Container[bIndex];
      m_Container[bIndex] = m_Container[aIndex];
      m_Container[aIndex] = TmpValue;
    }
  }

public:
  /**
   * @brief Default constructor.
   * It can build either a static or dynamic matrix, it depends on the template parameter.
   *
   * Static = you passed the _Rows/_Cols template parameter.
   * Dynamic = you passed MtxDynamic (or 0) as _Rows/_Cols template parameter.
   *
   */
  MatrixBase() = default;

  /**
   * @brief Construct a dynamic matrix.
   * You cannot use this constructor if the matrix is static.
   *
   * Static = you passed the _Rows/_Cols template parameter.
   * Dynamic = you passed MtxDynamic (or 0) as _Rows/_Cols template parameter.
   *
   * @param nRows
   * @param nCols
   */
  MatrixBase(size_t nRows, size_t nCols) {
    static_assert(m_bIsDynamic, "MatrixBase is static. You are trying to build a dynamic matrix, "
                                "use MtxDynamic on Rows_/Cols_ template params");
    m_Container.Resize(nRows, nCols);
  }

  /**
   * @brief Copy constructor
   *
   * @param CopiedMatrix
   */
  MatrixBase(const MatrixBase &CopiedMatrix) {
    if constexpr (m_bIsDynamic)
      m_Container.Resize(CopiedMatrix.RowCount(), CopiedMatrix.ColCount());

    std::memcpy(m_Container.Data(), CopiedMatrix.m_Container.Data(),
                sizeof(Type) * CopiedMatrix.m_Container.Size());
  }

  /**
   * @brief Access an element inside the matrix.
   * Returns a reference to the element at position [nRow][nCol] in the matrix.
   * The function checks whether [nRow][nCol] is within the bounds of valid elements in the
   * matrix, throwing an out_of_range exception if it is not (i.e., if [nRow][nCol] is greater than
   * its size).
   * Usage: Matrix(row, col);
   *
   * @see At()
   * @param nRow
   * @param nCol
   * @return Type&
   */
  Type &operator()(size_t nRow, size_t nCol) { return At(nRow, nCol); }

  /**
   * @brief Access an element inside the matrix.
   * Returns a const reference to the element at position [nRow][nCol] in the matrix.
   * The function checks whether [nRow][nCol] is within the bounds of valid elements in the
   * matrix, throwing an out_of_range exception if it is not (i.e., if [nRow][nCol] is greater than
   * its size).
   * Usage: Matrix(row, col);
   *
   * @see At()
   * @param nRow
   * @param nCol
   * @return Type&
   */
  const Type &operator()(size_t nRow, size_t nCol) const { return At(nRow, nCol); }

  /**
   * @brief Access an element inside the matrix.
   * Returns a reference to the element at position [nRow][nCol] in the matrix.
   * The function checks whether [nRow][nCol] is within the bounds of valid elements in the
   * matrix, throwing an out_of_range exception if it is not (i.e., if [nRow][nCol] is greater than
   * its size).
   *
   * @param nRow
   * @param nCol
   * @return Type&
   */
  Type &At(size_t nRow, size_t nCol) {
    BoundCheck(nRow, nCol);
    return m_Container[Index(nRow, nCol)];
  }

  /**
   * @brief Access an element inside the matrix.
   * Returns a constant reference to the element at position [nRow][nCol] in the matrix.
   * The function checks whether [nRow][nCol] is within the bounds of valid elements in the
   * matrix, throwing an out_of_range exception if it is not (i.e., if [nRow][nCol] is greater than
   * its size).
   *
   * @param nRow
   * @param nCol
   * @return Type&
   */
  const Type &At(size_t nRow, size_t nCol) const {
    // BoundCheck(nRow, nCol);
    return m_Container[Index(nRow, nCol)];
  }

  /**
   * @brief Returns the number of rows of this matrix.
   *
   * @return size_t
   */
  inline size_t RowCount() const { return m_Container.RowCount(); }

  /**
   * @brief Returns the number of columns of this matrix.
   *
   * @return size_t
   */
  inline size_t ColCount() const { return m_Container.ColCount(); }

  /**
   * @brief Fill the matrix with Value.
   *
   * @param Value
   */
  void Fill(const Type &Value) {
    for (size_t i = 0; i < m_Container.Size(); ++i)
      m_Container[i] = Value;
  }

  /**
   * @brief Swap aRow with bRow, if it is contiguous it uses MemSwap, otherwise it'll use the loop
   * swap. If you are swapping a row and the matrix is stored as row major, then the row values is
   * stored side by side in memory.
   *
   * @param aRow
   * @param bRow
   */
  void SwapRows(size_t aRow, size_t bRow) {
    if constexpr (AreEnumsEqual<m_MtxStorage, MtxRowMajor>())
      GenericMemSwap(aRow, bRow, m_Container.ColCount());
    else
      GenericLoopSwap(aRow, bRow, m_Container.ColCount());
  }

  /**
   * @brief Swap aCol with bCol, if it is contiguous it uses MemSwap, otherwise it'll use the loop
   * swap. If you are swapping a col and the matrix is stored as col major, then the col values is
   * stored side by side in memory.
   *
   * @param aCol
   * @param bCol
   */
  void SwapCols(size_t aCol, size_t bCol) {
    if constexpr (AreEnumsEqual<m_MtxStorage, MtxColMajor>())
      GenericMemSwap(aCol, bCol, m_Container.RowCount());
    else
      GenericLoopSwap(aCol, bCol, m_Container.RowCount());
  }

  /**
   * @brief Puts the matrix infos and values to string.
   * It'll print as follow:
   * Matrix<type>[rows][cols] // properties
   * a00 a01 ... a0n
   * a10 a11 ... a1n
   * ... ... ... ...
   * an0 an1 ... ann
   *
   * @return std::string
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
    for (size_t i = 0; i < m_Container.RowCount(); ++i) {
      for (size_t j = 0; j < m_Container.ColCount(); ++j)
        strData += fmt::format("{} ", m_Container[Index(i, j)]);
      strData += "\n";
    }

    return fmt::format("Matrix<{}>[{}][{}] // {}\n{}\n", typeid(Type).name(),
                       m_Container.RowCount(), m_Container.ColCount(), strOptions, strData);
  }
};
}; // namespace Mafs::Internal

#endif // MAFS_MATRIXBASE_H
