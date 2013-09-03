#ifndef CCTWQTUNITCELLPROPERTY_H
#define CCTWQTUNITCELLPROPERTY_H

#include "qcepproperty.h"
#include "cctwqtunitcell.h"

class CctwqtUnitCellProperty : public QcepProperty
{
  Q_OBJECT
public:
  explicit CctwqtUnitCellProperty(QcepSettingsSaverWPtr saver,
                                  QObject *parent,
                                  const char *name,
                                  CctwqtUnitCell value,
                                  QString toolTip);

  explicit CctwqtUnitCellProperty(QcepSettingsSaverWPtr saver,
                                  QObject *parent,
                                  const char *name,
                                  double a, double b, double c,
                                  double alpha, double beta, double gamma,
                                  QString toolTip);

  CctwqtUnitCell value() const;
  CctwqtUnitCell defaultValue() const;
  QString toString(const CctwUnitCell& mat);

  static void registerMetaTypes();

public slots:
  void setValue(CctwqtUnitCell val, int index);
  void setValue(CctwqtUnitCell val);
  void setDefaultValue(CctwqtUnitCell val);
  void resetValue();

signals:
  void valueChanged(CctwqtUnitCell val, int index);

private:
  CctwqtUnitCell m_Default;
  CctwqtUnitCell m_Value;
};

#define CCTWQT_UNITCELL_PROPERTY(propname) \
public: \
CctwqtUnitCell get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(CctwqtUnitCell val) \
{ \
  m_##propname.setValue(val); \
} \
\
CctwqtUnitCell def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(CctwqtUnitCell val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
CctwqtUnitCellProperty *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
CctwqtUnitCellProperty m_##propname;

#ifndef QT_NO_DATASTREAM

extern QDataStream &operator<<(QDataStream &stream, const CctwqtUnitCell &cell);
extern QDataStream &operator>>(QDataStream &stream, CctwqtUnitCell &cell);

#endif

#endif // CCTWQTUNITCELLPROPERTY_H
