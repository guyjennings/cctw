#include "cctwtransformer.h"
#include "cctwcommandline.h"

CctwTransformer::CctwTransformer(CctwInputDataInterface *input,
                                 CctwOutputDataInterface *output,
                                 CctwTransformInterface *xform,
                                 int osx, int osy, int osz)
  : m_InputData(input),
    m_OutputData(output),
    m_Transform(xform),
    m_OversampleX(osx),
    m_OversampleY(osy),
    m_OversampleZ(osz)
{
}

CctwTransformer *CctwTransformer::createNew(int argc, char *argv[],
                                           CctwInputDataInterface *input,    // The input data
                                           CctwOutputDataInterface *output,  // The output data
                                           CctwTransformInterface *xform)
{
  int osx = CctwCommandLine::integerValue(argc, argv, "--osx", 1);
  int osy = CctwCommandLine::integerValue(argc, argv, "--osy", 1);
  int osz = CctwCommandLine::integerValue(argc, argv, "--osz", 1);

  CctwTransformer *result = new CctwTransformer(input, output, xform, osx, osy, osz);

  return result;
}

void CctwTransformer::transformChunk(int nx, int ny, int nz)
{
}
