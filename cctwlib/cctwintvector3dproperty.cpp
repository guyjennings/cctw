#include "cctwintvector3dproperty.h"
#include "qcepmutexlocker.h"
#include "qcepdebug.h"
#include "qcepsettingssaver.h"
#include <QScriptEngine>
#include <stdio.h>

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

int CctwIntVector3DProperty::subValue(int axis) const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value(axis);
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

void CctwIntVector3DProperty::setSubValue(int axis, int value, int index)
{
  if (index == this->index()) {
    setSubValue(axis, value);
  }
}

void CctwIntVector3DProperty::setSubValue(int axis, int value)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (value != m_Value(axis)) {
    int newIndex = incIndex(1);

    emit subValueChanged(axis, value, newIndex);

    m_Value(axis) = value;

    emit valueChanged(m_Value, newIndex);
  }
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
    int newIndex = incIndex(1);

    if (debug()) {
      printMessage(tr("%1: CctwqtIntVector3DProperty::setValue(CctwIntVector3D %2) [%3]")
                   .arg(name()).arg(toString(val)).arg(index()));
    }

    for (int axis=0; axis<3; axis++) {
      if (val(axis) != m_Value(axis)) {
        emit subValueChanged(axis, val(axis), newIndex);
      }
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

void CctwIntVector3DProperty::linkTo(QSpinBox *xSpinBox, QSpinBox *ySpinBox, QSpinBox *zSpinBox)
{
  if (xSpinBox) {
    linkTo(0, xSpinBox);
  }

  if (ySpinBox) {
    linkTo(1, ySpinBox);
  }

  if (zSpinBox) {
    linkTo(2, zSpinBox);
  }
}

void CctwIntVector3DProperty::linkTo(int axis, QSpinBox *spinBox)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  CctwIntVector3DPropertySpinBoxHelper *helper
      = new CctwIntVector3DPropertySpinBoxHelper(spinBox, this, axis);

  helper->moveToThread(spinBox->thread());
  helper->connect();

  spinBox -> setValue(subValue(axis));
  spinBox -> setKeyboardTracking(false);

  setWidgetToolTip(spinBox);

  connect(this, SIGNAL(subValueChanged(int,int,int)), helper, SLOT(setSubValue(int,int,int)));
  connect(helper, SIGNAL(subValueChanged(int,int,int)), this, SLOT(setSubValue(int,int,int)));
}

CctwIntVector3DPropertySpinBoxHelper::CctwIntVector3DPropertySpinBoxHelper
  (QSpinBox *spinBox, CctwIntVector3DProperty *property, int axis)
  : QObject(spinBox),
    m_SpinBox(spinBox),
    m_Property(property),
    m_Axis(axis)
{
}

void CctwIntVector3DPropertySpinBoxHelper::connect()
{
  CONNECT_CHECK(QObject::connect(m_SpinBox, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)), Qt::DirectConnection));
}

void CctwIntVector3DPropertySpinBoxHelper::setSubValue(int axis, int value, int index)
{
  if (m_Property->index() == index) {
    if (m_Axis == axis) {
      if (m_SpinBox->value() != value) {
        bool block = m_SpinBox->blockSignals(true);
        m_SpinBox->setValue(value);
        m_SpinBox->blockSignals(block);
      }
    }
  }
}

void CctwIntVector3DPropertySpinBoxHelper::setValue(int value)
{
  emit subValueChanged(m_Axis, value, m_Property->incIndex(1));
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
