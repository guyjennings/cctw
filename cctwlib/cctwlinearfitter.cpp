#include "cctwlinearfitter.h"

CctwLinearFitter::CctwLinearFitter(QString name, QObject *parent) :
  CctwObject(name, parent)
{
  startNewFit();
}

void CctwLinearFitter::startNewFit()
{
  m_XData.resize(0);
  m_YData.resize(0);
  m_Slope = 0;
  m_Intercept = 0;
  m_RSquared = 0;
}

void CctwLinearFitter::appendPoint(double x, double y)
{
  m_XData.append(x);
  m_YData.append(y);
}

void CctwLinearFitter::performFit(int skipStart, int skipEnd)
{
  int start = skipStart, end = m_XData.count()-skipEnd;

  double n = 0, meanx = 0, meany = 0;

  for (int i=start; i<end; i++) {
    n += 1;
    meanx += m_XData[i];
    meany += m_YData[i];
  }

  meanx /= n;
  meany /= n;

  double ssxx = 0, ssyy = 0, ssxy = 0;

  for (int i=start; i<end; i++) {
    double x = m_XData[i] - meanx;
    double y = m_YData[i] - meany;

    ssxx += x*x;
    ssyy += y*y;
    ssxy += x*y;
  }

  ssxx /= n;
  ssyy /= n;
  ssxy /= n;

  m_Slope     = ssxy / ssxx;
  m_Intercept = meany - m_Slope*meanx;
  m_RSquared  = (ssxy*ssxy)/(ssxx*ssyy);
}

double CctwLinearFitter::slope()
{
  return m_Slope;
}

double CctwLinearFitter::intercept()
{
  return m_Intercept;
}

double CctwLinearFitter::rSquared()
{
  return m_RSquared;
}
