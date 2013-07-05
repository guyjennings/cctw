#ifndef CCTWVECTOR3D_H
#define CCTWVECTOR3D_H

template <typename T>
class CctwVector3D
{
public:
  CctwVector3D(T x, T y, T z);
  CctwVector3D();

public:
  T x() const { return m_Vector[0]; }
  T y() const { return m_Vector[1]; }
  T z() const { return m_Vector[2]; }

  T& operator () (int i) { return m_Vector[i]; }
  const T& operator() (int i) const { return m_Vector[i]; }

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

protected:
  T m_Vector[3];
};

typedef CctwVector3D<int>    CctwIntVector3D;
typedef CctwVector3D<double> CctwDoubleVector3D;

#endif // CCTWVECTOR3D_H
