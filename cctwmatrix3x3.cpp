#include "cctwmatrix3x3.h"
#include <math.h>

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
CctwMatrix3x3<T> CctwMatrix3x3<T>::operator+= (const CctwMatrix3x3<T>& mat)
{
  for (int row = 0; row < 3; ++row) {
      for (int col = 0; col < 3; ++col) {
          m_Matrix[col][row] += mat.m_Matrix[col][row];
      }
  }
  return *this;
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
CctwMatrix3x3<T> CctwMatrix3x3<T>::operator-= (const CctwMatrix3x3<T>& mat)
{
  for (int row = 0; row < 3; ++row) {
      for (int col = 0; col < 3; ++col) {
          m_Matrix[col][row] -= mat.m_Matrix[col][row];
      }
  }

  return *this;
}

template <typename T>
CctwMatrix3x3<T> CctwMatrix3x3<T>::operator * (const CctwMatrix3x3<T>& mat) const
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
CctwVector3D<T>  CctwMatrix3x3<T>::operator * (const CctwVector3D<T>& vec) const
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
bool CctwMatrix3x3<T>::operator == (const CctwMatrix3x3<T>& mat) const
{
  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      if (m_Matrix[col][row] != mat.m_Matrix[col][row]) {
        return false;
      }
    }
  }

  return true;
}

template <typename T>
bool CctwMatrix3x3<T>::operator != (const CctwMatrix3x3<T>& mat) const
{
  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      if (m_Matrix[col][row] != mat.m_Matrix[col][row]) {
        return true;
      }
    }
  }

  return false;
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

template <typename T>
double CctwMatrix3x3<T>::determinant() const
{
  double
      a = m_Matrix[0][0],
      b = m_Matrix[1][0],
      c = m_Matrix[2][0],
      d = m_Matrix[0][1],
      e = m_Matrix[1][1],
      f = m_Matrix[2][1],
      g = m_Matrix[0][2],
      h = m_Matrix[1][2],
      k = m_Matrix[2][2];

  return a*(e*k - f*h) + b*(f*g - d*k) + c*(d*h - e*g);
}

template <typename T>
CctwMatrix3x3<T> CctwMatrix3x3<T>::inverted(bool *invertible) const
{
  CctwMatrix3x3<T> inv;
  double det = determinant();

  if (det != 0.0) {
    double
        a = m_Matrix[0][0],
        b = m_Matrix[1][0],
        c = m_Matrix[2][0],
        d = m_Matrix[0][1],
        e = m_Matrix[1][1],
        f = m_Matrix[2][1],
        g = m_Matrix[0][2],
        h = m_Matrix[1][2],
        k = m_Matrix[2][2];

    inv.m_Matrix[0][0] = (e*k - f*h)/det;
    inv.m_Matrix[1][0] =-(b*k - c*h)/det;
    inv.m_Matrix[2][0] = (b*f - c*e)/det;

    inv.m_Matrix[0][1] =-(d*k - f*g)/det;
    inv.m_Matrix[1][1] = (a*k - c*g)/det;
    inv.m_Matrix[2][1] =-(a*f - c*d)/det;

    inv.m_Matrix[0][2] = (d*h - e*g)/det;
    inv.m_Matrix[1][2] =-(a*h - b*g)/det;
    inv.m_Matrix[2][2] = (a*e - b*d)/det;

    if (invertible) {
      *invertible = true;
    }
  } else {
    if (invertible) {
      *invertible = false;
    }
  }

  return inv;
}

template <typename T>
CctwMatrix3x3<T> CctwMatrix3x3<T>::transposed() const
{
  CctwMatrix3x3<T> tr;

  for (int i=0; i<3; i++) {
    for (int j=0; j<3; j++) {
      tr(j,i) = operator() (i,j);
    }
  }

  return tr;
}

template <typename T>
CctwMatrix3x3<T> CctwMatrix3x3<T>::rotationMatrix(double r1, double r2, double r3)
{
  CctwMatrix3x3<T> rx = CctwMatrix3x3<T>::rotX(r1);
  CctwMatrix3x3<T> ry = CctwMatrix3x3<T>::rotY(r2);
  CctwMatrix3x3<T> rz = CctwMatrix3x3<T>::rotZ(r3);

  return rx*ry*rz;
}

template <typename T>
CctwMatrix3x3<T> CctwMatrix3x3<T>::rotX(double r)
{
  CctwMatrix3x3<T> res;
  T cosr = ::cos(r),
    sinr = ::sin(r);

  res.m_Matrix[0][0] = 1;
  res.m_Matrix[1][0] = 0;
  res.m_Matrix[2][0] = 0;

  res.m_Matrix[0][1] = 0;
  res.m_Matrix[1][1] = cosr;
  res.m_Matrix[2][1] = sinr;

  res.m_Matrix[0][2] = 0;
  res.m_Matrix[1][2] = -sinr;
  res.m_Matrix[2][2] = cosr;

  return res;
}

template <typename T>
CctwMatrix3x3<T> CctwMatrix3x3<T>::rotY(double r)
{
  CctwMatrix3x3<T> res;
  T cosr = ::cos(r),
    sinr = ::sin(r);

  res.m_Matrix[0][0] = cosr;
  res.m_Matrix[1][0] = 0;
  res.m_Matrix[2][0] = -sinr;

  res.m_Matrix[0][1] = 0;
  res.m_Matrix[1][1] = 1;
  res.m_Matrix[2][1] = 0;

  res.m_Matrix[0][2] = sinr;
  res.m_Matrix[1][2] = 0;
  res.m_Matrix[2][2] = cosr;

  return res;
}

template <typename T>
CctwMatrix3x3<T> CctwMatrix3x3<T>::rotZ(double r)
{
  CctwMatrix3x3<T> res;
  T cosr = ::cos(r),
    sinr = ::sin(r);

  res.m_Matrix[0][0] = cosr;
  res.m_Matrix[1][0] = sinr;
  res.m_Matrix[2][0] = 0;

  res.m_Matrix[0][1] = -sinr;
  res.m_Matrix[1][1] = cosr;
  res.m_Matrix[2][1] = 0;

  res.m_Matrix[0][2] = 0;
  res.m_Matrix[1][2] = 0;
  res.m_Matrix[2][2] = 1;

  return res;
}

template <typename T>
CctwMatrix3x3<T> CctwMatrix3x3<T>::zero()
{
  return CctwMatrix3x3<T>(0,0,0,0,0,0,0,0,0);
}

template <typename T>
CctwMatrix3x3<T> CctwMatrix3x3<T>::identity()
{
  return CctwMatrix3x3<T>(1,0,0, 0,1,0, 0,0,1);
}

template class CctwMatrix3x3<int>;
template class CctwMatrix3x3<double>;
