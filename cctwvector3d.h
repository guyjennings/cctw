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

  bool operator==(const CctwVector3D<T> &vec) const;
  bool operator!=(const CctwVector3D<T> &vec) const;

  T length() const;
  T lengthSquared() const;
  T volume() const;

  CctwVector3D<T> normalized() const;
  void normalize();

  static T dotProduct(const CctwVector3D<T>& v1, const CctwVector3D<T>& v2);
  static CctwVector3D<T> crossProduct(const CctwVector3D<T> &v1, const CctwVector3D<T> &v2);
  static CctwVector3D<T> normal(const CctwVector3D<T>& v1, const CctwVector3D<T>& v2);

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
