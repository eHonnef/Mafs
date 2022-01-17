#include "doctest/doctest.h"
#include "Matrix/Matrix.cc"

TEST_CASE("Matrix to string/matrix name") {
  Matrix<int> a(5, 8);
  std::cout << "Print test:" << std::endl;
  std::cout << "This should print \"Matrix<i>[5][8]\": " << a << std::endl;
  std::cout << "This should also print \"Matrix<i>[5][8]\": " << a.to_string() << std::endl;
  a.print();
  std::cout << "=====================================================" << std::endl;
}

TEST_CASE("Matrix errors") {
  REQUIRE_THROWS(Matrix<int>({{0, 1, 2}, {3, 4, 5}, {6, 7}}));
  REQUIRE_THROWS(Matrix<int>({{0, 1, 2}, {3, 4, 5, 7}, {6, 7, 8}}));

  Matrix<int> a({{0, 1, 2}, {3, 4, 5}});
  REQUIRE_THROWS(a.at(10, 10));
}

TEST_CASE("Matrix structure") {
  Matrix<int> a(5, 8);
  REQUIRE(a.rows() == 5);
  REQUIRE(a.cols() == 8);
  REQUIRE(a.size() == (5 * 8));
}

TEST_CASE("Matrix access and values") {
  Matrix<int> a(5, 8);
  Matrix<int> b(5, 8, 10);

  a.at(4, 4) = 123;
  a.at(3, 3) = 987;

  b.at(4, 4) = 1234;
  b.at(3, 3) = 654;

  REQUIRE(a.at(1, 3) == int());
  REQUIRE(a.at(4, 4) == 123);
  REQUIRE(a.at(3, 3) == 987);

  REQUIRE(b.at(4, 4) == 1234);
  REQUIRE(b.at(3, 3) == 654);
  REQUIRE(b.at(1, 4) == 10);
}

TEST_CASE("Matrix brace initialize") {
  Matrix<int> a({{0, 1, 2}, {3, 4, 5}});
  REQUIRE(a.rows() == 2);
  REQUIRE(a.cols() == 3);

  REQUIRE(a.at(0, 0) == 0);
  REQUIRE(a.at(0, 1) == 1);
  REQUIRE(a.at(0, 2) == 2);
  REQUIRE(a.at(1, 0) == 3);
  REQUIRE(a.at(1, 1) == 4);
  REQUIRE(a.at(1, 2) == 5);
}

TEST_CASE("Remove matrix row") {
  Matrix<int> a(5, 8, 5);
  REQUIRE_THROWS(a.remove_row(5));

  // setting the values of row 1 to 3
  for (auto i = a.cols(); i-- > 0;)
    a.at(1, i) = 3;

  a.remove_row(1);

  REQUIRE(a.rows() == 4);
  // checking if every element is 5
  for (auto i = a.rows(); i-- > 0;)
    for (auto j = a.cols(); j-- > 0;)
      REQUIRE(a.at(i, j) == 5);
}

TEST_CASE("Matrix pop back row") {
  Matrix<int> a(5, 8, 5);

  // setting the values of row 4 to 3
  for (auto i = a.cols(); i-- > 0;)
    a.at(4, i) = 3;

  a.pop_back_row();

  REQUIRE(a.rows() == 4);
  // checking if every element is 5
  for (auto i = a.rows(); i-- > 0;)
    for (auto j = a.cols(); j-- > 0;)
      REQUIRE(a.at(i, j) == 5);
}

TEST_CASE("Matrix pop front row") {
  Matrix<int> a(5, 8, 5);

  // setting the values of row 0 to 3
  for (auto i = a.cols(); i-- > 0;)
    a.at(0, i) = 3;

  a.pop_front_row();

  REQUIRE(a.rows() == 4);
  // checking if every element is 5
  for (auto i = a.rows(); i-- > 0;)
    for (auto j = a.cols(); j-- > 0;)
      REQUIRE(a.at(i, j) == 5);
}

TEST_CASE("Remove matrix col") {
  Matrix<int> a(5, 8, 5);
  REQUIRE_THROWS(a.remove_col(9));

  // setting the values of col 3 to 9
  for (auto i = a.rows(); i-- > 0;)
    a.at(i, 3) = 9;

  a.remove_col(3);

  REQUIRE(a.cols() == 7);
  for (auto i = a.rows(); i-- > 0;)
    for (auto j = a.cols(); j-- > 0;)
      REQUIRE(a.at(i, j) == 5);
}

TEST_CASE("Matrix pop back col") {
  Matrix<int> a(5, 8, 5);

  // setting the values of col 7 to 9
  for (auto i = a.rows(); i-- > 0;)
    a.at(i, 7) = 9;

  a.pop_back_col();

  REQUIRE(a.cols() == 7);
  for (auto i = a.rows(); i-- > 0;)
    for (auto j = a.cols(); j-- > 0;)
      REQUIRE(a.at(i, j) == 5);
}

TEST_CASE("Matrix pop front col") {
  Matrix<int> a(5, 8, 5);

  // setting the values of col 0 to 9
  for (auto i = a.rows(); i-- > 0;)
    a.at(i, 0) = 9;

  a.pop_front_col();

  REQUIRE(a.cols() == 7);
  for (auto i = a.rows(); i-- > 0;)
    for (auto j = a.cols(); j-- > 0;)
      REQUIRE(a.at(i, j) == 5);
}

TEST_CASE("Matrix row insertion") {
  Matrix<int> a(5, 2, 5);

  a.insert_row(2, {9, 1});
  REQUIRE(a.at(2, 0) == 9);
  REQUIRE(a.at(2, 1) == 1);

  a.insert_row(2, {87});
  REQUIRE(a.at(2, 0) == 87);
  REQUIRE(a.at(2, 1) == int());

  a.insert_row(2, {7, 8, 2});
  REQUIRE(a.at(2, 0) == 7);
  REQUIRE(a.at(2, 1) == 8);
}

TEST_CASE("Matrix append row") {
  Matrix<int> a(5, 2, 5);

  a.append_row({9, 1});
  REQUIRE(a.at(5, 0) == 9);
  REQUIRE(a.at(5, 1) == 1);

  a.append_row({87});
  REQUIRE(a.at(6, 0) == 87);
  REQUIRE(a.at(6, 1) == int());

  a.append_row({7, 8, 2});
  REQUIRE(a.at(7, 0) == 7);
  REQUIRE(a.at(7, 1) == 8);
}

TEST_CASE("Matrix prepend row") {
  Matrix<int> a(5, 2, 5);

  a.prepend_row({9, 1});
  REQUIRE(a.at(0, 0) == 9);
  REQUIRE(a.at(0, 1) == 1);

  a.prepend_row({87});
  REQUIRE(a.at(0, 0) == 87);
  REQUIRE(a.at(0, 1) == int());

  a.prepend_row({7, 8, 2});
  REQUIRE(a.at(0, 0) == 7);
  REQUIRE(a.at(0, 1) == 8);
}

TEST_CASE("Matrix col insertion") {
  Matrix<int> a(2, 5, 5);

  a.insert_col(2, {9, 1});
  REQUIRE(a.at(0, 2) == 9);
  REQUIRE(a.at(1, 2) == 1);

  a.insert_col(2, {87});
  REQUIRE(a.at(0, 2) == 87);
  REQUIRE(a.at(1, 2) == int());

  a.insert_col(2, {7, 8, 2});
  REQUIRE(a.at(0, 2) == 7);
  REQUIRE(a.at(1, 2) == 8);
}

TEST_CASE("Matrix append col") {
  Matrix<int> a(2, 5, 5);

  a.append_col({9, 1});
  REQUIRE(a.at(0, 5) == 9);
  REQUIRE(a.at(1, 5) == 1);

  a.append_col({87});
  REQUIRE(a.at(0, 6) == 87);
  REQUIRE(a.at(1, 6) == int());

  a.append_col({7, 8, 2});
  REQUIRE(a.at(0, 7) == 7);
  REQUIRE(a.at(1, 7) == 8);
}

TEST_CASE("Matrix prepend col") {
  Matrix<int> a(2, 5, 5);

  a.prepend_col({9, 1});
  REQUIRE(a.at(0, 0) == 9);
  REQUIRE(a.at(1, 0) == 1);

  a.prepend_col({87});
  REQUIRE(a.at(0, 0) == 87);
  REQUIRE(a.at(1, 0) == int());

  a.prepend_col({7, 8, 2});
  REQUIRE(a.at(0, 0) == 7);
  REQUIRE(a.at(1, 0) == 8);
}

TEST_CASE("Matrix equality") {
  Matrix<int> a(5, 8);
  Matrix<int> b(5, 8, 10);
  Matrix<int> c(5, 8, 10);
  Matrix<int> d(5, 7, 10);

  REQUIRE(a != b);
  REQUIRE(b == c);
  REQUIRE(c != d);
}

TEST_CASE("Matrix reshape") {
  Matrix<int> a(2, 3);
  for (auto i = a.rows(); i-- > 0;)
    for (auto j = a.cols(); j-- > 0;)
      a.at(i, j) = i * a.cols() + j;

  a.reshape(3, 2);
  REQUIRE(a.rows() == 3);
  REQUIRE(a.cols() == 2);

  REQUIRE(a.at(0, 0) == 0);
  REQUIRE(a.at(0, 1) == 1);
  REQUIRE(a.at(1, 0) == 2);
  REQUIRE(a.at(1, 1) == 3);
  REQUIRE(a.at(2, 0) == 4);
  REQUIRE(a.at(2, 1) == 5);
}

TEST_CASE("Matrix reassign") {
  Matrix<int> a(2, 3, 10);
  Matrix<int> b(546, 10, 3);

  b = a;
  REQUIRE(a == b);
}

TEST_CASE("Row swap") {
  Matrix<int> a(4, 4, 7);

  for (auto i = a.cols(); i-- > 0;)
    a.at(0, i) = 5;

  a.swap_rows(0, 3);
  for (auto i = a.cols(); i-- > 0;)
    REQUIRE(a.at(3, i) == 5);
}

TEST_CASE("Col swap") {
  Matrix<int> a(4, 4, 7);

  for (auto i = a.rows(); i-- > 0;)
    a.at(i, 0) = 5;

  a.swap_cols(0, 3);
  for (auto i = a.rows(); i-- > 0;)
    REQUIRE(a.at(i, 3) == 5);
}

TEST_CASE("Matrix scalar multiplication") {
  Matrix<int> a({{0, 1, 2}, {3, 4, 5}});
  a *= 10;

  REQUIRE(a.at(0, 0) == 0);
  REQUIRE(a.at(0, 1) == 10);
  REQUIRE(a.at(0, 2) == 20);
  REQUIRE(a.at(1, 0) == 30);
  REQUIRE(a.at(1, 1) == 40);
  REQUIRE(a.at(1, 2) == 50);
}

TEST_CASE("Matrix division") {
  Matrix<int> a({{0, 1, 2}, {3, 4, 5}});
  a *= 10;
  a /= 10;

  REQUIRE(a.at(0, 0) == 0);
  REQUIRE(a.at(0, 1) == 1);
  REQUIRE(a.at(0, 2) == 2);
  REQUIRE(a.at(1, 0) == 3);
  REQUIRE(a.at(1, 1) == 4);
  REQUIRE(a.at(1, 2) == 5);
}

TEST_CASE("Sum of Matrices") {
  Matrix<int> a({{0, 1, 2}, {3, 4, 5}});
  Matrix<int> b({{0, 1, 2}, {3, 4, 5}});
  Matrix<int> c({{1, 2}, {4, 5}});

  REQUIRE_THROWS(c += a);

  a += b;
  REQUIRE(a.at(0, 0) == 0);
  REQUIRE(a.at(0, 1) == 2);
  REQUIRE(a.at(0, 2) == 4);
  REQUIRE(a.at(1, 0) == 6);
  REQUIRE(a.at(1, 1) == 8);
  REQUIRE(a.at(1, 2) == 10);
}

TEST_CASE("Subtraction of Matrices") {
  Matrix<int> a({{0, 1, 2}, {3, 4, 5}});
  Matrix<int> b({{0, 1, 2}, {3, 4, 5}});
  Matrix<int> c({{1, 2}, {4, 5}});

  REQUIRE_THROWS(c -= a);

  a -= b;
  REQUIRE(a.at(0, 0) == 0);
  REQUIRE(a.at(0, 1) == 0);
  REQUIRE(a.at(0, 2) == 0);
  REQUIRE(a.at(1, 0) == 0);
  REQUIRE(a.at(1, 1) == 0);
  REQUIRE(a.at(1, 2) == 0);
}

TEST_CASE("Matrices multiplication") {
  Matrix<int> a({{0, 1, 2}, {3, 4, 5}, {6, 7, 8}});
  Matrix<int> b({{0}, {1}, {2}});
  Matrix<int> c = a * b;

  REQUIRE(c.at(0, 0) == 5);
  REQUIRE(c.at(1, 0) == 14);
  REQUIRE(c.at(2, 0) == 23);

  a *= b;
  REQUIRE(a.at(0, 0) == 5);
  REQUIRE(a.at(1, 0) == 14);
  REQUIRE(a.at(2, 0) == 23);

  REQUIRE_THROWS(a *= b);
}