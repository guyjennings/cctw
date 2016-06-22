#ifndef CCTWINTVECTOR3DPROPERTY_H
#define CCTWINTVECTOR3DPROPERTY_H


#include "qcepproperty.h"
#include "cctwvector3d.h"

class CctwIntVector3DProperty : public QcepProperty
{
  Q_OBJECT
public:
  explicit CctwIntVector3DProperty(QcepSerializableObject *parent, const char *name, CctwIntVector3D value, QString toolTip);
  explicit CctwIntVector3DProperty(QcepSerializableObject *parent, const char *name, int x, int y, int z, QString toolTip);

  CctwIntVector3D value() const;
  CctwIntVector3D defaultValue() const;
  QString toString(const CctwIntVector3D& mat);
  int subValue(int axis) const;

  static void registerMetaTypes();
  static QScriptValue toScriptValue(QScriptEngine *engine, const CctwIntVector3D &vec);
  static void fromScriptValue(const QScriptValue &obj, CctwIntVector3D &vec);

  void linkTo(QSpinBox *xSpinBox, QSpinBox *ySpinBox, QSpinBox *zSpinBox);
  void linkTo(int axis, QSpinBox *spinBox);

public slots:
  void setValue(CctwIntVector3D val, int index);
  void setValue(CctwIntVector3D val);
  void incValue(CctwIntVector3D step);
  void setDefaultValue(CctwIntVector3D val);
  void resetValue();
  void setSubValue(int axis, int value, int index);
  void setSubValue(int axis, int value);

signals:
  void valueChanged(CctwIntVector3D val, int index);
  void subValueChanged(int axis, int val, int index);

private:
  CctwIntVector3D m_Default;
  CctwIntVector3D m_Value;
};

class CctwIntVector3DPropertySpinBoxHelper : public QObject {
  Q_OBJECT

public:
  CctwIntVector3DPropertySpinBoxHelper(QSpinBox *spinBox, CctwIntVector3DProperty *property, int axis);
  void connect();

public slots:
  void setSubValue(int axis, int value, int index);
  void setValue(int value);

signals:
  void subValueChanged(int axis, int value, int index);

private:
  QSpinBox                   *m_SpinBox;
  CctwIntVector3DProperty    *m_Property;
  int                         m_Axis;
};

#define CCTW_INTVECTOR3D_PROPERTY(propname) \
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
CctwIntVector3DProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
CctwIntVector3DProperty m_##propname;

#ifndef QT_NO_DATASTREAM

extern QDataStream &operator<<(QDataStream &stream, const CctwIntVector3D &vec);
extern QDataStream &operator>>(QDataStream &stream, CctwIntVector3D &vec);

#endif

#endif // CCTWINTVECTOR3DPROPERTY_H
