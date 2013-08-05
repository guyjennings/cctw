#include "cctwqtdataframemanager.h"

CctwqtDataFrameManager::CctwqtDataFrameManager(QObject *parent) :
  CctwqtObject(),
  m_Parent(parent)
{
}

void CctwqtDataFrameManager::printMessage(QString msg)
{
  if (m_Parent) {
    QMetaObject::invokeMethod(m_Parent, "printMessage", Q_ARG(QString, msg));
  }
}
