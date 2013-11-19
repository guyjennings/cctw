#ifndef CCTWDOUBLEVECTOR3DPROPERTY_H
#define CCTWDOUBLEVECTOR3DPROPERTY_H

#include "qcepproperty.h"
#include "cctwvector3d.h"

class CctwDoubleVector3DProperty : public QcepProperty
{
  Q_OBJECT
public:
  explicit CctwDoubleVector3DProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, CctwDoubleVector3D value, QString toolTip);
  explicit CctwDoubleVector3DProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, double x, double y, double z, QString toolTip);

  CctwDoubleVector3D value() const;
  CctwDoubleVector3D defaultValue() const;
  QString toString(const CctwDoubleVector3D& mat);

  static void registerMetaTypes();
  static QScriptValue toScriptValue(QScriptEngine *engine, const CctwDoubleVector3D &vec);
  static void fromScriptValue(const QScriptValue &obj, CctwDoubleVector3D &vec);

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

#define CCTW_DOUBLEVECTOR3D_PROPERTY(propname) \
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
CctwDoubleVector3DProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
CctwDoubleVector3DProperty m_##propname;

#ifndef QT_NO_DATASTREAM

extern QDataStream &operator<<(QDataStream &stream, const CctwDoubleVector3D &vec);
extern QDataStream &operator>>(QDataStream &stream, CctwDoubleVector3D &vec);

#endif

#endif // CCTWDOUBLEVECTOR3DPROPERTY_H
