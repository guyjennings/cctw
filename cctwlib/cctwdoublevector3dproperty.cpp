#include "cctwdoublevector3dproperty.h"
#include "qcepmutexlocker.h"
#include "qcepdebug.h"
#include "qcepsettingssaver.h"
#include <QScriptEngine>

CctwDoubleVector3DProperty::CctwDoubleVector3DProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, CctwDoubleVector3D value, QString toolTip) :
  QcepProperty(saver, parent, name, toolTip),
  m_Default(value),
  m_Value(value)
{
}

CctwDoubleVector3DProperty::CctwDoubleVector3DProperty(QcepSettingsSaverWPtr saver, QObject *parent, const char *name, double x, double y, double z, QString toolTip) :
  QcepProperty(saver, parent, name, toolTip),
  m_Default(CctwDoubleVector3D(x,y,z)),
  m_Value(CctwDoubleVector3D(x,y,z))
{
}

void CctwDoubleVector3DProperty::registerMetaTypes()
{
  qRegisterMetaType< CctwDoubleVector3D >("CctwDoubleVector3D");

  qRegisterMetaTypeStreamOperators< CctwDoubleVector3D >("CctwDoubleVector3D");

  registerCustomSaver("CctwDoubleVector3D", CctwDoubleVector3D::customSaver);
}

CctwDoubleVector3D CctwDoubleVector3DProperty::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

CctwDoubleVector3D CctwDoubleVector3DProperty::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

double CctwDoubleVector3DProperty::subValue(int axis) const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value(axis);
}

void CctwDoubleVector3DProperty::setValue(CctwDoubleVector3D val, int index)
{
  if (debug()) {
    printMessage(tr("%1 CctwqtDoubleVector3DProperty::setValue(CctwDoubleVector3D %2, int %3) [%4]")
                 .arg(name()).arg(toString(val)).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void CctwDoubleVector3DProperty::incValue(CctwDoubleVector3D step)
{
  setValue(m_Value + step);
}

void CctwDoubleVector3DProperty::setSubValue(int axis, double value, int index)
{
  if (index == this->index()) {
    setSubValue(axis, value);
  }
}

void CctwDoubleVector3DProperty::setSubValue(int axis, double value)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (value != m_Value(axis)) {
    int newIndex = incIndex(1);

    emit subValueChanged(axis, value, newIndex);

    m_Value(axis) = value;

    emit valueChanged(m_Value, newIndex);
  }
}

QString CctwDoubleVector3DProperty::toString(const CctwDoubleVector3D &val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QString res = tr("[ %1 %2 %3 ]").arg(val.x()).arg(val.y()).arg(val.z());

  return res;
}

void CctwDoubleVector3DProperty::setValue(CctwDoubleVector3D val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1 CctwqtDoubleVector3DProperty::setValue(CctwDoubleVector3D %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    int newIndex = incIndex(1);

    if (debug()) {
      printMessage(tr("%1: CctwqtDoubleVector3DProperty::setValue(CctwDoubleVector3D %2) [%3]")
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

    emit valueChanged(m_Value, newIndex);
  }
}

void CctwDoubleVector3DProperty::setDefaultValue(CctwDoubleVector3D val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void CctwDoubleVector3DProperty::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: CctwqtDoubleVector3DProperty::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

QScriptValue CctwDoubleVector3DProperty::toScriptValue(QScriptEngine *engine, const CctwDoubleVector3D &vec)
{
  QScriptValue obj = engine->newArray(3);

  obj.setProperty(0, vec.x());
  obj.setProperty(1, vec.y());
  obj.setProperty(2, vec.z());

  return obj;
}

void CctwDoubleVector3DProperty::fromScriptValue(const QScriptValue &obj, CctwDoubleVector3D &vec)
{
  vec.x() = obj.property(0).toNumber();
  vec.y() = obj.property(1).toNumber();
  vec.z() = obj.property(2).toNumber();
}

void CctwDoubleVector3DProperty::linkTo(QDoubleSpinBox *xSpinBox, QDoubleSpinBox *ySpinBox, QDoubleSpinBox *zSpinBox)
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

void CctwDoubleVector3DProperty::linkTo(int axis, QDoubleSpinBox *spinBox)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  CctwDoubleVector3DPropertyDoubleSpinBoxHelper *helper
      = new CctwDoubleVector3DPropertyDoubleSpinBoxHelper(spinBox, this, axis);

  helper->moveToThread(spinBox->thread());
  helper->connect();

  spinBox -> setValue(subValue(axis));
  spinBox -> setKeyboardTracking(false);

  setWidgetToolTip(spinBox);

  connect(this, SIGNAL(subValueChanged(int,double,int)), helper, SLOT(setSubValue(int,double,int)));
  connect(helper, SIGNAL(subValueChanged(int,double,int)), this, SLOT(setSubValue(int,double,int)));
}

CctwDoubleVector3DPropertyDoubleSpinBoxHelper::CctwDoubleVector3DPropertyDoubleSpinBoxHelper
  (QDoubleSpinBox *spinBox, CctwDoubleVector3DProperty *property, int axis)
  : QObject(spinBox),
    m_DoubleSpinBox(spinBox),
    m_Property(property),
    m_Axis(axis)
{
}

void CctwDoubleVector3DPropertyDoubleSpinBoxHelper::connect()
{
  CONNECT_CHECK(QObject::connect(m_DoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setValue(double)), Qt::DirectConnection));
}

void CctwDoubleVector3DPropertyDoubleSpinBoxHelper::setSubValue(int axis, double value, int index)
{
  if (m_Property->index() == index) {
    if (m_Axis == axis) {
      if (m_DoubleSpinBox->value() != value) {
        bool block = m_DoubleSpinBox->blockSignals(true);
        m_DoubleSpinBox->setValue(value);
        m_DoubleSpinBox->blockSignals(block);
      }
    }
  }
}

void CctwDoubleVector3DPropertyDoubleSpinBoxHelper::setValue(double value)
{
  emit subValueChanged(m_Axis, value, m_Property->incIndex(1));
}

#ifndef QT_NO_DATASTREAM

QDataStream &operator<<(QDataStream &stream, const CctwDoubleVector3D &vec)
{
  for (int i = 0; i < 3; ++i) {
    stream << vec(i);
  }

  return stream;
}

QDataStream &operator>>(QDataStream &stream, CctwDoubleVector3D &vec)
{
  double x;
  for (int i = 0; i < 3; ++i) {
    stream >> x;
    vec(i) = x;
  }

  return stream;
}

#endif
