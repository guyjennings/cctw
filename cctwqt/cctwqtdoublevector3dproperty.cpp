#include "cctwqtdoublevector3dproperty.h"
#include "qcepmutexlocker.h"
#include "qcepdebug.h"
#include "qcepsettingssaver.h"
#include <QScriptEngine>

CctwqtDoubleVector3DProperty::CctwqtDoubleVector3DProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, CctwDoubleVector3D value, QString toolTip) :
  QcepProperty(saver, parent, name, toolTip),
  m_Default(value),
  m_Value(value)
{
}

CctwqtDoubleVector3DProperty::CctwqtDoubleVector3DProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, double x, double y, double z, QString toolTip) :
  QcepProperty(saver, parent, name, toolTip),
  m_Default(CctwDoubleVector3D(x,y,z)),
  m_Value(CctwDoubleVector3D(x,y,z))
{
}

void CctwqtDoubleVector3DProperty::registerMetaTypes()
{
  qRegisterMetaType< CctwDoubleVector3D >("CctwDoubleVector3D");

  qRegisterMetaTypeStreamOperators< CctwDoubleVector3D >("CctwDoubleVector3D");

  registerCustomSaver("CctwDoubleVector3D", CctwDoubleVector3D::customSaver);
}

CctwDoubleVector3D CctwqtDoubleVector3DProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

CctwDoubleVector3D CctwqtDoubleVector3DProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void CctwqtDoubleVector3DProperty::setValue(CctwDoubleVector3D val, int index)
{
  if (debug()) {
    printMessage(tr("%1 CctwqtDoubleVector3DProperty::setValue(CctwDoubleVector3D %2, int %3) [%4]")
                 .arg(name()).arg(toString(val)).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void CctwqtDoubleVector3DProperty::incValue(CctwDoubleVector3D step)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: CctwqtDoubleVector3DProperty::incValue(CctwDoubleVector3D %2...)")
                 .arg(name()).arg(toString(step)));
  }

  m_Value += step;

  QcepSettingsSaverPtr saver(m_Saver);

  if (saver) {
    saver->changed(this);
  }

  emit valueChanged(m_Value, incIndex(1));
}

QString CctwqtDoubleVector3DProperty::toString(const CctwDoubleVector3D &val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QString res = tr("[ %1 %2 %3 ]").arg(val.x()).arg(val.y()).arg(val.z());

  return res;
}

void CctwqtDoubleVector3DProperty::setValue(CctwDoubleVector3D val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1 CctwqtDoubleVector3DProperty::setValue(CctwDoubleVector3D %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: CctwqtDoubleVector3DProperty::setValue(CctwDoubleVector3D %2) [%3]")
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

void CctwqtDoubleVector3DProperty::setDefaultValue(CctwDoubleVector3D val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void CctwqtDoubleVector3DProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: CctwqtDoubleVector3DProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

QScriptValue CctwqtDoubleVector3DProperty::toScriptValue(QScriptEngine *engine, const CctwDoubleVector3D &vec)
{
  QScriptValue obj = engine->newArray(3);

  obj.setProperty(0, vec.x());
  obj.setProperty(1, vec.y());
  obj.setProperty(2, vec.z());

  return obj;
}

void CctwqtDoubleVector3DProperty::fromScriptValue(const QScriptValue &obj, CctwDoubleVector3D &vec)
{
  vec.x() = obj.property(0).toNumber();
  vec.y() = obj.property(1).toNumber();
  vec.z() = obj.property(2).toNumber();
}
