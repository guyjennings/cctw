#ifndef CCTWTRANSFORMER_H
#define CCTWTRANSFORMER_H

#include "cctwinputdatainterface.h"
#include "cctwoutputdatainterface.h"
#include "cctwtransforminterface.h"

class CctwTransformer
{
public:
  CctwTransformer(CctwInputDataInterface *input,    // The input data
                  CctwOutputDataInterface *output,  // The output data
                  CctwTransformInterface *xform,    // The transform
                  int osx, int osy, int osz,        // Oversampling factors
                  int nTests);

  static const int MAX_CHUNK = 8*1024*1024; // Maximal size of a disk-resident chunk

  static const int MAX_REQUISITES = 128; // Maximal number of chunks required to produce an output chunk

public:
  static CctwTransformer *createNew(int argc, char *argv[],
                                    CctwInputDataInterface *input,    // The input data
                                    CctwOutputDataInterface *output,  // The output data
                                    CctwTransformInterface *xform);
                                                // Allocate and return a new transformer object according to the
                                                // command line passed in argc and argv and using the given
                                                // input data, output data and transform

  void transformChunk(int nx, int ny, int nz);
                                                // Transform chunk number nx,ny,nz of the output space
                                                // If parameters are invalid just do nothing

  static int XYZtoID(int max_x, int max_y, int max_z,
                     int x, int y, int z);

  void performTests();

private:
  CctwInputDataInterface  *m_InputData;
  CctwOutputDataInterface *m_OutputData;
  CctwTransformInterface  *m_Transform;
  int                      m_OversampleX;
  int                      m_OversampleY;
  int                      m_OversampleZ;
  int                      m_Tests;
};

#endif // CCTWTRANSFORMER_H
