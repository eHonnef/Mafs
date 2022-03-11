// /***************************************************************************************************
//  *
//  ***************************************************************************************************/
// #ifndef MATRIX_OPENMP_OPERATIONS_H
// #define MATRIX_OPENMP_OPERATIONS_H

// #include "Matrix.cc"
// #include <cmath>

// class openmp_op {
// public:
//   /*
//    * Returns the transpose of m.
//    */
//   template <typename T> static Matrix<T> transpose(const Matrix<T> &m) {}
//   /*
//    * Returns the upper triangular matrix of m.
//    * Matrix m must be square.
//    */
//   template <typename T> static Matrix<T> u_triangular(const Matrix<T> &m) {}
//   /*
//    * Returns the lower triangular matrix of m.
//    * Matrix m must be square.
//    */
//   template <typename T> static Matrix<T> l_triangular(const Matrix<T> &m) {}
//   /*
//    * Gets the array matrix from LUP's return and make the pivot matrix.
//    * Returns the pivot matrix.
//    */
//   static Matrix<int> make_pivot(const Matrix<int> &m) {}
//   /*
//    * Decomposes m into L and U using doolittle's method with partial pivoting.
//    * Matrices m and LU needs to be the same dimension.
//    * It'll change the LU matrix param.
//    * Returns the pivot vector, it is a 1x(N+1) matrix, where the element N+1 is the number of
//    * row exchanges needed for determinant calculation.
//    * Probably it'll give the wrong result if T isn't a double or float.
//    */
//   template <typename T> static Matrix<int> LUP_doolittle(const Matrix<T> &m, Matrix<T> &LU) {}
//   /*
//    * Splits the LU matrix (compact version) into L matrix and U matrix.
//    * L and U need to be the same dimension of LU.
//    * Params L and U will be modified.
//    */
//   template <typename T>
//   static void split_LU_doolittle(const Matrix<T> &LU, Matrix<T> &L, Matrix<T> &U) {}
//   /*
//    * Solves the X vector of the linear equation A*X=B using the LU and P matrix filled at
//    * LUP_doolittle, B is a vector given by the user. It'll return the vector X.
//    */
//   template <typename T>
//   static Matrix<T> solve_LU_doolittle(const Matrix<T> &LU, const Matrix<T> &B,
//                                       const Matrix<int> &P) {}
//   /*
//    * Forward call to the other solve_LU_doolittle function but only taking the matrices A and B.
//    */
//   template <typename T>
//   static Matrix<T> solve_LU_doolittle(const Matrix<T> &A, const Matrix<T> &B) {}
//   /*
//    * Returns the determinant of the NxN matrix m using doolittle's LU decomposition with partial
//    * pivoting.
//    * Matrix m must be a square matrix.
//    */
//   template <typename T> static T determinant(const Matrix<T> &m) {}
//   /*
//    * Returns the determinant of the NxN matrix given the matrix filled in LUP_doolittle.
//    * Matrix LU is filled at LUP_doolittle.
//    * Matrix pivot is the return of LUP_doolittle.
//    */
//   template <typename T> static T determinant_LU(const Matrix<T> &LU, const Matrix<int> &pivot) {}
//   /*
//    * Returns the inverse matrix of LU.
//    * Matrices LU and p are filled at LUP_doolittle.
//    */
//   template <typename T> static Matrix<T> inverse_LU(const Matrix<T> &LU, const Matrix<int> &p) {}
//   /*
//    * Returns the inverse matrix of m. Forwarding to the function inverse_LU.
//    * Matrix m must be square.
//    */
//   template <typename T> static Matrix<T> inverse(const Matrix<T> &m) {}
//   /*
//    * Returns the result of the multiplication of m1 * m2.
//    * The types U and V must be compatible.
//    * It'll return a matrix of type T.
//    */
//   template <typename T, typename U, typename V>
//   static Matrix<T> multiplication(const Matrix<U> &m1, const Matrix<V> &m2) {}
//   /*
//    * Returns an identity matrix.
//    * Param dimension is (duh) the desired matrix dimension.
//    */
//   template <typename T> static Matrix<T> identity(unsigned dimension) {}

// private:
// };
// #endif
// /***************************************************************************************************
//  *
//  ***************************************************************************************************/