#include "cctwvector3d.h"

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
