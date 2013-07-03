#ifndef CCTWVECTOR3D_H
#define CCTWVECTOR3D_H

template <typename T>
class CctwVector3D
{
public:
  CctwVector3D(T x, T y, T z);
  CctwVector3D();

public:
  T x() const { return m_X; }
  T y() const { return m_Y; }
  T z() const { return m_Z; }

  CctwVector3D<T> operator+(const CctwVector3D<T> &vec) const;
  CctwVector3D<T> operator-(const CctwVector3D<T> &vec) const;
  CctwVector3D<T> operator*(const CctwVector3D<T> &vec) const;
  CctwVector3D<T> operator/(const CctwVector3D<T> &vec) const;

  CctwVector3D<T> operator+(const T val) const;
  CctwVector3D<T> operator-(const T val) const;
  CctwVector3D<T> operator*(const T val) const;
  CctwVector3D<T> operator/(const T val) const;

//  template <typename T2>
//  CctwVector3D<T> operator+(const CctwVector3D<T2> &vec) const;

//  template <typename T2>
//  CctwVector3D<T> operator-(const CctwVector3D<T2> &vec) const;

//  template <typename T2>
//  CctwVector3D<T> operator*(const CctwVector3D<T2> &vec) const;

//  template <typename T2>
//  CctwVector3D<T> operator/(const CctwVector3D<T2> &vec) const;

  CctwVector3D<T> min(const CctwVector3D<T> &vec) const;
  CctwVector3D<T> max(const CctwVector3D<T> &vec) const;

private:
  T m_X;
  T m_Y;
  T m_Z;
};

typedef CctwVector3D<int>    CctwIntVector3D;
typedef CctwVector3D<double> CctwDoubleVector3D;

#endif // CCTWVECTOR3D_H
