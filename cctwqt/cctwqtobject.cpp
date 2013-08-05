#include "cctwqtobject.h"
#include "qcepproperty.h"

CctwqtObject::CctwqtObject(QObject *parent) :
  QObject(parent)
{
}

void CctwqtObject::printMessage(QString msg)
{
  if (parent()) {
    QMetaObject::invokeMethod( parent(), "printMessage", Q_ARG(QString, msg));
  }
}

void CctwqtObject::writeSettings(QSettings *set, QString section)
{
  QcepProperty::writeSettings(this, &staticMetaObject, section, set);
}

void CctwqtObject::readSettings(QSettings *set, QString section)
{
  QcepProperty::writeSettings(this, &staticMetaObject, section, set);
}
