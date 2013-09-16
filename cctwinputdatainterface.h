#ifndef CCTWINPUTDATAINTERFACE_H
#define CCTWINPUTDATAINTERFACE_H

#ifdef USE_QT
#include "cctwqtchunkeddata.h"
#include "cctwqtinputdataframemanager.h"
#else
#include "cctwchunkeddatainterface.h"
#endif

#ifdef USE_QT
class CctwInputDataInterface : public CctwqtChunkedData
{
  Q_OBJECT
public:
  CctwInputDataInterface(CctwIntVector3D dim,        // Data set dimension
                         CctwIntVector3D chunkSize,  // Chunk size
//                         CctwDoubleVector3D origin,  // Real coords of pixel 0,0,0
//                         CctwDoubleVector3D scale,   // Real offset of pixel 1,1,1
                         CctwqtInputDataFrameManager *manager,
                         QObject *parent);
#else
class CctwInputDataInterface : public CctwChunkedDataInterface
{
public:
  CctwInputDataInterface(CctwIntVector3D dim,        // Data set dimension
                         CctwIntVector3D chunkSize/*,  // Chunk size
                         CctwDoubleVector3D origin,  // Real coords of pixel 0,0,0
                         CctwDoubleVector3D scale*/);  // Real offset of pixel 1,1,1
#endif

#ifdef USE_QT
public slots:
#else
public:
#endif
  virtual int useChunk(int nx, int ny, int nz) = 0;
                                              // Indicate that we'll want to be using chunk number nx,ny,nz of the input data.
                                              // Return an identifier by which the chunk can be referenced, or a negative number on failure
                                              // The chunk should remain accessible until a releaseChunk call is made for that chunk.

  virtual double readData(int chunkId, int dx, int dy, int dz) = 0;
                                              // Read one data value from the input data chunk at relative voxel coordinates dx,dy,dz to
                                              // the lowest indexed corner of the chunk. chunkId will be an id returned by a call to useChunk
                                              // The routine should throw an exception or abort if any parameters are invalid

  virtual void releaseChunk(int chunkId) = 0;
                                              // Indicate that we've finished with a chunk of input data - chunkId is an identifier returned
                                              // by a previous call to useChunk.  The routine should throw an exception or abort if its parameter
                                              // is invalid.
};

#endif // CCTWINPUTDATAINTERFACE_H
