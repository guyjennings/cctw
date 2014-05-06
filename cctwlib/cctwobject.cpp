#include "cctwobject.h"
#include "qcepproperty.h"
#include "cctwapplication.h"

CctwObject::CctwObject(QString name, QObject *parent) :
  QObject(parent),
  m_ObjectNamer(this, name),
  m_Name(QcepSettingsSaverWPtr(), this, "name", name, "Object Name")
{
}

void CctwObject::printLine(QString line)
{
  if (parent()) {
    QMetaObject::invokeMethod( parent(), "printLine", Q_ARG(QString, line));
  }
}

void CctwObject::printMessage(QString msg, QDateTime dt)
{
  if (parent()) {
    QMetaObject::invokeMethod( parent(), "printMessage", Q_ARG(QString, msg), Q_ARG(QDateTime, dt));
  } else {
    printf("MESSAGE: %s %s\n",
           qPrintable(dt.toString("hh:mm:ss")), qPrintable(msg));
  }
}

void CctwObject::writeSettings(QSettings *set, QString section)
{
  const QMetaObject *meta = metaObject();

  while (meta) {
    QcepProperty::writeSettings(this, meta, section, set);

    meta = meta->superClass();

    if (meta == &QObject::staticMetaObject) break;
  }
}

void CctwObject::readSettings(QSettings *set, QString section)
{
  QcepProperty::readSettings(this, metaObject(), section, set);
}
