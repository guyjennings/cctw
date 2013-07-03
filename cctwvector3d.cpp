#include "cctwvector3d.h"

template <typename T>
CctwVector3D<T>::CctwVector3D(T x, T y, T z)
  : m_X(x),
    m_Y(y),
    m_Z(z)
{
}

template <typename T>
CctwVector3D<T>::CctwVector3D()
  : m_X(0), m_Y(0), m_Z(0)
{
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
  return CctwVector3D<T>(x()/vec.x(), y()/vec.y(), z()/vec.z());
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

template class CctwVector3D<int>;
template class CctwVector3D<double>;
