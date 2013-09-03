#include "cctwqtunitcellproperty.h"
#include "qcepmutexlocker.h"
#include "qcepdebug.h"
#include "qcepsettingssaver.h"

CctwqtUnitCellProperty::CctwqtUnitCellProperty(QcepSettingsSaverWPtr saver,
    QObject *parent,
    const char *name,
    CctwqtUnitCell value,
    QString toolTip) :
  QcepProperty(saver, parent, name, toolTip),
  m_Default(value),
  m_Value(value)
{
}

CctwqtUnitCellProperty::CctwqtUnitCellProperty(
    QcepSettingsSaverWPtr saver,
    QObject *parent,
    const char *name,
    double a, double b, double c,
    double alpha, double beta, double gamma,
    QString toolTip) :
  QcepProperty(saver, parent, name, toolTip),
  m_Default(CctwqtUnitCell(a,b,c,alpha,beta,gamma)),
  m_Value(CctwqtUnitCell(a,b,c,alpha,beta,gamma))
{
}

void CctwqtUnitCellProperty::registerMetaTypes()
{
  qRegisterMetaType< CctwqtUnitCell >("CctwqtUnitCell");

  qRegisterMetaTypeStreamOperators< CctwqtUnitCell >("CctwqtUnitCell");
}

CctwqtUnitCell CctwqtUnitCellProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

CctwqtUnitCell CctwqtUnitCellProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void CctwqtUnitCellProperty::setValue(CctwqtUnitCell val, int index)
{
  if (debug()) {
    printMessage(tr("%1 CctwqtUnitCellProperty::setValue(CctwUnitCell %2, int %3) [%4]")
                 .arg(name()).arg(toString(val)).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

QString CctwqtUnitCellProperty::toString(const CctwUnitCell &val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QString res = tr("[ %1 %2 %3 %4 %5 %6 ]").arg(val.a()).arg(val.b()).arg(val.c())
                                           .arg(val.alpha()).arg(val.beta()).arg(val.gamma());

  return res;
}

void CctwqtUnitCellProperty::setValue(CctwqtUnitCell val)
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

void CctwqtUnitCellProperty::setDefaultValue(CctwqtUnitCell val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void CctwqtUnitCellProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: CctwqtUnitCellProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

#ifndef QT_NO_DATASTREAM

QDataStream &operator<<(QDataStream &stream, const CctwqtUnitCell &cell)
{
  stream << cell.a();
  stream << cell.b();
  stream << cell.c();
  stream << cell.alpha();
  stream << cell.beta();
  stream << cell.gamma();

  return stream;
}

QDataStream &operator>>(QDataStream &stream, CctwqtUnitCell &cell)
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

