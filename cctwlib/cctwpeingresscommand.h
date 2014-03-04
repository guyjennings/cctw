#ifndef CCTWPEINGRESSCOMMAND_H
#define CCTWPEINGRESSCOMMAND_H

#include "cctwobject.h"

class CctwApplication;
class QwtPlot;

class CctwPEIngressCommand : public CctwObject
{
  Q_OBJECT
public:
  explicit CctwPEIngressCommand(CctwApplication *app, QObject *parent);

public slots:
  void testSlot();
  void analyzePEMetaData(QString path);
  void analyzeSpecDataFile(QString path);

private:
  double normalizedValue(double v);

private:
  CctwApplication *m_Application;
};

#endif // CCTWPEINGRESSCOMMAND_H
