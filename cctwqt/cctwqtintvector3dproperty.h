#ifndef CCTWQTINTVECTOR3DPROPERTY_H
#define CCTWQTINTVECTOR3DPROPERTY_H


#include "qcepproperty.h"
#include "../cctwvector3d.h"

class CctwqtIntVector3DProperty : public QcepProperty
{
  Q_OBJECT
public:
  explicit CctwqtIntVector3DProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, CctwIntVector3D value, QString toolTip);
  explicit CctwqtIntVector3DProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, int x, int y, int z, QString toolTip);

  CctwIntVector3D value() const;
  CctwIntVector3D defaultValue() const;
  QString toString(const CctwIntVector3D& mat);

  static void registerMetaTypes();
  static QScriptValue toScriptValue(QScriptEngine *engine, const CctwIntVector3D &vec);
  static void fromScriptValue(const QScriptValue &obj, CctwIntVector3D &vec);

public slots:
  void setValue(CctwIntVector3D val, int index);
  void setValue(CctwIntVector3D val);
  void incValue(CctwIntVector3D step);
  void setDefaultValue(CctwIntVector3D val);
  void resetValue();

signals:
  void valueChanged(CctwIntVector3D val, int index);

private:
  CctwIntVector3D m_Default;
  CctwIntVector3D m_Value;
};

#define CCTWQT_INTVECTOR3D_PROPERTY(propname) \
public: \
CctwIntVector3D get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(CctwIntVector3D val) \
{ \
  m_##propname.setValue(val); \
} \
\
CctwIntVector3D def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(CctwIntVector3D val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
CctwqtIntVector3DProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
CctwqtIntVector3DProperty m_##propname;

#ifndef QT_NO_DATASTREAM

extern QDataStream &operator<<(QDataStream &stream, const CctwIntVector3D &vec);
extern QDataStream &operator>>(QDataStream &stream, CctwIntVector3D &vec);

#endif

#endif // CCTWQTINTVECTOR3DPROPERTY_H
