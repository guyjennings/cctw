#ifndef CCTWOUTPUTDATAINTERFACE_H
#define CCTWOUTPUTDATAINTERFACE_H

#ifdef USE_QT
#include "cctwqtchunkeddata.h"
#include "cctwqtoutputdataframemanager.h"
#else
#include "cctwchunkeddatainterface.h"
#endif

#ifdef USE_QT
class CctwOutputDataInterface : public CctwqtChunkedData
{
  Q_OBJECT
public:
  CctwOutputDataInterface(CctwIntVector3D dim,        // Data set dimension
                          CctwIntVector3D chunkSize,  // Chunk size
//                          CctwDoubleVector3D origin,
//                          CctwDoubleVector3D scale,
                          CctwqtOutputDataFrameManager *manager,
                          QObject *parent);
#else
class CctwOutputDataInterface : public CctwChunkedDataInterface
{
public:
  CctwOutputDataInterface(CctwIntVector3D dim,        // Data set dimension
                          CctwIntVector3D chunkSize/*,  // Chunk size
                          CctwDoubleVector3D origin,
                          CctwDoubleVector3D scale*/);
#endif

#ifdef USE_QT
public slots:
#else
public:
#endif
  virtual int useChunk(int nx, int ny, int nz) = 0;
                                              // Indicate that we'll want to be using chunk number nx,ny,nz of the output data.
                                              // Return an identifier by which the chunk can be referenced, or a negative number on failure
                                              // The chunk should remain accessible until a releaseChunk call is made for that chunk.

  virtual void writeData(int chunkId, int dx, int dy, int dz, double value) = 0;
                                              // Write one data value to the output data chunk at relative voxel coordinates dx,dy,dz to
                                              // the lowest indexed corner of the chunk. chunkId will be an id returned by a call to useChunk
                                              // The routine should throw an exception or abort if any parameters are invalid

  virtual void releaseChunk(int chunkId) = 0;
                                              // Indicate that we've finished with a chunk of output data - chunkId is an identifier returned
                                              // by a previous call to useChunk.  The routine should throw an exception or abort if its parameter
                                              // is invalid.

  virtual void beginTransform() = 0;

  virtual void endTransform() = 0;
};

#endif // CCTWOUTPUTDATAINTERFACE_H