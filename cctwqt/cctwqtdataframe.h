#ifndef CCTWQTDATAFRAME_H
#define CCTWQTDATAFRAME_H

#include "cctwqtobject.h"
#include <QVector>
#include <QAtomicInt>

class CctwqtDataFrame : public CctwqtObject
{
  Q_OBJECT
public:
  explicit CctwqtDataFrame(int dimx, int dimy, QObject *parent);
  virtual ~CctwqtDataFrame();

signals:
  
public slots:
  int reference();
  int referenceCount();
  int dereference();
  QDateTime lastReferenced();

private:
  QMutex          m_Mutex;
  QAtomicInt      m_Counter;
  QDateTime       m_LastReferenced;
  int             m_DimX;
  int             m_DimY;
  QVector<double> m_Data;
};

#endif // CCTWQTDATAFRAME_H
