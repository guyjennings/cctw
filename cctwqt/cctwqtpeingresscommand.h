#ifndef CCTWQTPEINGRESSCOMMAND_H
#define CCTWQTPEINGRESSCOMMAND_H

#include "cctwobject.h"

class CctwApplication;
class QwtPlot;

class CctwqtPEIngressCommand : public CctwObject
{
  Q_OBJECT
public:
  explicit CctwqtPEIngressCommand(CctwApplication *app, QObject *parent);

public slots:
  void testSlot();
  void analyzePEMetaData(QString path);
  void analyzeSpecDataFile(QString path);

private:
  double normalizedValue(double v);

private:
  CctwApplication *m_Application;
};

#endif // CCTWQTPEINGRESSCOMMAND_H
