#ifndef CCTWTRANSFORMER_H
#define CCTWTRANSFORMER_H

#include "cctwinputdatainterface.h"
#include "cctwoutputdatainterface.h"
#include "cctwtransforminterface.h"
#include "cctwinputchunkindex.h"

#ifdef USE_QT
#include "cctwqtobject.h"
#endif

#ifdef USE_QT
class CctwTransformer : public CctwqtObject
{
  Q_OBJECT
public:
  CctwTransformer(CctwInputDataInterface *input,    // The input data
                  CctwOutputDataInterface *output,  // The output data
                  CctwTransformInterface *xform,    // The transform
                  int osx, int osy, int osz,        // Oversampling factors
                  int nTests,
                  QObject *parent);
#else
class CctwTransformer
{
public:
  CctwTransformer(CctwInputDataInterface *input,    // The input data
                  CctwOutputDataInterface *output,  // The output data
                  CctwTransformInterface *xform,    // The transform
                  int osx, int osy, int osz,        // Oversampling factors
                  int nTests);
#endif

  virtual ~CctwTransformer();

  static const int MAX_CHUNK = 8*1024*1024; // Maximal size of a disk-resident chunk

  static const int MAX_REQUISITES = 128; // Maximal number of chunks required to produce an output chunk

#ifdef USE_QT
public slots:
#else
public:
#endif
//  static CctwTransformer *createNew(int argc, char *argv[],
//                                    CctwInputDataInterface *input,    // The input data
//                                    CctwOutputDataInterface *output,  // The output data
//                                    CctwTransformInterface *xform);
//                                                // Allocate and return a new transformer object according to the
//                                                // command line passed in argc and argv and using the given
//                                                // input data, output data and transform

  static int XYZtoID(int max_x, int max_y, int max_z,
                     int x, int y, int z);

  void performTests();

  virtual void transformChunk(int nx, int ny, int nz) = 0;
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
