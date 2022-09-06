/*********************************************************************************
 * MatrixBasicOperationsTest.cpp
 * It has tests for the matrix's structure itself.
 *********************************************************************************/

#include <Mafs/Matrix/Matrix.hpp>
#include <Mafs/Matrix/Operations/Operations.hpp>
#include <doctest/doctest.h>
#include <iostream>

#define UNUSED(x) (void)(x)

TEST_CASE("Sum") {
  Mafs::Matrix<int, 2, 2, 1> MtxStatic;
  Mafs::Matrix<int, 0, 0, 1> MtxDyn(2, 2);
  Mafs::Matrix<int, 0, 0, 1> MtxDiff(3, 2);

  for (size_t i = 0; i < MtxStatic.RowCount(); ++i)
    for (size_t j = 0; j < MtxStatic.ColCount(); ++j) {
      MtxStatic(i, j) = 1;
      MtxDyn(i, j) = 2;
    }

  Mafs::Matrix mtx = (MtxStatic + MtxDyn);
  for (size_t i = 0; i < mtx.RowCount(); ++i)
    for (size_t j = 0; j < mtx.ColCount(); ++j)
      REQUIRE(mtx(i, j) == 3);

  Mafs::Matrix mtx2 = (MtxDyn + MtxStatic);
  for (size_t i = 0; i < mtx2.RowCount(); ++i)
    for (size_t j = 0; j < mtx2.ColCount(); ++j)
      REQUIRE(mtx2(i, j) == 3);
  
  REQUIRE_THROWS(MtxStatic + MtxDiff);
}
