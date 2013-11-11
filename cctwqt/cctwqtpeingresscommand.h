#ifndef CCTWQTPEINGRESSCOMMAND_H
#define CCTWQTPEINGRESSCOMMAND_H

#include "cctwqtobject.h"

class CctwqtApplication;

class CctwqtPEIngressCommand : public CctwqtObject
{
public:
  CctwqtPEIngressCommand(CctwqtApplication *app, QObject *parent = NULL);

public slots:
  void testSlot();
  void analyzePEMetaData(QString path);

private:
  CctwqtApplication *m_Application;
};

#endif // CCTWQTPEINGRESSCOMMAND_H
