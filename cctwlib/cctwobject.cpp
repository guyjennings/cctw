#include "cctwobject.h"
#include "qcepproperty.h"
#include "cctwapplication.h"

CctwObject::CctwObject(QString name, QObject *parent) :
  QObject(parent),
  m_ObjectNamer(this, name),
  m_Name(g_Saver, this, "name", name, "Object Name")
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
  }
}

void CctwObject::writeSettings(QSettings *set, QString section)
{
  const QMetaObject *meta = metaObject();

  while (meta) {
    QcepProperty::writeSettings(this, meta, section, set);

    meta = meta->superClass();
  }
}

void CctwObject::readSettings(QSettings *set, QString section)
{
  QcepProperty::readSettings(this, metaObject(), section, set);
}
