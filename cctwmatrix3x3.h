#ifndef CCTWMATRIX3X3_H
#define CCTWMATRIX3X3_H

#include "cctwvector3d.h"

template <typename T>
class CctwMatrix3x3
{
public:
  CctwMatrix3x3();
  CctwMatrix3x3(CctwVector3D<T> c1, CctwVector3D<T> c2, CctwVector3D<T> c3);
  CctwMatrix3x3(T a11, T a12, T a13, T a21, T a22, T a23, T a31, T a32, T a33);

public:
        T& operator() (int row, int col);
  const T& operator() (int row, int col) const;

  CctwMatrix3x3 operator+ (const CctwMatrix3x3& mat) const; // Matrix addition
  CctwMatrix3x3 operator- (const CctwMatrix3x3& mat) const; // Matrix subtraction
  CctwMatrix3x3 operator* (const CctwMatrix3x3& mat) const; // Matrix multiplication

  CctwVector3D<T>  operator* (const CctwVector3D<T>& vec) const;  // Matrix*Vector multiplication

  void setToIdentity();

  template <typename T1>
  friend class CctwMatrix3x3;

  template <typename T1>
  friend class CctwVector3D;

private:
  T m_Matrix[3][3];
};

typedef CctwMatrix3x3<int>    CctwIntMatrix3x3;
typedef CctwMatrix3x3<double> CctwDoubleMatrix3x3;

#endif // CCTWMATRIX3X3_H
