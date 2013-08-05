#ifndef CCTWQTDATAFRAME_H
#define CCTWQTDATAFRAME_H

#include "cctwqtobject.h"
#include <QVector>
#include <QAtomicInt>

class CctwqtDataFrame : public CctwqtObject
{
  Q_OBJECT
public:
  explicit CctwqtDataFrame(int dimx, int dimy, QAtomicInt *refcounter, QObject *parent);
  virtual ~CctwqtDataFrame();

signals:
  
public slots:
  
private:
  QAtomicInt     *m_Counter;
  int             m_DimX;
  int             m_DimY;
  QVector<double> m_Data;
};

#endif // CCTWQTDATAFRAME_H
