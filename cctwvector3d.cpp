#include "cctwvector3d.h"
#include <math.h>

template <typename T>
CctwVector3D<T>::CctwVector3D(T x, T y, T z)
{
  m_Vector[0] = x;
  m_Vector[1] = y;
  m_Vector[2] = z;
}

template <typename T>
CctwVector3D<T>::CctwVector3D()
{
  m_Vector[0] = 0;
  m_Vector[1] = 0;
  m_Vector[2] = 0;
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::operator+(const CctwVector3D<T> &vec) const
{
  return CctwVector3D<T>(x()+vec.x(), y()+vec.y(), z()+vec.z());
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::operator-(const CctwVector3D<T> &vec) const
{
  return CctwVector3D<T>(x()-vec.x(), y()-vec.y(), z()-vec.z());
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::operator*(const CctwVector3D<T> &vec) const
{
  return CctwVector3D<T>(x()*vec.x(), y()*vec.y(), z()*vec.z());
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::operator/(const CctwVector3D<T> &vec) const
{
  return CctwVector3D<T>(floor((double)x()/(double)vec.x()),
                         floor((double)y()/(double)vec.y()),
                         floor((double)z()/(double)vec.z()));
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::operator+(const T val) const
{
  return CctwVector3D<T>(x()+val, y()+val, z()+val);
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::operator-(const T val) const
{
  return CctwVector3D<T>(x()-val, y()-val, z()-val);
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::operator*(const T val) const
{
  return CctwVector3D<T>(x()*val, y()*val, z()*val);
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::operator/(const T val) const
{
  return CctwVector3D<T>(x()/val, y()/val, z()/val);
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::min(const CctwVector3D<T> &vec) const
{
  return CctwVector3D<T>((x()<vec.x()?x():vec.x()),
                         (y()<vec.y()?y():vec.y()),
                         (z()<vec.z()?z():vec.z()));
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::max(const CctwVector3D<T> &vec) const
{
  return CctwVector3D<T>((x()>vec.x()?x():vec.x()),
                         (y()>vec.y()?y():vec.y()),
                         (z()>vec.z()?z():vec.z()));
}

template <typename T>
T CctwVector3D<T>::length() const
{
  return ::sqrt((double) lengthSquared());
}

template <typename T>
T CctwVector3D<T>::lengthSquared() const
{
  return x()*x() + y()*y() + z()*z();
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::normalized() const
{
  T len = length();

  if (::fabs((double)len) < 1e-10) {
    return CctwVector3D<T>();
  } else {
    T sqrtLen = ::sqrt((double)len);
    return CctwVector3D(x() / sqrtLen,
                        y() / sqrtLen,
                        z() / sqrtLen);
  }
}

template <typename T>
void CctwVector3D<T>::normalize()
{
  T len = length();

  if (::fabs((double)len) < 1e-10) {
    return;
  } else {
    T sqrtLen = ::sqrt((double)len);
    m_Vector[0] /= sqrtLen;
    m_Vector[1] /= sqrtLen;
    m_Vector[2] /= sqrtLen;
  }
}

template <typename T>
T CctwVector3D<T>::dotProduct(const CctwVector3D<T> &v1, const CctwVector3D<T> &v2)
{
  return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::crossProduct(const CctwVector3D<T>& v1, const CctwVector3D<T>& v2)
{
  return CctwVector3D<T>(v1.y()*v2.z() - v1.z()*v2.y(),
                         v1.z()*v2.x() - v1.x()*v2.z(),
                         v1.x()*v2.y() - v1.y()*v2.x());
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::normal(const CctwVector3D<T> &v1, const CctwVector3D<T> &v2)
{
  return crossProduct(v1, v2).normalized();
}

template class CctwVector3D<int>;
template class CctwVector3D<double>;
