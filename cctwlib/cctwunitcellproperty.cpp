#include "cctwunitcellproperty.h"
#include "qcepmutexlocker.h"
#include "qcepdebug.h"
#include "qcepsettingssaver.h"
#include <QScriptEngine>

CctwUnitCellProperty::CctwUnitCellProperty(QcepSettingsSaverWPtr saver,
    QObject *parent,
    const char *name,
    CctwUnitCell value,
    QString toolTip) :
  QcepProperty(saver, parent, name, toolTip),
  m_Default(value),
  m_Value(value)
{
}

CctwUnitCellProperty::CctwUnitCellProperty(
    QcepSettingsSaverWPtr saver,
    QObject *parent,
    const char *name,
    double a, double b, double c,
    double alpha, double beta, double gamma,
    QString toolTip) :
  QcepProperty(saver, parent, name, toolTip),
  m_Default(CctwUnitCell(a,b,c,alpha,beta,gamma)),
  m_Value(CctwUnitCell(a,b,c,alpha,beta,gamma))
{
}

void CctwUnitCellProperty::registerMetaTypes()
{
  qRegisterMetaType< CctwUnitCell >("CctwUnitCell");

  qRegisterMetaTypeStreamOperators< CctwUnitCell >("CctwUnitCell");

  registerCustomSaver("CctwUnitCell", CctwUnitCell::customSaver);
}

CctwUnitCell CctwUnitCellProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

CctwUnitCell CctwUnitCellProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void CctwUnitCellProperty::setValue(CctwUnitCell val, int index)
{
  if (debug()) {
    printMessage(tr("%1 CctwqtUnitCellProperty::setValue(CctwUnitCell %2, int %3) [%4]")
                 .arg(name()).arg(toString(val)).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

QString CctwUnitCellProperty::toString(const CctwUnitCell &val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QString res = tr("[ %1 %2 %3 %4 %5 %6 ]").arg(val.a()).arg(val.b()).arg(val.c())
                                           .arg(val.alpha()).arg(val.beta()).arg(val.gamma());

  return res;
}

void CctwUnitCellProperty::setValue(CctwUnitCell val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1 CctwqtUnitCellProperty::setValue(CctwUnitCell %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: CctwqtUnitCellProperty::setValue(CctwUnitCell %2) [%3]")
                   .arg(name()).arg(toString(val)).arg(index()));
    }

    m_Value = val;

    QcepSettingsSaverPtr saver(m_Saver);

    if (saver) {
      saver->changed(this);
    }

    emit valueChanged(m_Value, incIndex(1));
  }
}

void CctwUnitCellProperty::setDefaultValue(CctwUnitCell val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void CctwUnitCellProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: CctwqtUnitCellProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

#ifndef QT_NO_DATASTREAM

QDataStream &operator<<(QDataStream &stream, const CctwUnitCell &cell)
{
  stream << cell.a();
  stream << cell.b();
  stream << cell.c();
  stream << cell.alpha();
  stream << cell.beta();
  stream << cell.gamma();

  return stream;
}

QDataStream &operator>>(QDataStream &stream, CctwUnitCell &cell)
{
  stream >> cell.a();
  stream >> cell.b();
  stream >> cell.c();
  stream >> cell.alpha();
  stream >> cell.beta();
  stream >> cell.gamma();

  return stream;
}

#endif

QScriptValue CctwUnitCellProperty::toScriptValue(QScriptEngine *engine, const CctwUnitCell &cell)
{
  QScriptValue obj = engine->newArray(6);

  obj.setProperty(0, cell.a());
  obj.setProperty(1, cell.b());
  obj.setProperty(2, cell.c());
  obj.setProperty(3, cell.alpha());
  obj.setProperty(4, cell.beta());
  obj.setProperty(5, cell.gamma());

  return obj;
}

void CctwUnitCellProperty::fromScriptValue(const QScriptValue &obj, CctwUnitCell &cell)
{
  cell.a() = obj.property(0).toNumber();
  cell.b() = obj.property(1).toNumber();
  cell.c() = obj.property(2).toNumber();
  cell.alpha() = obj.property(3).toNumber();
  cell.beta() = obj.property(4).toNumber();
  cell.gamma() = obj.property(5).toNumber();
}
