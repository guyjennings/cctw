#ifndef CCTWQTINPUTDATA_H
#define CCTWQTINPUTDATA_H

#include "cctwqtchunkeddata.h"
#include "cctwinputdatainterface.h"

class CctwqtInputData : public CctwqtChunkedData, public CctwInputDataInterface
{
  Q_OBJECT
public:
  explicit CctwqtInputData(CctwqtDataFrameManager *manager, QObject *parent = 0);
  
signals:
  
public slots:

public:
  virtual int useChunk(int nx, int ny, int nz);
                                              // Indicate that we'll want to be using chunk number nx,ny,nz of the input data.
                                              // Return an identifier by which the chunk can be referenced, or a negative number on failure
                                              // The chunk should remain accessible until a releaseChunk call is made for that chunk.

  virtual double readData(int chunkId, int dx, int dy, int dz);
                                              // Read one data value from the input data chunk at relative voxel coordinates dx,dy,dz to
                                              // the lowest indexed corner of the chunk. chunkId will be an id returned by a call to useChunk
                                              // The routine should throw an exception or abort if any parameters are invalid

  virtual void releaseChunk(int chunkId);
                                              // Indicate that we've finished with a chunk of input data - chunkId is an identifier returned
                                              // by a previous call to useChunk.  The routine should throw an exception or abort if its parameter
                                              // is invalid.
};

#endif // CCTWQTINPUTDATA_H
