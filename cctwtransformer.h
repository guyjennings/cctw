#ifndef CCTWTRANSFORMER_H
#define CCTWTRANSFORMER_H

#include "cctwinputdatainterface.h"
#include "cctwoutputdatainterface.h"
#include "cctwtransforminterface.h"

class CctwTransformer
{
public:
  CctwTransformer(CctwInputDataInterface *input,
                  CctwOutputDataInterface *output,
                  CctwTransformInterface *xform,
                  int osx, int osy, int osz);

public:
  void transformChunk(int nx, int ny, int nz);

private:
  CctwInputDataInterface  *m_InputData;
  CctwOutputDataInterface *m_OutputData;
  CctwTransformInterface  *m_Transform;
  int                      m_OversampleX;
  int                      m_OversampleY;
  int                      m_OversampleZ;
};

#endif // CCTWTRANSFORMER_H
