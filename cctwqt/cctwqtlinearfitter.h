#ifndef CCTWQTLINEARFITTER_H
#define CCTWQTLINEARFITTER_H

#include "cctwobject.h"

class CctwqtLinearFitter : public CctwObject
{
  Q_OBJECT
public:
  explicit CctwqtLinearFitter(QObject *parent = 0);
  
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

#endif // CCTWQTLINEARFITTER_H
