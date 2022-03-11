#ifndef MATRIX_H
#define MATRIX_H
/***************************************************************************************************
 *
 ***************************************************************************************************/
#include <algorithm>
#include <cstring>
#include <exception>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <typeinfo>

template <class T> class Matrix {
public:
  Matrix() = default;

  Matrix(unsigned dimension) {
    _rows = dimension;
    _cols = dimension;
    alloc();

    fill(T());
  }

  Matrix(unsigned rows, unsigned cols, T init_val = T()) {
    _rows = rows;
    _cols = cols;
    alloc();

    fill(init_val);
  }

  Matrix(const Matrix &m) {
    _rows = m.rows();
    _cols = m.cols();
    alloc();

    std::memcpy(_array, m.array(), size() * sizeof(T));
  }

  Matrix(const std::initializer_list<std::initializer_list<T>> &init) {
    _rows = init.size();
    _cols = init.begin()->size();
    alloc();

    unsigned row = 0;
    unsigned col = 0;
    for (const auto &it_r : init) {
      if (it_r.size() != _cols)
        throw std::runtime_error("The row <" + std::to_string(row) +
                                 "> has more/less columns than the others.");
      for (const auto &it_c : it_r)
        _array[index(row, col++)] = it_c;

      col = 0;
      row += 1;
    }
  }

  Matrix(unsigned rows, unsigned cols, const T *array) {
    _rows = rows;
    _cols = cols;
    alloc();

    std::memcpy(_array, array, rows * cols * sizeof(T));
  }

  ~Matrix() { dealloc(); }

  std::string to_string() const {
    std::ostringstream ss;
    ss << *this;
    return ss.str();
  }

  unsigned rows() const { return _rows; }
  unsigned cols() const { return _cols; }
  unsigned size() const { return _cols * _rows; }

  T &at(unsigned row, unsigned col) {
    bound_check(row, col);
    return _array[index(row, col)];
  }

  T &at(unsigned row, unsigned col) const {
    if (row >= _rows || col >= _cols)
      throw std::out_of_range("out of bounds");
    return _array[index(row, col)];
  }

  T &array_access(unsigned index) {
    bound_check(index);
    return _array[index];
  }

  T &array_access(unsigned index) const {
    if (index >= size())
      throw std::out_of_range("out of bounds");
    return _array[index];
  }

  T *array() const { return _array; }

  Matrix &operator=(const Matrix &m) {
    if (&m == this)
      return *this;

    if (_rows != m.rows() || _cols != m.cols()) {
      dealloc();

      _rows = m.rows();
      _cols = m.cols();

      alloc();
    }

    std::memcpy(_array, m.array(), size() * sizeof(T));
    return *this;
  }

  Matrix &operator+=(const Matrix &m) { return (*this = *this + m); }
  Matrix &operator-=(const Matrix &m) { return (*this = *this - m); }
  Matrix &operator*=(const Matrix &m) { return (*this = *this * m); }
  Matrix &operator*=(const T &value) { return (*this = *this * value); }
  Matrix &operator/=(const T &value) { return (*this = *this / value); }

  bool operator==(const Matrix &m) const {
    // @todo: improve this to compare doubles
    if (m.rows() != _rows)
      return false;
    else if (m.cols() != _cols)
      return false;

    for (auto i = _rows; i-- > 0;)
      for (auto j = _cols; j-- > 0;)
        if (at(i, j) != m.at(i, j))
          return false;

    return true;
  }

  bool operator!=(const Matrix &m) const { return !(*this == m); }

  Matrix operator+(const Matrix &m) {
    if (m.cols() != _cols || m.rows() != _rows)
      throw std::domain_error(
          "Two matrices must have an equal number of rows and columns to be added.");

    Matrix rtn(*this);
    for (auto i = size(); i-- > 0;)
      rtn.array_access(i) += m.array_access(i);

    return rtn;
  }

  Matrix operator-(const Matrix &m) {
    if (m.cols() != _cols || m.rows() != _rows)
      throw std::domain_error(
          "Two matrices must have an equal number of rows and columns to be added.");

    Matrix rtn(*this);
    for (auto i = size(); i-- > 0;)
      rtn.array_access(i) -= m.array_access(i);

    return rtn;
  }

  Matrix operator*(const Matrix &m) {
    // naive method
    if (_cols != m.rows())
      throw std::invalid_argument("Number of cols must be equal to rows.");

    Matrix rtn = Matrix(_rows, m.cols());
    for (unsigned i = 0; i < rtn.rows(); ++i)
      for (unsigned j = 0; j < rtn.cols(); ++j)
        for (unsigned k = 0; k < _cols; ++k)
          rtn.at(i, j) += (_array[index(i, k)] * m.at(k, j));

    return rtn;
  }

  Matrix operator*(const T &value) {
    Matrix rtn = Matrix(*this);
    for (auto i = size(); i-- > 0;)
      rtn.array_access(i) *= value;

    return rtn;
  }

  Matrix operator/(const T &value) {
    Matrix rtn = Matrix(*this);
    for (auto i = size(); i-- > 0;)
      rtn.array_access(i) /= value;

    return rtn;
  }

  void fill(const T &value) {
    // in the hope for a compiler optimization :D
    for (unsigned i = 0; i < size(); ++i)
      _array[i] = value;
  }

  void print() {
    std::cout << "Rows: " << _rows << std::endl;
    std::cout << "Cols: " << _cols << std::endl;

    std::cout.precision(5);

    for (unsigned i = 0; i < _rows; i++) {
      std::cout << std::setw(10);
      for (unsigned j = 0; j < _cols; j++)
        std::cout << _array[index(i, j)] << std::setw(10);

      std::cout << std::endl;
    }
  }

  void swap_rows(unsigned row_a, unsigned row_b) {
    bound_check(row_a, 0);
    bound_check(row_b, 0);
    T *tmp = new T[_cols];

    // save row_b to tmp array
    std::memcpy(tmp, _array + (row_b * _cols), sizeof(T) * _cols);

    // copy row_a to row_b
    std::memcpy(_array + (row_b * _cols), _array + (row_a * _cols), sizeof(T) * _cols);

    // copy tmp to row_a location
    std::memcpy(_array + (row_a * _cols), tmp, sizeof(T) * _cols);

    delete tmp;
  }

  void swap_cols(unsigned col_a, unsigned col_b) {
    bound_check(0, col_a);
    bound_check(0, col_b);

    // since the matrix is stored in a row major array, it doesn't make sense to use memcpy
    T tmp;
    for (unsigned i = 0; i < _rows; ++i) {
      tmp = _array[index(i, col_b)];
      _array[index(i, col_b)] = _array[index(i, col_a)];
      _array[index(i, col_a)] = tmp;
    }
  }

  void remove_row(unsigned row) {
    bound_check(row, _cols - 1);

    unsigned index_del = row * _cols;
    unsigned index_next = (row + 1) * _cols;

    T *tmp_array = new T[(_rows - 1) * _cols];

    std::memmove(tmp_array, _array, (index_del) * sizeof(T));
    std::memmove(tmp_array + index_del, _array + index_next, (size() - index_next) * sizeof(T));
    _rows -= 1;

    dealloc();
    _array = tmp_array;
  }

  void remove_col(unsigned col) {
    bound_check(_rows - 1, col);

    unsigned sIndexN = 0; // start index new array
    unsigned sIndexO = 0; // start index old array
    unsigned nIndex = 0;  // next index old array
    T *tmp_array = new T[_rows * (_cols - 1)];

    std::memmove(tmp_array, _array, col * sizeof(T));
    for (unsigned i = 0; i < (_rows - 1); ++i) {
      sIndexN = i * (_cols - 1) + col;
      sIndexO = i * _cols + col;
      nIndex = (i + 1) * _cols + col;
      std::memmove(tmp_array + sIndexN, _array + sIndexO + 1, (nIndex - sIndexO - 1) * sizeof(T));
    }

    // last "row" needs to be manually set
    sIndexN = (_rows - 1) * (_cols - 1) + col; // last row (or index) of the new array
    sIndexO = (_rows - 1) * _cols + col;       // last row (or index) of the old array
    std::memmove(tmp_array + sIndexN, _array + sIndexO + 1, (size() - sIndexO) * sizeof(T));

    _cols -= 1;
    dealloc();
    _array = tmp_array;
  }

  void pop_back_col() { remove_col(_cols - 1); }
  void pop_front_col() { remove_col(0); }

  void pop_back_row() { remove_row(_rows - 1); }
  void pop_front_row() { remove_row(0); }

  void reshape(unsigned rows, unsigned cols) {
    if ((rows * cols) != size())
      throw std::invalid_argument("Incompatible shape");

    _rows = rows;
    _cols = cols;
  }

  void append_row(const std::initializer_list<T> &values) { insert_row(_rows, values); }
  void prepend_row(const std::initializer_list<T> &values) { insert_row(0, values); }

  void append_col(const std::initializer_list<T> &values) { insert_col(_cols, values); }
  void prepend_col(const std::initializer_list<T> &values) { insert_col(0, values); }

  void insert_row(unsigned index, const std::initializer_list<T> &values) {
    if (index > _rows)
      index = _rows;

    unsigned index_add = index * _cols;
    unsigned index_next = (index + 1) * _cols;

    T *tmp_array = new T[(_rows + 1) * _cols];

    std::memmove(tmp_array, _array, (index_add) * sizeof(T));

    for (unsigned col = 0, i = index_add; i < index_next; ++i, ++col)
      if (col < values.size())
        tmp_array[i] = *(values.begin() + col);
      else
        tmp_array[i] = T();

    std::memmove(tmp_array + index_next, _array + index_add, (size() - index_add) * sizeof(T));

    _rows += 1;
    dealloc();
    _array = tmp_array;
  }

  void insert_col(unsigned index, const std::initializer_list<T> &values) {
    if (index > _cols)
      index = _cols;

    unsigned sIndexO = 0; // start index old array
    unsigned pos = 0;     // index of insertion in new array
    T *tmp_array = new T[_rows * (_cols + 1)];

    std::memmove(tmp_array, _array, index * sizeof(T));

    // adding the values
    for (unsigned i = 0; i < (_rows - 1); ++i) {
      pos = i * (_cols + 1) + index;
      sIndexO = i * _cols + index;
      // nIndex = (i + 1) * _cols + index;

      if (i < values.size())
        tmp_array[pos] = *(values.begin() + i);
      else
        tmp_array[pos] = T();

      std::memmove(tmp_array + pos + 1, _array + sIndexO, _cols * sizeof(T));
    }

    // last "row" needs to be manually set
    pos = (_rows - 1) * (_cols + 1) + index; // last index of the new array
    sIndexO = (_rows - 1) * _cols + index;   // last index of the old array
    std::memmove(tmp_array + pos + 1, _array + sIndexO, (size() - sIndexO) * sizeof(T));

    if ((_rows - 1) < values.size())
      tmp_array[pos] = *(values.begin() + (_rows - 1));
    else
      tmp_array[pos] = T();

    _cols += 1;
    dealloc();
    _array = tmp_array;
  }

private:
  T *_array = nullptr;
  unsigned _rows = 0;
  unsigned _cols = 0;

  unsigned index(unsigned row, unsigned col) const { return row * _cols + col; }

  void alloc() {
    if (_array != nullptr)
      dealloc();

    _array = new T[_rows * _cols];
  }

  void dealloc() {
    if (_array != nullptr) {
      delete[] _array;
      _array = nullptr;
    }
  }

  void bound_check(unsigned row, unsigned col) {
    if (row >= _rows || col >= _cols)
      throw std::out_of_range("out of range");
  }

  void bound_check(unsigned index) {
    if (index >= size())
      throw std::out_of_range("out of range");
  }
};
/***************************************************************************************************
 *
 ***************************************************************************************************/
template <class T> inline std::ostream &operator<<(std::ostream &outs, const Matrix<T> &m) {
  return outs << "Matrix<" << typeid(T).name() << ">[" << m.rows() << "][" << m.cols() << "]";
}

#endif // MATRIX_H
/***************************************************************************************************
 *
 ***************************************************************************************************/
