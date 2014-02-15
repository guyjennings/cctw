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
CctwVector3D<T> CctwVector3D<T>::operator + (const CctwVector3D<T> &vec) const
{
  return CctwVector3D<T>(x()+vec.x(), y()+vec.y(), z()+vec.z());
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::operator - (const CctwVector3D<T> &vec) const
{
  return CctwVector3D<T>(x()-vec.x(), y()-vec.y(), z()-vec.z());
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::operator * (const CctwVector3D<T> &vec) const
{
  return CctwVector3D<T>(x()*vec.x(), y()*vec.y(), z()*vec.z());
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::operator / (const CctwVector3D<T> &vec) const
{
  return CctwVector3D<T>(floor((double)x()/(double)vec.x()),
                         floor((double)y()/(double)vec.y()),
                         floor((double)z()/(double)vec.z()));
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::operator += (const CctwVector3D<T> &vec)
{
  for (int i=0; i<3; i++) {
    m_Vector[i] += vec.m_Vector[i];
  }

  return *this;
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::operator -= (const CctwVector3D<T> &vec)
{
  for (int i=0; i<3; i++) {
    m_Vector[i] -= vec.m_Vector[i];
  }

  return *this;
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::operator + (const T val) const
{
  return CctwVector3D<T>(x()+val, y()+val, z()+val);
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::operator - (const T val) const
{
  return CctwVector3D<T>(x()-val, y()-val, z()-val);
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::operator * (const T val) const
{
  return CctwVector3D<T>(x()*val, y()*val, z()*val);
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::operator / (const T val) const
{
  return CctwVector3D<T>(x()/val, y()/val, z()/val);
}

template <typename T>
bool CctwVector3D<T>::operator == (const CctwVector3D<T> &vec) const
{
  return x()==vec.x() && y()==vec.y() && z()==vec.z();
}

template <typename T>
bool CctwVector3D<T>::operator != (const CctwVector3D<T> &vec) const
{
  return x()!=vec.x() || y()!=vec.y() || z()!=vec.z();
}

template <typename T>
bool CctwVector3D<T>::operator > (const CctwVector3D<T> &vec) const
{
  if (z() > vec.z()) {
    return true;
  } else if (z() < vec.z()) {
    return false;
  } else if (y() > vec.y()) {
    return true;
  } else if (y() < vec.y()) {
    return false;
  } else if (x() > vec.x()) {
    return true;
  } else if (x() < vec.x()) {
    return false;
  } else {
    return false;
  }
}

template <typename T>
bool CctwVector3D<T>::operator >= (const CctwVector3D<T> &vec) const
{
  if (z() > vec.z()) {
    return true;
  } else if (z() < vec.z()) {
    return false;
  } else if (y() > vec.y()) {
    return true;
  } else if (y() < vec.y()) {
    return false;
  } else if (x() > vec.x()) {
    return true;
  } else if (x() < vec.x()) {
    return false;
  } else {
    return true;
  }
}

template <typename T>
bool CctwVector3D<T>::operator < (const CctwVector3D<T> &vec) const
{
  if (z() < vec.z()) {
    return true;
  } else if (z() > vec.z()) {
    return false;
  } else if (y() < vec.y()) {
    return true;
  } else if (y() > vec.y()) {
    return false;
  } else if (x() < vec.x()) {
    return true;
  } else if (x() > vec.x()) {
    return false;
  } else {
    return false;
  }
}

template <typename T>
bool CctwVector3D<T>::operator <= (const CctwVector3D<T> &vec) const
{
  if (z() < vec.z()) {
    return true;
  } else if (z() > vec.z()) {
    return false;
  } else if (y() < vec.y()) {
    return true;
  } else if (y() > vec.y()) {
    return false;
  } else if (x() < vec.x()) {
    return true;
  } else if (x() > vec.x()) {
    return false;
  } else {
    return true;
  }
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
T CctwVector3D<T>::volume() const
{
  return x()*y()*z();
}

template <typename T>
CctwVector3D<T> CctwVector3D<T>::normalized() const
{
  T len = length();

  if (::fabs((double)len) < 1e-10) {
    return CctwVector3D<T>(0,0,0);
  } else {
    return CctwVector3D(x() / len,
                        y() / len,
                        z() / len);
  }
}

template <typename T>
void CctwVector3D<T>::normalize()
{
  T len = length();

  if (::fabs((double)len) < 1e-10) {
    return;
  } else {
    m_Vector[0] /= len;
    m_Vector[1] /= len;
    m_Vector[2] /= len;
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

template <typename T>
void CctwVector3D<T>::setSettingsValue(QSettings *settings, QString name)
{
  settings->beginGroup(name);

  settings->setValue("x", x());
  settings->setValue("y", y());
  settings->setValue("z", z());

  settings->endGroup();
}

template <>
void CctwVector3D<int>::customSaver(const QVariant &val, QSettings *settings, QString name)
{
  CctwIntVector3D vec = val.value<CctwIntVector3D>();

  vec.setSettingsValue(settings, name);
}

template <>
void CctwVector3D<double>::customSaver(const QVariant &val, QSettings *settings, QString name)
{
  CctwDoubleVector3D vec = val.value<CctwDoubleVector3D>();

  vec.setSettingsValue(settings, name);
}

template <typename T>
QString CctwVector3D<T>::toString()
{
  return QString("[%1,%2,%3]").arg(x()).arg(y()).arg(z());
}

template class CctwVector3D<int>;
template class CctwVector3D<double>;
