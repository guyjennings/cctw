#ifndef CCTWTRANSFORMER_H
#define CCTWTRANSFORMER_H

#include "cctwobject.h"
#include "cctwinputdatainterface.h"
#include "cctwoutputdatainterface.h"
#include "cctwtransforminterface.h"
#include "cctwinputchunkindex.h"
#include "cctwdatablobs.h"
#include <QUrl>

class CctwApplication;

class CctwTransformer : public CctwObject
{
  Q_OBJECT
public:
  CctwTransformer(CctwApplication *application,
                  CctwInputDataInterface *input,    // The input data
                  CctwOutputDataInterface *output,  // The output data
                  CctwTransformInterface *xform,    // The transform
                  int osx, int osy, int osz,        // Oversampling factors
                  int nTests,
                  QObject *parent);

  virtual ~CctwTransformer();

  static const int MAX_CHUNK = 8*1024*1024; // Maximal size of a disk-resident chunk

  static const int MAX_REQUISITES = 128; // Maximal number of chunks required to produce an output chunk

public slots:

  static int XYZtoID(int max_x, int max_y, int max_z,
                     int x, int y, int z);

  void performTests();

  void transform();
  void checkTransform();

  QcepIntList dependencies(int chunkIdx);
  QList<CctwIntVector3D> dependencies(int cx, int cy, int cz);

public:
  CctwInputDataBlob*               inputBlob(int blobIdx, QString location);
  QList<CctwIntermediateDataBlob*> transformBlob(CctwInputDataBlob *blob);
  CctwIntermediateDataBlob*        mergeBlobs(CctwIntermediateDataBlob *blob1, CctwIntermediateDataBlob *blob2);
  CctwOutputDataBlob*              normalizeBlob(CctwIntermediateDataBlob *blob);
  void                             outputBlob(QString destination, CctwOutputDataBlob* blob);

private:
  void markInputChunkNeeded(CctwIntVector3D idx);
  void transformChunkNumber(int n);
  void runTransformChunkNumber(int n);

  void generateTestData(int blobIdx, QUrl location, CctwInputDataBlob *blob);
  void readHDF5InputBlob(int blobIdx, QUrl location, CctwInputDataBlob *blob);
  void readArbitraryInputBlob(int blobIdx, QUrl location, CctwInputDataBlob *blob);

  void writeHDF5OutputBlob(int blobIdx, QUrl location, CctwOutputDataBlob *blob);
  void writeArbitraryOutputBlob(int blobIdx, QUrl location, CctwOutputDataBlob *blob);

private:
  CctwApplication         *m_Application;
  QAtomicInt               m_MergeCounter;
  CctwInputDataInterface  *m_InputData;
  CctwOutputDataInterface *m_OutputData;
  CctwTransformInterface  *m_Transform;
  int                      m_OversampleX;
  int                      m_OversampleY;
  int                      m_OversampleZ;
  int                      m_Tests;
  CctwIntVector3D          m_ChunkCount;
  int                      m_ChunksTotal;
  int                     *m_ChunksUsed;

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

#endif // CCTWTRANSFORMER_H
