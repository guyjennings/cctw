#include "cctwqtobject.h"
#include "qcepproperty.h"
#include "cctwqtapplication.h"

CctwqtObject::CctwqtObject(QObject *parent) :
  QObject(parent),
  m_Name(g_Saver, this, "name", "", "Object Name")
{
}

void CctwqtObject::printLine(QString line)
{
  if (parent()) {
    QMetaObject::invokeMethod( parent(), "printLine", Q_ARG(QString, line));
  }
}

void CctwqtObject::printMessage(QString msg, QDateTime dt)
{
  if (parent()) {
    QMetaObject::invokeMethod( parent(), "printMessage", Q_ARG(QString, msg), Q_ARG(QDateTime, dt));
  }
}

void CctwqtObject::writeSettings(QSettings *set, QString section)
{
  const QMetaObject *meta = metaObject();

  while (meta) {
    QcepProperty::writeSettings(this, meta, section, set);

    meta = meta->superClass();
  }
}

void CctwqtObject::readSettings(QSettings *set, QString section)
{
  QcepProperty::readSettings(this, metaObject(), section, set);
}
