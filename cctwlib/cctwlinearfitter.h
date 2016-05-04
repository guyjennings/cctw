#ifndef CCTWLINEARFITTER_H
#define CCTWLINEARFITTER_H

#include "cctwobject.h"

class CctwLinearFitter : public CctwObject
{
  Q_OBJECT
public:
  explicit CctwLinearFitter(QString name, QcepObjectWPtr parent);
  
signals:
  
public slots:
  void startNewFit();
  void appendPoint(double x, double y);
  void performFit(int skipStart, int skipEnd);
  double slope();
  double intercept();
  double rSquared();

private:
  QVector<double> m_XData;
  QVector<double> m_YData;
  double          m_Slope;
  double          m_Intercept;
  double          m_RSquared;
};

#endif // CCTWLINEARFITTER_H
