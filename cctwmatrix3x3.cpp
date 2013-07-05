#include "cctwmatrix3x3.h"

template <typename T>
CctwMatrix3x3<T>::CctwMatrix3x3()
{
  setToIdentity();
}

template <typename T>
CctwMatrix3x3<T>::CctwMatrix3x3(CctwVector3D<T> c1, CctwVector3D<T> c2, CctwVector3D<T> c3)
{
  for (int row=0; row<3; ++row) {
    m_Matrix[0][row]=c1(row);
    m_Matrix[1][row]=c2(row);
    m_Matrix[2][row]=c3(row);
  }
}

template <typename T>
CctwMatrix3x3<T>::CctwMatrix3x3(T a11, T a12, T a13, T a21, T a22, T a23, T a31, T a32, T a33)
{
  m_Matrix[0][0] = a11;
  m_Matrix[0][1] = a21;
  m_Matrix[0][2] = a31;
  m_Matrix[1][0] = a12;
  m_Matrix[1][1] = a22;
  m_Matrix[1][2] = a32;
  m_Matrix[2][0] = a13;
  m_Matrix[2][1] = a23;
  m_Matrix[2][2] = a33;
}

template <typename T>
T& CctwMatrix3x3<T>::operator ()(int row, int col)
{
  return m_Matrix[col][row];
}

template <typename T>
const T& CctwMatrix3x3<T>::operator ()(int row, int col) const
{
  return m_Matrix[col][row];
}

template <typename T>
CctwMatrix3x3<T> CctwMatrix3x3<T>::operator+ (const CctwMatrix3x3<T>& mat) const
{
  CctwMatrix3x3<T> result;
  for (int row = 0; row < 3; ++row) {
      for (int col = 0; col < 3; ++col) {
          result.m_Matrix[col][row] = m_Matrix[col][row] + mat.m_Matrix[col][row];
      }
  }
  return result;
}

template <typename T>
CctwMatrix3x3<T> CctwMatrix3x3<T>::operator- (const CctwMatrix3x3<T>& mat) const
{
  CctwMatrix3x3<T> result;
  for (int row = 0; row < 3; ++row) {
      for (int col = 0; col < 3; ++col) {
          result.m_Matrix[col][row] = m_Matrix[col][row] - mat.m_Matrix[col][row];
      }
  }

  return result;
}

template <typename T>
CctwMatrix3x3<T> CctwMatrix3x3<T>::operator* (const CctwMatrix3x3<T>& mat) const
{
  CctwMatrix3x3<T> result;
  for (int row = 0; row < 3; ++row) {
      for (int col = 0; col < 3; ++col) {
          T sum(0.0f);
          for (int j = 0; j < 3; ++j) {
              sum += m_Matrix[j][row] * mat.m_Matrix[col][j];
          }
          result.m_Matrix[col][row] = sum;
      }
  }
  return result;
}

template <typename T>
CctwVector3D<T>  CctwMatrix3x3<T>::operator *(const CctwVector3D<T>& vec) const
{
  CctwVector3D<T> result;
  for (int col = 0; col < 3; ++col) {
    T sum(0.0f);
    for (int j = 0; j < 3; ++j) {
      sum += m_Matrix[col][j] * vec(j);
    }
    result(col) = sum;
  }
  return result;
}

template <typename T>
void CctwMatrix3x3<T>::setToIdentity()
{
  for (int col = 0; col < 3; ++col) {
      for (int row = 0; row < 3; ++row) {
          if (row == col) {
              m_Matrix[col][row] = 1.0f;
          } else {
              m_Matrix[col][row] = 0.0f;
          }
      }
  }
}

template class CctwMatrix3x3<int>;
template class CctwMatrix3x3<double>;
