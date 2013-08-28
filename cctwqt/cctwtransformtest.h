#ifndef CCTWTRANSFORMTEST_H
#define CCTWTRANSFORMTEST_H

#include "cctwqtobject.h"

class CctwqtApplication;

class CctwTransformTest : public CctwqtObject
{
  Q_OBJECT
public:
  explicit CctwTransformTest(CctwqtApplication *app, QObject *parent = 0);
  
signals:
  
public slots:
  
private:
  CctwqtApplication *m_Application;
};

#endif // CCTWTRANSFORMTEST_H
