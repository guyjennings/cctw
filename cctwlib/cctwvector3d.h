#ifndef CCTWVECTOR3D_H
#define CCTWVECTOR3D_H

#include <QSettings>
#include <QString>
#include <QScriptable>

template <typename T>
class CctwVector3D
{
public:
  CctwVector3D(T x, T y, T z);
  template <typename T2> CctwVector3D(const CctwVector3D<T2> &v);
  CctwVector3D();

public:
  T x() const { return m_Vector[0]; }
  T y() const { return m_Vector[1]; }
  T z() const { return m_Vector[2]; }
  T& x() { return m_Vector[0]; }
  T& y() { return m_Vector[1]; }
  T& z() { return m_Vector[2]; }

  T& operator () (int i) { return m_Vector[i]; }
  const T& operator() (int i) const { return m_Vector[i]; }

  CctwVector3D<T> operator + (const CctwVector3D<T> &vec) const;
  CctwVector3D<T> operator - (const CctwVector3D<T> &vec) const;
  CctwVector3D<T> operator * (const CctwVector3D<T> &vec) const;
  CctwVector3D<T> operator / (const CctwVector3D<T> &vec) const;

  CctwVector3D<T> operator += (const CctwVector3D<T> &vec);
  CctwVector3D<T> operator -= (const CctwVector3D<T> &vec);

  CctwVector3D<T> operator + (const T val) const;
  CctwVector3D<T> operator - (const T val) const;
  CctwVector3D<T> operator * (const T val) const;
  CctwVector3D<T> operator / (const T val) const;

  bool operator == (const CctwVector3D<T> &vec) const;
  bool operator != (const CctwVector3D<T> &vec) const;
  bool operator >  (const CctwVector3D<T>  &vec) const;
  bool operator <  (const CctwVector3D<T>  &vec) const;
  bool operator >= (const CctwVector3D<T>  &vec) const;
  bool operator <= (const CctwVector3D<T>  &vec) const;

  T length() const;
  T lengthSquared() const;
  T volume() const;

  CctwVector3D<T> normalized() const;
  void normalize();

  static T dotProduct(const CctwVector3D<T>& v1, const CctwVector3D<T>& v2);
  static CctwVector3D<T> crossProduct(const CctwVector3D<T> &v1, const CctwVector3D<T> &v2);
  static CctwVector3D<T> normal(const CctwVector3D<T>& v1, const CctwVector3D<T>& v2);

  CctwVector3D<T> min(const CctwVector3D<T> &vec) const;
  CctwVector3D<T> max(const CctwVector3D<T> &vec) const;

  void setSettingsValue(QSettings *settings, QString name);
  static void customSaver(const QVariant &val, QSettings *settings, QString name);

  QString toString();

protected:
  T m_Vector[3];
};

typedef CctwVector3D<int>    CctwIntVector3D;
typedef CctwVector3D<double> CctwDoubleVector3D;

Q_DECLARE_METATYPE(CctwIntVector3D)
Q_DECLARE_METATYPE(CctwDoubleVector3D)

#endif // CCTWVECTOR3D_H
