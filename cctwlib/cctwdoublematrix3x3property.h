#ifndef CCTWDOUBLEMATRIX3X3PROPERTY_H
#define CCTWDOUBLEMATRIX3X3PROPERTY_H

#include "qcepproperty.h"
#include "cctwmatrix3x3.h"

class CctwDoubleMatrix3x3Property : public QcepProperty
{
  Q_OBJECT
public:
  explicit CctwDoubleMatrix3x3Property(QcepSettingsSaverWPtr saver, QObject *parent, const char *name,
                                         CctwDoubleMatrix3x3 value, QString toolTip);
  explicit CctwDoubleMatrix3x3Property(QcepSettingsSaverWPtr saver, QObject *parent, const char *name,
                                         double r0c0, double r0c1, double r0c2,
                                         double r1c0, double r1c1, double r1c2,
                                         double r2c0, double r2c1, double r2c2,
                                         QString toolTip);

  CctwDoubleMatrix3x3 value() const;
  CctwDoubleMatrix3x3 defaultValue() const;
  QString toString(const CctwDoubleMatrix3x3& mat);

  static void registerMetaTypes();
  static QScriptValue toScriptValue(QScriptEngine *engine, const CctwDoubleMatrix3x3 &mat);
  static void fromScriptValue(const QScriptValue &obj, CctwDoubleMatrix3x3 &mat);

public slots:
  void setValue(CctwDoubleMatrix3x3 val, int index);
  void setValue(CctwDoubleMatrix3x3 val);
  void incValue(CctwDoubleMatrix3x3 step);
  void setDefaultValue(CctwDoubleMatrix3x3 val);
  void resetValue();

signals:
  void valueChanged(CctwDoubleMatrix3x3 val, int index);

private:
  CctwDoubleMatrix3x3 m_Default;
  CctwDoubleMatrix3x3 m_Value;
};

#define CCTW_DOUBLEMATRIX3X3_PROPERTY(propname) \
public: \
CctwDoubleMatrix3x3 get_##propname() const \
{ \
  return m_##propname.value(); \
} \
\
void set_##propname(CctwDoubleMatrix3x3 val) \
{ \
  m_##propname.setValue(val); \
} \
\
CctwDoubleMatrix3x3 def_##propname() const \
{ \
  return m_##propname.defaultValue(); \
} \
\
void setdef_##propname(CctwDoubleMatrix3x3 val) \
{ \
  m_##propname.setDefaultValue(val); \
} \
\
void reset_##propname() \
{ \
  m_##propname.resetValue(); \
} \
\
CctwDoubleMatrix3x3Property *prop_##propname() { \
  return &m_##propname; \
} \
\
private: \
CctwDoubleMatrix3x3Property m_##propname;

#ifndef QT_NO_DATASTREAM

template <typename T>
QDataStream &operator<<(QDataStream &stream, const CctwMatrix3x3<T> &matrix)
{
    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col)
            stream << double(matrix(row, col));
    return stream;
}

template <typename T>
QDataStream &operator>>(QDataStream &stream, CctwMatrix3x3<T> &matrix)
{
    double x;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            stream >> x;
            matrix(row, col) = T(x);
        }
    }
    return stream;
}

#endif

#endif // CCTWDOUBLEMATRIX3X3PROPERTY_H
