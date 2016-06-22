#include "cctwdoublematrix3x3property.h"
#include "qcepmutexlocker.h"
#include "qcepdebug.h"
#include <QScriptEngine>
#include <stdio.h>
#include "qcepobject.h"
#include "qcepserializableobject.h"

CctwDoubleMatrix3x3Property::CctwDoubleMatrix3x3Property(QcepSerializableObject *parent, const char *name,
                                                         CctwDoubleMatrix3x3 value, QString toolTip) :
  QcepProperty(parent, name, toolTip),
  m_Default(value),
  m_Value(value)
{
}

CctwDoubleMatrix3x3Property::CctwDoubleMatrix3x3Property(QcepSerializableObject *parent, const char *name,
                                                         double r0c0, double r0c1, double r0c2,
                                                         double r1c0, double r1c1, double r1c2,
                                                         double r2c0, double r2c1, double r2c2,
                                                         QString toolTip) :
  QcepProperty(parent, name, toolTip),
  m_Default(),
  m_Value()
{
  CctwDoubleMatrix3x3 val;

  val(0,0) = r0c0; val(0,1) = r0c1; val(0,2) = r0c2;
  val(1,0) = r1c0; val(1,1) = r1c1; val(1,2) = r1c2;
  val(2,0) = r2c0; val(2,1) = r2c1; val(2,2) = r2c2;

  m_Default = val;
  m_Value = val;
}

void CctwDoubleMatrix3x3Property::registerMetaTypes()
{
  qRegisterMetaType< CctwDoubleMatrix3x3 >("CctwDoubleMatrix3x3");

  qRegisterMetaTypeStreamOperators< CctwDoubleMatrix3x3 >("CctwDoubleMatrix3x3");

  registerCustomSaver("CctwDoubleMatrix3x3", CctwMatrix3x3<double>::customSaver);
}

CctwDoubleMatrix3x3 CctwDoubleMatrix3x3Property::value() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value;
}

CctwDoubleMatrix3x3 CctwDoubleMatrix3x3Property::defaultValue() const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Default;
}

double CctwDoubleMatrix3x3Property::subValue(int row, int col) const
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Value(row, col);
}

void CctwDoubleMatrix3x3Property::setValue(CctwDoubleMatrix3x3 val, int index)
{
  if (debug()) {
    printMessage(tr("%1 CctwqtDoubleMatrix3x3Property::setValue(CctwDoubleMatrix3x3 %2, int %3) [%4]")
                 .arg(name()).arg(toString(val)).arg(index).arg(this->index()));
  }

  if (index == this->index()) {
    setValue(val);
  }
}

void CctwDoubleMatrix3x3Property::incValue(CctwDoubleMatrix3x3 step)
{
  setValue(m_Value + step);
}

void CctwDoubleMatrix3x3Property::setSubValue(int row, int col, double value, int index)
{
  if (index == this->index()) {
    setSubValue(row, col, value);
  }
}

void CctwDoubleMatrix3x3Property::setSubValue(int row, int col, double value)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (value != m_Value(row, col)) {
    int newIndex = incIndex(1);

    emit subValueChanged(row, col, value, newIndex);

    m_Value(row, col) = value;

    emit valueChanged(m_Value, newIndex);
  }
}

QString CctwDoubleMatrix3x3Property::toString(const CctwDoubleMatrix3x3 &val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QString res = "[";

  for (int row=0; row<3; row++) {
    res += " [ ";
    for (int col=0; col<3; col++) {
      res += tr("%1 ").arg(val(row, col));
    }
    res += "]";
  }

  res += "]";

  return res;
}

void CctwDoubleMatrix3x3Property::setValue(CctwDoubleMatrix3x3 val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1 CctwqtDoubleMatrix3x3Property::setValue(CctwDoubleMatrix3x3 %2)")
                 .arg(name()).arg(toString(val)));
  }

  if (val != m_Value) {
    int newIndex = incIndex(1);

    if (debug()) {
      printMessage(tr("%1: CctwqtDoubleMatrix3x3Property::setValue(CctwDoubleMatrix3x3 %2) [%3]")
                   .arg(name()).arg(toString(val)).arg(index()));
    }

    for (int row=0; row<3; row++) {
      for (int col=0; col<3; col++) {
        if(val(row,col) != m_Value(row,col)) {
          emit subValueChanged(row, col, val(row,col), newIndex);
        }
      }
    }

    m_Value = val;

    if (m_Parent) {
      m_Parent->propertyChanged(this);
    }

    emit valueChanged(m_Value, newIndex);
  }
}

void CctwDoubleMatrix3x3Property::setDefaultValue(CctwDoubleMatrix3x3 val)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Default = val;
}

void CctwDoubleMatrix3x3Property::resetValue()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("%1: CctwqtDoubleMatrix3x3Property::resetValue").arg(name()));
  }

  setValue(defaultValue());
}

QScriptValue CctwDoubleMatrix3x3Property::toScriptValue(QScriptEngine *engine, const CctwDoubleMatrix3x3 &mat)
{
  QScriptValue obj = engine->newArray(3);

  for (int i=0; i<3; i++) {
    obj.setProperty(i, engine->newArray(3));
  }

  for (int r=0; r<3; r++) {
    for (int c=0; c<3; c++) {
      obj.property(r).setProperty(c, mat(r,c));
    }
  }

//  for (int r=0; r<3; r++) {
//    for (int c=0; c<3; c++) {
//      obj.setProperty(tr("r%1c%2").arg(r).arg(c), mat(r,c));
//    }
//  }

  return obj;
}

void CctwDoubleMatrix3x3Property::fromScriptValue(const QScriptValue &obj, CctwDoubleMatrix3x3 &mat)
{
  for (int r=0; r<3; r++) {
    for (int c=0; c<3; c++) {
      mat(r,c)=obj.property(r).property(c).toNumber();
//      mat(r,c) = obj.property(tr("r%1c%2").arg(r).arg(c)).toNumber();
    }
  }
}

//void CctwDoubleMatrix3x3Property::linkTo(QDoubleSpinBox *xSpinBox, QDoubleSpinBox *ySpinBox, QDoubleSpinBox *zSpinBox)
//{
//  if (xSpinBox) {
//    linkTo(0, xSpinBox);
//  }

//  if (ySpinBox) {
//    linkTo(1, ySpinBox);
//  }

//  if (zSpinBox) {
//    linkTo(2, zSpinBox);
//  }
//}

void CctwDoubleMatrix3x3Property::linkTo(int row, int col, QDoubleSpinBox *spinBox)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  CctwDoubleMatrix3x3PropertyDoubleSpinBoxHelper *helper
      = new CctwDoubleMatrix3x3PropertyDoubleSpinBoxHelper(spinBox, this, row, col);

  helper->moveToThread(spinBox->thread());
  helper->connect();

  spinBox -> setValue(subValue(row, col));
  spinBox -> setKeyboardTracking(false);

  setWidgetToolTip(spinBox);

  connect(this, SIGNAL(subValueChanged(int,int,double,int)), helper, SLOT(setSubValue(int,int,double,int)));
  connect(helper, SIGNAL(subValueChanged(int,int,double,int)), this, SLOT(setSubValue(int,int,double,int)));
}

CctwDoubleMatrix3x3PropertyDoubleSpinBoxHelper::CctwDoubleMatrix3x3PropertyDoubleSpinBoxHelper
  (QDoubleSpinBox *spinBox, CctwDoubleMatrix3x3Property *property, int row, int col)
  : QObject(spinBox),
    m_DoubleSpinBox(spinBox),
    m_Property(property),
    m_Row(row),
    m_Col(col)
{
}

void CctwDoubleMatrix3x3PropertyDoubleSpinBoxHelper::connect()
{
  CONNECT_CHECK(QObject::connect(m_DoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setValue(double)), Qt::DirectConnection));
}

void CctwDoubleMatrix3x3PropertyDoubleSpinBoxHelper::setSubValue(int row, int col, double value, int index)
{
  if (m_Property->index() == index) {
    if (m_Row == row && m_Col == col) {
      if (m_DoubleSpinBox->value() != value) {
        bool block = m_DoubleSpinBox->blockSignals(true);
        m_DoubleSpinBox->setValue(value);
        m_DoubleSpinBox->blockSignals(block);
      }
    }
  }
}

void CctwDoubleMatrix3x3PropertyDoubleSpinBoxHelper::setValue(double value)
{
  emit subValueChanged(m_Row, m_Col, value, m_Property->incIndex(1));
}

