#ifndef CCTWQTDOUBLEVECTOR3DPROPERTY_H
#define CCTWQTDOUBLEVECTOR3DPROPERTY_H

#include "qcepproperty.h"
#include "cctwvector3d.h"

class CctwqtDoubleVector3DProperty : public QcepProperty
{
  Q_OBJECT
public:
  explicit CctwqtDoubleVector3DProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, CctwDoubleVector3D value, QString toolTip);
  explicit CctwqtDoubleVector3DProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, double x, double y, double z, QString toolTip);

  CctwDoubleVector3D value() const;
  CctwDoubleVector3D defaultValue() const;
  QString toString(const CctwDoubleVector3D& mat);

  static void registerMetaTypes();

public slots:
  void setValue(CctwDoubleVector3D val, int index);
  void setValue(CctwDoubleVector3D val);
  void incValue(CctwDoubleVector3D step);
  void setDefaultValue(CctwDoubleVector3D val);
  void resetValue();

signals:
  void valueChanged(CctwDoubleVector3D val, int index);

private:
  CctwDoubleVector3D m_Default;
  CctwDoubleVector3D m_Value;
};

#define CCTWQT_DOUBLEVECTOR3D_PROPERTY(propname) \
public: \
CctwDoubleVector3D get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(CctwDoubleVector3D val) \
{ \
  m_##propname.setValue(val); \
} \
\
CctwDoubleVector3D def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(CctwDoubleVector3D val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
CctwqtDoubleVector3DProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
CctwqtDoubleVector3DProperty m_##propname;

#ifndef QT_NO_DATASTREAM

template <typename T>
QDataStream &operator<<(QDataStream &stream, const CctwVector3D<T> &vec)
{
  for (int i = 0; i < 3; ++i) {
    stream << double(vec(i));
  }

  return stream;
}

template <typename T>
QDataStream &operator>>(QDataStream &stream, CctwVector3D<T> &vec)
{
  double x;
  for (int i = 0; i < 3; ++i) {
    stream >> x;
    vec(i) = T(x);
  }

  return stream;
}

#endif

#endif // CCTWQTDOUBLEVECTOR3DPROPERTY_H
