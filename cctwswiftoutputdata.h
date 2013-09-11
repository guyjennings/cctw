#ifndef CCTWSWIFTOUTPUTDATA_H
#define CCTWSWIFTOUTPUTDATA_H

#include "cctwoutputdatainterface.h"

#ifdef USE_QT
class CctwSwiftOutputData : public CctwOutputDataInterface
{
  Q_OBJECT
public:
  CctwSwiftOutputData(CctwIntVector3D dim,        // Data set dimension
                      CctwIntVector3D chunkSize,  // Chunk size
                      CctwDoubleVector3D origin,
                      CctwDoubleVector3D scale,
                      CctwqtDataFrameManager *manager,
                      QObject *parent);
#else
class CctwSwiftOutputData : public CctwOutputDataInterface
{
public:
  CctwSwiftOutputData(CctwIntVector3D dim,        // Data set dimension
                      CctwIntVector3D chunkSize,  // Chunk size
                      CctwDoubleVector3D origin,
                      CctwDoubleVector3D scale);
#endif

public:
//  static CctwSwiftOutputData* createNew(int argc, char *argv[]);
//                                              // Allocate and return a swift output data object according to the command line passed
//                                              // in argc and argv

  virtual int useChunk(int nx, int ny, int nz);
                                              // Indicate that we'll want to be using chunk number nx,ny,nz of the output data.
                                              // Return an identifier by which the chunk can be referenced, or a negative number on failure
                                              // The chunk should remain accessible until a releaseChunk call is made for that chunk.

  virtual void writeData(int chunkId, int dx, int dy, int dz, double value);
                                              // Write one data value to the output data chunk at relative voxel coordinates dx,dy,dz to
                                              // the lowest indexed corner of the chunk. chunkId will be an id returned by a call to useChunk
                                              // The routine should throw an exception or abort if any parameters are invalid

  virtual void releaseChunk(int chunkId);
                                              // Indicate that we've finished with a chunk of output data - chunkId is an identifier returned
                                              // by a previous call to useChunk.  The routine should throw an exception or abort if its parameter
                                              // is invalid.
};

#endif // CCTWSWIFTOUTPUTDATA_H
