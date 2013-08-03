#ifndef CCTWQTDATAFRAME_H
#define CCTWQTDATAFRAME_H

#include <QObject>
#include <QVector>
#include <QAtomicInt>

class CctwqtDataFrame : public QObject
{
  Q_OBJECT
public:
  explicit CctwqtDataFrame(int dimx, int dimy, QAtomicInt *refcounter = 0);
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
