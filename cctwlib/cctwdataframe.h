#ifndef CCTWDATAFRAME_H
#define CCTWDATAFRAME_H

#include "cctwobject.h"
#include <QVector>
#include <QAtomicInt>

class CctwDataFrame : public CctwObject
{
  Q_OBJECT
public:
  explicit CctwDataFrame(int dimx, int dimy, QString name, QObject *parent);
  virtual ~CctwDataFrame();

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

#endif // CCTWDATAFRAME_H
