#ifndef CCTWQTPEINGRESSCOMMAND_H
#define CCTWQTPEINGRESSCOMMAND_H

#include "cctwobject.h"

class CctwqtApplication;
class QwtPlot;

class CctwqtPEIngressCommand : public CctwObject
{
  Q_OBJECT
public:
  explicit CctwqtPEIngressCommand(CctwqtApplication *app, QObject *parent);

public slots:
  void testSlot();
  void analyzePEMetaData(QString path);
  void analyzeSpecDataFile(QString path);

private:
  double normalizedValue(double v);

private:
  CctwqtApplication *m_Application;
};

#endif // CCTWQTPEINGRESSCOMMAND_H
