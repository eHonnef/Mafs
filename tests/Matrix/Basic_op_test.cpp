#include "doctest/doctest.h"
#include "Matrix/basic_op.cc"

typedef basic_op OP;

TEST_CASE("Matrix transposition") {
  Matrix<int> a({{0, 1, 2}, {3, 4, 5}});
  Matrix<int> b = OP::transpose(a);
  // {0,3}
  // {1,4}
  // {2,5}
  REQUIRE(b.rows() == 3);
  REQUIRE(b.cols() == 2);
  REQUIRE(b.at(0, 0) == 0);
  REQUIRE(b.at(0, 1) == 3);
  REQUIRE(b.at(1, 0) == 1);
  REQUIRE(b.at(1, 1) == 4);
  REQUIRE(b.at(2, 0) == 2);
  REQUIRE(b.at(2, 1) == 5);
}

TEST_CASE("Upper triangular matrix") {
  Matrix<int> a(10, 10, 7);

  a = OP::u_triangular(a);
  for (auto i = a.rows(); i-- > 0;)
    for (auto j = i; j-- > i;)
      REQUIRE(a.at(i, j) == 7);

  for (unsigned i = 1; i < a.rows(); ++i)
    for (unsigned j = 0; j < i; ++j)
      REQUIRE(a.at(i, j) == 0);
}

TEST_CASE("Lower triangular matrix") {
  Matrix<int> a(10, 10, 7);

  a = OP::l_triangular(a);
  for (auto i = a.rows(); i-- > 0;)
    for (auto j = i; j-- > i;)
      REQUIRE(a.at(i, j) == 0);

  for (unsigned i = 1; i < a.rows(); ++i)
    for (unsigned j = 0; j < i; ++j)
      REQUIRE(a.at(i, j) == 7);
}

TEST_CASE("LU decomposition") {
  Matrix<double> a({{0.448, 0.832, 0.193}, {0.421, 0.784, -0.207}, {-0.319, 0.884, 0.279}});
  Matrix<double> LU(a.rows(), a.cols());

  OP::LUP_doolittle(a, LU);

  REQUIRE(LU.at(0, 0) == doctest::Approx(0.448).epsilon(0.001));
  REQUIRE(LU.at(0, 1) == doctest::Approx(0.832).epsilon(0.001));
  REQUIRE(LU.at(0, 2) == doctest::Approx(0.193).epsilon(0.0001));
  REQUIRE(LU.at(1, 0) == doctest::Approx(-0.712).epsilon(0.001));
  REQUIRE(LU.at(1, 1) == doctest::Approx(1.476).epsilon(0.001));
  REQUIRE(LU.at(1, 2) == doctest::Approx(0.416).epsilon(0.01));
  REQUIRE(LU.at(2, 0) == doctest::Approx(0.939).epsilon(0.001));
  REQUIRE(LU.at(2, 1) == doctest::Approx(0.0014).epsilon(0.1));
  REQUIRE(LU.at(2, 2) == doctest::Approx(-0.388).epsilon(0.01));
}

TEST_CASE("LU decomposition split") {
  Matrix<int> LU({{1, 2, 3}, {7, 8, 4}, {5, 6, 9}});
  Matrix<int> L(LU.rows(), LU.rows());
  Matrix<int> U(LU.rows(), LU.rows());

  OP::split_LU_doolittle(LU, L, U);

  REQUIRE(L.at(0, 0) == 1);
  REQUIRE(L.at(0, 1) == 0);
  REQUIRE(L.at(0, 2) == 0);
  REQUIRE(L.at(1, 0) == 7);
  REQUIRE(L.at(1, 1) == 1);
  REQUIRE(L.at(1, 2) == 0);
  REQUIRE(L.at(2, 0) == 5);
  REQUIRE(L.at(2, 1) == 6);
  REQUIRE(L.at(2, 2) == 1);

  REQUIRE(U.at(0, 0) == 1);
  REQUIRE(U.at(0, 1) == 2);
  REQUIRE(U.at(0, 2) == 3);
  REQUIRE(U.at(1, 0) == 0);
  REQUIRE(U.at(1, 1) == 8);
  REQUIRE(U.at(1, 2) == 4);
  REQUIRE(U.at(2, 0) == 0);
  REQUIRE(U.at(2, 1) == 0);
  REQUIRE(U.at(2, 2) == 9);
}

TEST_CASE("Matrix determinant") {
  Matrix<double> a({{0.448, 0.832, 0.193}, {0.421, 0.784, -0.207}, {-0.319, 0.884, 0.279}});
  REQUIRE(OP::determinant(a) == doctest::Approx(0.2572821));
}

TEST_CASE("Identity Matrix") {
  Matrix<int> a = OP::identity<int>(10);

  for (auto i = 0; i < 10; ++i) {
    for (auto j = 0; j < 10; ++j) {
      if (i == j)
        REQUIRE(a.at(i, j) == 1);
      else
        REQUIRE(a.at(i, j) == 0);
    }
  }
}

TEST_CASE("LU doolittle solver") {
  Matrix<double> A({{0.448, 0.832, 0.193}, {0.421, 0.784, -0.207}, {-0.319, 0.884, 0.279}});
  Matrix<double> B({{1}, {2}, {0}});

  Matrix<double> LU(A.rows(), A.cols());
  Matrix<int> P = OP::LUP_doolittle(A, LU);

  Matrix<double> y = OP::solve_LU_doolittle(LU, B, P);
  REQUIRE(y.at(0, 0) == doctest::Approx(1.082).epsilon(0.01));
  REQUIRE(y.at(1, 0) == doctest::Approx(1.251).epsilon(0.001));
  REQUIRE(y.at(2, 0) == doctest::Approx(-2.723).epsilon(0.001));
}

TEST_CASE("Inverse Matrix") {
  Matrix<double> a({{1, 2, 3}, {0, 1, 4}, {5, 6, 0}});
  a = OP::inverse(a);

  REQUIRE(a.at(0, 0) == doctest::Approx(-24));
  REQUIRE(a.at(0, 1) == doctest::Approx(18));
  REQUIRE(a.at(0, 2) == doctest::Approx(5));
  REQUIRE(a.at(1, 0) == doctest::Approx(20));
  REQUIRE(a.at(1, 1) == doctest::Approx(-15));
  REQUIRE(a.at(1, 2) == doctest::Approx(-4));
  REQUIRE(a.at(2, 0) == doctest::Approx(-5));
  REQUIRE(a.at(2, 1) == doctest::Approx(4));
  REQUIRE(a.at(2, 2) == doctest::Approx(1));
}

TEST_CASE("Further LU decomposition test") {
  // PA = LU
  // det(PA) == det(LU)
  Matrix<double> A({{1, 2, 3, 4}, {1, 2, -3, -4}, {-1, 0, 2, 3}, {1, -4, -1, 1}});
  Matrix<double> LU(A.rows());
  Matrix<int> P = OP::make_pivot(OP::LUP_doolittle(A, LU));

  Matrix<double> L(LU.rows());
  Matrix<double> U(LU.rows());

  OP::split_LU_doolittle(LU, L, U);

  Matrix<double> b = OP::multiplication<double>(L, U);
  Matrix<double> c = OP::multiplication<double>(P, A);

  REQUIRE(c == b);
  REQUIRE(OP::determinant(c) == doctest::Approx(OP::determinant(b)));
}
