#include "cctwqtobject.h"

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
