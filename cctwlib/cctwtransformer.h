#ifndef CCTWTRANSFORMER_H
#define CCTWTRANSFORMER_H

#include "cctwobject.h"
#include "cctwinputdatainterface.h"
#include "cctwoutputdatainterface.h"
#include "cctwtransforminterface.h"
#include "cctwinputchunkindex.h"

class CctwTransformer : public CctwObject
{
  Q_OBJECT
public:
  CctwTransformer(CctwInputDataInterface *input,    // The input data
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

  virtual void transform() = 0;

private:
  void markInputChunkNeeded(CctwIntVector3D idx);

protected:
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
};

#endif // CCTWTRANSFORMER_H
