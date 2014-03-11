#ifndef CCTWINPUTDATAINTERFACE_H
#define CCTWINPUTDATAINTERFACE_H

#include "cctwchunkeddata.h"
#include "cctwinputdataframemanager.h"

class CctwInputDataInterface : public CctwChunkedData
{
  Q_OBJECT
public:
  CctwInputDataInterface(CctwApplication *application,
                         CctwIntVector3D dim,        // Data set dimension
                         CctwIntVector3D chunkSize,  // Chunk size
                         CctwInputDataFrameManager *manager,
                         QString name,
                         QObject *parent);

public slots:

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
