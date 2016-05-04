#include "cctwobject.h"
#include "qcepproperty.h"
#include "cctwapplication.h"
#include <QMetaProperty>
#include "cctwunitcell.h"
#include "qcepobject.h"

CctwObject::CctwObject(QString name, QcepObjectWPtr parent) :
  QcepObject(name, parent)
{
}

//void CctwObject::printLine(QString line)
//{
//#ifdef NO_GUI
//  printf("%s\n", qPrintable(line));
//#else
//  if (parent()) {
//    QMetaObject::invokeMethod( parent(), "printLine", Q_ARG(QString, line));
//  }
//#endif
//}

//void CctwObject::printMessage(QString msg, QDateTime dt)
//{
//#ifdef NO_GUI
//  printf("%s\n", qPrintable(msg));
//#else
//  if (parent()) {
//    QMetaObject::invokeMethod( parent(), "printMessage", Q_ARG(QString, msg), Q_ARG(QDateTime, dt));
//  } else {
//    printf("MESSAGE: %s %s\n",
//           qPrintable(dt.toString("hh:mm:ss")), qPrintable(msg));
//  }
//#endif
//}

//void CctwObject::writeSettings(QSettings *set, QString section)
//{
//  const QMetaObject *meta = metaObject();

//  while (meta) {
//    QcepProperty::writeSettings(this, meta, section, set);

//    meta = meta->superClass();

//    if (meta == &QObject::staticMetaObject) break;
//  }
//}

//void CctwObject::readSettings(QSettings *set, QString section)
//{
//  QcepProperty::readSettings(this, metaObject(), section, set);
//}

QString CctwObject::scriptValueLiteral(QVariant v)
{
  if (v.canConvert<CctwIntVector3D>()) {
    CctwIntVector3D dv = v.value<CctwIntVector3D>();
    return tr("[%1,%2,%3]").arg(dv.x()).arg(dv.y()).arg(dv.z());
  } else if (v.canConvert<CctwDoubleVector3D>()) {
    CctwDoubleVector3D dv = v.value<CctwDoubleVector3D>();
    return tr("[%1,%2,%3]").arg(dv.x()).arg(dv.y()).arg(dv.z());
  } else if (v.canConvert<CctwDoubleMatrix3x3>()) {
    CctwDoubleMatrix3x3 m = v.value<CctwDoubleMatrix3x3>();
    return tr("[ [%1,%2,%3], [%4,%5,%6], [%7,%8,%9] ]")
        .arg(m(0,0)).arg(m(0,1)).arg(m(0,2))
        .arg(m(1,0)).arg(m(1,1)).arg(m(1,2))
        .arg(m(2,0)).arg(m(2,1)).arg(m(2,2));
  } else if (v.canConvert<CctwUnitCell>()) {
    CctwUnitCell u = v.value<CctwUnitCell>();
    return tr("[%1,%2,%3, %4,%5,%6]")
        .arg(u.a()).arg(u.b()).arg(u.c())
        .arg(u.alpha()).arg(u.beta()).arg(u.gamma());
  } else if (v.type() == QMetaType::QString) {
    return "\"" + CctwApplication::addSlashes(v.toString()) + "\"";
  } else {
    return v.toString();
  }
}

QString CctwObject::settingsScript()
{
  QString res = "";
  const QMetaObject* metaObject = this->metaObject();

  for (int i=1; i < metaObject->propertyCount(); i++) {
    QMetaProperty prop = metaObject->property(i);

    if (prop.isStored()) {
      res += tr("%1.%2 = %3;\n").arg(this->get_Name())
                                .arg(prop.name())
                                .arg(scriptValueLiteral(this->property(prop.name())));
    }
  }

  return res;
}
