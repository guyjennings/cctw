#ifndef CCTWQTTRANSFORMER_H
#define CCTWQTTRANSFORMER_H

#include "cctwtransformer.h"

#include <QObject>
#include "qcepproperty.h"

class CctwqtApplication;

class CctwqtTransformer : public CctwTransformer
{
  Q_OBJECT
public:
  explicit CctwqtTransformer(CctwqtApplication *application,
                             CctwInputDataInterface *input,    // The input data
                             CctwOutputDataInterface *output,  // The output data
                             CctwTransformInterface *xform,    // The transform
                             int osx, int osy, int osz,
                             QObject *parent);
  
signals:
  
public slots:
  void transformChunk(int nx, int ny, int nz);
  void transform();
  void checkTransform();

  QcepIntList dependencies(int chunkIdx);
  QList<CctwIntVector3D> dependencies(int cx, int cy, int cz);

private:
  void transformChunkNumber(int n);
  void runTransformChunkNumber(int n);

private:
  CctwqtApplication *m_Application;
  QAtomicInt         m_MergeCounter;

public:
  Q_PROPERTY(double wallTime READ get_WallTime WRITE set_WallTime STORED false)
  QCEP_DOUBLE_PROPERTY(WallTime)

//  Q_PROPERTY(int blocksAvailable READ get_BlocksAvailable WRITE set_BlocksAvailable STORED false)
//  QCEP_INTEGER_PROPERTY(BlocksAvailable)

//  Q_PROPERTY(int blocksAllocated READ get_BlocksAllocated WRITE set_BlocksAllocated STORED false)
//  QCEP_INTEGER_PROPERTY(BlocksAllocated)

  Q_PROPERTY(int blocksLimit READ get_BlocksLimit WRITE set_BlocksLimit)
  QCEP_INTEGER_PROPERTY(BlocksLimit)

  Q_PROPERTY(int blocksMax READ get_BlocksMax WRITE set_BlocksMax STORED false)
  QCEP_INTEGER_PROPERTY(BlocksMax)
};

#endif // CCTWQTTRANSFORMER_H
