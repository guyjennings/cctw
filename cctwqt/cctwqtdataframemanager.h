#ifndef CCTWQTDATAFRAMEMANAGER_H
#define CCTWQTDATAFRAMEMANAGER_H

#include "cctwqtobject.h"

class CctwqtDataFrameManager : public CctwqtObject
{
  Q_OBJECT
public:
  explicit CctwqtDataFrameManager(QObject *parent);
  
signals:
  
public slots:
  void printMessage(QString msg);

private:
  QObject                               *m_Parent;
};

#endif // CCTWQTDATAFRAMEMANAGER_H
