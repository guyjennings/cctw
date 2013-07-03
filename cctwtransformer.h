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
                  int osx, int osy, int osz);       // Oversampling factors

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

private:
  CctwInputDataInterface  *m_InputData;
  CctwOutputDataInterface *m_OutputData;
  CctwTransformInterface  *m_Transform;
  int                      m_OversampleX;
  int                      m_OversampleY;
  int                      m_OversampleZ;
};

#endif // CCTWTRANSFORMER_H
