#include "cctwintvector3dproperty.h"
#include "qcepmutexlocker.h"
#include "qcepdebug.h"
#include "qcepsettingssaver.h"
#include <QScriptEngine>

CctwIntVector3DProperty::CctwIntVector3DProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, CctwIntVector3D value, QString toolTip) :
  QcepProperty(saver, parent, name, toolTip),
  m_Default(value),
  m_Value(value)
{
}

CctwIntVector3DProperty::CctwIntVector3DProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, int x, int y, int z, QString toolTip) :
  QcepProperty(saver, parent, name, toolTip),
  m_Default(CctwIntVector3D(x,y,z)),
  m_Value(CctwIntVector3D(x,y,z))
{
}

void CctwIntVector3DProperty::registerMetaTypes()
{
  qRegisterMetaType< CctwIntVector3D >("CctwIntVector3D");

  qRegisterMetaTypeStreamOperators< CctwIntVector3D >("CctwIntVector3D");

  registerCustomSaver("CctwIntVector3D", CctwIntVector3D::customSaver);
}

CctwIntVector3D CctwIntVector3DProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

CctwIntVector3D CctwIntVector3DProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

void CctwIntVector3DProperty::setValue(CctwIntVector3D val, int index)
{
  if (debug()) {
    printMessage(tr("%1 CctwqtIntVector3DProperty::setValue(CctwIntVector3D %2, int %3) [%4]")
                 .arg(name()).arg(toString(val)).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void CctwIntVector3DProperty::incValue(CctwIntVector3D step)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES) || debug()) {
    printMessage(tr("%1: CctwqtIntVector3DProperty::incValue(CctwIntVector3D %2...)")
                 .arg(name()).arg(toString(step)));
  }

  m_Value += step;

  QcepSettingsSaverPtr saver(m_Saver);

  if (saver) {
    saver->changed(this);
  }

  emit valueChanged(m_Value, incIndex(1));
}

QString CctwIntVector3DProperty::toString(const CctwIntVector3D &val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QString res = tr("[ %1 %2 %3 ]").arg(val.x()).arg(val.y()).arg(val.z());

  return res;
}

void CctwIntVector3DProperty::setValue(CctwIntVector3D val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1 CctwqtIntVector3DProperty::setValue(CctwIntVector3D %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    if (debug()) {
      printMessage(tr("%1: CctwqtIntVector3DProperty::setValue(CctwIntVector3D %2) [%3]")
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

void CctwIntVector3DProperty::setDefaultValue(CctwIntVector3D val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void CctwIntVector3DProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: CctwqtIntVector3DProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

QScriptValue CctwIntVector3DProperty::toScriptValue(QScriptEngine *engine, const CctwIntVector3D &vec)
{
  QScriptValue obj = engine->newArray(3);

  obj.setProperty(0, vec.x());
  obj.setProperty(1, vec.y());
  obj.setProperty(2, vec.z());

  return obj;
}

void CctwIntVector3DProperty::fromScriptValue(const QScriptValue &obj, CctwIntVector3D &vec)
{
  vec.x() = obj.property(0).toInteger();
  vec.y() = obj.property(1).toInteger();
  vec.z() = obj.property(2).toInteger();
}

#ifndef QT_NO_DATASTREAM

QDataStream &operator<<(QDataStream &stream, const CctwIntVector3D &vec)
{
  for (int i = 0; i < 3; ++i) {
    stream << vec(i);
  }

  return stream;
}

QDataStream &operator>>(QDataStream &stream, CctwIntVector3D &vec)
{
  int x;
  for (int i = 0; i < 3; ++i) {
    stream >> x;
    vec(i) = x;
  }

  return stream;
}

#endif
