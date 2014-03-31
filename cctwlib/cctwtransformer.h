#ifndef CCTWTRANSFORMER_H
#define CCTWTRANSFORMER_H

#include "cctwobject.h"
#include "cctwchunkeddata.h"
#include "cctwtransforminterface.h"
#include <QUrl>

class CctwApplication;

typedef QPair<int,int>             CctwDependency;
typedef QVector < CctwDependency > CctwDependencies;

Q_DECLARE_METATYPE(CctwDependencies)

class CctwTransformer : public CctwObject
{
  Q_OBJECT
public:
  CctwTransformer(CctwApplication        *application,
                  CctwChunkedData        *input,    // The input data
                  CctwChunkedData        *output,  // The output data
                  CctwTransformInterface *xform,    // The transform
                  int osx, int osy, int osz,        // Oversampling factors
                  int nTests,
                  QString name,
                  QObject *parent);

  virtual ~CctwTransformer();

public slots:
  void performTests();

  void transform();
  void checkTransform();

  QcepIntList dependencies(int n);
  QList<CctwIntVector3D> dependencies(int cx, int cy, int cz);

  void transformChunkNumber(int n);
  void dummyTransformChunkNumber(int n);

  void clearDependencies();
  void addDependency(int f, int t);
  void completedDependencies();
  int  countDependencies();

  void dummyTransform1();
  void dummyTransform2();
  void dummyTransform3();

public:
  virtual void writeSettings(QSettings *set, QString section);
  virtual void readSettings(QSettings *set, QString section);

private:
  void markInputChunkNeeded(CctwIntVector3D idx);
  void runTransformChunkNumber(int n);
  void runDummyTransformChunkNumber(int n);

private:
  CctwApplication         *m_Application;
  QAtomicInt               m_MergeCounter;
  CctwChunkedData         *m_InputData;
  CctwChunkedData         *m_OutputData;
  CctwTransformInterface  *m_Transform;
  int                      m_OversampleX;
  int                      m_OversampleY;
  int                      m_OversampleZ;
  int                      m_Tests;
  CctwIntVector3D          m_ChunkCount;
  int                      m_ChunksTotal;
  int                     *m_ChunksUsed;
  QMutex                   m_DependencyMutex;
  CctwDependencies         m_Dependencies;

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

  Q_PROPERTY(int blocksHeld READ get_BlocksHeld WRITE set_BlocksHeld STORED false)
  QCEP_INTEGER_PROPERTY(BlocksHeld)

  Q_PROPERTY(int blocksRead READ get_BlocksRead WRITE set_BlocksRead STORED false)
  QCEP_INTEGER_PROPERTY(BlocksRead)

  Q_PROPERTY(int blocksWritten READ get_BlocksWritten WRITE set_BlocksWritten STORED false)
  QCEP_INTEGER_PROPERTY(BlocksWritten)

  Q_PROPERTY(int transformOptions READ get_TransformOptions WRITE set_TransformOptions)
  QCEP_INTEGER_PROPERTY(TransformOptions)
};

#endif // CCTWTRANSFORMER_H
