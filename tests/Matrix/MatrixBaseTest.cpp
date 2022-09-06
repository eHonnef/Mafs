/*********************************************************************************
 * MatrixBaseTest.cpp
 * It has tests for the matrix's structure itself.
 *********************************************************************************/

#include <doctest/doctest.h>

#define private public
#define protected public
#include <Mafs/Matrix/Matrix.hpp>

#define UNUSED(x) (void)(x)

template <typename T, size_t Rows_, size_t Cols_, size_t Options_>
void RangeFill(Mafs::Matrix<T, Rows_, Cols_, Options_> &Matrix) {
  for (size_t i = 0; i < Matrix.m_Container.Size(); ++i)
    Matrix.m_Container.Data()[i] = i;
}

template <typename T, size_t Rows_, size_t Cols_, size_t Options_>
bool CheckIfEquals(Mafs::Matrix<T, Rows_, Cols_, Options_> &lMatrix,
                   Mafs::Matrix<T, Rows_, Cols_, Options_> &rMatrix) {

  // This function was created before the operator== were implemented.
  // It is used to check if two matrices are equal.
  if (lMatrix.RowCount() != rMatrix.RowCount() || lMatrix.ColCount() != rMatrix.ColCount())
    return false;

  for (size_t i = 0; i < rMatrix.RowCount(); ++i)
    for (size_t j = 0; j < rMatrix.ColCount(); ++j)
      if (rMatrix.At(i, j) != lMatrix.At(i, j))
        return false;

  return true;
}

TEST_CASE("Basic Case, instantiate") {
  Mafs::Matrix<int, 1, 1, 1> MtxStatic;
  Mafs::Matrix<int, 0, 0, 1> MtxDyn(3, 5);

  // To avoid warnings
  UNUSED(MtxStatic);
  UNUSED(MtxDyn);
}

TEST_CASE("Copy constructor") {
  Mafs::Matrix<int, 3, 5, 1> MtxStatic;
  RangeFill(MtxStatic);

  Mafs::Matrix<int, 0, 0, 1> MtxDyn(3, 5);
  RangeFill(MtxDyn);

  Mafs::Matrix MtxDynCopy(MtxDyn);
  CheckIfEquals(MtxDynCopy, MtxDyn);

  Mafs::Matrix MtxStaticCopy(MtxStatic);
  CheckIfEquals(MtxStaticCopy, MtxStatic);
}

TEST_CASE("Operator=") {}

TEST_CASE("Change matrix data") {
  Mafs::Matrix<int, 0, 0, 1> MtxDyn(3, 5);

  MtxDyn(2, 1) = 3;
  MtxDyn.At(1, 2) = 3;

  REQUIRE(MtxDyn.At(2, 1) == 3);
  REQUIRE(MtxDyn(1, 2) == 3);
}

TEST_CASE("Container static initialization test") {
  constexpr int nRows = 2;
  constexpr int nCols = 3;
  constexpr int nMult = 42;

  Mafs::Internal::Container<int, nRows, nCols> Container;

  REQUIRE(Container.Size() == (nRows * nCols));
  REQUIRE(Container.RowCount() == nRows);
  REQUIRE(Container.ColCount() == nCols);

  for (size_t i = 0; i < Container.Size(); ++i)
    Container.Data()[i] = i * nMult;

  for (int i = 0; i < nRows * nCols; ++i)
    REQUIRE(Container.Data()[i] == (i * nMult));
}

TEST_CASE("Container dynamic initialization test") {
  constexpr int nRows = 3;
  constexpr int nCols = 4;
  constexpr int nMult = 42;

  Mafs::Internal::Container<int, Mafs::MtxDynamic, Mafs::MtxDynamic> Container(nRows, nCols);

  REQUIRE(Container.Size() == (nRows * nCols));
  REQUIRE(Container.RowCount() == nRows);
  REQUIRE(Container.ColCount() == nCols);

  for (size_t i = 0; i < Container.Size(); ++i)
    Container.Data()[i] = i * nMult;

  for (int i = 0; i < nRows * nCols; ++i)
    REQUIRE(Container.Data()[i] == (i * nMult));
}

TEST_CASE("Container dynamic resize") {
  constexpr int nRows = 3;
  constexpr int nCols = 4;

  Mafs::Internal::Container<int, Mafs::MtxDynamic, Mafs::MtxDynamic> Container(nRows, nCols);

  // Shrink
  constexpr int nRowsShrink = 2;
  constexpr int nColsShrink = 2;
  Container.Resize(nRowsShrink, nColsShrink);

  REQUIRE(Container.Size() == (nRowsShrink * nColsShrink));
  REQUIRE(Container.RowCount() == nRowsShrink);
  REQUIRE(Container.ColCount() == nColsShrink);

  // Increase
  constexpr int nRowsIncrease = 10;
  constexpr int nColsIncrease = 11;
  Container.Resize(nRowsIncrease, nColsIncrease);

  REQUIRE(Container.Size() == (nRowsIncrease * nColsIncrease));
  REQUIRE(Container.RowCount() == nRowsIncrease);
  REQUIRE(Container.ColCount() == nColsIncrease);
}

TEST_CASE("MatrixBase options") {
  Mafs::Matrix<int, 1, 1, Mafs::MtxRowMajor> MtxRow;
  Mafs::Matrix<int, 1, 1, Mafs::MtxColMajor> MtxCol;

  REQUIRE(AreEnumsEqual(MtxRow.m_MtxStorage, Mafs::MtxRowMajor));
  REQUIRE(AreEnumsEqual(MtxCol.m_MtxStorage, Mafs::MtxColMajor));
}

TEST_CASE("MatrixBase dynamic instantiation") {
  constexpr int Options = Mafs::MtxRowMajor;
  Mafs::Matrix<int, Mafs::MtxDynamic, Mafs::MtxDynamic, Options> Matrix(3, 5);
  Matrix.Fill(3);

  for (size_t i = 0; i < Matrix.m_Container.Size(); ++i)
    REQUIRE(Matrix.m_Container.Data()[i] == 3);
}

TEST_CASE("MatrixBase static instantiation") {
  constexpr int Options = Mafs::MtxRowMajor;
  Mafs::Matrix<int, 3, 5, Options> Matrix;
  Matrix.Fill(3);

  for (size_t i = 0; i < Matrix.m_Container.Size(); ++i)
    REQUIRE(Matrix.m_Container.Data()[i] == 3);
}

TEST_CASE("MatrixBase row major indexing") {
  Mafs::Matrix<int, Mafs::MtxDynamic, Mafs::MtxDynamic, Mafs::MtxRowMajor> Matrix(2, 3);
  RangeFill(Matrix);

  // Expected:
  // 0 1 2
  // 3 4 5

  REQUIRE(Matrix(0, 0) == 0);
  REQUIRE(Matrix(0, 1) == 1);
  REQUIRE(Matrix(0, 2) == 2);
  REQUIRE(Matrix(1, 0) == 3);
  REQUIRE(Matrix(1, 1) == 4);
  REQUIRE(Matrix(1, 2) == 5);
}

TEST_CASE("MatrixBase col major indexing") {
  Mafs::Matrix<int, 2, 3, Mafs::MtxColMajor> Matrix;
  RangeFill(Matrix);

  // Expected
  // 0 2 4
  // 1 3 5

  REQUIRE(Matrix(0, 0) == 0);
  REQUIRE(Matrix(1, 0) == 1);
  REQUIRE(Matrix(0, 1) == 2);
  REQUIRE(Matrix(1, 1) == 3);
  REQUIRE(Matrix(0, 2) == 4);
  REQUIRE(Matrix(1, 2) == 5);
}

TEST_CASE("MatrixBase row swap (row major)") {
  Mafs::Matrix<int, 3, 5, Mafs::MtxRowMajor> Matrix;
  RangeFill(Matrix);

  // Matrix Initial:
  // 00 01 02 03 04
  // 05 06 07 08 09
  // 10 11 12 13 14

  Matrix.SwapRows(0, 1);
  // Matrix expected:
  // 05 06 07 08 09
  // 00 01 02 03 04
  // 10 11 12 13 14
  REQUIRE(Matrix(0, 0) == 5);
  REQUIRE(Matrix(0, 1) == 6);
  REQUIRE(Matrix(0, 2) == 7);
  REQUIRE(Matrix(0, 3) == 8);
  REQUIRE(Matrix(0, 4) == 9);

  REQUIRE(Matrix(1, 0) == 0);
  REQUIRE(Matrix(1, 1) == 1);
  REQUIRE(Matrix(1, 2) == 2);
  REQUIRE(Matrix(1, 3) == 3);
  REQUIRE(Matrix(1, 4) == 4);

  Matrix.SwapRows(0, 2);
  // Matrix expected:
  // 10 11 12 13 14
  // 00 01 02 03 04
  // 05 06 07 08 09
  REQUIRE(Matrix(0, 0) == 10);
  REQUIRE(Matrix(0, 1) == 11);
  REQUIRE(Matrix(0, 2) == 12);
  REQUIRE(Matrix(0, 3) == 13);
  REQUIRE(Matrix(0, 4) == 14);

  REQUIRE(Matrix(2, 0) == 5);
  REQUIRE(Matrix(2, 1) == 6);
  REQUIRE(Matrix(2, 2) == 7);
  REQUIRE(Matrix(2, 3) == 8);
  REQUIRE(Matrix(2, 4) == 9);

  Matrix.SwapRows(2, 1);
  // Matrix expected:
  // 10 11 12 13 14
  // 05 06 07 08 09
  // 00 01 02 03 04
  REQUIRE(Matrix(1, 0) == 5);
  REQUIRE(Matrix(1, 1) == 6);
  REQUIRE(Matrix(1, 2) == 7);
  REQUIRE(Matrix(1, 3) == 8);
  REQUIRE(Matrix(1, 4) == 9);

  REQUIRE(Matrix(2, 0) == 0);
  REQUIRE(Matrix(2, 1) == 1);
  REQUIRE(Matrix(2, 2) == 2);
  REQUIRE(Matrix(2, 3) == 3);
  REQUIRE(Matrix(2, 4) == 4);
}

TEST_CASE("MatrixBase row swap (col major)") {
  constexpr int nType = Mafs::MtxDynamic;
  Mafs::Matrix<int, nType, nType, Mafs::MtxColMajor> Matrix(3, 5);
  RangeFill(Matrix);

  // Matrix Initial:
  // 00 03 06 09 12
  // 01 04 07 10 13
  // 02 05 08 11 14

  Matrix.SwapRows(0, 1);
  // Matrix expected:
  // 01 04 07 10 13
  // 00 03 06 09 12
  // 02 05 08 11 14
  REQUIRE(Matrix(0, 0) == 1);
  REQUIRE(Matrix(0, 1) == 4);
  REQUIRE(Matrix(0, 2) == 7);
  REQUIRE(Matrix(0, 3) == 10);
  REQUIRE(Matrix(0, 4) == 13);

  REQUIRE(Matrix(1, 0) == 0);
  REQUIRE(Matrix(1, 1) == 3);
  REQUIRE(Matrix(1, 2) == 6);
  REQUIRE(Matrix(1, 3) == 9);
  REQUIRE(Matrix(1, 4) == 12);

  Matrix.SwapRows(0, 2);
  // Matrix expected:
  // 02 05 08 11 14
  // 00 03 06 09 12
  // 01 04 07 10 13
  REQUIRE(Matrix(0, 0) == 2);
  REQUIRE(Matrix(0, 1) == 5);
  REQUIRE(Matrix(0, 2) == 8);
  REQUIRE(Matrix(0, 3) == 11);
  REQUIRE(Matrix(0, 4) == 14);

  REQUIRE(Matrix(2, 0) == 1);
  REQUIRE(Matrix(2, 1) == 4);
  REQUIRE(Matrix(2, 2) == 7);
  REQUIRE(Matrix(2, 3) == 10);
  REQUIRE(Matrix(2, 4) == 13);

  Matrix.SwapRows(2, 1);
  // Matrix expected:
  // 02 05 08 11 14
  // 01 04 07 10 13
  // 00 03 06 09 12
  REQUIRE(Matrix(1, 0) == 1);
  REQUIRE(Matrix(1, 1) == 4);
  REQUIRE(Matrix(1, 2) == 7);
  REQUIRE(Matrix(1, 3) == 10);
  REQUIRE(Matrix(1, 4) == 13);

  REQUIRE(Matrix(2, 0) == 0);
  REQUIRE(Matrix(2, 1) == 3);
  REQUIRE(Matrix(2, 2) == 6);
  REQUIRE(Matrix(2, 3) == 9);
  REQUIRE(Matrix(2, 4) == 12);
}

TEST_CASE("MatrixBase col swap (row major)") {
  Mafs::Matrix<int, 3, 5, Mafs::MtxRowMajor> Matrix;
  RangeFill(Matrix);

  // Matrix Initial:
  // 00 01 02 03 04
  // 05 06 07 08 09
  // 10 11 12 13 14

  Matrix.SwapCols(3, 1);
  // Matrix expected:
  // 00 03 02 01 04
  // 05 08 07 06 09
  // 10 13 12 11 14
  REQUIRE(Matrix(0, 1) == 3);
  REQUIRE(Matrix(1, 1) == 8);
  REQUIRE(Matrix(2, 1) == 13);

  REQUIRE(Matrix(0, 3) == 1);
  REQUIRE(Matrix(1, 3) == 6);
  REQUIRE(Matrix(2, 3) == 11);

  Matrix.SwapCols(0, 4);
  // Matrix expected:
  // 04 03 02 01 00
  // 09 08 07 06 05
  // 14 13 12 11 10
  REQUIRE(Matrix(0, 0) == 4);
  REQUIRE(Matrix(1, 0) == 9);
  REQUIRE(Matrix(2, 0) == 14);

  REQUIRE(Matrix(0, 4) == 0);
  REQUIRE(Matrix(1, 4) == 5);
  REQUIRE(Matrix(2, 4) == 10);
}

TEST_CASE("MatrixBase col swap (col major)") {
  constexpr int nType = Mafs::MtxDynamic;
  Mafs::Matrix<int, nType, nType, Mafs::MtxColMajor> Matrix(3, 5);
  RangeFill(Matrix);

  // Matrix Initial:
  // 00 03 06 09 12
  // 01 04 07 10 13
  // 02 05 08 11 14

  Matrix.SwapCols(3, 1);
  // Matrix expected:
  // 00 09 06 03 12
  // 01 10 07 04 13
  // 02 11 08 05 14
  REQUIRE(Matrix(0, 1) == 9);
  REQUIRE(Matrix(1, 1) == 10);
  REQUIRE(Matrix(2, 1) == 11);

  REQUIRE(Matrix(0, 3) == 3);
  REQUIRE(Matrix(1, 3) == 4);
  REQUIRE(Matrix(2, 3) == 5);

  Matrix.SwapCols(0, 4);
  // Matrix expected:
  // 12 09 06 03 00
  // 13 10 07 04 01
  // 14 11 08 05 02
  REQUIRE(Matrix(0, 0) == 12);
  REQUIRE(Matrix(1, 0) == 13);
  REQUIRE(Matrix(2, 0) == 14);

  REQUIRE(Matrix(0, 4) == 0);
  REQUIRE(Matrix(1, 4) == 1);
  REQUIRE(Matrix(2, 4) == 2);
}
