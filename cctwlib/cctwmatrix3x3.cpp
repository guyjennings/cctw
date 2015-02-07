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
    m_Matrix[row][0]=c1(row);
    m_Matrix[row][1]=c2(row);
    m_Matrix[row][2]=c3(row);
  }
}

template <typename T>
CctwMatrix3x3<T>::CctwMatrix3x3(T a11, T a12, T a13, T a21, T a22, T a23, T a31, T a32, T a33)
{
  m_Matrix[0][0] = a11;
  m_Matrix[0][1] = a12;
  m_Matrix[0][2] = a13;
  m_Matrix[1][0] = a21;
  m_Matrix[1][1] = a22;
  m_Matrix[1][2] = a23;
  m_Matrix[2][0] = a31;
  m_Matrix[2][1] = a32;
  m_Matrix[2][2] = a33;
}

template <typename T>
T& CctwMatrix3x3<T>::operator ()(int row, int col)
{
  return m_Matrix[row][col];
}

template <typename T>
const T& CctwMatrix3x3<T>::operator ()(int row, int col) const
{
  return m_Matrix[row][col];
}

template <typename T>
CctwMatrix3x3<T> CctwMatrix3x3<T>::operator+ (const CctwMatrix3x3<T>& mat) const
{
  CctwMatrix3x3<T> result;
  for (int row = 0; row < 3; ++row) {
      for (int col = 0; col < 3; ++col) {
          result.m_Matrix[row][col] = m_Matrix[row][col] + mat.m_Matrix[row][col];
      }
  }
  return result;
}

template <typename T>
CctwMatrix3x3<T> CctwMatrix3x3<T>::operator+= (const CctwMatrix3x3<T>& mat)
{
  for (int row = 0; row < 3; ++row) {
      for (int col = 0; col < 3; ++col) {
          m_Matrix[row][col] += mat.m_Matrix[row][col];
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
          result.m_Matrix[row][col] = m_Matrix[row][col] - mat.m_Matrix[row][col];
      }
  }

  return result;
}

template <typename T>
CctwMatrix3x3<T> CctwMatrix3x3<T>::operator-= (const CctwMatrix3x3<T>& mat)
{
  for (int row = 0; row < 3; ++row) {
      for (int col = 0; col < 3; ++col) {
          m_Matrix[row][col] -= mat.m_Matrix[row][col];
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
              sum += m_Matrix[row][j] * mat.m_Matrix[j][col];
          }
          result.m_Matrix[row][col] = sum;
      }
  }
  return result;
}

template <typename T>
CctwVector3D<T>  CctwMatrix3x3<T>::operator * (const CctwVector3D<T>& vec) const
{
  CctwVector3D<T> result;
  for (int row = 0; row < 3; ++row) {
    T sum(0.0f);
    for (int j = 0; j < 3; ++j) {
      sum += m_Matrix[row][j] * vec(j);
    }
    result(row) = sum;
  }
  return result;
}

template <typename T>
bool CctwMatrix3x3<T>::operator == (const CctwMatrix3x3<T>& mat) const
{
  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      if (m_Matrix[row][col] != mat.m_Matrix[row][col]) {
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
      if (m_Matrix[row][col] != mat.m_Matrix[row][col]) {
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
              m_Matrix[row][col] = 1.0f;
          } else {
              m_Matrix[row][col] = 0.0f;
          }
      }
  }
}

template <typename T>
double CctwMatrix3x3<T>::determinant() const
{
  double
      a = m_Matrix[0][0],
      b = m_Matrix[0][1],
      c = m_Matrix[0][2],
      d = m_Matrix[1][0],
      e = m_Matrix[1][1],
      f = m_Matrix[1][2],
      g = m_Matrix[2][0],
      h = m_Matrix[2][1],
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

  res(1,1) = cosr;
  res(1,2) = -sinr;
  res(2,1) = sinr;
  res(2,2) = cosr;

  return res;
}

template <typename T>
CctwMatrix3x3<T> CctwMatrix3x3<T>::rotY(double r)
{
  CctwMatrix3x3<T> res;
  T cosr = ::cos(r),
    sinr = ::sin(r);

  res(0,0) = cosr;
  res(0,2) = sinr;
  res(2,0) = -sinr;
  res(2,2) = cosr;

  return res;
}

template <typename T>
CctwMatrix3x3<T> CctwMatrix3x3<T>::rotZ(double r)
{
  CctwMatrix3x3<T> res;
  T cosr = ::cos(r),
    sinr = ::sin(r);

  res(0,0) = cosr;
  res(0,1) = -sinr;
  res(1,0) = sinr;
  res(1,1) = cosr;

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

template <typename T>
void CctwMatrix3x3<T>::setSettingsValue(QSettings *settings, QString name)
{
  settings->beginGroup(name);

  for (int r=0; r<3; r++) {
    for (int c=0; c<3; c++) {
      settings->setValue(QString("r%1c%2").arg(r).arg(c), operator()(r,c));
    }
  }

  settings->endGroup();
}

template <>
void CctwMatrix3x3<double>::customSaver(const QVariant &val, QSettings *settings, QString name)
{
  CctwDoubleMatrix3x3 mat = val.value<CctwDoubleMatrix3x3 >();

  mat.setSettingsValue(settings, name);
}

template <>
void CctwMatrix3x3<int>::customSaver(const QVariant &val, QSettings *settings, QString name)
{
  CctwIntMatrix3x3 mat = val.value<CctwIntMatrix3x3 >();

  mat.setSettingsValue(settings, name);
}

template class CctwMatrix3x3<int>;
template class CctwMatrix3x3<double>;
