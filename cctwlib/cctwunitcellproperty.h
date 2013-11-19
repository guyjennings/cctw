#ifndef CCTWUNITCELLPROPERTY_H
#define CCTWUNITCELLPROPERTY_H

#include "qcepproperty.h"
#include "cctwunitcell.h"

class CctwUnitCellProperty : public QcepProperty
{
  Q_OBJECT
public:
  explicit CctwUnitCellProperty(QcepSettingsSaverWPtr saver,
                                  QObject *parent,
                                  const char *name,
                                  CctwUnitCell value,
                                  QString toolTip);

  explicit CctwUnitCellProperty(QcepSettingsSaverWPtr saver,
                                  QObject *parent,
                                  const char *name,
                                  double a, double b, double c,
                                  double alpha, double beta, double gamma,
                                  QString toolTip);

  CctwUnitCell value() const;
  CctwUnitCell defaultValue() const;
  QString toString(const CctwUnitCell& mat);

  static void registerMetaTypes();
  static QScriptValue toScriptValue(QScriptEngine *engine, const CctwUnitCell &cell);
  static void fromScriptValue(const QScriptValue &obj, CctwUnitCell &cell);

public slots:
  void setValue(CctwUnitCell val, int index);
  void setValue(CctwUnitCell val);
  void setDefaultValue(CctwUnitCell val);
  void resetValue();

signals:
  void valueChanged(CctwUnitCell val, int index);

private:
  CctwUnitCell m_Default;
  CctwUnitCell m_Value;
};

#define CCTW_UNITCELL_PROPERTY(propname) \
public: \
CctwUnitCell get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(CctwUnitCell val) \
{ \
  m_##propname.setValue(val); \
} \
\
CctwUnitCell def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(CctwUnitCell val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
CctwUnitCellProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
CctwUnitCellProperty m_##propname;

#ifndef QT_NO_DATASTREAM

extern QDataStream &operator<<(QDataStream &stream, const CctwUnitCell &cell);
extern QDataStream &operator>>(QDataStream &stream, CctwUnitCell &cell);

#endif

#endif // CCTWQTUNITCELLPROPERTY_H
